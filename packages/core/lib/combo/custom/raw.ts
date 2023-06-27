import { promises as fs } from 'fs';

import { Options } from '../options';

export const raw = async (opts: Options, filename: string) => {
  if (process.env.ROLLUP) {
    return opts.fetch!(`${filename}`);
  } else {
    const data = await fs.readFile(__dirname + '/../../../data/static/' + filename);
    return data;
  }
};
