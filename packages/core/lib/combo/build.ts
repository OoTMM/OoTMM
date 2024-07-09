import childProcess from 'child_process';
import fs from 'fs';
import path from 'path';

import { Options } from './options';
import { fileExists } from './util';

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

export async function build(opts: Options) {
  /* Clone dependencies */
  await cloneDependencies();

  /* Resolve paths */
  const installDir = path.resolve(__dirname, '..', '..', 'build');
  const buildDir = path.resolve(installDir, 'tree', opts.debug ? 'Debug' : 'Release');
  const sourceDir = path.resolve(__dirname, '..', '..');
  const binDir = path.resolve(installDir, 'bin');
  const ovlDir = path.resolve(binDir, 'ovl');

  /* Remove old overlays */
  await fs.promises.rm(ovlDir, { recursive: true, force: true });

  /* Make directories */
  await fs.promises.mkdir(buildDir, { recursive: true });
  await fs.promises.mkdir(installDir, { recursive: true });

  /* Build and install with CMake */
  await runCommand('cmake', ['-B', buildDir, '-S', sourceDir, '-G', 'Ninja', `-DCMAKE_BUILD_TYPE=${opts.debug ? 'Debug' : 'Release'}`]);
  await runCommand('cmake', ['--build', buildDir]);
  await runCommand('cmake', ['--install', buildDir, '--prefix', installDir]);

  return binDir;
}
