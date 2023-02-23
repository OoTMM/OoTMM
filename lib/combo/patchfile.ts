type Patch = {
  romAddr: number;
  data: Buffer;
};

export class Patchfile {
  private patches: Patch[] = [];

  constructor() {
  }

  addPatch(romAddr: number, data: Buffer) {
    this.patches.push({ romAddr, data });
  }

  toBuffer(): Buffer {
    const buffers: Buffer[] = [];
    const header = Buffer.alloc(0x10, 0xff);
    header.write('OoTMM-PF', 'utf8');
    header.writeUInt32LE(0x00000000, 0x8); /* Revision */
    header.writeUInt32LE(this.patches.length, 0xc);
    buffers.push(header);

    for (const patch of this.patches) {
      const patchHeader = Buffer.alloc(8);
      patchHeader.writeUInt32LE(patch.romAddr, 0x0);
      patchHeader.writeUInt32LE(patch.data.length, 0x4);
      buffers.push(patchHeader);
      buffers.push(patch.data);
    }

    return Buffer.concat(buffers);
  }

  applyToRom(rom: Buffer) {
    for (const patch of this.patches) {
      patch.data.copy(rom, patch.romAddr);
    }
  }
}
