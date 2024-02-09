import JSZip from 'jszip';

import { Random, shuffle } from '../random';
import { Patchfile } from '../patch-build/patchfile';
import { toU32Buffer } from '../util';
import { DecompressedRoms } from '../decompress';
import { Game } from '../config';
import { RomBuilder } from '../rom-builder';

const OOT_AUDIOSEQ_ADDR = 0x29de0;
const OOT_AUDIOSEQ_SIZE = 0x4f690;

const MM_AUDIOSEQ_ADDR = 0x46af0;
const MM_AUDIOSEQ_SIZE = 0x51480;

const OOT_MUSICS = {
  "Hyrule Field": 0x02,
  "Dodongos Cavern": 0x18,
  "Kakariko Adult": 0x19,
  "Battle": 0x1A,
  "Boss Battle": 0x1B,
  "Inside Deku Tree": 0x1C,
  "Market": 0x1D,
  "Title Theme": 0x1E,
  "House": 0x1F,
  "Jabu Jabu": 0x26,
  "Kakariko Child": 0x27,
  "Fairy Fountain": 0x28,
  "Zelda Theme": 0x29,
  "Fire Temple": 0x2A,
  "Forest Temple": 0x2C,
  "Castle Courtyard": 0x2D,
  "Ganondorf Theme": 0x2E,
  "Lon Lon Ranch": 0x2F,
  "Goron City": 0x30,
  "Miniboss Battle": 0x38,
  "Temple of Time": 0x3A,
  "Kokiri Forest": 0x3C,
  "Lost Woods": 0x3E,
  "Spirit Temple": 0x3F,
  "Horse Race": 0x40,
  "Ingo Theme": 0x42,
  "Fairy Flying": 0x4A,
  "Deku Tree": 0x4B,
  "Windmill Hut": 0x4C,
  "Shooting Gallery": 0x4E,
  "Sheik Theme": 0x4F,
  "Zoras Domain": 0x50,
  "Shop": 0x55,
  "Chamber of the Sages": 0x56,
  "Ice Cavern": 0x58,
  "Kaepora Gaebora": 0x5A,
  "Shadow Temple": 0x5B,
  "Water Temple": 0x5C,
  "Gerudo Valley": 0x5F,
  "Potion Shop": 0x60,
  "Kotake and Koume": 0x61,
  "Castle Escape": 0x62,
  "Castle Underground": 0x63,
  "Ganondorf Battle": 0x64,
  "Ganon Battle": 0x65,
  "Fire Boss": 0x6B,
  "Mini-game": 0x6C,
};

const MM_MUSICS = {
  "Termina Field": 0x02,
  "Stone Tower Temple": 0x06,
  "Stone Tower Temple Inverted": 0x07,
  "Southern Swamp": 0x0c,
  "Aliens": 0x0d,
  "Mini Game": 0x0e,
  "Sharp's Curse": 0x0f,
  "Great Bay Coast": 0x10,
  "Ikana Valley": 0x11,
  "Court of the Deku King": 0x12,
  "Mountain Village": 0x13,
  "Pirates' Fortress": 0x14,
  "Clock Town Day 1": 0x15,
  "Clock Town Day 2": 0x16,
  "Clock Town Day 3": 0x17,
  "Boss Battle": 0x1b,
  "Woodfall Temple": 0x1c,
  "Stock Pot Inn": 0x1f,
  "Mini-game": 0x25,
  "Goron Race": 0x26,
  "Music Box House": 0x27,
  "Fairy's Fountain": 0x28,
  "Marine Research Laboratory": 0x2c,
  "Romani Ranch": 0x2f,
  "Goron Village": 0x30,
  "Mayor Dotour": 0x31,
  "Zora Hall": 0x36,
  "Mini Boss": 0x38,
  "Astral Observatory": 0x3a,
  "Bomber's Hideout": 0x3b,
  "Milk Bar Latte": 0x3c,
  "Woods of Mystery": 0x3e,
  "Gorman Race": 0x40,
  "Gorman Bros.": 0x42,
  "Kotake's Potion Shop": 0x43,
  "Store": 0x44,
  "Target Practice": 0x46,
  "Sword Training": 0x50,
  "Final Hours": 0x57,
  "Snowhead Temple": 0x65,
  "Great Bay Temple": 0x66,
  "Majora's Wrath": 0x69,
  "Majora's Incarnation": 0x6a,
  "Majora's Mask": 0x6b,
  "Ikana Castle": 0x6f,
  "Woodfall Clear": 0x78,
  "Snowhead Clear": 0x79,
};

type MusicFile = {
  type: 'bgm';
  seq: Buffer;
  bankId: number;
  name: string;
  game: Game;
}

