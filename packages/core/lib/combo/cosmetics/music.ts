import JSZip from 'jszip';

import { Random, shuffle } from '../random';
import { toU32Buffer } from '../util';
import { Game } from '../config';
import { RomBuilder } from '../rom-builder';

const BANKS_OOT_TO_MM: {[k: number]: number} = {
  0x03: 0x03,
  0x05: 0x04,
  0x08: 0x05,
  0x09: 0x06,
  0x0D: 0x07,
  0x11: 0x08,
  0x12: 0x09,
  0x14: 0x0A,
  0x15: 0x0B,
  0x16: 0x0C,
  0x1C: 0x0D,
  0x1D: 0x0E,
  0x23: 0x0F,
  0x24: 0x10,
};

const BANKS_MM_TO_OOT: {[k: number]: number} = {
  0x03: 0x03,
  0x04: 0x05,
  0x05: 0x08,
  0x06: 0x09,
  0x07: 0x0D,
  0x08: 0x11,
  0x09: 0x12,
  0x0A: 0x14,
  0x0B: 0x15,
  0x0C: 0x16,
  0x0D: 0x1C,
  0x0E: 0x1D,
  0x0F: 0x23,
  0x10: 0x24,
};

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
  bankIdOot: number | null;
  bankIdMm: number | null;
  name: string;
  games: Game[];
};

function saneName(name: string) {
  /* Force NFC */
  name = name.normalize('NFC');

  /* A diacritics */
  for (const m of ['á', 'à', 'â', 'ä']) {
    name = name.replace(m, 'a');
  }
  for (const m of ['Á', 'À', 'Â', 'Ä']) {
    name = name.replace(m, 'A');
  }

  /* E diacritics */
  for (const m of ['é', 'è', 'ê', 'ë']) {
    name = name.replace(m, 'e');
  }
  for (const m of ['É', 'È', 'Ê', 'Ë']) {
    name = name.replace(m, 'E');
  }

  /* I diacritics */
  for (const m of ['í', 'ì', 'î', 'ï']) {
    name = name.replace(m, 'i');
  }
  for (const m of ['Í', 'Ì', 'Î', 'Ï']) {
    name = name.replace(m, 'I');
  }

  /* O diacritics */
  for (const m of ['ó', 'ò', 'ô', 'ö']) {
    name = name.replace(m, 'o');
  }
  for (const m of ['Ó', 'Ò', 'Ô', 'Ö']) {
    name = name.replace(m, 'O');
  }

  /* U diacritics */
  for (const m of ['ú', 'ù', 'û', 'ü']) {
    name = name.replace(m, 'u');
  }
  for (const m of ['Ú', 'Ù', 'Û', 'Ü']) {
    name = name.replace(m, 'U');
  }

  /* Y diacritics */
  for (const m of ['ý', 'ÿ']) {
    name = name.replace(m, 'y');
  }
  for (const m of ['Ý', 'Ÿ']) {
    name = name.replace(m, 'Y');
  }

  /* C diacritics */
  for (const m of ['ç']) {
    name = name.replace(m, 'c');
  }
  for (const m of ['Ç']) {
    name = name.replace(m, 'C');
  }

  /* N diacritics */
  for (const m of ['ñ']) {
    name = name.replace(m, 'n');
  }
  for (const m of ['Ñ']) {
    name = name.replace(m, 'N');
  }

  /* AE */
  name = name.replace('æ', 'ae');
  name = name.replace('Æ', 'AE');

  /* OE */
  name = name.replace('œ', 'oe');
  name = name.replace('Œ', 'OE');

  /* Remove every other non-ascii */
  name = name.replace(/[^ -~]/g, '');

  return name;
}

class MusicInjector {
  private musics: MusicFile[];
  private namesBuffer: Buffer;

  constructor(
    private builder: RomBuilder,
    private random: Random,
    private musicZipData: Buffer,
  ) {
    this.musics = [];
    this.namesBuffer = Buffer.alloc(256 * 2 * 48);
  }

