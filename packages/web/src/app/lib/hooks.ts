import { useState, useEffect, useMemo } from 'preact/hooks';
import { getManifest, Manifest } from './api';
import { manifestEntry } from './util';

export function useManifest() {
  const [state, setState] = useState<{ data: Manifest | null, error: Error | null }>({ data: null, error: null });

  useEffect(() => {
    getManifest()
      .then((data) => setState({ data, error: null }))
      .catch((error) => setState({ data: null, error }));
  }, []);

  return state;
}

export function useManifestEntry(version: string) {
  const manifestState = useManifest();
  const derived = useMemo(() => {
    if (!manifestState.data) {
      return { data: null, error: manifestState.error };
    }
    try {
      const entry = manifestEntry(manifestState.data, version);
      return { data: entry, error: null };
    } catch (error) {
      return { data: null, error: error as Error };
    }
  }, [manifestState.data, version]);
  return derived;
}
