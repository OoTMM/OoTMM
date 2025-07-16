import { createContext, ReactNode, useCallback, useContext, useEffect, useRef, useState } from 'react';
import { COSMETICS, Cosmetics } from '@ootmm/core';

import * as API from '../api';
import { localStoragePrefixedSet } from '../util';
import { loadFileLocal, saveFileLocal } from '../db';
import { useUpdateEffect } from '../hooks/useUpdateEffect';

const COSMETICS_FILE_KEYS = COSMETICS.filter(c => c.type === 'file').map(c => c.key);

function saveToLocalStorage(cosmetics: Cosmetics) {
  const savedCosmetics = { ...cosmetics };
  for (const c of COSMETICS_FILE_KEYS) {
    delete savedCosmetics[c];
  }
  localStoragePrefixedSet('cosmetics', savedCosmetics);
}

async function saveFiles(cosmetics: Cosmetics, prevCosmetics: Cosmetics): Promise<void> {
  const promises = [];
  for (const c of COSMETICS_FILE_KEYS) {
    const data = cosmetics[c];
    if (prevCosmetics[c] !== data) {
      promises.push(saveFileLocal(`cosmetics:${c}`, data as File).catch(console.error));
    }
  }
  await Promise.all(promises);
}

type CosmeticsContext = {
  cosmetics: Cosmetics;
  setCosmetic: (key: keyof Cosmetics, value: any) => void;
};

const CosmeticsContext = createContext<CosmeticsContext>(null as any);

export function CosmeticsProvider({ children }: { children: ReactNode }) {
  const [cosmetics, setCosmetics] = useState(() => API.initialCosmetics());
  const prevCosmetics = useRef(cosmetics);
  const filesLoaded = useRef(false);

  const setCosmetic = useCallback((key: keyof Cosmetics, value: any) => {
    setCosmetics(state => ({ ...state, [key]: value }));
  }, []);

  useUpdateEffect(() => {
    saveToLocalStorage(cosmetics);
    if (filesLoaded.current) {
      saveFiles(cosmetics, prevCosmetics.current);
    }
    prevCosmetics.current = cosmetics;
  }, [cosmetics]);

  useEffect(() => {
    /* Initial load */
    if (filesLoaded.current) return;

    const promises = [];
    for (const c of COSMETICS_FILE_KEYS) {
      promises.push(loadFileLocal(`cosmetics:${c}`).then(x => setCosmetic(c, x)).catch(console.error));
    }

    Promise.allSettled(promises).finally(() => {
      filesLoaded.current = true;
    });
  }, []);

  return (
    <CosmeticsContext.Provider value={{ cosmetics, setCosmetic }}>
      {children}
    </CosmeticsContext.Provider>
  );
}

export function useCosmetics() {
  return useContext(CosmeticsContext).cosmetics;
}

export function useSetCosmetic() {
  return useContext(CosmeticsContext).setCosmetic;
}
