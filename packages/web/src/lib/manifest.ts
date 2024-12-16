export async function getManifest() {
  const manifest = await fetch(`${__STATIC_URL__}/config.json`);
  return manifest.json();
}

export async function getManifestForVersion(version: string) {
  const manifest = await getManifest();
  let data: {} | undefined = undefined;
  if (version === 'stable') {
    const keys = Object.keys(manifest);
    const versions = keys.filter((key) => key.match(/^v\d+\.\d+(\.\d+)?$/));
    const latest = versions.sort((a, b) => a.localeCompare(b, undefined, { numeric: true })).pop()!;
    data = manifest[latest];
  } else {
    data = manifest[version];
  }

  if (!data) {
    throw new Error('Version not found: ' + version);
  }

  return data;
}
