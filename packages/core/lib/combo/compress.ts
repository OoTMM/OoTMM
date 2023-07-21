import fs from 'fs/promises';
import path from 'path';
import crypto from 'crypto';
import { Buffer } from 'buffer';
import Yaz0 from 'yaz0';

import { fileExists } from './util';

function fsRetry<T>(fn: () => Promise<T>, retries = 5): Promise<T> {
  return fn().catch((err) => {
    if (retries > 0) {
      return new Promise((reesolve, reject) => {
        setTimeout(() => {
          fsRetry(fn, retries - 1).then(reesolve, reject);
        }, 100);
      });
    }
    throw err;
  });
}

export const compressFile = async (data: Buffer): Promise<Buffer> => {
  let filename = "";

  if (!process.env.ROLLUP) {
    const hash = crypto.createHash('sha256').update(data).digest('hex');
    const dir = path.resolve('build', 'cache', 'yaz0', hash.slice(0, 2));
    filename = path.resolve(dir, hash);

    /* Check for the file in cache */
    await fs.mkdir(dir, { recursive: true });
    if (await fileExists(filename)) {
      return fsRetry(() => fs.readFile(filename));
    }
  }

  /* Cache miss - compress */
  const compressed = await Yaz0.compress(data, 7);
  if (!process.env.ROLLUP) {
    await fsRetry(() => fs.writeFile(filename, compressed));
  }
  return compressed;
};
