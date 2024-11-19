
import { Options } from '../options';
import { Game, CONFIG } from '../config';
import { Patchfile } from '../patch-build/patchfile';
import { Addresses, FileAddress, GameAddresses } from '../addresses';
import { PATCH_GROUP_VALUES, PatchGroup } from './group';
import { bufReadU16BE, bufReadU32BE, bufWriteU32BE } from '../util/buffer';

export class Patcher {
  private ranges: [number, number][];
  private files: Set<string>;
  private opts: Options;
  private game: Game;
  private rom: Uint8Array;
  private patches: Uint8Array;
  private addresses: Addresses;
  private objectTable: number[];
  private patchfile: Patchfile;
  private cursor: number;
  private patchGroups: Set<number>;
  private enabled: boolean;

  constructor(opts: Options, game: Game, rom: Uint8Array, patchGroups: PatchGroup[], addresses: GameAddresses, patches: Uint8Array, patchfile: Patchfile) {
    this.ranges = [];
    this.files = new Set;
    this.opts = opts;
    this.game = game;
    this.rom = rom;
    this.patchGroups = new Set(patchGroups.map(x => PATCH_GROUP_VALUES[x]));
    this.patches = patches;
    this.addresses = addresses[game];
    this.patchfile = patchfile;
    this.objectTable = this.makeObjectTable();
    this.cursor = 0;
    this.enabled = true;
  }

  private makeObjectTable() {
    const objectTable: number[] = [];
    const meta = CONFIG[this.game];
    let addr = meta.objectTableAddr;
    for (let i = 0; i < meta.objectCount; ++i) {
      const vstart = bufReadU32BE(this.rom, addr);
      addr += 0x08;
      objectTable.push(vstart);
    }
    return objectTable;
  }

  private objectToPhysical(objectId: number, offset: number) {
    if (objectId >= this.objectTable.length) {
      throw new Error(`Object ID ${objectId} out of range`);
    }
    const vstart = this.objectTable[objectId];
    return vstart + offset;
  }

  private rangeCheck(start: number, size: number) {
    if (process.env.NODE_ENV === 'production')
      return;

    /* Check for an overlap with an existing range, in any order */
    for (const [rstart, rsize] of this.ranges) {
      if ((start >= rstart && start < rstart + rsize) || (rstart >= start && rstart < start + size)) {
        throw new Error(`Overlapping range at VROM 0x${start.toString(16)} (${this.game})`);
      }
    }

    /* Add the range */
    this.ranges.push([start, size]);
  }

  private patch(fileAddr: FileAddress, data: Uint8Array) {
    if (this.enabled) {
      //this.rangeCheck(romAddr, data.length);
      this.patchfile.addPatch(fileAddr.name, fileAddr.offset, data);
    }
  }

  private fileFromRAM(addr: number) {
    const f = this.addresses.fileFromRAM(addr);
    this.files.add(f.name);
    return f;
  }

  private fileFromROM(addr: number) {
    const f = this.addresses.fileFromROM(addr);
    this.files.add(f.name);
    return f;
  }

  patchASM(patch: Uint8Array) {
    const addr = bufReadU32BE(patch, this.cursor + 0x00);
    const size = bufReadU32BE(patch, this.cursor + 0x04);
    this.cursor += 0x08;
    const data = patch.subarray(this.cursor, this.cursor + size);
    this.cursor += size;
    const fileAddr = this.fileFromRAM(addr);
    this.patch(fileAddr, data);
  }

