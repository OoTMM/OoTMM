const TYPES_TO_VALUES = {
  global: 0,
  oot: 1,
  mm: 2,
};

export type PatchType = keyof typeof TYPES_TO_VALUES;

const VALUES_TO_TYPES: {[k: number]: PatchType} = {};
for (const [k, v] of Object.entries(TYPES_TO_VALUES)) {
  VALUES_TO_TYPES[v] = k as PatchType;
}

type Patch = {
  type: PatchType;
  romAddr: number;
  data: Buffer;
};

export class Patchfile {
  private patches: Patch[] = [];
  public readonly hash: string;

  constructor(hashOrBuffer : string | Buffer) {
    if (typeof hashOrBuffer === 'string') {
      this.hash = hashOrBuffer;
    } else {
      const header = hashOrBuffer.subarray(0, 0x18);
      if (header.toString('utf8', 0, 8) !== 'OoTMM-PF') {
        throw new Error('Invalid patch file');
      }
      this.hash = header.toString('utf8', 0x10, 0x18);
      let offset = 0x18;
      const patchCount = header.readUInt32LE(0xc);
      for (let i = 0; i < patchCount; ++i) {
        const patchHeader = hashOrBuffer.subarray(offset, offset + 0xc);
        offset += 0xc;
        const type = VALUES_TO_TYPES[patchHeader.readUInt32LE(0x0)];
        const romAddr = patchHeader.readUInt32LE(0x4);
        const dataLen = patchHeader.readUInt32LE(0x8);
        const data = hashOrBuffer.subarray(offset, offset + dataLen);
        offset += dataLen;
        this.patches.push({ type, romAddr, data });
      }
    }
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
      const patchHeader = Buffer.alloc(0xc);
      let addr = patch.romAddr;
      patchHeader.writeUInt32LE(TYPES_TO_VALUES[patch.type], 0x0);
      patchHeader.writeUInt32LE(addr, 0x4);
      patchHeader.writeUInt32LE(patch.data.length, 0x8);
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
