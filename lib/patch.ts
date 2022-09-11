import fs from 'fs/promises';
import path from 'path';

import { Game, PATH_BUILD, CONFIG } from "./config";

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
    addr += 0x10;
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

export const patchGame = async (game: Game) => {
  console.log("Patching " + game + "...");
  const rom = await fs.readFile(path.resolve(PATH_BUILD, 'roms', `${game}_decompressed.z64`));
  const patch = await fs.readFile(path.resolve(PATH_BUILD, `${game}_patch.bin`));
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
      break;
    case 0x03:
      break;
    case 0x04:
      break;
    case 0x05:
      break;
    default:
      throw new Error("Invalid patch type: " + type);
    }
  }
};
