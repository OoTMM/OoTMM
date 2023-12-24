import { Buffer } from 'buffer';

import { compressFile } from './compress';
import { CONFIG, GAMES, Game } from './config';
import { DecompressedRoms } from './decompress';
import { DmaData, DmaDataRecord } from './dma';
import { Monitor } from './monitor';
import { Patchfile } from './patch-build/patchfile';

const rol = (v: number, b: number) => (((v << b) | (v >>> (32 - b))) & 0xffffffff) >>> 0;

function checksum(rom: Buffer) {
  const seed = 0xdf26f436;
  let t1 = seed;
  let t2 = seed;
  let t3 = seed;
  let t4 = seed;
  let t5 = seed;
  let t6 = seed;

  for (let i = 0; i < 0x100000 / 4; ++i) {
    const offset = 0x1000 + i * 4;
    const d = rom.readUInt32BE(offset);
    if ((((t6 + d) & 0xffffffff) >>> 0) < t6) {
      t4 = ((t4 + 1) & 0xffffffff) >>> 0;
    }
    t6 = ((t6 + d) & 0xffffffff) >>> 0;
    t3 = ((t3 ^ d) & 0xffffffff) >>> 0;
    const r = rol(d, ((d & 0x1f) >>> 0));
    t5 = ((t5 + r) & 0xffffffff) >>> 0;
    if (t2 > d) {
      t2 = (t2 ^ r) >>> 0;
    } else {
      t2 = (t2 ^ t6 ^ d) >>> 0;
    }
    const offset2 = 0x750 + ((i * 4) & 0xff);
    const x = rom.readUInt32BE(offset2);
    t1 = ((t1 + ((x ^ d) >>> 0)) & 0xffffffff) >>> 0;
  }

  return [(t6 ^ t4 ^ t3) >>> 0, (t5 ^ t2 ^ t1) >>> 0];
};

type Payload = {
  pstart: number;
  psize: number;
}

type PackerGameState = {
  dma: DmaData;
  dmaAddr: number;
  packedFiles: number;
  payload?: Payload;
}

type FileData = { type: 'uncompressed' | 'compressed', data: Buffer } | { type: 'dummy' };

class Packer {
  private rom: Buffer;
  private paddr: number;
  private gs: {[k in Game]: PackerGameState};
  private extraDma: DmaDataRecord[];

  constructor(
    private monitor: Monitor,
    private roms: DecompressedRoms,
    private patchfiles: Patchfile[],
  ) {
    this.rom = Buffer.alloc(0x4000000);
    this.paddr = 0;
    this.gs = {
      oot: {
        dma: new DmaData(Buffer.alloc(CONFIG['oot'].dmaCount * 0x10)),
        dmaAddr: 0,
        packedFiles: 0,
      },
      mm: {
        dma: new DmaData(Buffer.alloc(CONFIG['mm'].dmaCount * 0x10)),
        dmaAddr: 0,
        packedFiles: 0,
      },
    };
    this.extraDma = [];
  }

  async run() {
    /* Apply patches and compress */
    this.monitor.log("Pack: Pre-compress patches");
    for (const p of this.patchfiles) {
      for (const g of GAMES) {
        const rom = this.roms[g].rom;
        for (const pp of p.gamePatches[g].data) {
          pp.data.copy(rom, pp.addr);
        }
      }
    }

    /* Pack OoT and MM */
    this.monitor.log("Pack: Compress");
    await this.packFiles('oot', 27);
    const mmBase = this.paddr;
    await this.packFiles('mm', 31);
    await this.packFiles('oot');
    await this.packFiles('mm');

    /* Write the DMA */
    this.gs.oot.dma.data().copy(this.rom, CONFIG['oot'].dmaAddr);
    this.gs.mm.dma.data().copy(this.rom, CONFIG['mm'].dmaAddr + mmBase);

    /* Post compress global patches */
    this.monitor.log("Pack: Post-compress patches");
    const patch = this.patchfiles[0];

    /* Add extra files */
    this.monitor.log("Pack: Add extra files");
    for (const p of this.patchfiles) {
      for (const f of p.newFiles) {
        await this.addFile(f.vrom, f.data, f.compressed);
      }
    }

    /* Pack the payload */
    this.monitor.log("Pack: Write payloads");
    const meta = Buffer.alloc(0x1000);
    const ootPayload = this.gs.oot.payload;
    const mmPayload = this.gs.mm.payload;

    if (!ootPayload || !mmPayload) {
      throw new Error("Payload not found");
    }

    meta.writeUInt32BE(ootPayload.pstart, 0x08);
    meta.writeUInt32BE(ootPayload.psize, 0x0c);
    meta.writeUInt32BE(mmPayload.pstart, 0x10);
    meta.writeUInt32BE(mmPayload.psize, 0x14);

    /* Pack custom DMA */
    this.monitor.log("Pack: Write custom DMA");
    const customDma = new DmaData(Buffer.alloc(this.extraDma.length * 0x10));
    for (let i = 0; i < this.extraDma.length; ++i) {
      customDma.write(i, this.extraDma[i]);
    }
    const customDmaAddr = this.addData(customDma.data());
    meta.writeUInt32BE(customDmaAddr, 0x00);
    meta.writeUInt32BE(this.extraDma.length, 0x04);

    /* Check that it fits */
    if (this.paddr + meta.length > 0x4000000) {
      throw new Error("ROM doesn't fit");
    }

    /* Write the meta */
    meta.copy(this.rom, this.rom.length - meta.length);

    /* Patch rom header */
    Buffer.from('OOT+MM COMBO       ').copy(this.rom, 0x20);
    Buffer.from('NEDEP').copy(this.rom, 0x3b);

    this.fixChecksum();

    return this.rom;
  }

