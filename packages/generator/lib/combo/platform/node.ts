import fs from 'node:fs/promises';
import path from 'node:path';

export { codegen } from '../codegen';
export { compressFile } from '../compress';
export { png } from '../util/png';
export { raw } from '../custom/raw';

export async function loadDataZip(): Promise<Uint8Array> {
  return fs.readFile(new URL('../../../dist/data.zip', import.meta.url));
}

export async function fileExists(filename: string): Promise<boolean> {
  try {
    await fs.access(filename);
    return true;
  } catch {
    return false;
  }
}

export async function writeGeneratedFile(filename: string, data: string): Promise<void> {
  await fs.mkdir(path.dirname(filename), { recursive: true });
  const previous = await fileExists(filename) ? await fs.readFile(filename, 'utf8') : undefined;
  if (previous !== data) {
    await fs.writeFile(filename, data);
  }
}

export async function writeCustomObject(name: string, data: Uint8Array): Promise<void> {
  const dir = path.resolve('build', 'custom');
  await fs.mkdir(dir, { recursive: true });
  await fs.writeFile(path.resolve(dir, name.toLowerCase() + '.zobj'), data);
}

export async function readCosmeticFile(filename: string): Promise<Uint8Array> {
  return fs.readFile(filename);
}
