import { createContext, useCallback, useContext, useState } from 'react';
import { OptionRandomSettings } from '@ootmm/core';

import * as API from '../api';
import { useUpdateEffect } from '../hooks/useUpdateEffect';
import { localStoragePrefixedSet } from '../util';

type RandomSettingsContext = {
  randomSettings: OptionRandomSettings;
  patchRandomSettings: (random: Partial<OptionRandomSettings>) => void;
};

const RandomSettingsContext = createContext<RandomSettingsContext>(null as any);

export function RandomSettingsProvider({ children }: { children: React.ReactNode }) {
  const [randomSettings, setRandomSettings] = useState(() => API.initialRandomSettings());

  const patchRandomSettings = useCallback((random: Partial<OptionRandomSettings>) => {
    setRandomSettings(state => ({ ...state, ...random }));
  }, []);

  useUpdateEffect(() => {
    localStoragePrefixedSet('randomSettings', randomSettings);
  }, [randomSettings]);

  return (
    <RandomSettingsContext.Provider value={{ randomSettings, patchRandomSettings }}>
      {children}
    </RandomSettingsContext.Provider>
  );
}

export function useRandomSettings() {
  return useContext(RandomSettingsContext).randomSettings;
}

export function usePatchRandomSettings() {
  return useContext(RandomSettingsContext).patchRandomSettings;
}
