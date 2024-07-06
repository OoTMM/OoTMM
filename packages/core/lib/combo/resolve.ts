import fs from 'fs';
import path from 'path';

import { FileResolver, Options } from './options';
import { build } from './build';

export const makeResolver = async (opts: Options): Promise<FileResolver> => {
  if (opts.resolver) {
    return opts.resolver;
  }

  if (!process.env.BROWSER) {
    const installDir = await build(opts);
    const fetch = async (filename: string) => {
      return fs.promises.readFile(path.resolve(installDir, filename));
    };
    const glob = async (pattern: RegExp) => {
      const files = await fs.promises.readdir(installDir, { recursive: true });
      return files.filter(f => pattern.test(f));
    };
    return { fetch, glob };
  } else {
    throw new Error('No resolver available in browser');
  }
};
