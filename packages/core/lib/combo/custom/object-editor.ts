export type ObjectEditorOut = {
  data: Buffer;
  offsets: number[];
};

export class ObjectEditor {
  private segments: Map<number, Buffer> = new Map();
  private seen: Map<number, number> = new Map();
  private out: Buffer[] = [];
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

  submitList(aList: Buffer) {
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

  processList(aList: Buffer) {
    const size = aList.length;
    const list = Buffer.from(aList);

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
          const fmt = (list.readUInt32BE(i + 0x28) >> 19) & 3;
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
          const data3 = list.readUInt32BE(i + 8 * 6 + 4);
          const w = (((data3 >>> 12) & 0xfff) / 4) + 1;
          const h = (((data3 >>>  0) & 0xfff) / 4) + 1;
          const addr = list.readUInt32BE(i + 4);
          const newAddr = this.copy(addr, (w * h * bpp) / 8);
          list.writeUInt32BE(newAddr, i + 4);
        } else if (op2 === 0xe8) {
          /* Palette */
          const count = ((list.readUInt32BE(i + 0x24) & 0xffffff) >> 14) + 1;
          const addr = list.readUInt32BE(i + 4);
          const newAddr = this.copy(addr, count * 2);
          list.writeUInt32BE(newAddr, i + 4);
        }
      } else if (op === 0xda) {
        const addr = list.readUInt32BE(i + 4);
        const newAddr = this.copy(addr, 0x40);
        list.writeUInt32BE(newAddr, i + 4);
      } else if (op === 0xde) {
        /* List */
        const addr = list.readUInt32BE(i + 4);
        const newAddr = this.processListAddr(addr);
        list.writeUInt32BE(newAddr, i + 4);
      }
    }

    const outAddr = this.outSize | (this.segOut << 24);
    this.outSize += size;
    this.out.push(list);
    if (this.outSize % 16) {
      const extraSize = 16 - (this.outSize % 16);
      const extraBuf = Buffer.alloc(extraSize);
      this.outSize += extraSize;
      this.out.push(extraBuf);
    }

    return outAddr;
  }

  loadSegment(num: number, data: Buffer | null) {
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
      const data = d.readUInt32BE(i);
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

  stripList(list: Buffer, from: number, to: number) {
    const size = list.length;
    const out = Buffer.alloc(size - (to - from));
    list.copy(out, 0, 0, from);
    list.copy(out, from, to);
    return out;
  }

  combineListsAddrs(lists: number[]) {
    const newList = Buffer.alloc(lists.length * 8);
    for (let i = 0; i < lists.length; i++) {
      const op = 0xde;
      const op2 = (i === lists.length - 1) ? 0x01 : 0x00;
      newList.writeUInt32BE((((op << 24) >>> 0) | (op2 << 16)) >>> 0, i * 8);
      newList.writeUInt32BE(lists[i], i * 8 + 4);
    }
    return newList;
  }

  build(): ObjectEditorOut {
    return {
      data: Buffer.concat(this.out),
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
