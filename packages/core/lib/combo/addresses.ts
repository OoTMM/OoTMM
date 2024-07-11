import { FILES } from '@ootmm/data';
import { CONFIG, Game } from './config';
import { DecompressedRoms } from './decompress';
import { DmaData } from './dma';

export type FileAddress = {
  name: string;
  addr: number;
  offset: number;
};

type Entry = {
  name: string;
  vstart: number;
  vend: number;
  base: number;
};

const DATA_VRAM_OOT: Entry[] = [
  { name: 'oot/boot', vstart: 0x80000460, vend: 0x80006830, base: 0x1060 },
  { name: 'oot/code', vstart: 0x800110a0, vend: 0x80114dd0, base: 0xa87000 },
  { name: 'oot/ovl_title', vstart: 0x80800000, vend: 0x808009c0, base: 0xb9da40 },
  { name: 'oot/ovl_file_choose', vstart: 0x80803880, vend: 0x8081379f, base: 0xba12c0 },
  { name: 'oot/ovl_player_actor', vstart: 0x808301c0, vend: 0x80856720, base: 0xbcdb70 },
  { name: 'oot/ovl_kaleido_scope', vstart: 0x808137c0, vend: 0x8083014f, base: 0xbb11e0 },
];

const DATA_VRAM_MM: Entry[] = [
  { name: 'mm/boot', vstart: 0x80080060, vend: 0x8009b110, base: 0x1060 },
  { name: 'mm/code', vstart: 0x800a5ac0, vend: 0x801e3fa0, base: 0xb3c000 },
  { name: 'mm/ovl_title',vstart: 0x80800000, vend: 0x80800910, base: 0xc7a4e0 },
  { name: 'mm/ovl_opening',vstart: 0x80803df0, vend: 0x80804010, base: 0xc7e2d0 },
  { name: 'mm/ovl_file_choose',vstart: 0x80804010, vend: 0x80814e80, base: 0xc7e4f0 },
  { name: 'mm/ovl_player_actor', vstart: 0x8082da90, vend: 0x80862af0, base: 0xca7f00 },
  { name: 'mm/ovl_kaleido_scope',vstart: 0x808160a0, vend: 0x8082da50, base: 0xc90550 },
];

const DATA_VRAM = {
  oot: DATA_VRAM_OOT,
  mm: DATA_VRAM_MM,
};

export class Addresses {
  private vram: Entry[];
  private vrom: Entry[];
  private dma: DmaData;

  constructor(
    private game: Game,
    private rom: Buffer,
  ) {
    const gc = CONFIG[game];
    const dmaData = rom.subarray(gc.dmaAddr, gc.dmaAddr + gc.dmaCount * 0x10);
    this.dma = new DmaData(dmaData);
    this.vram = this.makeVramTable();
    this.vrom = this.makeVromTable();
  }

  private filenameFromVaddr(vaddr: number): string {
    for (let i = 0; i < this.dma.count(); ++i) {
      const entry = this.dma.read(i);
      if (vaddr >= entry.virtStart && vaddr < entry.virtEnd) {
        return FILES[this.game][i];
      }
    }

    throw new Error(`Virtual address ${vaddr.toString(16)} not found in dma table`);
  }

  private makeVramTable() {
    const vram = [...DATA_VRAM[this.game]];
    const meta = CONFIG[this.game];

    /* Resolve actors */
    let addr = meta.actorsOvlAddr;
    for (let i = 0; i < meta.actorsOvlCount; ++i) {
      const base = this.rom.readUInt32BE(addr + 0x00);
      const vstart = this.rom.readUInt32BE(addr + 0x08);
      const vend = this.rom.readUInt32BE(addr + 0x0c);
      addr += 0x20;
      if (vstart > 0) {
        const name = this.filenameFromVaddr(base);
        vram.push({ name: `${this.game}/${name}`, vstart, vend, base });
      }
    }

    /* Resolve effects */
    addr = meta.effectsOvlAddr;
    for (let i = 0; i < meta.effectsOvlCount; ++i) {
      const base = this.rom.readUInt32BE(addr + 0x00);
      const vstart = this.rom.readUInt32BE(addr + 0x08);
      const vend = this.rom.readUInt32BE(addr + 0x0c);
      addr += 0x1c;
      if (vstart > 0) {
        const name = this.filenameFromVaddr(base);
        vram.push({ name: `${this.game}/${name}`, vstart, vend, base });
      }
    }

    return vram;
  }

  makeVromTable() {
    const entries: Entry[] = [];

    for (let i = 0; i < this.dma.count(); ++i) {
      const dmaEntry = this.dma.read(i);
      if (dmaEntry.physEnd === 0xffffffff) {
        continue;
      }
      const filename = FILES[this.game][i];
      const vstart = dmaEntry.virtStart;
      const vend = dmaEntry.virtEnd;
      const base = dmaEntry.virtStart;

      entries.push({ name: `${this.game}/${filename}`, vstart, vend, base });
    }

    return entries;
  }

  private fileLookup(addr: number, table: Entry[]): FileAddress {
    for (const entry of table) {
      if (addr >= entry.vstart && addr < entry.vend) {
        const offset = addr - entry.vstart;
        return { name: entry.name, offset, addr: entry.base + offset };
      }
    }

    throw new Error(`Address ${addr.toString(16)} not found in vram/vrom table`);
  }

  fileFromRAM(addr: number) {
    return this.fileLookup(addr, this.vram);
  }

  fileFromROM(addr: number) {
    return this.fileLookup(addr, this.vrom);
  }
};

export type GameAddresses = {
  oot: Addresses;
  mm: Addresses;
};

export function makeAddresses(roms: DecompressedRoms): GameAddresses {
  return {
    oot: new Addresses("oot", roms.oot.rom),
    mm: new Addresses("mm", roms.mm.rom),
  };
}
