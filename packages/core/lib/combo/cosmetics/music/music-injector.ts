import JSZip from 'jszip';

import { Random, sample, shuffle } from '../../random';
import { toU32Buffer } from '../../util';
import { Game } from '../../config';
import { RomBuilder } from '../../rom-builder';
import { Monitor } from '../../monitor';
import { LogWriter } from '../../util/log-writer';
import { concatUint8Arrays } from 'uint8array-extras';

import { SongType } from './enums';
import { MusicGroup } from './music-groups';
import { MusicEntry, SEQS } from './SEQS';

type MusicFile = {
  songType: SongType;
  seq: Uint8Array;
  bankIdOot: number | null;
  bankIdMm: number | null;
  bankCustom: { meta: Uint8Array, data: Uint8Array } | null;
  filename: string;
  name: string;
  games: Game[];
  musicGroups: MusicGroup[];
  isSongtest?: boolean;
};

const DIACRITICS_BASES = {
  'á': 'a',
  'à': 'a',
  'â': 'a',
  'ä': 'a',
  'Á': 'A',
  'À': 'A',
  'Â': 'A',
  'Ä': 'A',
  'é': 'e',
  'è': 'e',
  'ê': 'e',
  'ë': 'e',
  'É': 'E',
  'È': 'E',
  'Ê': 'E',
  'Ë': 'E',
  'í': 'i',
  'ì': 'i',
  'î': 'i',
  'ï': 'i',
  'Í': 'I',
  'Ì': 'I',
  'Î': 'I',
  'Ï': 'I',
  'ó': 'o',
  'ò': 'o',
  'ô': 'o',
  'ö': 'o',
  'Ó': 'O',
  'Ò': 'O',
  'Ô': 'O',
  'Ö': 'O',
  'ú': 'u',
  'ù': 'u',
  'û': 'u',
  'ü': 'u',
  'Ú': 'U',
  'Ù': 'U',
  'Û': 'U',
  'Ü': 'U',
  'ý': 'y',
  'ÿ': 'y',
  'Ý': 'Y',
  'Ÿ': 'Y',
  'ç': 'c',
  'Ç': 'C',
  'ñ': 'n',
  'Ñ': 'N',
  'æ': 'ae',
  'Æ': 'AE',
  'œ': 'oe',
  'Œ': 'OE',
};

function saneName(name: string) {
  /* Force NFC */
  name = name.normalize('NFC');

  /* Remove diacritics */
  for (const [base, repl] of Object.entries(DIACRITICS_BASES)) {
    name = name.replaceAll(base, repl);
  }

  /* Remove every other non-ascii */
  name = name.replace(/[^ -~]/g, '');

  return name;
}

function isValidSongReplacement(entry: MusicEntry, file: MusicFile) {
  /* See if song works for slot by checking type, game, and category */
  return entry.songType === file.songType
    && (entry.seqOot === undefined || file.games.includes('oot'))
    && (entry.seqMm === undefined || file.games.includes('mm'))
    && entry.musicGroups.some(eg => file.musicGroups.some(fg => fg === eg)); // Categorization~
}

function isValidBackupPlacement(entry: MusicEntry, file: MusicFile) {
  /*
   * If a song can not find a suitable slot when running out of music,
   * then ignore categories altogether like they originally did
   */
  return entry.songType === file.songType
    && (entry.seqOot === undefined || file.games.includes('oot'))
    && (entry.seqMm === undefined || file.games.includes('mm'))
}

function mmrSampleBank(sb: number) {
  if (sb === 0xff) {
    return 0xff;
  }
  return sb + 8;
}

class MusicInjector {
  private musics: MusicFile[];
  private namesBuffer: Uint8Array;
  private bankId: number;

  constructor(
    private writer: LogWriter,
    private monitor: Monitor,
    private builder: RomBuilder,
    private random: Random,
    private musicZipData: Uint8Array,
  ) {
    this.musics = [];
    this.namesBuffer = new Uint8Array(256 * 2 * 48);
    this.bankId = 0x60;
  }

