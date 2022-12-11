import { PNG } from 'pngjs';

const parsePNG = async (data: Buffer) => new Promise<PNG>((resolve, reject) => {
  const png = new PNG({
    colorType: 6,
    bitDepth: 8
  });
  png.parse(data, (err, res) => {
    if (err)
      return reject(err);
    resolve(res);
  });
});

export class KeepFile {
  private offset = 0;
  private data: Buffer[] = [];

  private paddingBuffer(size: number) {
    const paddingSize = (16 - (size % 16)) % 16;
    if (paddingSize === 0) {
      return null;
    }
    const padding = Buffer.alloc(paddingSize);
    padding.fill(0xff);
    return padding;
  }

  addData(data: Buffer) {
    const paddingBuf = this.paddingBuffer(data.length);
    if (paddingBuf) {
      data = Buffer.concat([data, paddingBuf]);
    }
    this.data.push(data);
    const offset = this.offset;
    this.offset += data.length;
    return offset;
  }

  async addTexture(data: Buffer, format: 'RGBA32') {
    const png = await parsePNG(data);
    return this.addData(png.data);
  }

  pack() {
    return Buffer.concat(this.data);
  }
};
