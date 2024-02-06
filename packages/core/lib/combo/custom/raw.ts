import fs from 'fs';

import { Options } from '../options';

export const raw = async (opts: Options, filename: string) => {
  if (process.env.BROWSER) {
    return opts.fetch!(`${filename}`);
  } else {
    const data = await fs.promises.readFile(__dirname + '/../../../data/static/' + filename);
    return data;
  }
};
