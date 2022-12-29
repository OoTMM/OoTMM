import { Buffer } from 'buffer';

import { Game, CONFIG } from "./config";
import { Options } from './options';

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

class Patcher {
  private game: Game;
  private opts: Options;
  private rom: Buffer;
  private patches: Buffer;
  private vram: VRamEntry[];
  private objectTable: number[];
  private cursor: number;

  constructor(game: Game, rom: Buffer, patches: Buffer, opts: Options) {
    this.game = game;
    this.rom = rom;
    this.patches = patches;
    this.opts = opts;
    this.vram = this.makeVramTable();
    this.objectTable = this.makeObjectTable();
    this.cursor = 0;
  }

  private makeVramTable() {
    const vram = [...DATA_VRAM[this.game]];
    const meta = CONFIG[this.game];
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
    return vram;
  }

  private makeObjectTable() {
    const objectTable: number[] = [];
    const meta = CONFIG[this.game];
    let addr = meta.objectTableAddr;
    for (let i = 0; i < meta.objectCount; ++i) {
      const vstart = this.rom.readUInt32BE(addr);
      addr += 0x08;
      objectTable.push(vstart);
    }
    return objectTable;
  }

  private virtualToPhysical(addr: number) {
    for (const entry of this.vram) {
      if (addr >= entry.vstart && addr < entry.vend) {
        return entry.base + (addr - entry.vstart);
      }
    }
    throw new Error(`Virtual address ${addr.toString(16)} not found in vram table`);
  }

  private objectToPhysical(objectId: number, offset: number) {
    if (objectId >= this.objectTable.length) {
      throw new Error(`Object ID ${objectId} out of range`);
    }
    const vstart = this.objectTable[objectId];
    return vstart + offset;
  }

  patchASM(patch: Buffer) {
    const addr = patch.readUInt32BE(this.cursor + 0x00);
    const size = patch.readUInt32BE(this.cursor + 0x04);
    this.cursor += 0x08;
    const data = patch.subarray(this.cursor, this.cursor + size);
    this.cursor += size;
    const paddr = this.virtualToPhysical(addr);
    data.copy(this.rom, paddr);
  }

  patchLoadStore(patch: Buffer) {
    const bits = patch.readUInt16BE(this.cursor + 0x00);
    const unsigned = patch.readUInt16BE(this.cursor + 0x02);
    const count = patch.readUInt32BE(this.cursor + 0x04) / 4;
    this.cursor += 0x08;

    /* Compute the correct MIPS opcodes for the load/store */
    let op_load = 0;
    let op_store = 0;

    switch (bits) {
    case 8: op_load = 0x20; break;
    case 16: op_load = 0x21; break;
    case 32: op_load = 0x23; break;
    default: throw new Error(`Invalid bits ${bits}`);
    }
    op_store = op_load | 0x08;
    if (unsigned) {
      op_load |= 0x04;
    }

    /* Patch the load/store instructions */
    for (let i = 0; i < count; ++i) {
      const addr = patch.readUInt32BE(this.cursor + i * 4);
      const paddr = this.virtualToPhysical(addr);
      let instr = this.rom.readUInt32BE(paddr);
      let op = (instr >>> 26) & 0x3f;
      switch (op) {
      case 0x20:
      case 0x21:
      case 0x23:
      case 0x24:
      case 0x25:
      case 0x27:
        op = op_load;
        break;
      case 0x28:
      case 0x29:
      case 0x2b:
      case 0x2c:
      case 0x2d:
      case 0x2f:
        op = op_store;
        break;
      default:
        throw new Error(`Invalid load/store opcode ${op.toString(16)} at ${paddr.toString(16)}`);
      }
      instr = ((instr & 0x3ffffff) | (op << 26)) >>> 0;
      this.rom.writeUInt32BE(instr, paddr);
    }

    this.cursor += count * 4;
  }

  patchRelHiLo(patch: Buffer) {
    const target = patch.readUInt32BE(this.cursor + 0x00);
    const count = patch.readUInt32BE(this.cursor + 0x04) / 4;
    this.cursor += 0x08;

    /* Computet the hi/lo pair */
    const target_lo = target & 0xffff;
    let target_hi = (target >>> 16) & 0xffff;
    if (target_lo & 0x8000) {
      target_hi += 1;
    }

    /* Patch the MIPS instructions */
    for (let i = 0; i < count; ++i) {
      const addr = patch.readUInt32BE(this.cursor + i * 4);
      const paddr = this.virtualToPhysical(addr);
      let instr = this.rom.readUInt32BE(paddr);
      const op = (instr >>> 26) & 0x3f;
      let value = target_lo;
      if (op === 0x0f) {
        value = target_hi;
      }
      instr = ((instr & 0xffff0000) | value) >>> 0;
      this.rom.writeUInt32BE(instr, paddr);
    }

    this.cursor += count * 4;
  }

