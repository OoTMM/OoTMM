import { promises as fs } from 'fs';

export const raw = async (filename: string) => {
  if (process.env.ROLLUP) {
    return fetch(`/${filename}.bin`).then(x => x.arrayBuffer()).then(x => Buffer.from(x));
  } else {
    const data = await fs.readFile(__dirname + '/../../../data/static/' + filename + '.bin');
    return data;
  }
};
