import path from 'node:path';
import fs from 'node:fs/promises';
import { CHECKS } from '@ootmm/core';

const MANIFESTS_DIR = path.resolve(import.meta.dirname, '../../build/manifests');

async function buildManifestChecks() {
  const data: any = {};
  data['version'] = 1;
  data['checks'] = [];

  for (const check of CHECKS) {
    data.checks.push({
      key: check.key,
      location: check.location,
    });
  }

  data.checks.sort((a: any, b: any) => a.key - b.key);

  const file = path.join(MANIFESTS_DIR, 'checks.json');
  await fs.writeFile(file, JSON.stringify(data), 'utf-8');
}

export async function buildManifests() {
  await fs.mkdir(MANIFESTS_DIR, { recursive: true });
  await buildManifestChecks();
}
