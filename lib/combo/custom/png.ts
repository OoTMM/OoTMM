import fs from 'fs/promises';

let PNG: any = null;
if (!process.env.ROLLUP) {
  PNG = require('pngjs').PNG;
};

const parsePNG = async (data: Buffer) => new Promise<any>((resolve, reject) => {
  const png = new PNG({
    colorType: 6,
    bitDepth: 8
  });
  png.parse(data, (err: any, res: any) => {
    if (err)
      return reject(err);
    resolve(res);
  });
});

export const png = async (filename: string) => {
  if (process.env.ROLLUP) {
    return fetch(`/${filename}.bin`).then(x => x.arrayBuffer()).then(x => Buffer.from(x));
  } else {
    const data = await fs.readFile(__dirname + '/../../../data/' + filename + '.png');
    const pngData = await parsePNG(data);
    const pngBuffer = Buffer.from(pngData.data);
    await fs.mkdir(__dirname + '/../../../build/assets', { recursive: true });
    await fs.writeFile(__dirname + '/../../../build/assets/' + filename + '.bin', pngBuffer);
    return pngBuffer;
  }
};