  private isMaxBank() {
    return this.bankId >= 0xf0;
  }

  private addCustomBank(meta: Uint8Array, data: Uint8Array) {
    const bankId = this.bankId++;
    const dataVrom = this.appendAudio(data);
    const dataSize = data.length;
    const prefix = toU32Buffer([dataVrom, dataSize]);
    const fullmeta = concatUint8Arrays([prefix, meta]);
    const customFile = this.builder.fileByNameRequired('custom/bank_table');
    const offset = (bankId - 0x60) * 0x10;
    customFile.data.set(fullmeta, offset);
    return bankId;
  }

  private registerName(seqId: number, name: string) {
    /* Cut name to 48 characters */
    name = name.slice(0, 48);

    /* Write to buffer */
    const offset = seqId * 48;
    const nameEncoded = new TextEncoder().encode(name);
    this.namesBuffer.set(nameEncoded, offset);
  }

  private async loadMusicsOotrs(files: JSZip.JSZipObject[]) {
    for (const f of files) {
      /* Get the music zip */
      const musicZipBuffer = await f.async('uint8array');
      let musicZip: JSZip;
      try {
        musicZip = await JSZip.loadAsync(musicZipBuffer);
      } catch (e) {
        this.monitor.warn(`Skipped music file ${f.name}: invalid zip file`);
        continue;
      }

      /* Look for custom bank data */
      const filesBank = musicZip.file(/\.z?bank$/);
      if (filesBank.length > 1) {
        this.monitor.warn(`Skipped music file ${f.name}: multiple bank files`);
        continue;
      }
      const filesBankmeta = musicZip.file(/\.z?bankmeta$/);
      if (filesBankmeta.length > 1) {
        this.monitor.warn(`Skipped music file ${f.name}: multiple bankmeta files`);
        continue;
      }

      if (filesBank.length !== filesBankmeta.length) {
        this.monitor.warn(`Skipped music file ${f.name}: bank and bankmeta mismatch`);
        continue;
      }

      const badFiles = musicZip.file(/\.z?sound$/);
      if (badFiles.length > 0) {
        this.monitor.warn(`Skipped music file ${f.name}: unsupported files found`);
        continue;
      }

      /* Find the meta file */
      const metaFile = musicZip.file(/\.meta$/);
      if (metaFile.length !== 1) {
        this.monitor.warn(`Skipped music file ${f.name}: multiple metadata files`);
        continue;
      }

      /* Find the seq file */
      const seqFiles = musicZip.file(/\.seq$/);
      if (seqFiles.length !== 1) {
        this.monitor.warn(`Skipped music file ${f.name}: multiple sequence files`);
        continue;
      }

      /* Parse the metadata */
      const metaRaw = await metaFile[0].async('text');
      const meta = metaRaw.split(/\r?\n/);
      if (meta.length < 3) {
        this.monitor.warn(`Skipped music file ${f.name}: metadata must have at least 3 lines`);
        continue;
      }

      const filename = f.name.split('/').pop()!;
      const isSongtest = filename.toLowerCase().includes('songtest');
      const name = saneName(meta[0]);

      let songType: SongType;
      songType = (meta[2] === 'fanfare' || meta[2] === 'f') ? SongType.Fanfare : SongType.Bgm;

      let rawGroups: string[];
      let musicGroups: MusicGroup[];

      /*
       * Handle music groups, if there is 4 lines
       * grab the music groups the creator added,
       * otherwise use defaults from MusicGroup
       */
      if (meta.length >= 4) {
        rawGroups = meta[3].split(',').map(x => x.trim());

        const typeAndGroups = MusicGroup.matchGroups(rawGroups, f.name, this.monitor);
        if (!typeAndGroups) continue; // warn sent and null return, so continue to the next song

        musicGroups = typeAndGroups.groups;

      } else {
        musicGroups = (songType === SongType.Fanfare)
                    ? [...MusicGroup.AllFanfareGroups]
                    : [...MusicGroup.AllBgmGroups];
      }

      const games: Game[] = ['oot'];

      let bankCustom: { meta: Uint8Array, data: Uint8Array } | null = null;
      let bankIdOot: number | null = null;
      let bankIdMm: number | null = null;

      if (filesBank.length) {
        const bank = await filesBank[0].async('uint8array');
        const bankmeta = await filesBankmeta[0].async('uint8array');
        if (bankmeta.length !== 0x08) {
          this.monitor.warn(`Skipped music file ${f.name}: invalid bankmeta length`);
          continue;
        }

        bankCustom = { meta: bankmeta, data: bank };
        games.push('mm');
      } else {
        bankIdOot = parseInt(meta[1], 16);
        if (bankIdOot >= 2) {
          bankIdMm = bankIdOot + 0x30;
          games.push('mm');
        }
      }

      /* Add the music */
      let seq = await seqFiles[0].async('uint8array');
      if (seq.length & 0xf) {
        const pad = new Uint8Array(16 - (seq.length & 0xf));
        pad.fill(0x00);
        seq = concatUint8Arrays([seq, pad]);
      }
      const music: MusicFile = { songType, seq, bankIdOot, bankIdMm, bankCustom, filename, name, games, musicGroups, isSongtest };
      this.musics.push(music);
    }
  }

