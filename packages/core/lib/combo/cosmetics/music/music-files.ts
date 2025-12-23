/**
 * Music Files
 *
 * This file contains parsers for custom music files.
 * These parsers handle reading the data from the ZIP
 * files and adding them to the single OOTMM MusicFile
 * type that contains all the music file's data.
 */

import JSZip from 'jszip';
import { concatUint8Arrays } from 'uint8array-extras';

import { Game } from '../../config';
import { Monitor } from '../../monitor';

import { MusicGroup } from './music-groups';
import { SongType } from './enums';

export type MusicFile = {
  filename: string;
  isSongtest?: boolean;
  seq: Uint8Array;
  bankCustom: { meta: Uint8Array, data: Uint8Array } | null;
  name: string;
  games: Game[];
  bankIdMm: number | null;
  bankIdOot: number | null;
  songType: SongType;
  musicGroups: MusicGroup[];
};

type MusicFiles = {
  metadata: JSZip.JSZipObject;
  seq: JSZip.JSZipObject;
  zbank?: JSZip.JSZipObject;
  bankmeta?: JSZip.JSZipObject
};

type MusicMetadata = {
  name: string;
  instrumentSet: number;
  songType: SongType;
  musicGroups: MusicGroup[];
};

const LIGATURES: Record<string, string> = {
  'æ': 'ae',
  'Æ': 'AE',
  'œ': 'oe',
  'Œ': 'OE',
  'ß': 'ss',
  'ẞ': 'SS',
  'ﬀ': 'ff',
  'ﬁ': 'fi',
  'ﬂ': 'fl',
  'ﬃ': 'ffi',
  'ﬄ': 'ffl',
  'ĳ': 'ij',
  'Ĳ': 'IJ',
  'ǉ': 'lj',
  'ǈ': 'Lj',
  'ǌ': 'nj',
  'ǋ': 'Nj',
};

function saneName(name: string) {
  return name
    .normalize('NFD')
    .replace(
      new RegExp(Object.keys(LIGATURES).join('|'), 'g'),
      m => LIGATURES[m]
    ) // Remove ligatures
    .replace(/\p{M}/gu, '') // Remove diacritics
    .replace(/[^ -~]/g, ''); // Remove non-ASCII
}

// #region Abstract Parser
abstract class MusicParser {
  constructor(protected monitor: Monitor) {}

  abstract validateFiles(zip: JSZip): Promise<MusicFiles | null>;
  abstract extractMetadata(zipName: string, files: any): Promise<MusicMetadata | null>;
  abstract parse(f: JSZip.JSZipObject): Promise<MusicFile | null>;

  protected async loadZip(f: JSZip.JSZipObject): Promise<JSZip | null> {
    try {
      return await JSZip.loadAsync(await f.async('uint8array'));
    } catch {
      this.monitor.warn(`Skipped music file ${f.name}: invalid zip file`);
      return null;
    }
  }

  protected padArrayTo16(array: Uint8Array): Uint8Array {
    if (array.length & 0x0f) {
      const pad = new Uint8Array(0x10 - (array.length & 0xf));
      pad.fill(0);
      return concatUint8Arrays([array, pad]);
    }
    return array;
  }

  protected buildFileMap(zip: JSZip): { files: JSZip.JSZipObject[]; fileMap: Record<string, JSZip.JSZipObject[]>; } {
    const files = Object.values(zip.files) as JSZip.JSZipObject[];
    const fileMap: Record<string, JSZip.JSZipObject[]> = {};

    for (const f of files) {
      const ext = f.name.split('.').pop()?.toLowerCase();
      if (ext) (fileMap[ext] ??= []).push(f);
    }

    return { files, fileMap };
  }

  protected hasUnsupportedFiles(zipName: string, fileMap: Record<string, JSZip.JSZipObject[]>): boolean {
    if ((fileMap['zsound'] ?? []).length > 0) {
      this.monitor.warn(`Skipped music file ${zipName}: custom audio samples not currently supported`);
      return true;
    }

    return false;
  }

