import fs from 'fs';
import path from 'path';
import { bufReadU32BE, bufWriteU16BE } from './buffer';
import { FileResolver } from '../file-resolver';

// import { PNG } from 'pngjs'

// let PNG: any = null;
// if (!process.env.__IS_BROWSER__) {
//   PNG = require('pngjs').PNG;
// };

const rawPng = async (data: Uint8Array) => new Promise<Uint8Array>(async (resolve, reject) => {
  const { PNG } = await import('pngjs');

  const png = new PNG({
    colorType: 6,
    bitDepth: 8
  });
  png.parse(Buffer.from(data), (err: any, res: any) => {
    if (err)
      return reject(err);
    resolve(new Uint8Array(res.data));
  });
});

const parsePngRgba32 = async (data: Uint8Array) => {
  return rawPng(data);
};

const parsePngRgba16 = async (data: Uint8Array) => {
  const rgba32 = await rawPng(data);
  const rgba16 = new Uint8Array(rgba32.length / 2);
  for (let i = 0; i < rgba32.length; i += 4) {
    const d = bufReadU32BE(rgba32, i);
    const r = (d >> 24) & 0xff;
    const g = (d >> 16) & 0xff;
    const b = (d >> 8) & 0xff;
    const a = d & 0xff;
    const r16 = (r >> 3) & 0x1f;
    const g16 = (g >> 3) & 0x1f;
    const b16 = (b >> 3) & 0x1f;
    const a1 = (a >> 7) & 1;
    const color = (r16 << 11) | (g16 << 6) | (b16 << 1) | a1;
    bufWriteU16BE(rgba16, i / 2, color);
  }
  return rgba16;
};

const parsePngI4 = async (data: Uint8Array) => {
  const rgba32 = await rawPng(data);
  const i4 = new Uint8Array(rgba32.length / 8);
  for (let i = 0; i < rgba32.length; i += 8) {
    const d1 = bufReadU32BE(rgba32, i);
    const r1 = (d1 >> 24) & 0xff;
    const d2 = bufReadU32BE(rgba32, i + 4);
    const r2 = (d2 >> 24) & 0xff;
    const v = ((r1 >> 4) << 4) | (r2 >> 4);
    i4[i / 8] = v;
  }
  return i4;
};

const parsePngBitmask = async (data: Uint8Array) => {
  const rgba32 = await rawPng(data);
  const bitmask = new Uint8Array(rgba32.length / 32);
  for (let i = 0; i < rgba32.length; i += 32) {
    let v = 0;
    for (let j = 0; j < 8; ++j) {
      const d = bufReadU32BE(rgba32, i + j * 4);
      const r = (d >> 24) & 0xff;
      if (r >= 0x80)
        v |= 1 << j;
    }
    bitmask[i / 32] = v;
  }
  return bitmask;
};

export const png = async (filename: string, mode: 'rgba32' | 'rgba16' | 'i4' | 'bitmask') => {
  if (process.env.__IS_BROWSER__) {
    return new FileResolver().fetch(`${filename}.bin`);
  } else {
    const data = await fs.promises.readFile(__dirname + '/../../../data/assets/' + filename + '.png').then((d) => new Uint8Array(d.buffer, d.byteOffset, d.byteLength));
    let pngBuffer: Uint8Array;
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
    case 'bitmask':
      pngBuffer = await parsePngBitmask(data);
      break;
    }

    const outPath = path.resolve(__dirname, '../../../build/assets', filename + '.bin');
    await fs.promises.mkdir(path.dirname(outPath), { recursive: true });
    await fs.promises.writeFile(outPath, pngBuffer);
    return pngBuffer;
  }
};
