import fs from 'node:fs/promises';
import path from 'node:path';
import { parse as parseYaml } from 'yaml';
import * as CSV from 'csv/sync';

export const ROOT_DIR = path.resolve(import.meta.dirname, '..', '..', '..');
export const DATA_DIR = path.join(ROOT_DIR, 'data');
export const DIST_DIR = path.resolve(import.meta.dirname, '..', 'dist');

export async function emitJson(filename: string, data: any) {
  return emit(filename, JSON.stringify(data));
}

export async function emit(filename: string, data: string) {
  const fullpath = path.join(DIST_DIR, filename);
  const dirPath = path.dirname(fullpath);
  await fs.mkdir(dirPath, { recursive: true });
  await fs.writeFile(fullpath, data, 'utf8');
}

export async function loadYaml(patterns: string | string[]): Promise<any> {
  patterns = Array.isArray(patterns) ? patterns : [patterns];
  let data: any = null;

  for (const p of patterns) {
    const match = fs.glob(p, { cwd: DATA_DIR });
    for await (const name of match) {
      const filename = path.join(DATA_DIR, name);
      const file = await fs.readFile(filename, 'utf8');
      const parsed = parseYaml(file);

      if (data === null) {
        data = parsed;
      } else if (Array.isArray(data) && Array.isArray(parsed)) {
        data.push(...parsed);
      } else if (typeof data === 'object' && typeof parsed === 'object') {
        data = { ...data, ...parsed };
      } else {
        throw new Error(`Incompatible YAML structures in files: existing type ${typeof data}, new type ${typeof parsed}`);
      }
    }
  }
  return data;
}

export function loadTxt(name: string): Promise<string> {
  return fs.readFile(path.join(DATA_DIR, name), 'utf8');
}

export async function loadCsv(name: string): Promise<any[]> {
  const content = await fs.readFile(path.join(DATA_DIR, name), 'utf8');
  return CSV.parse(content, { columns: true, skip_empty_lines: true, trim: true });
}
