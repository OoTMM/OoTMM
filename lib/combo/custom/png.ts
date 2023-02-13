import fs from 'fs/promises';

let PNG: any = null;
if (!process.env.ROLLUP) {
  PNG = require('pngjs').PNG;
};

const rawPng = async (data: Buffer) => new Promise<Buffer>((resolve, reject) => {
  const png = new PNG({
    colorType: 6,
    bitDepth: 8
  });
  png.parse(data, (err: any, res: any) => {
    if (err)
      return reject(err);
    resolve(Buffer.from(res.data));
  });
});

const parsePngRgba32 = async (data: Buffer) => {
  return rawPng(data);
};

const parsePngRgba16 = async (data: Buffer) => {
  const rgba32 = await rawPng(data);
  const rgba16 = Buffer.alloc(rgba32.length / 2);
  for (let i = 0; i < rgba32.length; i += 4) {
    const d = rgba32.readUInt32BE(i);
    const r = (d >> 24) & 0xff;
    const g = (d >> 16) & 0xff;
    const b = (d >> 8) & 0xff;
    const a = d & 0xff;
    const r16 = (r >> 3) & 0x1f;
    const g16 = (g >> 3) & 0x1f;
    const b16 = (b >> 3) & 0x1f;
    const a1 = (a >> 7) & 1;
    const color = (r16 << 11) | (g16 << 6) | (b16 << 1) | a1;
    rgba16.writeUInt16BE(color, i / 2);
  }
  return rgba16;
};

const parsePngI4 = async (data: Buffer) => {
  const rgba32 = await rawPng(data);
  const i4 = Buffer.alloc(rgba32.length / 8);
  for (let i = 0; i < rgba32.length; i += 8) {
    const d1 = rgba32.readUInt32BE(i);
    const r1 = (d1 >> 24) & 0xff;
    const d2 = rgba32.readUInt32BE(i + 4);
    const r2 = (d2 >> 24) & 0xff;
    const v = ((r1 >> 4) << 4) | (r2 >> 4);
    i4.writeUint8(v, i / 8);
  }
  return i4;
};

export const png = async (filename: string, mode: 'rgba32' | 'rgba16' | 'i4') => {
  if (process.env.ROLLUP) {
    return fetch(`/${filename}.bin`).then(x => x.arrayBuffer()).then(x => Buffer.from(x));
  } else {
    const data = await fs.readFile(__dirname + '/../../../data/assets/' + filename + '.png');
    let pngBuffer: Buffer;
    switch (mode) {
    case 'rgba32':
      pngBuffer = await parsePngRgba32(data);
      break;
    case 'rgba16':
      pngBuffer = await parsePngRgba16(data);
      break;
    case 'i4':
      pngBuffer = await parsePngI4(data);
      break;
    }
    await fs.mkdir(__dirname + '/../../../build/assets', { recursive: true });
    await fs.writeFile(__dirname + '/../../../build/assets/' + filename + '.bin', pngBuffer);
    return pngBuffer;
  }
};
