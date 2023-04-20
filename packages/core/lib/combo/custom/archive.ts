import { compressFile } from '../compress';
import { DmaData } from '../dma';

export type CustomArchivePackResult = {
  data: Buffer;
  dmaAddr: number;
  dmaCount: number;
  objectTableAddr: number;
  objectTableCount: number;
}

export class CustomArchive {
  private paddr = 0x03f80000;
  private vaddr = 0x08000000;
  private data = new Array<Buffer>;
  private dma: Buffer[] = [];
  private objectId = 0x2000;
  private objectTable: Buffer[] = [];

  private paddingBuffer(size: number) {
    const paddingSize = (16 - (size % 16)) % 16;
    if (paddingSize === 0) {
      return null;
    }
    const padding = Buffer.alloc(paddingSize);
    padding.fill(0xff);
    return padding;
  }

  async addData(buffer: Buffer) {
    const paddr = this.paddr;
    const paddingBuffer = this.paddingBuffer(buffer.length);
    this.paddr += buffer.length;
    this.data.push(buffer);
    if (paddingBuffer) {
      this.paddr += paddingBuffer.length;
      this.data.push(paddingBuffer);
    }
    return paddr;
  }

  async addBuffer(buffer: Buffer, shouldCompress: boolean) {
    /* Compress the custom file */
    let compressed: Buffer = buffer;

    if (shouldCompress) {
      compressed = await compressFile(buffer);
    }
    const paddingBuf = this.paddingBuffer(compressed.length);
    if (paddingBuf) {
      compressed = Buffer.concat([compressed, paddingBuf]);
    }

    /* Add the DMA entry */
    const dmaBuffer = Buffer.alloc(0x10);
    const dma = new DmaData(dmaBuffer);
    dma.write(0, {
      virtStart: this.vaddr,
      virtEnd: this.vaddr + buffer.length,
      physStart: this.paddr,
      physEnd: shouldCompress ? this.paddr + compressed.length : 0,
    });
    this.dma.push(dmaBuffer);

    /* Add the actual data */
    this.data.push(compressed);
    this.paddr += compressed.length;
    this.vaddr += Math.floor((buffer.length + 15) / 16) * 16;

    return dma.read(0);
  }

  async addObject(buffer: Buffer, shouldCompress: boolean) {
    const dma = await this.addBuffer(buffer, shouldCompress);
    const objEntry = Buffer.alloc(0x8);
    objEntry.writeUInt32BE(dma.virtStart, 0);
    objEntry.writeUInt32BE(dma.virtEnd, 4);
    this.objectTable.push(objEntry);
    const objectId = this.objectId;
    this.objectId += 1;
    return objectId;
  }

  pack(): CustomArchivePackResult {
    const data = Buffer.concat([...this.data, ...this.dma, ...this.objectTable]);
    const dmaAddr = this.paddr;
    const dmaCount = this.dma.length;
    const objectTableAddr = dmaAddr + dmaCount * 0x10;
    const objectTableCount = this.objectTable.length;
    return { data, dmaAddr, dmaCount, objectTableAddr, objectTableCount };
  }
}