type AudioSeq = {
  buffers: Buffer[];
  size: number;
}

class MusicInjector {
  private audioSeq: {
    oot: AudioSeq;
    mm: AudioSeq;
  };
  private musics: MusicFile[];

  constructor(
    private builder: RomBuilder,
    private random: Random,
    private musicZipData: Buffer,
  ) {
    this.audioSeq = {
      oot: {
        buffers: [],
        size: 0,
      },
      mm: {
        buffers: [],
        size: 0,
      },
    }
    this.musics = [];
  }

  private async loadMusicsOotrs(files: JSZip.JSZipObject[]) {
    for (const f of files) {
      /* Get the music zip */
      const musicZipBuffer = await f.async('nodebuffer');
      const musicZip = await JSZip.loadAsync(musicZipBuffer);

      /* Look for unsupported stuff */
      const badFiles = musicZip.file(/\.z?(bank|bankmeta|sound|)$/);
      if (badFiles.length > 0) {
        continue;
      }

      /* Find the meta file */
      const metaFile = musicZip.file(/\.meta$/);
      if (metaFile.length !== 1) {
        continue;
      }

      /* Find the seq file */
      const seqFiles = musicZip.file(/\.seq$/);
      if (seqFiles.length !== 1) {
        continue;
      }

      /* Parse the metadata */
      const metaRaw = await metaFile[0].async('text');
      const meta = metaRaw.split(/\r?\n/);
      const name = meta[0];
      const bankId = Number(meta[1]);
      const type = meta[2];
      const game = 'oot';
      if (type !== 'bgm') {
        continue;
      }

      /* Add the music */
      const seq = await seqFiles[0].async('nodebuffer');
      const music: MusicFile = { type, seq, bankId, name, game };
      this.musics.push(music);
    }
  }

  private async loadMusicsMmrs(files: JSZip.JSZipObject[]) {
    for (const f of files) {
      /* Get the music zip */
      const musicZipBuffer = await f.async('nodebuffer');
      const musicZip = await JSZip.loadAsync(musicZipBuffer);

      /* Look for unsupported stuff */
      const badFiles = musicZip.file(/\.z?(bank|bankmeta|sound|)$/);
      if (badFiles.length > 0) {
        continue;
      }

      /* Find the zseq file */
      const zseqFiles = musicZip.file(/\.zseq$/);
      if (zseqFiles.length !== 1) {
        continue;
      }

      /* Extract the bank ID from the zseq filename */
      let zseqFilename = zseqFiles[0].name;
      if (zseqFilename.includes('/')) {
        zseqFilename = zseqFilename.split('/').pop()!;
      }
      const bankIdRaw = zseqFilename.split('.')[0];
      const bankId = parseInt(bankIdRaw, 16);

      /* Add the music */
      const seq = await zseqFiles[0].async('nodebuffer');
      const game = 'mm';
      const type = 'bgm';
      const name = '???';
      const music: MusicFile = { type, seq, bankId, name, game };
      this.musics.push(music);
    }
  }

  private async loadMusics(data: Buffer) {
    const zip = await JSZip.loadAsync(data);
    await this.loadMusicsOotrs(zip.file(/\.ootrs$/));
    await this.loadMusicsMmrs(zip.file(/\.mmrs$/));
  }

  private appendAudioSeq(s: AudioSeq, seq: Buffer) {
    const size = s.size;
    s.size += seq.length;
    s.buffers.push(seq);
    if (s.size % 16) {
      const rem = 16 - (s.size % 16);
      const z = Buffer.alloc(rem);
      s.buffers.push(z);
      s.size += rem;
    }
    return size;
  }

  private async injectMusicOotOffsetId(slot: number, seqOffset: number, seqLength: number, bankId: number) {
    const fileCodeOot = this.builder.fileByNameRequired('oot/code');

    /* Patch the bank ID */
    const bankVrom = 0x102911 + 0xDD + (slot * 2);
    const bankIdBuf = Buffer.alloc(1);
    bankIdBuf.writeUInt8(bankId);
    bankIdBuf.copy(fileCodeOot.data, bankVrom);

    /* Add the pointer */
    const seqTablePtr = 0x102ae0 + 0x10 * slot;
    const seqTableData = toU32Buffer([seqOffset, seqLength]);
    seqTableData.copy(fileCodeOot.data, seqTablePtr);
  }

