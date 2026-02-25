import fs from 'fs';

import { FileResolver } from '../file-resolver';

export const raw = async (filename: string) => {
  if (process.env.__IS_BROWSER__) {
    return new FileResolver().fetch(`${filename}`);
  } else {
    const data = await fs.promises.readFile(__dirname + '/../../../data/static/' + filename);
    return data;
  }
};
