import { readFileSync, writeFileSync, mkdirSync, globSync } from 'node:fs';
import path from 'node:path';
import { parse as parseYaml } from 'yaml';
import * as CSV from 'csv/sync';

export const ROOT_DIR = path.resolve(__dirname, '..', '..', '..');
export const DATA_DIR = path.join(ROOT_DIR, 'data');
export const DIST_DIR = path.resolve(__dirname, '..', 'dist');

export function emit(filename: string, data: any) {
  mkdirSync(DIST_DIR, { recursive: true });
  writeFileSync(path.join(DIST_DIR, `${filename}.json`), JSON.stringify(data));
}

export function loadYaml(patterns: string | string[]): any {
  patterns = Array.isArray(patterns) ? patterns : [patterns];
  let data: any = null;

  for (const p of patterns) {
    const matchedFiles = globSync(p, { cwd: DATA_DIR });
    if (matchedFiles.length === 0) {
      throw new Error(`No files matched the pattern: ${p}`);
    }
    for (const name of matchedFiles) {
      const file = readFileSync(path.join(DATA_DIR, name), 'utf8');
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

export function loadTxt(name: string): string {
  return readFileSync(path.join(DATA_DIR, name), 'utf8');
}

export function loadCsv(name: string): any[] {
  const content = readFileSync(path.join(DATA_DIR, name), "utf8");
  return CSV.parse(content, { columns: true, skip_empty_lines: true, trim: true });
}
