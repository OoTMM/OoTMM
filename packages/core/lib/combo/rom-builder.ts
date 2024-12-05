import { compressFile } from './compress';
import { CONFIG, Game } from './config';
import { DmaData, DmaDataRecord } from './dma';
import { bufReadU32BE, bufWriteU32BE } from './util/buffer';

const rol = (v: number, b: number) => (((v << b) | (v >>> (32 - b))) & 0xffffffff) >>> 0;

function checksum(rom: Uint8Array) {
  const seed = 0xdf26f436;
  let t1 = seed;
  let t2 = seed;
  let t3 = seed;
  let t4 = seed;
  let t5 = seed;
  let t6 = seed;

  for (let i = 0; i < 0x100000 / 4; ++i) {
    const offset = 0x1000 + i * 4;
    const d = bufReadU32BE(rom, offset);
    if ((((t6 + d) & 0xffffffff) >>> 0) < t6) {
      t4 = ((t4 + 1) & 0xffffffff) >>> 0;
    }
    t6 = ((t6 + d) & 0xffffffff) >>> 0;
    t3 = ((t3 ^ d) & 0xffffffff) >>> 0;
    const r = rol(d, ((d & 0x1f) >>> 0));
    t5 = ((t5 + r) & 0xffffffff) >>> 0;
    if (t2 > d) {
      t2 = (t2 ^ r) >>> 0;
    } else {
      t2 = (t2 ^ t6 ^ d) >>> 0;
    }
    const offset2 = 0x750 + ((i * 4) & 0xff);
    const x = bufReadU32BE(rom, offset2);
    t1 = ((t1 + ((x ^ d) >>> 0)) & 0xffffffff) >>> 0;
  }

  return [(t6 ^ t4 ^ t3) >>> 0, (t5 ^ t2 ^ t1) >>> 0];
}

type RomFileRange = [number, number];

type RomFileVram = {
  oot?: RomFileRange;
  mm?: RomFileRange;
}

export type RomFile = {
  type: 'compressed' | 'uncompressed' | 'dummy';
  name?: string;
  game: 'oot' | 'mm' | 'custom' | 'raw';
  index?: number;
  injected: boolean;
  data: Uint8Array;
  paddr?: number;
  vaddr?: number;
  vsize?: number;
  vram?: { [k in Game]?: [number, number] };
  alias?: RomFile;
  dma?: DmaDataRecord;
};

type AddFileArgs = Omit<RomFile, 'injected'>;

export class RomBuilder {
  private files: RomFile[];
  private out: Uint8Array;
  private paddr: number;
  private vaddr: number;
  private dma: { oot: DmaData; mm: DmaData; };
  private extraDma: DmaDataRecord[];

  constructor() {
    this.files = [];
    this.out = new Uint8Array(64 * 1024 * 1024);
    this.paddr = 0;
    this.vaddr = 0xc0000000;
    this.dma = {
      oot: new DmaData(new Uint8Array(CONFIG.oot.dmaCount * 0x10)),
      mm: new DmaData(new Uint8Array(CONFIG.mm.dmaCount * 0x10)),
    };
    this.extraDma = [];
  }

  currentPaddr() {
    return this.paddr;
  }

  allFiles(): RomFile[] {
    return this.files;
  }

  private addPaddr(size: number) {
    const oldAddr = this.paddr;
    this.paddr += size;
    if (this.paddr > 0x3fff000) {
      throw new Error(`ROM too large`);
    }
    return oldAddr;
  }

  async inject(file: RomFile) {
    if (file.injected)
      return;

    /* Inject */
    let dma: DmaDataRecord;

    if (file.alias) {
      await this.inject(file.alias);
      file.paddr = file.alias.paddr;
      const originalDma = file.alias.dma!;
      dma = { ...originalDma };
      const vaddr = file.vaddr || 0;
      const vsize = file.alias.dma!.virtEnd - file.alias.dma!.virtStart;
      dma.virtStart = vaddr;
      dma.virtEnd = vaddr + vsize;
    } else {
      switch (file.type) {
      case 'uncompressed': {
        if (file.paddr === undefined) {
          file.paddr = this.addPaddr((file.data.length + 0xf) & ~0xf);
        }
        const vaddr = file.vaddr || 0;
        if (!file.alias) {
          this.out.set(file.data, file.paddr);
        }
        dma = { physStart: file.paddr, physEnd: 0, virtStart: vaddr, virtEnd: vaddr + file.data.length };
        /* Make the original data alias the copy */
        file.data = this.out.subarray(file.paddr, file.paddr + file.data.length);
        break;
      }
      case 'compressed': {
        const uncompressedSize = file.data.length;
        const uncompressedSizeAligned = (uncompressedSize + 0xf) & ~0xf;
        const compressedData = await compressFile(file.data);
        const compressedSize = compressedData.length;
        const compressedSizeAligned = (compressedSize + 0xf) & ~0xf;
        if (file.paddr === undefined) {
          file.paddr = this.addPaddr(compressedSizeAligned);
        }
        const vaddr = file.vaddr || 0;
        this.out.set(compressedData, file.paddr);
        dma = { physStart: file.paddr, physEnd: file.paddr + compressedSizeAligned, virtStart: vaddr, virtEnd: vaddr + uncompressedSizeAligned };
        /* We cannot alias compressed data */
        file.data = new Uint8Array(0);
        break;
      }
      case 'dummy':
        const vaddr = file.vaddr || 0;
        const vsize = file.vsize || 0;
        dma = { physStart: 0xffffffff, physEnd: 0xffffffff, virtStart: vaddr, virtEnd: vaddr + vsize };
        break;
      }
    }

    file.dma = dma;

    /* Record the DMA */
    switch (file.game) {
    case 'oot':
      this.dma.oot.write(file.index!, dma);
      break;
    case 'mm':
      this.dma.mm.write(file.index!, dma);
      break;
    case 'custom':
      this.extraDma.push(dma);
      break;
    }

    /* Mark as injected */
    file.injected = true;
  }

