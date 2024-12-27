import { promises as fs } from 'fs';
import { makeRandoFileSystem } from './combo.new/file-system-builder';
import { buildRom } from './combo.new/rom-builder';
import path from 'path';

async function run() {
  const fileSystem = await makeRandoFileSystem();
  const rom = await buildRom(fileSystem);
  const dir = path.resolve(__dirname, '..', 'out');
  await fs.mkdir(dir, { recursive: true });
  await fs.writeFile(path.resolve(dir, 'OoTMM.z64'), rom);
}

run().catch((err) => {
  console.error(err);
  process.exit(1);
});