  private async loadMusicsMmrs(files: JSZip.JSZipObject[]) {
    for (const f of files) {
      /* Get the music zip */
      const musicZipBuffer = await f.async('uint8array');
      let musicZip: JSZip;
      try {
        musicZip = await JSZip.loadAsync(musicZipBuffer);
      } catch (e) {
        this.monitor.warn(`Skipped music file ${f.name}: invalid zip file`);
        continue;
      }

      /* Look for custom bank data */
      const filesBank = musicZip.file(/\.z?bank$/);
      if (filesBank.length > 1) {
        this.monitor.warn(`Skipped music file ${f.name}: multiple bank files`);
        continue;
      }
      const filesBankmeta = musicZip.file(/\.z?bankmeta$/);
      if (filesBankmeta.length > 1) {
        this.monitor.warn(`Skipped music file ${f.name}: multiple bankmeta files`);
        continue;
      }

      if (filesBank.length !== filesBankmeta.length) {
        this.monitor.warn(`Skipped music file ${f.name}: bank and bankmeta mismatch`);
        continue;
      }

      const badFiles = musicZip.file(/\.z?sound$/);
      if (badFiles.length > 0) {
        this.monitor.warn(`Skipped music file ${f.name}: unsupported files found`);
        continue;
      }

      /* Find the zseq file */
      const zseqFiles = musicZip.file(/\.zseq$/);
      if (zseqFiles.length !== 1) {
        this.monitor.warn(`Skipped music file ${f.name}: multiple sequence files`);
        continue;
      }

      /* Get the categories.txt file */
      const categoriesTxt = musicZip.file('categories.txt');
      if (!categoriesTxt) {
        this.monitor.warn(`Skipped music file ${f.name}: categories.txt not found`);
        continue;
      }
      const categoriesData = await categoriesTxt.async('text');
      const rawGroups = categoriesData.split(/[,-]/).map(x => x.trim());

      /* Extract the bank ID from the zseq filename */
      let zseqFilename = zseqFiles[0].name;
      if (zseqFilename.includes('/')) {
        zseqFilename = zseqFilename.split('/').pop()!;
      }
      const bankIdRaw = zseqFilename.split('.')[0];

      /* Add the music */
      let seq = await zseqFiles[0].async('uint8array');
      if (seq.length & 0xf) {
        const pad = new Uint8Array(16 - (seq.length & 0xf));
        pad.fill(0x00);
        seq = concatUint8Arrays([seq, pad]);
      }

      const games: Game[] = ['mm'];

      /* Extract the song type and music groups from the categories.txt file */
      let songType: SongType;
      let musicGroups: MusicGroup[];

      const typeAndGroups = MusicGroup.matchGroups(rawGroups, f.name, this.monitor);
      if (!typeAndGroups) continue; // warn sent and null return, so continue to the next song

      songType = typeAndGroups.type;
      musicGroups = typeAndGroups.groups;

      const filename = f.name.split('/').pop()!;
      const isSongtest = filename.toLowerCase().includes('songtest');
      const name = saneName(filename.replace('.mmrs', ''));

      let bankCustom: { meta: Uint8Array, data: Uint8Array } | null = null;
      let bankIdOot: number | null = null;
      let bankIdMm: number | null = null;

      if (filesBank.length) {
        const bank = await filesBank[0].async('uint8array');
        const bankmeta = await filesBankmeta[0].async('uint8array');
        if (bankmeta.length !== 0x08) {
          this.monitor.warn(`Skipped music file ${f.name}: invalid bankmeta length`);
          continue;
        }

        const sampleBank1 = mmrSampleBank(bankmeta[0x02]);
        const sampleBank2 = mmrSampleBank(bankmeta[0x03]);
        const sampleBanks = new Uint8Array([sampleBank1, sampleBank2]);
        bankmeta.set(sampleBanks, 0x02);
        bankCustom = { meta: bankmeta, data: bank };
        games.push('oot');
      } else {
        bankIdMm = parseInt(bankIdRaw, 16);
        if (bankIdMm >= 2) {
          bankIdOot = bankIdMm + 0x30;
          games.push('oot');
        }
      }

      const music: MusicFile = { songType, seq, bankIdOot, bankIdMm, bankCustom, filename, name, games, musicGroups, isSongtest };
      this.musics.push(music);
    }
  }

