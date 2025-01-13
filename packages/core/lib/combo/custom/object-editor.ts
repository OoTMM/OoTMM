import { concatUint8Arrays } from "uint8array-extras";
import { bufReadU16BE, bufReadU32BE, bufReadU8, bufWriteU32BE } from "../util/buffer";

export type ObjectEditorOut = {
  data: Uint8Array;
  offsets: number[];
};

export class ObjectEditor {
  private segments: Map<number, Uint8Array> = new Map();
  private seen: Map<number, number> = new Map();
  private out: Uint8Array[] = [];
  private outSize: number = 0;
  private outOffsets: number[] = [];

  constructor(
    private readonly segOut: number,
  ) {
  }

  submitOut(listAddr: number) {
    this.outOffsets.push(listAddr);
  }

  submitListAddr(addr: number) {
    this.submitOut(this.processListAddr(addr));
  }

  submitList(aList: Uint8Array) {
    this.submitOut(this.processList(aList));
  }

  processListAddr(addr: number) {
    const seenAddr = this.seen.get(addr);
    if (seenAddr !== undefined) {
      return seenAddr;
    }
    const data = this.listData(addr);
    if (!data) {
      return addr;
    }
    const newAddr = this.processList(data);
    this.seen.set(addr, newAddr);
    return newAddr;
  }

  processList(aList: Uint8Array) {
    const size = aList.length;
    const list = new Uint8Array(aList);

    for (let i = 0; i < size; i += 8) {
      const data = bufReadU32BE(list, i);
      const op = (data >>> 24);
      if (op === 0x01) {
        /* Vertices */
        const count = (data >> 12) & 0xff;
        const addr = bufReadU32BE(list, i + 4);
        const newAddr = this.copy(addr, count * 0x10);
        bufWriteU32BE(list, i + 4, newAddr);
      } else if (op === 0xfd) {
        /* Texture or palette */
        const data2 = bufReadU32BE(list, i + 8 * 1);
        const op2 = data2 >>> 24;
        if (op2 === 0xf5) {
          /* Texture */
          const fmt = (bufReadU32BE(list, i + 0x28) >>> 19) & 3;
          let bpp = 0;
          switch (fmt) {
          case 0:
            bpp = 4;
            break;
          case 1:
            bpp = 8;
            break;
          case 2:
            bpp = 16;
            break;
          case 3:
            bpp = 32;
            break;
          default:
            throw new Error(`Unknown texture format 0x${fmt.toString(16)}`);
          }
          const data3 = bufReadU32BE(list, i + 8 * 6 + 4);
          const w = (((data3 >>> 12) & 0xfff) / 4) + 1;
          const h = (((data3 >>>  0) & 0xfff) / 4) + 1;
          const addr = bufReadU32BE(list, i + 4);
          const newAddr = this.copy(addr, (w * h * bpp) / 8);
          bufWriteU32BE(list, i + 4, newAddr);
        } else if (op2 === 0xe8) {
          /* Palette */
          const count = ((bufReadU32BE(list, i + 0x24) & 0xffffff) >> 14) + 1;
          const addr = bufReadU32BE(list, i + 4);
          const newAddr = this.copy(addr, count * 2);
          bufWriteU32BE(list, i + 4, newAddr);
        }
      } else if (op === 0xda) {
        const addr = bufReadU32BE(list, i + 4);
        const newAddr = this.copy(addr, 0x40);
        bufWriteU32BE(list, i + 4, newAddr);
      } else if (op === 0xde) {
        /* List */
        const addr = bufReadU32BE(list, i + 4);
        const newAddr = this.processListAddr(addr);
        bufWriteU32BE(list, i + 4, newAddr);
      }
    }

    const outAddr = this.outSize | (this.segOut << 24);
    this.outSize += size;
    this.out.push(list);
    if (this.outSize % 16) {
      const extraSize = 16 - (this.outSize % 16);
      const extraBuf = new Uint8Array(extraSize);
      this.outSize += extraSize;
      this.out.push(extraBuf);
    }

    return outAddr;
  }

  private emitData(data: Uint8Array) {
    const newAddr = this.outSize | (this.segOut << 24);
    this.outSize += data.length;
    this.out.push(data);
    if (this.outSize % 16) {
      const extraSize = 16 - (this.outSize % 16);
      const extraBuf = new Uint8Array(extraSize);
      this.outSize += extraSize;
      this.out.push(extraBuf);
    }
    return newAddr;
  }

