import JSZip from 'jszip';

import { Random, shuffle } from '../random';
import { Patchfile } from '../patch-build/patchfile';
import { toU32Buffer } from '../util';
import { DecompressedRoms } from '../decompress';
import { Game } from '../config';

const OOT_AUDIOSEQ_ADDR = 0x29de0;
const OOT_AUDIOSEQ_SIZE = 0x4f690;

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

type MusicFile = {
  type: 'bgm';
  seq: Buffer;
  bankId: number;
  name: string;
  game: Game;
}

class MusicInjector {
  private audioSeqSize: number;
  private audioSeqBuffers: Buffer[];
  private musics: MusicFile[];

  constructor(
    private roms: DecompressedRoms,
    private random: Random,
    private patch: Patchfile,
    private musicZipData: Buffer,
  ) {
    this.audioSeqSize = 0;
    this.audioSeqBuffers = [];
    this.musics = [];
  }

  private async loadMusics(data: Buffer) {
    const zip = await JSZip.loadAsync(data);
    const ootrs = zip.file(/\.ootrs$/);
    for (const f of ootrs) {
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
      const game = 'oot';
      const type = meta[2];
      if (type !== 'bgm') {
        continue;
      }

      /* Add the music */
      const seq = await seqFiles[0].async('nodebuffer');
      const music: MusicFile = { type, seq, bankId, name, game };
      this.musics.push(music);
    }
  }

  private async injectMusicOot(slot: string, music: MusicFile) {
    /* Patch the bank ID */
    const bankVrom = 0xB89911 + 0xDD + (OOT_MUSICS[slot as keyof typeof OOT_MUSICS] * 2);
    const bankIdBuf = Buffer.alloc(1);
    bankIdBuf.writeUInt8(music.bankId);
    this.patch.addDataPatch('oot', bankVrom, bankIdBuf);

    /* Add the seq data in the rom */
    const offset = this.audioSeqSize;
    this.audioSeqSize += music.seq.length;
    this.audioSeqBuffers.push(music.seq);
    if (this.audioSeqSize % 16) {
      const rem = 16 - (this.audioSeqSize % 16);
      const z = Buffer.alloc(rem);
      this.audioSeqBuffers.push(z);
      this.audioSeqSize += rem;
    }

    /* Add the pointer */
    const seqTablePtr = 0x00B89AE0 + 0x10 * OOT_MUSICS[slot as keyof typeof OOT_MUSICS];
    const seqTableData = toU32Buffer([offset, music.seq.length]);
    this.patch.addDataPatch('oot', seqTablePtr, seqTableData);
  }

  private replaceAudioseqOot() {
    /* Add the merged audioseq at a fixed fake vrom ADDR */
    const data = Buffer.concat(this.audioSeqBuffers);
    const vrom = 0xe0000000;
    this.patch.addNewFile(vrom, data, false);

    /* Disable battle music */
    const z = Buffer.alloc(1);
    z.writeUInt8(0);
    this.patch.addDataPatch('oot', 0xbe447F, z);
  }

  async run() {
    /* Init the audio seq data */
    this.audioSeqSize = OOT_AUDIOSEQ_SIZE;
    this.audioSeqBuffers = [this.roms.oot.rom.subarray(OOT_AUDIOSEQ_ADDR, OOT_AUDIOSEQ_ADDR + OOT_AUDIOSEQ_SIZE)];

    /* Extract the list of musics */
    await this.loadMusics(this.musicZipData);

    /* Bind a custom music to every OoT track */
    const musics = shuffle(this.random, this.musics);
    const slots = shuffle(this.random, Object.keys(OOT_MUSICS));

    for (;;) {
      if (musics.length === 0 || slots.length === 0) {
        break;
      }

      const music = musics.pop()!;
      const slot = slots.pop()!;

      await this.injectMusicOot(slot, music);
    }

    this.replaceAudioseqOot();
  }
}

export async function randomizeMusic(roms: DecompressedRoms, patch: Patchfile, random: Random, data: Buffer) {
  const injector = new MusicInjector(roms, random, patch, data);
  await injector.run();
}
