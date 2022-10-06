import fs from 'fs/promises';
import path from 'path';

import { Game, PATH_BUILD, CONFIG } from "./config";
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
];

const DATA_VRAM = {
  oot: DATA_VRAM_OOT,
  mm: DATA_VRAM_MM,
};

const makeVRamTable = (game: Game, rom: Buffer) => {
  const vram = [...DATA_VRAM[game]];
  const meta = CONFIG[game];
  let addr = meta.actorsOvlAddr;
  for (let i = 0; i < meta.actorsOvlCount; ++i) {
    const base = rom.readUInt32BE(addr + 0x00);
    const vstart = rom.readUInt32BE(addr + 0x08);
    const vend = rom.readUInt32BE(addr + 0x0c);
    addr += 0x20;
    if (vstart > 0) {
      vram.push({ vstart, vend, base });
    }
  }
  return vram;
};

const virtualToPhysical = (vram: VRamEntry[], addr: number) => {
  for (const entry of vram) {
    if (addr >= entry.vstart && addr < entry.vend) {
      return entry.base + (addr - entry.vstart);
    }
  }
  throw new Error(`Virtual address ${addr.toString(16)} not found in vram table`);
};

const patchASM = (game: Game, rom: Buffer, vram: VRamEntry[], patch: Buffer, cursor: number) => {
  const addr = patch.readUInt32BE(cursor + 0x00);
  const size = patch.readUInt32BE(cursor + 0x04);
  cursor += 0x08;
  const data = patch.subarray(cursor, cursor + size);
  cursor += size;
  const paddr = virtualToPhysical(vram, addr);
  data.copy(rom, paddr);
  return cursor;
};

const patchLoadStore = (game: Game, rom: Buffer, vram: VRamEntry[], patch: Buffer, cursor: number) => {
  const bits = patch.readUInt16BE(cursor + 0x00);
  const unsigned = patch.readUInt16BE(cursor + 0x02);
  const count = patch.readUInt32BE(cursor + 0x04) / 4;
  cursor += 0x08;

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
    const addr = patch.readUInt32BE(cursor + i * 4);
    const paddr = virtualToPhysical(vram, addr);
    let instr = rom.readUInt32BE(paddr);
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
    rom.writeUInt32BE(instr, paddr);
  }

  cursor += count * 4;
  return cursor;
};

const patchRelHiLo = (game: Game, rom: Buffer, vram: VRamEntry[], patch: Buffer, cursor: number) => {
  const target = patch.readUInt32BE(cursor + 0x00);
  const count = patch.readUInt32BE(cursor + 0x04) / 4;
  cursor += 0x08;

  /* Computet the hi/lo pair */
  const target_lo = target & 0xffff;
  let target_hi = (target >>> 16) & 0xffff;
  if (target_lo & 0x8000) {
    target_hi += 1;
  }

  /* Patch the MIPS instructions */
  for (let i = 0; i < count; ++i) {
    const addr = patch.readUInt32BE(cursor + i * 4);
    const paddr = virtualToPhysical(vram, addr);
    let instr = rom.readUInt32BE(paddr);
    const op = (instr >>> 26) & 0x3f;
    let value = target_lo;
    if (op === 0x0f) {
      value = target_hi;
    }
    instr = ((instr & 0xffff0000) | value) >>> 0;
    rom.writeUInt32BE(instr, paddr);
  }

  cursor += count * 4;
  return cursor;
};

const patchRelJump = (game: Game, rom: Buffer, vram: VRamEntry[], patch: Buffer, cursor: number) => {
  let target = patch.readUInt32BE(cursor + 0x00);
  const count = patch.readUInt32BE(cursor + 0x04) / 4;
  cursor += 0x08;

  target = (target & 0x0fffffff) >>> 2;

  /* Patch the MIPS instructions */
  for (let i = 0; i < count; ++i) {
    const addr = patch.readUInt32BE(cursor + i * 4);
    const paddr = virtualToPhysical(vram, addr);
    let instr = rom.readUInt32BE(paddr);
    instr = ((instr & 0xfc000000) | target) >>> 0;
    rom.writeUInt32BE(instr, paddr);
  }

  cursor += count * 4;
  return cursor;
};

const patchWrite32 = (game: Game, rom: Buffer, vram: VRamEntry[], patch: Buffer, cursor: number) => {
  const value = patch.readUInt32BE(cursor + 0x00);
  const count = patch.readUInt32BE(cursor + 0x04) / 4;
  cursor += 0x08;

  /* Patch the MIPS instructions */
  for (let i = 0; i < count; ++i) {
    const addr = patch.readUInt32BE(cursor + i * 4);
    const paddr = virtualToPhysical(vram, addr);
    rom.writeUInt32BE(value, paddr);
  }

  cursor += count * 4;
  return cursor;
};

export const patchGame = async (opts: Options, game: Game) => {
  console.log("Patching " + game + "...");
  const rom = await fs.readFile(path.resolve(PATH_BUILD, 'roms', `${game}_decompressed.z64`));
  const patch = await fs.readFile(path.resolve(PATH_BUILD, opts.debug ? 'Debug' : 'Release', `${game}_patch.bin`));
  const vram = makeVRamTable(game, rom);
  let cursor = 0;
  for (;;) {
    /* Align on a 4-byte boundary */
    cursor = (cursor + 3) & ~3;
    if (cursor >= patch.length) {
      break;
    }

    /* Read the patch type */
    const type = patch.readUInt32BE(cursor);
    cursor += 4;

    switch (type) {
    case 0x01:
      cursor = patchASM(game, rom, vram, patch, cursor);
      break;
    case 0x02:
      cursor = patchLoadStore(game, rom, vram, patch, cursor);
      break;
    case 0x03:
      cursor = patchRelHiLo(game, rom, vram, patch, cursor);
      break;
    case 0x04:
      cursor = patchRelJump(game, rom, vram, patch, cursor);
      break;
    case 0x05:
      cursor = patchWrite32(game, rom, vram, patch, cursor);
      break;
    default:
      throw new Error("Invalid patch type: " + type);
    }
  }
  return rom;
};
