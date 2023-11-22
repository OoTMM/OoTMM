import { CONFIG, Game } from "./config";
import { DecompressedRoms } from "./decompress";

type VRamEntry = {
  vstart: number;
  vend: number;
  base: number;
};

const DATA_VRAM_OOT: VRamEntry[] = [
  { vstart: 0x80000460, vend: 0x80006830, base: 0x1060,   }, /* boot */
  { vstart: 0x800110a0, vend: 0x80114dd0, base: 0xa87000, }, /* code */
  { vstart: 0x80800000, vend: 0x808009c0, base: 0xb9da40, }, /* ovl_title */
  { vstart: 0x80803880, vend: 0x8081379f, base: 0xba12c0, }, /* ovl_file_choose */
  { vstart: 0x808301c0, vend: 0x80856720, base: 0xbcdb70, }, /* ovl_player_actor */
  { vstart: 0x808137c0, vend: 0x8083014f, base: 0xbb11e0, }, /* kaleido_scope */
];

const DATA_VRAM_MM: VRamEntry[] = [
  { vstart: 0x80080060, vend: 0x8009b110, base: 0x1060,   }, /* boot */
  { vstart: 0x800a5ac0, vend: 0x801e3fa0, base: 0xb3c000, }, /* code */
  { vstart: 0x80800000, vend: 0x80800910, base: 0xc7a4e0, }, /* ovl_title */
  { vstart: 0x80803df0, vend: 0x80804010, base: 0xc7e2d0, }, /* ovl_opening */
  { vstart: 0x80804010, vend: 0x80814e80, base: 0xc7e4f0, }, /* ovl_file_choose */
  { vstart: 0x8082da90, vend: 0x80862af0, base: 0xca7f00, }, /* ovl_player_actor */
  { vstart: 0x808160a0, vend: 0x8082da50, base: 0xc90550, }, /* kaleido_scope */
];

const DATA_VRAM = {
  oot: DATA_VRAM_OOT,
  mm: DATA_VRAM_MM,
};

export class Addresses {
  private vram: VRamEntry[];

  constructor(
    private game: Game,
    private rom: Buffer,
  ) {
    this.vram = this.makeVramTable();
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
        vram.push({ vstart, vend, base });
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
        vram.push({ vstart, vend, base });
      }
    }

    return vram;
  }

  virtualToPhysical(addr: number) {
    for (const entry of this.vram) {
      if (addr >= entry.vstart && addr < entry.vend) {
        return entry.base + (addr - entry.vstart);
      }
    }
    throw new Error(`Virtual address ${addr.toString(16)} not found in vram table`);
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