  patchRelJump(patch: Buffer) {
    let target = patch.readUInt32BE(this.cursor + 0x00);
    const count = patch.readUInt32BE(this.cursor + 0x04) / 4;
    this.cursor += 0x08;

    target = (target & 0x0fffffff) >>> 2;

    /* Patch the MIPS instructions */
    for (let i = 0; i < count; ++i) {
      const addr = patch.readUInt32BE(this.cursor + i * 4);
      const paddr = this.virtualToPhysical(addr);
      let instr = this.rom.readUInt32BE(paddr);
      instr = ((instr & 0xfc000000) | target) >>> 0;
      this.rom.writeUInt32BE(instr, paddr);
    }

    this.cursor += count * 4;
  }

  patchWrite32(patch: Buffer) {
    const value = patch.readUInt32BE(this.cursor + 0x00);
    const count = patch.readUInt32BE(this.cursor + 0x04) / 4;
    this.cursor += 0x08;

    /* Patch the MIPS instructions */
    for (let i = 0; i < count; ++i) {
      const addr = patch.readUInt32BE(this.cursor + i * 4);
      const paddr = this.virtualToPhysical(addr);
      this.rom.writeUInt32BE(value, paddr);
    }

    this.cursor += count * 4;
  }

  patchFunc = (patch: Buffer) => {
    const addr = patch.readUInt32BE(this.cursor + 0x00);
    const func = patch.readUInt32BE(this.cursor + 0x04);
    this.cursor += 0x08;

    const paddr = this.virtualToPhysical(addr);

    this.rom.writeUInt32BE((0x08000000 | (((func >>> 2) & 0x03ffffff) >>> 0)) >>> 0, paddr);
    this.rom.writeUInt32BE(0x0, paddr + 4);
  }

  patchObject(patch: Buffer) {
    const objectId = patch.readUInt32BE(this.cursor + 0x00);
    const offset = patch.readUInt32BE(this.cursor + 0x04);
    const size = patch.readUInt32BE(this.cursor + 0x08);
    this.cursor += 0x0c;

    const paddr = this.objectToPhysical(objectId, offset);
    const data = patch.subarray(this.cursor, this.cursor + size);
    this.cursor += size;
    data.copy(this.rom, paddr);
  }

  patchCall = (patch: Buffer) => {
    const addr = patch.readUInt32BE(this.cursor + 0x00);
    const func = patch.readUInt32BE(this.cursor + 0x04);
    this.cursor += 0x08;

    const paddr = this.virtualToPhysical(addr);

    this.rom.writeUInt32BE((0x0c000000 | (((func >>> 2) & 0x03ffffff) >>> 0)) >>> 0, paddr);
  }

  patchVROM(patch: Buffer) {
    const paddr = patch.readUInt32BE(this.cursor + 0x00);
    const size = patch.readUInt32BE(this.cursor + 0x04);
    this.cursor += 0x08;
    const data = patch.subarray(this.cursor, this.cursor + size);
    this.cursor += size;
    data.copy(this.rom, paddr);
  }

  async run() {
    this.cursor = 0;
    for (;;) {
      /* Align on a 8-byte boundary */
      this.cursor = (this.cursor + 7) & ~7;
      if (this.cursor >= this.patches.length) {
        break;
      }

      /* Read the patch type */
      const type = this.patches.readUInt32BE(this.cursor);
      this.cursor += 4;

      switch (type) {
      case 0x01:
        this.patchASM(this.patches);
        break;
      case 0x02:
        this.patchLoadStore(this.patches);
        break;
      case 0x03:
        this.patchRelHiLo(this.patches);
        break;
      case 0x04:
        this.patchRelJump(this.patches);
        break;
      case 0x05:
        this.patchWrite32(this.patches);
        break;
      case 0x06:
        this.patchFunc(this.patches);
        break;
      case 0x07:
        this.patchObject(this.patches);
        break;
      case 0x08:
        this.patchCall(this.patches);
        break;
      case 0x09:
        this.patchVROM(this.patches);
        break;
      default:
        throw new Error("Invalid patch type: " + type);
      }
    }
    return this.rom;
  }
}

export const patchGame = async (rom: Buffer, patches: Buffer, opts: Options, game: Game) => {
  const patcher = new Patcher(game, rom, patches, opts);
  return patcher.run();
};