  private async loadMusics(data: Uint8Array) {
    const zip = await JSZip.loadAsync(data);
    await this.loadMusicsOotrs(zip.file(/\.ootrs$/));
    await this.loadMusicsMmrs(zip.file(/\.mmrs$/));
  }

  private appendAudio(seq: Uint8Array) {
    const vrom = this.builder.addFile({ game: 'custom', type: 'uncompressed', data: seq })!;
    return vrom;
  }

  private async injectMusicMeta(game: Game, slot: number, vrom: number, seqLength: number, bankId: number, name: string) {
    const fileSeqTable = this.builder.fileByNameRequired(`${game}/seq_table`);
    const fileSeqBanks = this.builder.fileByNameRequired(`${game}/seq_banks`);

    /* Patch the bank ID */
    fileSeqBanks.data[slot] = bankId;

    /* Add the pointer */
    const seqTableData = toU32Buffer([vrom, seqLength]);
    fileSeqTable.data.set(seqTableData, slot * 0x10);

    /* Register the name */
    this.registerName(game === 'mm' ? slot + 256 : slot, name);
  }

  private async injectMusic(slot: string, music: MusicFile) {
    const entry = SEQS[slot];
    const vrom = this.appendAudio(music.seq);
    let customBankId: number | null = null;

    if (music.bankCustom) {
        /* Ensure custom music bankmeta has the correct cache load type. */
        if (entry.bankCacheType !== undefined){
          music.bankCustom.meta[1] = entry.bankCacheType;
        }

        customBankId = this.addCustomBank(music.bankCustom.meta, music.bankCustom.data);
    }

    for (const id of entry.seqOot || []) {
      await this.injectMusicMeta('oot', id, vrom, music.seq.length, customBankId || music.bankIdOot!, music.name);
    }

    for (const id of entry.seqMm || []) {
      await this.injectMusicMeta('mm', id, vrom, music.seq.length, customBankId || music.bankIdMm!, music.name);
    }
  }

  private patchOot() {
    /* Disable battle music */
    const filePlayerActor = this.builder.fileByNameRequired('oot/ovl_player_actor');
    filePlayerActor.data[0x1690f] = 0;
  }

