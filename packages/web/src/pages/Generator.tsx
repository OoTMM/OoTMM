import { useRoute } from 'preact-iso';

import { useManifestVersion } from '../lib/hooks';

export function Generator() {
  const { params } = useRoute();
  const { data, isReady, error } = useManifestVersion(params.id);

  if (error) {
    return <div>Error: {error.message}</div>;
  } else if (!isReady) {
    return <div>Loading...</div>;
  } else {
    return <iframe className="generator" src={`${process.env.STATIC_URL}/${data.generator.iframe.path}`}></iframe>;
  }
}