  protected validateSingleFile(
    zipName: string,
    filesOrMap: JSZip.JSZipObject[] | Record<string, JSZip.JSZipObject[]>,
    extNames: string | string[],
    required: boolean = false
  ): JSZip.JSZipObject | null {
    const exts = Array.isArray(extNames) ? extNames : [extNames];
    const files = Array.isArray(filesOrMap) ? filesOrMap : exts.flatMap(ext => filesOrMap[ext] ?? []);

    if (files.length > 1) {
      this.monitor.warn(`Skipped music file ${zipName}: multiple ${exts.join('/')} files`);
      return null;
    }

    if (required && files.length === 0) {
      this.monitor.warn(`Skipped music file ${zipName}: missing required ${exts.join('/')} file`);
      return null;
    }

    return files[0] ?? null;
  }

  protected validateBankFiles(
    zipName: string,
    zbankFiles: JSZip.JSZipObject[],
    bankmetaFiles: JSZip.JSZipObject[]
  ): { zbank: JSZip.JSZipObject; bankmeta: JSZip.JSZipObject } | null {
    const zbank = this.validateSingleFile(zipName, zbankFiles, 'zbank');
    const bankmeta = this.validateSingleFile(zipName, bankmetaFiles, 'bankmeta');

    /* Ensure there is a pair of bank files */
    if ((zbank && !bankmeta) || (!zbank && bankmeta)) {
      const missingFile = zbank ? 'zbank' : 'bankmeta';

      this.monitor.warn(`Skipped music file ${zipName}: missing expected ${missingFile} file`);
      return null;
    }

    return {
      zbank: zbank,
      bankmeta: bankmeta,
    };
  }

  protected async buildMusicFile(
    f: JSZip.JSZipObject,
    songFiles: MusicFiles,
    metadata: MusicMetadata,
    primaryGame: Game
  ): Promise<MusicFile | null> {
    const filename = f.name.split('/').pop()!;
    const isSongtest = filename.toLowerCase().includes('songtest');
    const games: Game[] = [primaryGame];

    const altGame = primaryGame === 'oot' ? 'mm' : 'oot'; // We want the opposite of primary
    const isOotrSong = primaryGame === 'oot';
    const isMmrSong = primaryGame === 'mm';
    const bankId = metadata.instrumentSet;

    const seq = this.padArrayTo16(await songFiles.seq.async('uint8array'));
    let bankCustom: { meta: Uint8Array; data: Uint8Array } | null = null;
    let bankIdOot: number | null = null;
    let bankIdMm: number | null = null;

    if (songFiles.zbank && songFiles.bankmeta) {
      const zbank = await songFiles.zbank.async('uint8array');
      const bankmeta = await songFiles.bankmeta.async('uint8array');

      if (bankmeta.length !== 8) {
        this.monitor.warn(`Skipped music file ${f.name}: invalid bankmeta length`);
        return null;
      }

      /* Update sample bank IDs for OOTMM */
      if (isMmrSong) {
        bankmeta.set(
          new Uint8Array([
            bankmeta[0x02] === 0xff ? 0xff : bankmeta[0x02] + 8,
            bankmeta[0x03] === 0xff ? 0xff : bankmeta[0x03] + 8,
          ]),
          0x02
        );
      }

      bankCustom = { meta: bankmeta, data: zbank };
      games.push(altGame);
    } else if (bankId >= 2) {
      bankIdOot = isOotrSong ? bankId : bankId + 0x30;
      bankIdMm = isMmrSong ? bankId : bankId + 0x30;
      games.push(altGame);
    }

    return {
      filename: filename,
      isSongtest: isSongtest,
      seq: seq,
      bankCustom: bankCustom,
      name: metadata.name,
      games: games,
      bankIdOot: bankIdOot,
      bankIdMm: bankIdMm,
      songType: metadata.songType,
      musicGroups: metadata.musicGroups,
    };
  }
}
// #endregion

// #region OOTRS Parser
export class OotrsParser extends MusicParser {
  async validateFiles(zip: JSZip) {
    /* Gather files */
    const { files, fileMap } = this.buildFileMap(zip);

    /* Check for unsupported files */
    if (this.hasUnsupportedFiles(zip.name, fileMap)) return null;

    /* Handle rest of files */
    const seq = this.validateSingleFile(zip.name, fileMap['seq'] ?? [], 'seq', true);
    const meta = this.validateSingleFile(zip.name, fileMap['meta'] ?? [], 'meta', true);
    const bankPair = this.validateBankFiles(zip.name, fileMap['zbank'] ?? [], fileMap['bankmeta'] ?? []);
    if (!seq || !meta || !bankPair) return null;

    return {
      metadata: meta!,
      seq: seq!,
      zbank: bankPair.zbank, // null is fine
      bankmeta: bankPair.bankmeta, // null is fine
    };
  }