  addFile(args: AddFileArgs) {
    const file: RomFile = { ...args, vram: { ...args.vram }, injected: false };
    if (file.game === 'custom' && file.vaddr === undefined) {
      const size = file.data.length;
      const sizeAligned = (size + 0xf) & ~0xf;
      file.vaddr = this.vaddr;
      this.vaddr += sizeAligned;
    }
    this.files.push(file);
    return file.vaddr;
  }

  removeFile(name: string) {
    const f = this.fileByNameRequired(name);
    if (!f.injected && f.type !== 'dummy') {
      f.type = 'dummy';
      f.vsize = f.data.length;
      f.data = new Uint8Array(0);
    }
  }

  alias(to: string, from: string) {
    const fileTo = this.fileByNameRequired(to);
    const fileFrom = this.fileByNameRequired(from);
    fileFrom.alias = fileTo;
    fileFrom.data = fileTo.data;
  }

  fileByIndex(game: Game, index: number): RomFile | null {
    for (const file of this.files) {
      if (file.game === game && file.index === index)
        return file;
    }
    return null;
  }

  fileByName(name: string): RomFile | null {
    for (const file of this.files) {
      if (file.name === name)
        return file;
    }
    return null;
  }

  fileByNameRequired(name: string): RomFile {
    const file = this.fileByName(name);
    if (!file)
      throw new Error(`File not found: ${name}`);
    return file;
  }

  fileByVRAM(game: 'oot' | 'mm', vram: number): RomFile | null {
    for (const file of this.files) {
      const allVram = file.vram;
      if (!allVram)
        continue;
      const gameVram = allVram[game];
      if (!gameVram)
        continue;
      if (vram >= gameVram[0] && vram < gameVram[1])
        return file;
    }
    return null;
  }

  private fixChecksum() {
    const [c1, c2] = checksum(this.out);
    bufWriteU32BE(this.out, 0x10, c1);
    bufWriteU32BE(this.out, 0x14, c2);
  }

  async run() {
    /* Inject all files */
    for (const file of this.files) {
      await this.inject(file);
    }

    /* Write DMA */
    const fileDmaOot = this.fileByName('oot/dmadata')!;
    const fileDmaMm = this.fileByName('mm/dmadata')!;
    this.out.set(this.dma.oot.data(), fileDmaOot.paddr!);
    this.out.set(this.dma.mm.data(), fileDmaMm.paddr!);


    /* Build extra DMA table */
    const extraDmaBuffer = new Uint8Array(this.extraDma.length * 0x10);
    const extraDma = new DmaData(extraDmaBuffer);
    for (let i = 0; i < this.extraDma.length; ++i) {
      extraDma.write(i, this.extraDma[i]);
    }

    /* Write extra DMA */
    const extraDmaPaddr = this.paddr;
    this.out.set(extraDmaBuffer, extraDmaPaddr);

    /* Build meta */
    const meta = new Uint8Array(0x1000);
    const payloadOot = this.fileByName('oot/payload')!;
    const payloadMm = this.fileByName('mm/payload')!;
    bufWriteU32BE(meta, 0x00, extraDmaPaddr);
    bufWriteU32BE(meta, 0x04, this.extraDma.length);
    bufWriteU32BE(meta, 0x08, payloadOot.paddr!);
    bufWriteU32BE(meta, 0x0c, payloadOot.data.length);
    bufWriteU32BE(meta, 0x10, payloadMm.paddr!);
    bufWriteU32BE(meta, 0x14, payloadMm.data.length);
    this.out.set(meta, this.out.length - meta.length);

    /* Patch rom header */
    this.out.set(new TextEncoder().encode('OOT+MM COMBO       '), 0x20);
    this.out.set(new TextEncoder().encode('NEDEP'), 0x3b);

    /* Fix checksum */
    this.fixChecksum();

    return { rom: this.out, size: this.paddr };
  }
};
