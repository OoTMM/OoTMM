import { promises as fs } from 'fs';
import path from 'path';
import { sync as globSync } from 'glob';
import JSZip from 'jszip';
import childProcess from 'child_process';

import { codegen as comboCodegen } from './combo/codegen';
import { buildCustom, customAssetsKeep, customFiles } from './build/custom';
import { Monitor } from './combo/monitor';
import { cosmeticsAssets } from './combo/cosmetics';
import { decompressGames } from './combo/decompress';
import { Patchfile } from './combo/patch-build/patchfile';
import { CodeGen } from './combo/util/codegen';

import { setupAssetsMap } from './build/build-assets-map';
import { makeVanillaFileSystem } from './shared/file-system-import-vanilla';
import { secretCreateKey, secretEncode } from './shared/secret';
import { deflateSync } from 'zlib';

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

async function buildOld() {
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

async function build() {
  /* Build the vanilla filesystem and extract a blob from it */
  const romOot = await readFileUint8(__dirname + '/../../../roms/oot.z64');
  const romMm = await readFileUint8(__dirname + '/../../../roms/mm.z64');
  const fileSystem = await makeVanillaFileSystem({ oot: romOot, mm: romMm });

  /* Create the secret key */
  const fsBlobData = fileSystem.blob().data;
  //const secretKey = secretCreateKey(fileSystem.blob().data);

  /* Build custom assets */
  await buildCustom(fileSystem);
  const randoBlob = fileSystem.blob();

  /* Build the xdelta */
  const compressedRandoBlob = deflateSync(randoBlob.data, { level: 6, windowBits: 15, dictionary: randoBlob.data });
  //const secretFileSystem = secretEncode(compressedRandoBlob, secretKey);

  console.log(compressedRandoBlob);
}

build().catch((err) => {
  console.error(err);
  process.exit(1);
});
