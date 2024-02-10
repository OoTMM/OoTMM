import { compressFile } from './compress';
import { CONFIG, Game } from './config';
import { DmaData, DmaDataRecord } from './dma';

const rol = (v: number, b: number) => (((v << b) | (v >>> (32 - b))) & 0xffffffff) >>> 0;

function checksum(rom: Buffer) {
  const seed = 0xdf26f436;
  let t1 = seed;
  let t2 = seed;
  let t3 = seed;
  let t4 = seed;
  let t5 = seed;
  let t6 = seed;

  for (let i = 0; i < 0x100000 / 4; ++i) {
    const offset = 0x1000 + i * 4;
    const d = rom.readUInt32BE(offset);
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
    const x = rom.readUInt32BE(offset2);
    t1 = ((t1 + ((x ^ d) >>> 0)) & 0xffffffff) >>> 0;
  }

  return [(t6 ^ t4 ^ t3) >>> 0, (t5 ^ t2 ^ t1) >>> 0];
}

export type RomFile = {
  name?: string;
  type: 'compressed' | 'uncompressed' | 'dummy';
  game: 'oot' | 'mm' | 'custom' | 'raw';
  index?: number;
  injected: boolean;
  data: Buffer;
  paddr?: number;
  vaddr?: number;
  alias?: RomFile;
  dma?: DmaDataRecord;
};

type AddFileArgs = Omit<RomFile, 'injected'>;

export class RomBuilder {
  private files: RomFile[];
  private out: Buffer;
  private paddr: number;
  private vaddr: number;
  private dma: { oot: DmaData; mm: DmaData; };
  private extraDma: DmaDataRecord[];

  constructor() {
    this.files = [];
    this.out = Buffer.alloc(64 * 1024 * 1024);
    this.paddr = 0;
    this.vaddr = 0xc0000000;
    this.dma = {
      oot: new DmaData(Buffer.alloc(CONFIG.oot.dmaCount * 0x10)),
      mm: new DmaData(Buffer.alloc(CONFIG.mm.dmaCount * 0x10)),
    };
    this.extraDma = [];
  }

  currentPaddr() {
    return this.paddr;
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
        file.data = Buffer.alloc(0);
        break;
      }
      case 'dummy':
        dma = { physStart: 0xffffffff, physEnd: 0xffffffff, virtStart: file.vaddr || 0, virtEnd: file.vaddr || 0 };
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

  addFile(args: AddFileArgs): void {
    const file: RomFile = { ...args, injected: false };
    if (file.game === 'custom' && file.vaddr === undefined) {
      const size = file.data.length;
      const sizeAligned = (size + 0xf) & ~0xf;
      file.vaddr = this.vaddr;
      this.vaddr += sizeAligned;
    }
    this.files.push(file);
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

  private fixChecksum() {
    const [c1, c2] = checksum(this.out);
    this.out.writeUInt32BE(c1, 0x10);
    this.out.writeUInt32BE(c2, 0x14);
  }

  async run() {
    /* Inject all files */
    for (const file of this.files) {
      await this.inject(file);
    }

    /* Write DMA */
    const fileDmaOot = this.fileByName('oot/dmadata')!;
    const fileDmaMm = this.fileByName('mm/dmadata')!;
    this.dma.oot.data().copy(this.out, fileDmaOot.paddr!);
    this.dma.mm.data().copy(this.out, fileDmaMm.paddr!);

    /* Build extra DMA table */
    const extraDmaBuffer = Buffer.alloc(this.extraDma.length * 0x10);
    const extraDma = new DmaData(extraDmaBuffer);
    for (let i = 0; i < this.extraDma.length; ++i) {
      extraDma.write(i, this.extraDma[i]);
    }

    /* Write extra DMA */
    const extraDmaPaddr = this.paddr;
    extraDmaBuffer.copy(this.out, extraDmaPaddr);

    /* Build meta */
    const meta = Buffer.alloc(0x1000);
    const payloadOot = this.fileByName('oot/payload')!;
    const payloadMm = this.fileByName('mm/payload')!;
    meta.writeUInt32BE(extraDmaPaddr, 0x00);
    meta.writeUInt32BE(this.extraDma.length, 0x04);
    meta.writeUInt32BE(payloadOot.paddr!, 0x08);
    meta.writeUInt32BE(payloadOot.data.length, 0x0c);
    meta.writeUInt32BE(payloadMm.paddr!, 0x10);
    meta.writeUInt32BE(payloadMm.data.length, 0x14);
    meta.copy(this.out, this.out.length - meta.length);

    /* Patch rom header */
    Buffer.from('OOT+MM COMBO       ').copy(this.out, 0x20);
    Buffer.from('NEDEP').copy(this.out, 0x3b);

    /* Fix checksum */
    this.fixChecksum();

    return { rom: this.out, size: this.paddr };
  }
};
