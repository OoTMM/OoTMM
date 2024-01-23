import childProcess from "child_process";
import fs from 'fs';
import path from 'path';

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
  await fs.promises.mkdir(thirdPartyDir, { recursive: true });
  return new Promise((resolve, reject) => {
    const proc = childProcess.spawn('git', ['clone', '--depth', '50', 'https://github.com/decompals/ultralib', thirdPartyDir + '/ultralib'], { stdio: 'inherit' });
    proc.on('close', (code) => {
      if (code !== 0)
        return reject(new Error(`git clone failed with code ${code}`));
      fs.promises.writeFile(stampFile, '').then(_ => resolve(null));
    });
  });
};

async function runCommand(cmd: string, args: string[]) {
  return new Promise((resolve, reject) => {
    const proc = childProcess.spawn(cmd, args, { stdio: 'inherit' });
    proc.on('close', (code) => {
      if (code === 0) {
        resolve(null);
      } else {
        reject(new Error(`${cmd} exited with code ${code}`));
      }
    });
  });
}

async function make(opts: Options) {
  /* Clone dependencies */
  await cloneDependencies();

  /* Resolve paths */
  const installDir = path.resolve(__dirname, '..', '..', 'build');
  const buildDir = path.resolve(installDir, 'tree', opts.debug ? 'Debug' : 'Release');
  const sourceDir = path.resolve(__dirname, '..', '..');

  /* Make directories */
  await fs.promises.mkdir(buildDir, { recursive: true });
  await fs.promises.mkdir(installDir, { recursive: true });

  /* Build and install with CMake */
  await runCommand('cmake', ['-B', buildDir, '-S', sourceDir, '-G', 'Ninja', `-DCMAKE_BUILD_TYPE=${opts.debug ? 'Debug' : 'Release'}`]);
  await runCommand('cmake', ['--build', buildDir]);
  await runCommand('cmake', ['--install', buildDir, '--prefix', installDir]);
}

const getBuildArtifacts = async (root: string): Promise<BuildOutput> => {
  const [oot, mm] = await Promise.all(GAMES.map(async (g) => {
    const [payload, patches] = await Promise.all([
      fs.promises.readFile(path.resolve(root, g + '_payload.bin')),
      fs.promises.readFile(path.resolve(root, g + '_patch.bin')),
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
  if (!process.env.BROWSER) {
    await make(opts);
    return getBuildArtifacts('build/bin');
  } else {
    return fetchBuildArtifacts(opts);
  }
};
