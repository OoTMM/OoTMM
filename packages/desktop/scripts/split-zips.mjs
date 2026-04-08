/**
 * Splits the single neu build release zip into one zip per architecture.
 * Output: dist/ootmm-<arch>.zip, each containing the binary + resources.neu
 */
import { readdirSync, rmSync } from 'node:fs';
import { resolve } from 'node:path';
import { execFileSync } from 'node:child_process';

const distDir = resolve(import.meta.dirname, '../dist');
const appDir = resolve(distDir, 'ootmm');

const binaries = readdirSync(appDir).filter(f => f !== 'resources.neu');

for (const binary of binaries) {
  const arch = binary.replace(/^ootmm-/, '').replace(/\.exe$/, '');
  const outPath = resolve(distDir, `ootmm-${arch}.zip`);
  execFileSync('zip', [outPath, binary, 'resources.neu'], { cwd: appDir, stdio: 'inherit' });
  console.log(`Created ootmm-${arch}.zip`);
}

rmSync(appDir, { recursive: true });
rmSync(resolve(distDir, 'ootmm-release.zip'));
