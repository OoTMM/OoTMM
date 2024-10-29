import { concatUint8Arrays } from 'uint8array-extras';
import { bufReadU32BE, bufWriteU32BE } from '../util/buffer';

export type SplitObject = {
  data: Uint8Array;
  offsets: number[];
};

class ObjectSplitter {
  private seen: Map<number, number> = new Map();
  private out: Uint8Array[] = [];
  private outSize: number = 0;
  private outOffsets: number[] = [];

  constructor(
    private readonly object: Uint8Array,
    private readonly segIn: number,
    private readonly segOut: number,
  ) {}

  splitList(offset: number) {
    const size = this.listSize(offset);
    const list = this.object.slice(offset, offset + size);
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
        const data2 = bufReadU32BE(list, i + 8);
        const op2 = data2 >>> 24;
        if (op2 === 0xf5) {
          /* Texture */
          const fmt = bufReadU32BE(list, i + 0x28) >>> 19 & 3;
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
      }
    }
    this.outOffsets.push(this.outSize | (this.segOut << 24));
    this.outSize += size;
    this.out.push(list);
    if (this.outSize % 16) {
      const extraSize = 16 - (this.outSize % 16);
      const extraBuf = new Uint8Array(extraSize);
      this.outSize += extraSize;
      this.out.push(extraBuf);
    }
  }

  makeObject(): SplitObject {
    return {
      data: concatUint8Arrays(this.out),
      offsets: this.outOffsets,
    };
  }

  private listSize(offset: number) {
    let i = offset;
    for (;;) {
      const data = bufReadU32BE(this.object, i);
      const op = (data >>> 24);
      i += 8;
      if (op === 0xdf) {
        return i - offset;
      }
    }
  }

  private copy(addr: number, size: number): number {
    const seg = (addr >>> 24);
    if (seg !== this.segIn) {
      return addr;
    }
    addr &= 0xffffff;
    const seenAddr = this.seen.get(addr);
    if (seenAddr !== undefined) {
      return seenAddr;
    }
    const newAddr = this.outSize | (this.segOut << 24);
    this.outSize += size;
    this.out.push(this.object.slice(addr, addr + size));
    this.seen.set(addr, newAddr);
    return newAddr;
  }
};

export const splitObject = (object: Uint8Array, offsets: number[], segIn: number, segOut: number) => {
  const splitter = new ObjectSplitter(object, segIn, segOut);
  for (const offset of offsets) {
    splitter.splitList(offset);
  }
  return splitter.makeObject();
};
