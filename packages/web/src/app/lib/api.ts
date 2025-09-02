type ManifestComponentIframe = {
  path: string;
};

type ManifestComponent = {
  iframe?: ManifestComponentIframe;
};


export type ManifestEntry = {
  generator?: ManifestComponent;
};

export type Manifest = {[key: string]: ManifestEntry};

export async function getManifest(): Promise<Manifest> {
  const manifest = await fetch(`${__STATIC_URL__}/config.json`);
  return manifest.json();
}