  private addData(data: Buffer) {
    const size = data.length;
    const sizeAligned = (size + 0xf) & ~0xf;
    const paddr = this.paddr;
    data.copy(this.rom, paddr);
    this.paddr += sizeAligned;
    return paddr;
  }

  private async addFile(vrom: number, data: Buffer, compressed: boolean) {
    if (data.length === 0) {
      this.extraDma.push({ physStart: 0xffffffff, physEnd: 0xffffffff, virtStart: vrom, virtEnd: vrom });
      return;
    }

    const physStart = this.paddr;
    const virtStart = vrom;
    let size = data.length;
    let sizeAligned = (size + 0xf) & ~0xf;
    const virtSize = (size + 0xf) & ~0xf;
    const virtEnd = (virtStart + virtSize) >>> 0;
    let physEnd = 0;

    if (compressed) {
      data = await compressFile(data);
      size = data.length;
      sizeAligned = (size + 0xf) & ~0xf;
      physEnd = physStart + sizeAligned;
    }

    data.copy(this.rom, physStart);
    this.paddr += sizeAligned;
    this.extraDma.push({ physStart, physEnd, virtStart, virtEnd });

    if (vrom === 0xf0000000) {
      this.gs.oot.payload = { pstart: physStart, psize: sizeAligned };
    } else if (vrom === 0xf0100000) {
      this.gs.mm.payload = { pstart: physStart, psize: sizeAligned };
    }
  }

  private async packFiles(game: Game, count?: number) {
    const config = CONFIG[game];
    const gs = this.gs[game];
    const gameData = this.roms[game];
    const gameRom = gameData.rom;
    const compressedDma = new DmaData(gameData.dma);
    const uncompressedDma = new DmaData(gameRom.subarray(config.dmaAddr, config.dmaAddr + config.dmaCount * 0x10));
    const removedFiles = new Set<number>();

    for (const p of this.patchfiles) {
      for (const f of p.gamePatches[game].removedFiles) {
        removedFiles.add(f);
      }
    }

    /* If count is not provided - pack all */
    if (count === undefined) {
      count = config.dmaCount - gs.packedFiles;
    }

    /* Prepare files */
    const filePromises: Promise<FileData>[] = [];
    for (let i = 0; i < count; ++i) {
      const fileId = gs.packedFiles + i;
      const compressedDmaEntry = compressedDma.read(fileId);
      const uncompressedDmaEntry = uncompressedDma.read(fileId);

      /* Check if the file is dummy */
      if (uncompressedDmaEntry.physEnd === 0xffffffff || removedFiles.has(fileId)) {
        filePromises.push(Promise.resolve({ type: 'dummy' }));
        continue;
      }

      /* Get the data */
      let data = gameRom.subarray(uncompressedDmaEntry.physStart, uncompressedDmaEntry.physStart + uncompressedDmaEntry.virtEnd - uncompressedDmaEntry.virtStart);

      /* Check if the file is compressed */
      if (compressedDmaEntry.physEnd !== 0) {
        filePromises.push(new Promise(async (resolve, reject) => {
          data = await compressFile(data);
          resolve({ type: 'compressed', data });
        }));
      } else {
        filePromises.push(Promise.resolve({ type: 'uncompressed', data }));
      }
    }

    /* Wait for all files to be ready */
    const files = await Promise.all(filePromises);

    /* Pack files */
    for (let i = 0; i < count; ++i) {
      const file = files[i];
      const fileId = gs.packedFiles + i;
      const uncompressedDmaEntry = uncompressedDma.read(fileId);

      /* Check if the file is dummy */
      if (file.type === 'dummy') {
        gs.dma.write(fileId, { ...uncompressedDmaEntry, physStart: 0xffffffff, physEnd: 0xffffffff });
        continue;
      }

      /* Write the file data */
      const size = file.data.length;
      const sizeAligned = (size + 0xf) & ~0xf;
      const paddr = this.paddr;
      file.data.copy(this.rom, paddr);
      this.paddr += sizeAligned;

      /* Write the DMA entry */
      if (file.type === 'compressed') {
        gs.dma.write(fileId, { ...uncompressedDmaEntry, physStart: paddr, physEnd: paddr + sizeAligned });
      } else {
        gs.dma.write(fileId, { ...uncompressedDmaEntry, physStart: paddr, physEnd: 0 });
      }
    }

    /* Update the game state */
    gs.packedFiles += count;
  }

  private fixChecksum() {
    this.monitor.log("Fixing the checksum");
    const [c1, c2] = checksum(this.rom);
    this.rom.writeUInt32BE(c1, 0x10);
    this.rom.writeUInt32BE(c2, 0x14);
  }
}

export function pack(monitor: Monitor, roms: DecompressedRoms, patchfiles: Patchfile[]) {
  const packer = new Packer(monitor, roms, patchfiles);
  return packer.run();
}
