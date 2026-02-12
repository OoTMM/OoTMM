import JSZip from 'jszip';
import { concatUint8Arrays } from 'uint8array-extras';

import { Random, sample, shuffle } from '../../random';
import { toU32Buffer } from '../../util';
import { Game } from '../../config';
import { RomBuilder } from '../../rom-builder';
import { Monitor } from '../../monitor';
import { LogWriter } from '../../util/log-writer';

import { MusicEntry, SEQS } from './SEQS';
import { OotrsParser, MmrsParser, MusicFile } from './music-files';

// #region Categorization Helpers
function isValidSongReplacement(entry: MusicEntry, file: MusicFile) {
  /* See if song works for slot by checking type, game, and category */
  return entry.songType === file.songType
    && (entry.seqOot === undefined || file.games.includes('oot'))
    && (entry.seqMm === undefined || file.games.includes('mm'))
    && entry.musicGroups.some(eg => file.musicGroups.some(fg => fg === eg)); // Categorization~
}

function isValidBackupPlacement(entry: MusicEntry, file: MusicFile) {
  /**
   * If a song can not find a suitable slot when running out of music,
   * then ignore categories altogether like they originally did
   */
  return entry.songType === file.songType
    && (entry.seqOot === undefined || file.games.includes('oot'))
    && (entry.seqMm === undefined || file.games.includes('mm'))
}
// #endregion

// #region Music Injector
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
    /* Create an .ootrs parser, storing only the monitor */
    const songParser = new OotrsParser(this.monitor);

    /**
     * Loop through .ootrs files in the music ZIP.
     * If a file fails to parse, it sends a warning and
     * returns null, all the way up. If it succeeds, then
     * the music file is added to the music list.
     */
    for (const f of files) {
      const song = await songParser.parse(f);
      if (!song) continue;

      this.musics.push(song);
    }
  }

  private async loadMusicsMmrs(files: JSZip.JSZipObject[]) {
    /* Create an .mmrs parser, storing only the monitor */
    const songParser = new MmrsParser(this.monitor);

    /**
     * Loop through .mmrs files in the music ZIP.
     * If a file fails to parse, it sends a warning and
     * returns null, all the way up. If it succeeds, then
     * the music file is added to the music list.
     */
    for (const f of files) {
      const song = await songParser.parse(f);
      if (!song) continue;

      this.musics.push(song);
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
        /**
         * Temporary band-aid fix for custom music files.
         * Custom music files using a custom instrument bank
         * may build on top of a vanilla bank that uses an
         * incorrect audio cache load type for the intended
         * audio sequence or area. This sets the audio cache
         * load type to the correct audio cache type for the
         * corresponding sequence slot.
         */
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
    let musicsName = new Array();

    this.writer.indent('Music');

    /**
     * Assigns songtest song to all songtest-able song slots.
     * Only one songtest song file is allowed, if multiple are
     * detected, this block is skipped.
     */
    const songtestSongs = Array.from(musics).filter(x => x.isSongtest === true);
    if (songtestSongs.length === 1) {
        /* Get the songtest song, then delete it from the song list */
        const songtestSong = songtestSongs[0];
        musics.delete(songtestSong);

        /**
         * Assign the songtest song to all songtest slots, then
         * remove the slot from the pool of available slots.
         */
        for (const [slot, _] of songtestSlots) {
          unassignedSlots = unassignedSlots.filter(s => s !== slot);

          await this.injectMusic(slot, songtestSong);

          const entry = SEQS[slot];
          musicsName.push(`${entry.name} (Songtest): ${songtestSong.name} (${songtestSong.filename})`);
        }
    } else if (songtestSongs.length > 1) {
        this.monitor.warn(`Skipping songtest assigment: multiple songtest files found`);
    }

    /**
     * Loops through the remaining song slots after songtest runs.
     * Using while is volatile, so there are multiple candidacy checks
     * with categorization. It if runs out of candidates, it places
     * remaining songs without any regard to their music groups.
     * (Songs cannot be placed across type, e.g. no fanfares in BGM slots)
     */
    while (musics.size > 0 && unassignedSlots.length > 0) {
      const slotCandidates = new Map<string, MusicFile[]>();

      /* Run through unassigned song slots and assign their candidates */
      for (const slot of unassignedSlots) {
        const bankCheck = (x: MusicFile) => !this.isMaxBank() || x.bankCustom === null;
        const mainCheck = (x: MusicFile) => isValidSongReplacement(SEQS[slot], x) && bankCheck(x);
        const backupCheck = (x: MusicFile) => isValidBackupPlacement(SEQS[slot], x) && bankCheck(x);

        /**
         * Get the candidates based on the main and backup conditions.
         * If there are less than 30 song slots remaining, fallback to
         * completely random assignment.
         * (Size may need to be tuned for OOTMM's amount of songs available)
         */
        const candidates = Array.from(musics).filter(musics.size > 30 ? mainCheck : backupCheck);
        slotCandidates.set(slot, candidates);
      }

      /**
       * If we've gotten to this point, we've hit a true dead end.
       * When this happens, break out of the loop because there is not
       * enough songs to fit the slots available.
       */
      const allZero = [...slotCandidates.values()].every(c => c.length === 0);
      if (allZero) {
        this.monitor.warn(`No valid candidates found for any remaining ${unassignedSlots.length} slots with ${musics.size} music files left!`);
        break;
      }

      /* Pick a song slot that has candidates */
      const [slot, candidates] = [...slotCandidates.entries()].find(([_, c]) => c.length > 0)!;

      /**
       * Assign a random candidate to the slot,
       * then remove the song and slot from the pool.
       */
      const music = sample(this.random, candidates);
      musics.delete(music);
      unassignedSlots = unassignedSlots.filter(s => s !== slot);

      await this.injectMusic(slot, music);

      const entry = SEQS[slot];
      musicsName.push(`${entry.name}: ${music.name} (${music.filename})`);
    }

    for(let entry of musicsName.sort()) {
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
// #endregion

// #region Randomization Call
export async function randomizeMusic(writer: LogWriter, monitor: Monitor, builder: RomBuilder, random: Random, data: Uint8Array) {
  const injector = new MusicInjector(writer, monitor, builder, random, data);
  await injector.run();
}
// #endregion