  patchLoadStore(patch: Uint8Array) {
    const bits = bufReadU16BE(patch, this.cursor + 0x00);
    const unsigned = bufReadU16BE(patch, this.cursor + 0x02);
    const count = bufReadU32BE(patch, this.cursor + 0x04) / 4;
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
      const addr = bufReadU32BE(patch, this.cursor + i * 4);
      const fileAddr = this.fileFromRAM(addr);
      let instr = bufReadU32BE(this.rom, fileAddr.addr);
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
        throw new Error(`Invalid load/store opcode ${op.toString(16)} at ${fileAddr.addr.toString(16)}`);
      }
      instr = ((instr & 0x3ffffff) | (op << 26)) >>> 0;
      const instrBuffer = new Uint8Array(4);
      bufWriteU32BE(instrBuffer, 0, instr);
      this.patch(fileAddr, instrBuffer);
    }

    this.cursor += count * 4;
  }

  patchRelHiLo(patch: Uint8Array) {
    const target = bufReadU32BE(patch, this.cursor + 0x00);
    const count = bufReadU32BE(patch, this.cursor + 0x04) / 4;
    this.cursor += 0x08;

    /* Computet the hi/lo pair */
    const target_lo = target & 0xffff;
    let target_hi = (target >>> 16) & 0xffff;
    if (target_lo & 0x8000) {
      target_hi += 1;
    }

    /* Patch the MIPS instructions */
    for (let i = 0; i < count; ++i) {
      const addr = bufReadU32BE(patch, this.cursor + i * 4);
      const fileAddr = this.fileFromRAM(addr);
      let instr = bufReadU32BE(this.rom, fileAddr.addr);
      const op = (instr >>> 26) & 0x3f;
      let value = target_lo;
      if (op === 0x0f) {
        value = target_hi;
      }
      instr = ((instr & 0xffff0000) | value) >>> 0;
      const instrBuffer = new Uint8Array(4);
      bufWriteU32BE(instrBuffer, 0, instr);
      this.patch(fileAddr, instrBuffer);
    }

    this.cursor += count * 4;
  }

  patchRelJump(patch: Uint8Array) {
    let target = bufReadU32BE(patch, this.cursor + 0x00);
    const count = bufReadU32BE(patch, this.cursor + 0x04) / 4;
    this.cursor += 0x08;

    target = (target & 0x0fffffff) >>> 2;

    /* Patch the MIPS instructions */
    for (let i = 0; i < count; ++i) {
      const addr = bufReadU32BE(patch, this.cursor + i * 4);
      const fileAddr = this.fileFromRAM(addr);
      let instr = bufReadU32BE(this.rom, fileAddr.addr);
      instr = ((instr & 0xfc000000) | target) >>> 0;
      const instrBuffer = new Uint8Array(4);
      bufWriteU32BE(instrBuffer, 0, instr);
      this.patch(fileAddr, instrBuffer);
    }

    this.cursor += count * 4;
  }

  patchWrite32(patch: Uint8Array) {
    const value = bufReadU32BE(patch, this.cursor + 0x00);
    const valueBuffer = new Uint8Array(4);
    bufWriteU32BE(valueBuffer, 0, value);

    const count = bufReadU32BE(patch, this.cursor + 0x04) / 4;
    this.cursor += 0x08;

    /* Patch the MIPS instructions */
    for (let i = 0; i < count; ++i) {
      const addr = bufReadU32BE(patch, this.cursor + i * 4);
      const fileAddr = this.fileFromRAM(addr);
      this.patch(fileAddr, valueBuffer);
    }

    this.cursor += count * 4;
  }

  patchFunc = (patch: Uint8Array) => {
    const addr = bufReadU32BE(patch, this.cursor + 0x00);
    const func = bufReadU32BE(patch, this.cursor + 0x04);
    this.cursor += 0x08;

    const fileAddr = this.fileFromRAM(addr);

    const buffer = new Uint8Array(8);
    bufWriteU32BE(buffer, 0, (0x08000000 | (((func >>> 2) & 0x03ffffff) >>> 0)) >>> 0);
    bufWriteU32BE(buffer, 4, 0);
    this.patch(fileAddr, buffer);
  }

  patchObject(patch: Uint8Array) {
    const objectId = bufReadU32BE(patch, this.cursor + 0x00);
    const offset = bufReadU32BE(patch, this.cursor + 0x04);
    const size = bufReadU32BE(patch, this.cursor + 0x08);
    this.cursor += 0x0c;

    const paddr = this.objectToPhysical(objectId, offset);
    const fileAddr = this.fileFromROM(paddr);
    const data = patch.subarray(this.cursor, this.cursor + size);
    this.cursor += size;
    this.patch(fileAddr, data);
  }

  patchCall = (patch: Uint8Array) => {
    const addr = bufReadU32BE(patch, this.cursor + 0x00);
    const func = bufReadU32BE(patch, this.cursor + 0x04);
    this.cursor += 0x08;

    const fileAddr = this.fileFromRAM(addr);
    const instr = ((0x0c000000 | (((func >>> 2) & 0x03ffffff) >>> 0)) >>> 0);
    const instrBuffer = new Uint8Array(4);
    bufWriteU32BE(instrBuffer, 0, instr);
    this.patch(fileAddr, instrBuffer);
  }

  patchVROM(patch: Uint8Array) {
    const paddr = bufReadU32BE(patch, this.cursor + 0x00);
    const size = bufReadU32BE(patch, this.cursor + 0x04);
    this.cursor += 0x08;
    const data = patch.subarray(this.cursor, this.cursor + size);
    this.cursor += size;
    const fileAddr = this.fileFromROM(paddr);
    this.patch(fileAddr, data);
  }

  patchGroup(patch: Uint8Array) {
    const groupId = bufReadU32BE(patch, this.cursor + 0x00) >>> 0;
    this.cursor += 0x04;
    this.enabled = (groupId === 0 || this.patchGroups.has(groupId));
  }

  run() {
    this.cursor = 0;
    for (;;) {
      /* Align on a 8-byte boundary */
      this.cursor = (this.cursor + 7) & ~7;
      if (this.cursor >= this.patches.length) {
        break;
      }

      /* Read the patch type */
      const type = bufReadU32BE(this.patches, this.cursor) >>> 0;
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
      case 0xffffffff:
        this.patchGroup(this.patches);
        break;
      default:
        throw new Error("Invalid patch type: " + type);
      }
    }

    return { files: this.files };
  }
}
