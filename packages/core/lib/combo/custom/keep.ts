import { concatUint8Arrays } from 'uint8array-extras';

export class KeepFile {
  private offset = 0;
  private data: Uint8Array[] = [];

  private paddingBuffer(size: number) {
    const paddingSize = (16 - (size % 16)) % 16;
    if (paddingSize === 0) {
      return null;
    }
    const padding = new Uint8Array(paddingSize);
    padding.fill(0xff);
    return padding;
  }

  addData(data: Uint8Array) {
    const paddingBuf = this.paddingBuffer(data.length);
    if (paddingBuf) {
      data = concatUint8Arrays([data, paddingBuf]);
    }
    this.data.push(data);
    const offset = this.offset;
    this.offset += data.length;
    return offset;
  }

  pack() {
    return concatUint8Arrays(this.data);
  }
};