  async extractMetadata(zipName: string, files: MusicFiles): Promise<MusicMetadata | null> {
    const rawTxt = await files.metadata.async('text');
    const meta = rawTxt.split(/\r?\n/);

    /* Probably unneeded, songs can default to bgm easily with music groups */
    if (meta.length < 3) {
      this.monitor.warn(`Skipped music file ${zipName}: OOTRS metadata must have at least 3 lines`);
      return null;
    }

    const name = saneName(meta[0]);
    const instrumentSet = parseInt(meta[1], 16);
    const songType = meta[2] === 'fanfare' ? SongType.Fanfare : SongType.Bgm; // just 'f' is not valid

    if (isNaN(instrumentSet)) {
      this.monitor.warn(`Skipped music file ${zipName}: meta instrument set "${instrumentSet}" is not a valid hex number`);
      return null;
    }

    const musicGroups = meta[3]
      ? MusicGroup.matchGroups(meta[3].split(',').map((x: string) => x.trim()), zipName, this.monitor)?.groups ?? []
      : songType === SongType.Fanfare
      ? [...MusicGroup.AllFanfareGroups]
      : [...MusicGroup.AllBgmGroups];

    return {
      name: name,
      instrumentSet: instrumentSet,
      songType: songType,
      musicGroups: musicGroups,
    };
  }

  async parse(f: JSZip.JSZipObject): Promise<MusicFile | null> {
    const zip = await this.loadZip(f);
    if (!zip) return null;

    const songFiles = await this.validateFiles(zip);
    if (!songFiles) return null;

    const metadata = await this.extractMetadata(zip.name, songFiles);
    if (!metadata) return null;

    return await this.buildMusicFile(zip, songFiles, metadata, 'oot');
  }
}
// #endregion

// #region MMRS Parser
export class MmrsParser extends MusicParser {
  async validateFiles(zip: JSZip) {
    /* Gather files */
    const { files, fileMap } = this.buildFileMap(zip);
    const categoriesFiles = files.filter(f => f.name.toLowerCase().endsWith('categories.txt'));

    /* Check for unsupported files */
    if (this.hasUnsupportedFiles(zip.name, fileMap)) return null;

    /* Handle rest of files */
    const seq = this.validateSingleFile(zip.name, fileMap, ['zseq', 'aseq', 'seq'], true);
    const categories = this.validateSingleFile(zip.name, categoriesFiles, 'categories.txt', true);
    const bankPair = this.validateBankFiles(zip.name, fileMap['zbank'] ?? [], fileMap['bankmeta'] ?? []);
    if (!seq || !categories || !bankPair) return null;

    return {
      metadata: categories,
      seq: seq,
      zbank: bankPair.zbank, // null is fine
      bankmeta: bankPair.bankmeta, // null is fine
    };
  }

  async extractMetadata(zipName: string, files: MusicFiles): Promise<MusicMetadata | null> {
    const rawCategories = await files.metadata.async('text');
    const rawGroups = rawCategories.split(/[,-]/).map((x: string) => x.trim());

    /* Extract the metadata from the sequence filename */
    const name = files.seq.replace(/\.[^/.]+$/, '');
    const instrumentSet = parseInt(name, 16);

    if (isNaN(instrumentSet)) {
      this.monitor.warn(`Skipped music file ${zipName}: zseq filename "${files.seq.name}" is not a valid hex number`);
      return null;
    }

    /* Extract the type and music groups from categories.txt */
    const typeAndGroups = MusicGroup.matchGroups(rawGroups, zipName, this.monitor);
    if (!typeAndGroups) return null;

    const { type: songType, groups: musicGroups } = typeAndGroups;

    return {
      name: name,
      instrumentSet: instrumentSet,
      songType: songType,
      musicGroups: musicGroups,
    };
  }

  async parse(f: JSZip.JSZipObject): Promise<MusicFile | null> {
    const zip = await this.loadZip(f);
    if (!zip) return null;

    const songFiles = await this.validateFiles(zip);
    if (!songFiles) return null;

    const metadata = await this.extractMetadata(zip.name, songFiles);
    if (!metadata) return null;

    return await this.buildMusicFile(zip, songFiles, metadata, 'oot');
  }
}
// #endregion