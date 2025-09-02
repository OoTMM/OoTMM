import { Manifest, ManifestEntry } from './api';

export function manifestLatest(manifest: Manifest): string {
  const keys = Object.keys(manifest);
  const versions = keys.filter((key) => key.match(/^v\d+\.\d+(\.\d+)?$/));
  const latest = versions.sort((a, b) => a.localeCompare(b, undefined, { numeric: true })).pop()!;
  return latest;
}

export function manifestEntry(manifest: Manifest, version: string): ManifestEntry {
  if (version === 'stable')
    version = manifestLatest(manifest);
  const data = manifest[version];
  if (!data) {
    throw new Error('Version not found: ' + version);
  }

  return data;
}
