import { useManifest } from '../lib/hooks';
import { manifestLatest } from '../lib/util';

export function GeneratorVersions() {
  const manifest = useManifest();

  /* Ensure the manifest is properly loaded */
  if (!manifest.data) {
    if (manifest.error) {
      return (
        <h1>Error: {manifest.error.message}</h1>
      );
    } else {
      return (
        <h1>Loading...</h1>
      );
    }
  }

  let versions = Object.keys(manifest.data).filter((key) => key.match(/^v\d+\.\d+(\.\d+)?$/));
  const latestVersion = manifestLatest(manifest.data);
  versions = versions.filter((key) => key !== latestVersion).sort((a, b) => b.localeCompare(a, undefined, { numeric: true }));

  return (
    <main>
      <h1>Old Versions</h1>
      <ul>
        {versions.map((version) => (
          <li key={version}><a href={`/gen/${version}`}>{version}</a></li>
        ))}
      </ul>
    </main>
  );
}
