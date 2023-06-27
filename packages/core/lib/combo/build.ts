import { spawn } from "child_process";
import fs from "fs/promises";
import path from "path";
import { Buffer } from "buffer";

import { GAMES } from "./config";
import { Options } from "./options";
import { fileExists } from "./util";

type BuildGameOutput = {
  payload: Buffer;
  patches: Buffer;
};

export type BuildOutput = {
  oot: BuildGameOutput;
  mm: BuildGameOutput;
};

const cloneDependencies = async () => {
  const thirdPartyDir = path.resolve('third_party');
  const stampFile = path.resolve(thirdPartyDir, '.stamp');
  if (await fileExists(stampFile))
    return;
  await fs.mkdir(thirdPartyDir, { recursive: true });
  return new Promise((resolve, reject) => {
    const proc = spawn('git', ['clone', '--depth', '50', 'https://github.com/decompals/ultralib', thirdPartyDir + '/ultralib'], { stdio: 'inherit' });
    proc.on('close', (code) => {
      if (code !== 0)
        return reject(new Error(`git clone failed with code ${code}`));
      fs.writeFile(stampFile, '').then(_ => resolve(null));
    });
  });
};

const make = async (opts: Options) => {
  await cloneDependencies();
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

const fetchBuildArtifacts = async (opts: Options): Promise<BuildOutput> => {
  const [oot, mm] = await Promise.all(GAMES.map(async (g) => {
    const [payload, patches] = await Promise.all([
      opts.fetch!(`${g}_payload.bin`),
      opts.fetch!(`${g}_patch.bin`),
    ]);
    return { payload, patches };
  }));
  return { oot, mm };
};

export const build = async (opts: Options): Promise<BuildOutput> => {
  if (!process.env.ROLLUP) {
    await make(opts);
    return getBuildArtifacts('build' + (opts.debug ? '/Debug' : '/Release'));
  } else {
    return fetchBuildArtifacts(opts);
  }
};
