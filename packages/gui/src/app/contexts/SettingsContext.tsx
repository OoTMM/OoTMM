import { createContext, useCallback, useContext, useEffect, useRef, useState } from 'react';
import { isEqual } from 'lodash';
import { Items, Settings, SettingsPatch, makeSettings, mergeSettings } from '@ootmm/core';

import * as API from '../api';
import { localStoragePrefixedSet } from '../util';

type SettingsContext = {
  settings: Settings;
  itemPool: Items;
  locations: string[];
  setSettings: (newSettings: Settings | ((s: Settings) => Settings)) => void;
  patchSettings: (patch: SettingsPatch) => void;
};

const SettingsContext = createContext<SettingsContext>(null as any);

export function SettingsProvider({ children }: { children: React.ReactNode }) {
  const [settings, setSettingsRaw] = useState(() => API.initialSettings());
  const [itemPool, setItemPool] = useState<Items>({});
  const [locations, setLocations] = useState<string[]>([]);
  const ticket = useRef(0);

  const setSettings = useCallback((newSettings: Settings | ((s: Settings) => Settings)) => {
    setSettingsRaw((settings) => {
      if (typeof newSettings === 'function') {
        newSettings = newSettings(settings);
      }
      return makeSettings(newSettings);
    });
  }, []);

  const patchSettings = useCallback((patch: SettingsPatch) => {
    setSettingsRaw(state => mergeSettings(state, patch));
  }, []);

  /* Save to localStorage */
  useEffect(() => {
    localStoragePrefixedSet('settings', settings);
  }, [settings]);

  /* On settings update, re-trigger stuff */
  useEffect(() => {
    const currentTicket = ++ticket.current;
    Promise.all([
      API.itemPool(settings),
      API.locationList(settings),
    ]).then(([newItemPool, newLocations]) => {
      if (ticket.current !== currentTicket) return;
      setItemPool(newItemPool);
      setLocations(newLocations);
      const startingItems = API.restrictItemsByPool(settings.startingItems, newItemPool);
      if (isEqual(settings.startingItems, startingItems)) return;
      setSettings(state => makeSettings({ ...state, startingItems }));
    });
  }, [settings]);


  return (
    <SettingsContext.Provider value={{ settings, itemPool, locations, setSettings, patchSettings }}>
      {children}
    </SettingsContext.Provider>
  );
}

export function useSettings() {
  const context = useContext(SettingsContext);
  return context.settings;
}

export function useSetting<T extends keyof Settings>(key: T) {
  const settings = useSettings();
  const [setting, setSetting] = useState(settings[key]);

  useEffect(() => {
    const newSetting = settings[key];
    if (!isEqual(setting, newSetting)) {
      setSetting(newSetting);
    }
  }, [settings[key]]);

  return setting;
}

export function useSetSettings() {
  return useContext(SettingsContext).setSettings;
}

export function usePatchSettings() {
  return useContext(SettingsContext).patchSettings;
}

export function useItemPool() {
  return useContext(SettingsContext).itemPool;
}

export function useLocations() {
  return useContext(SettingsContext).locations;
}
