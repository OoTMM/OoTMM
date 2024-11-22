import { promises as fs } from 'fs';
import path from 'path';
import { sync as globSync } from 'glob';
import JSZip from 'jszip';
import childProcess from 'child_process';

import { codegen as comboCodegen } from './combo/codegen';
import { customAssetsKeep, customFiles } from './combo/custom';
import { Monitor } from './combo/monitor';
import { cosmeticsAssets } from './combo/cosmetics';
import { custom } from './combo/custom';
import { decompressGames } from './combo/decompress';
import { Patchfile } from './combo/patch-build/patchfile';
import { CodeGen } from './combo/util/codegen';

import { setupAssetsMap } from './build/build-assets-map';

const env = process.env.NODE_ENV || 'development';
const isProd = (env === 'production');

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

export async function buildNative() {
  const cmakeBuildType = isProd ? 'Release' : 'Debug';

  /* Resolve paths */
  const installDir = path.resolve(__dirname, '..', 'build');
  const buildDir = path.resolve(installDir, 'tree', cmakeBuildType);
  const sourceDir = path.resolve(__dirname, '..');
  const binDir = path.resolve(installDir, 'bin');
  const ovlDir = path.resolve(binDir, 'ovl');

  /* Remove old overlays */
  await fs.rm(ovlDir, { recursive: true, force: true });

  /* Make directories */
  await fs.mkdir(buildDir, { recursive: true });
  await fs.mkdir(installDir, { recursive: true });

  /* Build and install with CMake */
  await runCommand('cmake', ['-B', buildDir, '-S', sourceDir, '-G', 'Ninja', `-DCMAKE_BUILD_TYPE=${cmakeBuildType}`]);
  await runCommand('cmake', ['--build', buildDir]);
  await runCommand('cmake', ['--install', buildDir, '--prefix', installDir]);

  return binDir;
}

function readFileUint8(path: string): Promise<Uint8Array> {
  return fs.readFile(path).then((x) => new Uint8Array(x.buffer, x.byteOffset, x.byteLength));
}

async function codegenCustomAssets(monitor: Monitor) {
  /* We can't run that during CI because it requires the ROMs */
  if (process.env.CI)
    return;

  const [oot, mm] = await Promise.all([
    readFileUint8('../../roms/oot.z64'),
    readFileUint8('../../roms/mm.z64'),
  ]);

  const roms = await decompressGames(monitor, { oot, mm });
  const customDefines = await custom(monitor, roms, new Patchfile());
  const cg = new CodeGen('build/include/custom.h', 'CUSTOM_H');
  for (const [key, value] of customDefines) {
    cg.define(key, value);
  }
  await cg.emit();
}

async function build() {
  const dummyMonitor = new Monitor({});

  await Promise.all([
    codegenCustomAssets(dummyMonitor),
    comboCodegen(dummyMonitor),
    cosmeticsAssets(),
    setupAssetsMap(),
  ]);

  const installDir = await buildNative();

  /* Create the zip */
  const zip = new JSZip();

  /* Zip the install tree */
  const files = globSync('**/*', { cwd: installDir, nodir: true });
  await Promise.all(files.map(async (filename) => zip.file(filename, await fs.readFile(path.join(installDir, filename)))));

  /* Add the extra assets */
  await customAssetsKeep();
  await customFiles();

  for (const basePath of ["build/assets", "data/static"]) {
    const matches = globSync(['**/*.bin', '**/*.zobj'], { cwd: basePath });
    for (const filename of matches) {
      const data = await fs.readFile(path.join(basePath, filename));
      zip.file(filename, data);
    }
  }

  /* Compress the data */
  const zipBuf = await zip.generateAsync({ type: 'uint8array', compression: 'DEFLATE', compressionOptions: { level: 9 } });
  await fs.mkdir('dist', { recursive: true });
  await fs.writeFile('dist/data.zip', zipBuf);
}

build().catch((err) => {
  console.error(err);
  process.exit(1);
});
