import JSZip from 'jszip';

import { Random, shuffle } from '../random';
import { Patchfile } from '../patch-build/patchfile';
import { toU32Buffer } from '../util';
import { DecompressedRoms } from '../decompress';

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

class MusicInjector {
  private audioSeqSize: number;
  private audioSeqBuffers: Buffer[];

  constructor(
    private roms: DecompressedRoms,
    private random: Random,
    private patch: Patchfile,
    private musicZipData: Buffer,
  ) {
    this.audioSeqSize = 0;
    this.audioSeqBuffers = [];
  }

  private async injectMusicOot(slot: string, data: Buffer) {
    const zip = await JSZip.loadAsync(data);

    /* Look for unsupported stuff */
    const badFiles = zip.file(/\.z?(bank|bankmeta|sound|)$/);
    if (badFiles.length > 0) {
      throw new Error(`Unsupported music file`);
    }

    /* Find the meta file */
    const meta = await zip.file(/\.meta$/)[0].async('text');

    /* Extract the bank ID */
    const metaLines = meta.replaceAll('\r', '').split('\n');
    const bankId = Number(metaLines[1]);

    /* Patch the bank ID */
    const bankVrom = 0xB89911 + 0xDD + (OOT_MUSICS[slot as keyof typeof OOT_MUSICS] * 2);
    const bankIdBuf = Buffer.alloc(1);
    bankIdBuf.writeUInt8(bankId);
    this.patch.addDataPatch('oot', bankVrom, bankIdBuf);

    /* Find the seq file */
    const seqFiles = zip.file(/\.seq$/);
    if (seqFiles.length !== 1) {
      throw new Error(`Invalid music file`);
    }
    const seq = await seqFiles[0].async('nodebuffer');

    /* Add the seq data in the rom */
    const offset = this.audioSeqSize;
    this.audioSeqSize += seq.length;
    this.audioSeqBuffers.push(seq);
    if (this.audioSeqSize % 16) {
      const rem = 16 - (this.audioSeqSize % 16);
      const z = Buffer.alloc(rem);
      this.audioSeqBuffers.push(z);
      this.audioSeqSize += rem;
    }

    /* Add the pointer */
    const seqTablePtr = 0x00B89AE0 + 0x10 * OOT_MUSICS[slot as keyof typeof OOT_MUSICS];
    const seqTableData = toU32Buffer([offset, seq.length]);
    this.patch.addDataPatch('oot', seqTablePtr, seqTableData);

    /* DEBUG */
    const metaName = zip.file(/\.meta$/)[0].name;
  }

  private replaceAudioseqOot() {
    /* Add the merged audioseq at a fixed fake vrom ADDR */
    const data = Buffer.concat(this.audioSeqBuffers);
    const vrom = 0xe0000000;
    this.patch.addNewFile(vrom, data, false);
  }

  async run() {
    /* Init the audio seq data */
    this.audioSeqSize = OOT_AUDIOSEQ_SIZE;
    this.audioSeqBuffers = [this.roms.oot.rom.subarray(OOT_AUDIOSEQ_ADDR, OOT_AUDIOSEQ_ADDR + OOT_AUDIOSEQ_SIZE)];

    /* Extract the list of musics */
    const zip = await JSZip.loadAsync(this.musicZipData);

    /* Bind a custom music to every OoT track */
    const musics = shuffle(this.random, zip.file(/\.ootrs$/));
    const slots = shuffle(this.random, Object.keys(OOT_MUSICS));

    for (;;) {
      if (musics.length === 0 || slots.length === 0) {
        break;
      }

      const music = musics.pop()!;
      const slot = slots.pop()!;
      const musicBuffer = await music.async('nodebuffer');

      await this.injectMusicOot(slot, musicBuffer);
    }

    this.replaceAudioseqOot();
  }
}

export async function randomizeMusic(roms: DecompressedRoms, patch: Patchfile, random: Random, data: Buffer) {
  const injector = new MusicInjector(roms, random, patch, data);
  await injector.run();
}