  private async injectMusicOot(slot: string, music: MusicFile) {
    /* Add the seq data in the rom */
    const offset = this.appendAudioSeq(this.audioSeq.oot, music.seq);

    /* Patch the music */
    const slotId = OOT_MUSICS[slot as keyof typeof OOT_MUSICS];
    await this.injectMusicOotOffsetId(slotId, offset, music.seq.length, music.bankId);

    /* Special cases */
    if (slot === 'Fairy Fountain') {
      await this.injectMusicOotOffsetId(0x57, offset, music.seq.length, music.bankId);
    }
  }

  private async injectMusicMmOffsetId(slot: number, seqOffset: number, seqLength: number, bankId: number) {
    const fileCodeMm = this.builder.fileByNameRequired('mm/code');

    /* Patch the bank ID */
    const bankVrom = 0x13b961 + 0x101 + (slot * 2);
    const bankIdBuf = Buffer.alloc(1);
    bankIdBuf.writeUInt8(bankId);
    bankIdBuf.copy(fileCodeMm.data, bankVrom);

    /* Add the pointer */
    const seqTablePtr = 0x13bb80 + 0x10 * slot;
    const seqTableData = toU32Buffer([seqOffset, seqLength]);
    seqTableData.copy(fileCodeMm.data, seqTablePtr);
  }

  private async injectMusicMm(slot: string, music: MusicFile) {
    /* Add the seq data in the rom */
    const offset = this.appendAudioSeq(this.audioSeq.mm, music.seq);

    /* Patch the music */
    const slotId = MM_MUSICS[slot as keyof typeof MM_MUSICS];
    await this.injectMusicMmOffsetId(slotId, offset, music.seq.length, music.bankId);

    /* Special cases */
    if (slot === 'Clock Town Day 1') {
      await this.injectMusicMmOffsetId(0x1d, offset, music.seq.length, music.bankId);
    }

    if (slot === 'Clock Town Day 2') {
      await this.injectMusicMmOffsetId(0x23, offset, music.seq.length, music.bankId);
    }
  }

  private async injectMusic(game: Game, slot: string, music: MusicFile) {
    if (game === 'oot') {
      return this.injectMusicOot(slot, music);
    } else {
      return this.injectMusicMm(slot, music);
    }
  }

  private replaceAudioseqOot() {
    /* Add the merged audioseq at a fixed fake vrom ADDR */
    const data = Buffer.concat(this.audioSeq.oot.buffers);
    const vrom = 0xe0000000;
    this.builder.addFile({ game: 'custom', type: 'uncompressed', vaddr: vrom, data });

    /* Disable battle music */
    const filePlayerActor = this.builder.fileByNameRequired('oot/ovl_player_actor');
    const z = Buffer.alloc(1);
    z.writeUInt8(0);
    z.copy(filePlayerActor.data, 0x1690f);
  }

  private replaceAudioseqMm() {
    /* Add the merged audioseq at a fixed fake vrom ADDR */
    const data = Buffer.concat(this.audioSeq.mm.buffers);
    const vrom = 0xe8000000;
    this.builder.addFile({ game: 'custom', type: 'uncompressed', vaddr: vrom, data });

    /* Disable battle music */
    const filePlayerActor = this.builder.fileByNameRequired('mm/ovl_player_actor');
    const z = Buffer.alloc(2);
    z.writeUInt16BE(0x1000);
    z.copy(filePlayerActor.data, 0x16818);
  }

  private async shuffleMusics(game: Game) {
    const musicsDefs = game === 'oot' ? OOT_MUSICS : MM_MUSICS;
    const musics = shuffle(this.random, this.musics.filter(m => m.game === game));
    const slots = shuffle(this.random, Object.keys(musicsDefs));

    for (;;) {
      if (musics.length === 0 || slots.length === 0) {
        break;
      }

      const music = musics.pop()!;
      const slot = slots.pop()!;

      await this.injectMusic(game, slot, music);
    }
  }

  async run() {
    /* Init the audio seq data */
    const audioseqOot = this.builder.fileByNameRequired('oot/Audioseq');
    const audioseqMm = this.builder.fileByNameRequired('mm/Audioseq');

    this.audioSeq.oot.size = audioseqOot.data.length;
    this.audioSeq.oot.buffers = [audioseqOot.data];
    this.audioSeq.mm.size = audioseqMm.data.length;
    this.audioSeq.mm.buffers = [audioseqMm.data];

    /* Extract the list of musics */
    await this.loadMusics(this.musicZipData);

    /* Shuffle musics */
    await this.shuffleMusics('oot');
    await this.shuffleMusics('mm');

    /* Inject the new audioseq */
    this.replaceAudioseqOot();
    this.replaceAudioseqMm();
  }
}

export async function randomizeMusic(builder: RomBuilder, random: Random, data: Buffer) {
  const injector = new MusicInjector(builder, random, data);
  await injector.run();
}
