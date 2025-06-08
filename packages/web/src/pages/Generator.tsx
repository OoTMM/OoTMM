import { useRoute } from 'preact-iso';

import { useManifestEntry } from '../lib/hooks';

export function Generator() {
  const { params } = useRoute();
  const { data, error } = useManifestEntry(params.id);

  if (error) {
    return <div>Error: {error.message}</div>;
  } else if (data === null) {
    return <div>Loading...</div>;
  } else {
    return <iframe className="generator" src={`${__STATIC_URL__}/${data.generator.iframe.path}`}></iframe>;
  }
}
