export type PatchType = 'pre-compress' | 'post-compress';

type Patch = {
  type: PatchType;
  romAddr: number;
  data: Buffer;
};

export class Patchfile {
  private patches: Patch[] = [];
  private hash = "XXXXXXXX";

  constructor() {
  }

  setHash(hash: string) {
    this.hash = hash;
  }

  addPatch(type: PatchType, romAddr: number, data: Buffer) {
    this.patches.push({ type, romAddr, data });
  }

  toBuffer(): Buffer {
    const buffers: Buffer[] = [];
    const header = Buffer.alloc(0x18, 0xff);
    header.write('OoTMM-PF', 'utf8');
    header.writeUInt32LE(0x00000000, 0x8); /* Revision */
    header.writeUInt32LE(this.patches.length, 0xc);
    header.write(this.hash, 0x10, 0x8, 'utf8');
    buffers.push(header);

    for (const patch of this.patches) {
      const patchHeader = Buffer.alloc(8);
      let addr = patch.romAddr;
      if (patch.type === 'post-compress') {
        addr = (addr | 0x80000000) >>> 0;
      }
      patchHeader.writeUInt32LE(addr, 0x0);
      patchHeader.writeUInt32LE(patch.data.length, 0x4);
      buffers.push(patchHeader);
      buffers.push(patch.data);
    }

    return Buffer.concat(buffers);
  }

  apply(rom: Buffer, type: PatchType) {
    for (const patch of this.patches) {
      if (patch.type !== type) {
        continue;
      }
      patch.data.copy(rom, patch.romAddr);
    }
  }
}