  processJointIndexAddr(addr: number, limbCount: number) {
    const data = this.segData(addr, 2 * 3 * (limbCount + 1));
    if (!data)
      return { addr, max: 0 };
    const newAddr = this.emitData(data);
    let max = 0;
    for (let i = 0; i < (limbCount + 1) * 3; ++i) {
      const index = bufReadU16BE(data, i * 2);
      if (index > max)
        max = index;
    }
    return { addr: newAddr, max };
  }

  processFrameDataAddr(addr: number, count: number) {
    const data = this.segData(addr, 2 * count);
    if (!data)
      return addr;
    return this.emitData(data);
  }

  processAnimationHeaderAddr(addr: number, limbCount: number) {
    const data = this.segData(addr, 0x10);
    if (!data)
      return addr;

    const newData = new Uint8Array(data);
    const frameCount = bufReadU16BE(data, 0x00);
    const newJointData = this.processJointIndexAddr(bufReadU32BE(data, 0x08), limbCount);
    const newFrameData = this.processFrameDataAddr(bufReadU32BE(data, 0x04), newJointData.max + frameCount);

    bufWriteU32BE(newData, 0x04, newFrameData);
    bufWriteU32BE(newData, 0x08, newJointData.addr);

    const newAddr = this.emitData(newData);
    return newAddr;
  }

  processSkeletonStandardLimbAddr(addr: number) {
    const data = this.segData(addr, 0x0c);
    if (!data)
      return addr;
    const newData = new Uint8Array(data);
    const newList = this.processListAddr(bufReadU32BE(data, 0x08));
    bufWriteU32BE(newData, 0x08, newList);
    return this.emitData(newData);
  }

  processSkeletonLimbsAddr(addr: number, count: number) {
    const data = this.segData(addr, 4 * count);
    if (!data)
      return addr;
    const newData = new Uint8Array(data);
    for (let i = 0; i < count; ++i) {
      const newAddr = this.processSkeletonStandardLimbAddr(bufReadU32BE(data, i * 4));
      bufWriteU32BE(newData, i * 4, newAddr);
    }
    return this.emitData(newData);
  }

  processFlexSkeletonHeaderAddr(addr: number) {
    const data = this.segData(addr, 0xc);
    if (!data)
      return addr;
    const newData = new Uint8Array(data);
    const newLimbs = this.processSkeletonLimbsAddr(bufReadU32BE(data, 0), bufReadU8(data, 0x08));
    bufWriteU32BE(newData, 0, newLimbs);
    return this.emitData(newData);
  }

  loadSegment(num: number, data: Uint8Array | null) {
    if (data === null) {
      this.segments.delete(num);
    } else {
      this.segments.set(num, data);
    }
  }

  segData(addr: number, size?: number) {
    const seg = (addr >>> 24);
    addr &= 0xffffff;
    const data = this.segments.get(seg);
    if (!data) {
      return null;
    }
    if (size === undefined) {
      return data.subarray(addr);
    }
    return data.subarray(addr, addr + size);
  }

  listSize(addr: number) {
    const d = this.segData(addr);
    if (!d) {
      return 0;
    }

    let i = 0;
    for (;;) {
      const data = bufReadU32BE(d, i);
      const op = (data >>> 24);
      const op2 = (data >>> 16) & 0xff;
      i += 8;
      if (op === 0xdf || (op == 0xde && op2 == 0x01)) {
        return i;
      }
    }
  }

  listData(addr: number) {
    const size = this.listSize(addr);
    if (!size) {
      return null;
    }
    return this.segData(addr, size);
  }

  stripList(list: Uint8Array, from: number, to: number) {
    const size = list.length;
    const out = new Uint8Array(size - (to - from));
    out.set(list.subarray(0, from));
    out.set(list.subarray(to), from);
    return out;
  }

  combineListsAddrs(lists: number[]) {
    const newList = new Uint8Array(lists.length * 8);
    for (let i = 0; i < lists.length; i++) {
      const op = 0xde;
      const op2 = (i === lists.length - 1) ? 0x01 : 0x00;
      bufWriteU32BE(newList, i * 8, (((op << 24) >>> 0) | (op2 << 16)) >>> 0);
      bufWriteU32BE(newList, i * 8 + 4, lists[i]);
    }
    return newList;
  }

  build(): ObjectEditorOut {
    return {
      data: concatUint8Arrays(this.out),
      offsets: this.outOffsets,
    };
  }

  private copy(addr: number, size: number): number {
    const sub = this.segData(addr, size);
    if (!sub) {
      return addr;
    }
    addr &= 0xffffff;
    const seenAddr = this.seen.get(addr);
    if (seenAddr !== undefined) {
      return seenAddr;
    }
    const newAddr = this.outSize | (this.segOut << 24);
    this.outSize += sub.length;
    this.out.push(sub);
    this.seen.set(addr, newAddr);
    return newAddr;
  }
};