  private registerName(seqId: number, name: string) {
    /* Cut name to 48 characters */
    name = name.slice(0, 48);

    /* Write to buffer */
    const offset = seqId * 48;
    this.namesBuffer.write(name, offset, 'utf-8');
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
      const name = saneName(meta[0]);
      const bankIdOot = Number(meta[1]);
      const type = meta[2];
      const games: Game[] = ['oot'];
      if (type !== 'bgm') {
        continue;
      }
      let bankIdMm: number | null = null;

      if (BANKS_OOT_TO_MM[bankIdOot] !== undefined) {
        bankIdMm = BANKS_OOT_TO_MM[bankIdOot];
        games.push('mm');
      }

      /* Add the music */
      const seq = await seqFiles[0].async('nodebuffer');
      const music: MusicFile = { type, seq, bankIdOot, bankIdMm, name, games };
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
      const bankIdMm = parseInt(bankIdRaw, 16);

      /* Add the music */
      const seq = await zseqFiles[0].async('nodebuffer');
      const games: Game[] = ['mm'];
      const type = 'bgm';
      const basename = f.name.split('/').pop()!;
      const name = saneName(basename.replace('.mmrs', ''));

      let bankIdOot: number | null = null;
      if (BANKS_MM_TO_OOT[bankIdMm] !== undefined) {
        bankIdOot = BANKS_MM_TO_OOT[bankIdMm];
        games.push('oot');
      }

      const music: MusicFile = { type, seq, bankIdOot, bankIdMm, name, games };
      this.musics.push(music);
    }
  }

  private async loadMusics(data: Buffer) {
    const zip = await JSZip.loadAsync(data);
    await this.loadMusicsOotrs(zip.file(/\.ootrs$/));
    await this.loadMusicsMmrs(zip.file(/\.mmrs$/));
  }

  private appendAudio(seq: Buffer) {
    const vrom = this.builder.addFile({ game: 'custom', type: 'uncompressed', data: seq })!;
    return vrom;
  }

  private async injectMusicOffsetId(game: Game, slot: number, vrom: number, seqLength: number, bankId: number, name: string) {
    const fileSeqTable = this.builder.fileByNameRequired(`${game}/seq_table`);
    const fileSeqBanks = this.builder.fileByNameRequired(`${game}/seq_banks`);

    /* Patch the bank ID */
    const bankIdBuf = Buffer.alloc(1);
    bankIdBuf.writeUInt8(bankId);
    bankIdBuf.copy(fileSeqBanks.data, slot);

    /* Add the pointer */
    const seqTableData = toU32Buffer([vrom, seqLength]);
    seqTableData.copy(fileSeqTable.data, slot * 0x10);

    /* Register the name */
    this.registerName(game === 'mm' ? slot + 256 : slot, name);
  }

  private async injectMusicOot(slot: string, music: MusicFile) {
    /* Add the seq data in the rom */
    const vrom = this.appendAudio(music.seq);

    /* Patch the music */
    const slotId = OOT_MUSICS[slot as keyof typeof OOT_MUSICS];
    await this.injectMusicOffsetId('oot', slotId, vrom, music.seq.length, music.bankIdOot!, music.name);

    /* Special cases */
    if (slot === 'Fairy Fountain') {
      await this.injectMusicOffsetId('oot', 0x57, vrom, music.seq.length, music.bankIdOot!, music.name);
    }
  }

  private async injectMusicMm(slot: string, music: MusicFile) {
    /* Add the seq data in the rom */
    const vrom = this.appendAudio(music.seq);

    /* Patch the music */
    const slotId = MM_MUSICS[slot as keyof typeof MM_MUSICS];
    await this.injectMusicOffsetId('mm', slotId, vrom, music.seq.length, music.bankIdMm!, music.name);

    /* Special cases */
    if (slot === 'Clock Town Day 1') {
      await this.injectMusicOffsetId('mm', 0x1d, vrom, music.seq.length, music.bankIdMm!, music.name);
    }

    if (slot === 'Clock Town Day 2') {
      await this.injectMusicOffsetId('mm', 0x23, vrom, music.seq.length, music.bankIdMm!, music.name);
    }
  }

  private async injectMusic(game: Game, slot: string, music: MusicFile) {
    if (game === 'oot') {
      return this.injectMusicOot(slot, music);
    } else {
      return this.injectMusicMm(slot, music);
    }
  }

  private patchOot() {
    /* Disable battle music */
    const filePlayerActor = this.builder.fileByNameRequired('oot/ovl_player_actor');
    const z = Buffer.alloc(1);
    z.writeUInt8(0);
    z.copy(filePlayerActor.data, 0x1690f);
  }

  private patchMm() {
    /* Disable battle music */
    const filePlayerActor = this.builder.fileByNameRequired('mm/ovl_player_actor');
    const z = Buffer.alloc(2);
    z.writeUInt16BE(0x1000);
    z.copy(filePlayerActor.data, 0x16818);
  }

  private async shuffleMusics(game: Game) {
    const musicsDefs = game === 'oot' ? OOT_MUSICS : MM_MUSICS;
    const musics = shuffle(this.random, this.musics.filter(m => m.games.includes(game)));
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
    /* Extract the list of musics */
    await this.loadMusics(this.musicZipData);

    /* Shuffle musics */
    await this.shuffleMusics('oot');
    await this.shuffleMusics('mm');

    /* Run misc. patches */
    this.patchOot();
    this.patchMm();

    /* Inject the music names */
    this.builder.addFile({ game: 'custom', type: 'uncompressed', vaddr: 0xf1000000, data: this.namesBuffer });
  }
}

export async function randomizeMusic(builder: RomBuilder, random: Random, data: Buffer) {
  const injector = new MusicInjector(builder, random, data);
  await injector.run();
}
