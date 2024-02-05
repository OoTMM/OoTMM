
export type SplitObject = {
  data: Buffer;
  offsets: number[];
};

class ObjectSplitter {
  private seen: Map<number, number> = new Map();
  private out: Buffer[] = [];
  private outSize: number = 0;
  private outOffsets: number[] = [];

  constructor(
    private readonly object: Buffer,
    private readonly segIn: number,
    private readonly segOut: number,
  ) {}

  splitList(offset: number) {
    const size = this.listSize(offset);
    const list = Buffer.from(this.object.subarray(offset, offset + size));
    for (let i = 0; i < size; i += 8) {
      const data = list.readUInt32BE(i);
      const op = (data >>> 24);
      if (op === 0x01) {
        /* Vertices */
        const count = (data >> 12) & 0xff;
        const addr = list.readUInt32BE(i + 4);
        const newAddr = this.copy(addr, count * 0x10);
        list.writeUInt32BE(newAddr, i + 4);
      } else if (op === 0xfd) {
        /* Texture or palette */
        const data2 = list.readUInt32BE(i + 8 * 1);
        const op2 = data2 >>> 24;
        if (op2 === 0xf5) {
          /* Texture */
          const fmt = (data >>> 16) & 0xff;
          let bpp = 0;
          switch (fmt) {
          case 0x50:
          case 0x90:
            bpp = 4;
            break;
          case 0x10:
          case 0x70:
            bpp = 16;
            break;
          default:
            throw new Error(`Unknown texture format 0x${fmt.toString(16)}`);
          }
          const data3 = list.readUInt32BE(i + 8 * 6 + 4);
          const w = (((data3 >>> 12) & 0xfff) / 4) + 1;
          const h = (((data3 >>>  0) & 0xfff) / 4) + 1;
          const addr = list.readUInt32BE(i + 4);
          const newAddr = this.copy(addr, (w * h * bpp) / 8);
          list.writeUInt32BE(newAddr, i + 4);
        } else if (op2 === 0xe8) {
          /* Palette */
          const addr = list.readUInt32BE(i + 4);
          const newAddr = this.copy(addr, 32);
          list.writeUInt32BE(newAddr, i + 4);
        }
      }
    }
    this.outOffsets.push(this.outSize | (this.segOut << 24));
    this.outSize += size;
    this.out.push(list);
    if (this.outSize % 16) {
      const extraSize = 16 - (this.outSize % 16);
      const extraBuf = Buffer.alloc(extraSize);
      this.outSize += extraSize;
      this.out.push(extraBuf);
    }
  }

  makeObject(): SplitObject {
    return {
      data: Buffer.concat(this.out),
      offsets: this.outOffsets,
    };
  }

  private listSize(offset: number) {
    let i = offset;
    for (;;) {
      const data = this.object.readUInt32BE(i);
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
    this.out.push(Buffer.from(this.object.subarray(addr, addr + size)));
    this.seen.set(addr, newAddr);
    return newAddr;
  }
};

export const splitObject = (object: Buffer, offsets: number[], segIn: number, segOut: number) => {
  const splitter = new ObjectSplitter(object, segIn, segOut);
  for (const offset of offsets) {
    splitter.splitList(offset);
  }
  return splitter.makeObject();
};