  private patchMm() {
    /* Disable battle music */
    const filePlayerActor = this.builder.fileByNameRequired('mm/ovl_player_actor');
    filePlayerActor.data[0x16818] = 0x10;
    filePlayerActor.data[0x16819] = 0x00;
  }

  private async shuffleMusics() {
    const shuffledSlots = shuffle(this.random, Object.keys(SEQS));
    const songtestSlots = Object.entries(SEQS).filter(([_, entry]) => entry.canSongtest === true);
    const musics = new Set(this.musics);

    let unassignedSlots = [...shuffledSlots];
    let musics_name = new Array();

    this.writer.indent('Music');

    /* Assign songtest song to all songtest-able song slots */
    const songtestSongs = Array.from(musics).filter(x => x.isSongtest === true);
    if (songtestSongs.length === 1) {
        const songtestSong = songtestSongs[0];
        musics.delete(songtestSong); // Remove song from pool of files

        for (const [slot, _] of songtestSlots) {
          unassignedSlots = unassignedSlots.filter(s => s !== slot); // Remove slot from unassigned

          await this.injectMusic(slot, songtestSong);

          const entry = SEQS[slot];
          musics_name.push(`${entry.name} (Songtest): ${songtestSong.name} (${songtestSong.filename})`);
        }
    } else if (songtestSongs.length > 1) {
        this.monitor.warn(`Skipping songtest assigment: multiple songtest files found`);
    }

    /* Assign music to all remaining unassigned song slots */
    while (musics.size > 0 && unassignedSlots.length > 0) {
      /* Figure out the candidates for remaining slots */
      const slotCandidates = new Map<string, MusicFile[]>();

      for (const slot of unassignedSlots) {
        const bankCheck = (x: MusicFile) => !this.isMaxBank() || x.bankCustom === null;
        const mainCheck = (x: MusicFile) => isValidSongReplacement(SEQS[slot], x) && bankCheck(x);
        const backupCheck = (x: MusicFile) => isValidBackupPlacement(SEQS[slot], x) && bankCheck(x);

        // Size cond. might need to change (96 BGM, 11 Fanfares available in SEQS)
        const candidates = Array.from(musics).filter(musics.size > 30 ? mainCheck : backupCheck);
        slotCandidates.set(slot, candidates);
      }

      /* Check if all remaining slots have zero candidates (true dead-end situation) */
      const allZero = [...slotCandidates.values()].every(c => c.length === 0);
      if (allZero) {
        this.monitor.warn(`No valid candidates found for any remaining ${unassignedSlots.length} slots with ${musics.size} music files left!`);
        break; // In this situation, we may end up in an infinite loop, so we have to break
      }

      /* Pick a slot with candidates */
      const [slot, candidates] = [...slotCandidates.entries()].find(([_, c]) => c.length > 0)!;

      /* Assign a random music file from the candidates */
      const music = sample(this.random, candidates);
      musics.delete(music);
      unassignedSlots = unassignedSlots.filter(s => s !== slot); // remove slot from unassigned

      await this.injectMusic(slot, music);

      const entry = SEQS[slot];
      musics_name.push(`${entry.name}: ${music.name} (${music.filename})`);
    }

    for(let entry of musics_name.sort()) {
      this.writer.write(entry);
    }

    this.writer.unindent();
  }

  async run() {
    /* Extract the list of musics */
    await this.loadMusics(this.musicZipData);

    /* Shuffle musics */
    await this.shuffleMusics();

    /* Run misc. patches */
    this.patchOot();
    this.patchMm();

    /* Inject the music names */
    this.builder.addFile({ game: 'custom', type: 'uncompressed', vaddr: 0xf1000000, data: this.namesBuffer });
  }
}

export async function randomizeMusic(writer: LogWriter, monitor: Monitor, builder: RomBuilder, random: Random, data: Uint8Array) {
  const injector = new MusicInjector(writer, monitor, builder, random, data);
  await injector.run();
}
