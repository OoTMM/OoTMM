import { Game } from '@ootmm/core';

import { compressFile } from './compress';
import { CONFIG } from './config';
import { DmaData, DmaDataRecord } from './dma';
import { bufReadU32BE, bufWriteU32BE } from './util/buffer';

function u32(v: number) {
  return v >>> 0;
}

function checksum(rom: Uint8Array): [number, number] {
  const INI_PC = 0x80000400;
  const seed_ipl3 = 0x91;
  let s6: number;
  let a0: number;
  let a1: number;
  let at: number;
  let lo: number;
  let ra: number;
  let t0: number;
  let t1: number;
  let v0: number;
  let a3: number;
  let t2: number;
  let t3: number;
  let s0: number;
  let a2: number;
  let t4: number;
  let t5: number;
  let v1: number;
  let t6: number;
  let t7: number;
  let t8: number;
  let t9: number;

  s6 = seed_ipl3;
  a0 = INI_PC;

  a1 = s6;
  at = 0x5D588B65;
  lo = u32(a1 * at);

  s6 = 0xA0000200;
  ra = 0x100000;

  t0 = 0;
  t1 = a0;
  v0 = lo;
  v0 += 1;
  a3 = v0;
  t2 = v0;
  t3 = v0;
  s0 = v0;
  a2 = v0;
  t4 = v0;
  t5 = 0x20;

  do {
    v0 = bufReadU32BE(rom, (t1 - INI_PC + 0x1000));
    v1 = u32(a3 + v0);
    at = v1 < a3 ? 1 : 0;

    a1 = v1;
    if (at !== 0) {
      t2 = u32(t2 + 1);
    }

    v1 = v0 & 0x1f;
    t7 = u32(t5 - v1);
    t8 = v0 >>> t7;
    t6 = u32(v0 << v1);
    a0 = u32(t6 | t8);
    at = a2 < v0 ? 1 : 0;
    a3 = a1;
    t3 = u32(t3 ^ v0);

    s0 = u32(s0 + a0);
    if (at === 0) {
      a2 = u32(a2 ^ a0);
    } else {
      t9 = u32(a3 ^ v0);
      a2 = u32(a2 ^ t9);
    }

    t7 = bufReadU32BE(rom, (s6 - 0xA0000004 + 0x000514 + 0x40));
    t0 = u32(t0 + 4);
    s6 = u32(s6 + 4);
    t7 = u32(t7 ^ v0);
    t4 = u32(t4 + t7);
    t7 = 0xA00002FF;
    t1 = u32(t1 + 4);

    s6 = u32(s6 & t7);
  } while (t0 != ra);

  t6 = u32(a3 ^ t2);
  a3 = u32(t6 ^ t3);

  t8 = u32(s0 ^ a2);
  s0 = u32(t8 ^ t4);

  return [a3, s0];
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
