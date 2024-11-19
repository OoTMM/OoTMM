import { useState, useEffect } from 'preact/hooks';
import { getManifestForVersion } from './manifest';

export function useManifestVersion(version: string) {
  const [state, setState] = useState<{ data: any | null, isReady: boolean, error: Error | null }>({ data: null, isReady: false, error: null });

  useEffect(() => {
    getManifestForVersion(version)
      .then((data) => setState({ data, isReady: true, error: null }))
      .catch((error) => setState({ data: null, isReady: false, error }));
  }, [version]);

  return state;
}
