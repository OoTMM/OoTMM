import { spawn } from "child_process";
import fs from "fs/promises";
import path from "path";

import { GAMES, PATH_BUILD } from "./config";
import { Options } from "./options";

type BuildGameOutput = {
  payload: Buffer;
  patches: Buffer;
};

export type BuildOutput = {
  oot: BuildGameOutput;
  mm: BuildGameOutput;
};

const make = async (opts: Options) => {
  return new Promise((resolve, reject) => {
    const args = ['-j', '32'];
    if (opts.debug) {
      args.push('DEBUG=1');
    }
    const proc = spawn('make', args, { stdio: 'inherit' });
    proc.on('close', (code) => {
      if (code === 0) {
        resolve(null);
      } else {
        reject(new Error(`make exited with code ${code}`));
      }
    });
  });
};

const getBuildArtifacts = async (root: string): Promise<BuildOutput> => {
  const [oot, mm] = await Promise.all(GAMES.map(async (g) => {
    const [payload, patches] = await Promise.all([
      fs.readFile(path.resolve(root, g + '_payload.bin')),
      fs.readFile(path.resolve(root, g + '_patch.bin')),
    ]);
    return { payload, patches };
  }));
  return { oot, mm };
};

export const build = async (opts: Options): Promise<BuildOutput> => {
  if (!process.env.ROLLUP) {
    await make(opts);
    return getBuildArtifacts(PATH_BUILD + (opts.debug ? '/Debug' : '/Release'));
  } else {
    return getBuildArtifacts(__dirname + '/data');
  }
};
