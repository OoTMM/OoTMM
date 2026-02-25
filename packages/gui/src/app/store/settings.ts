import { Items, makeSettings, mergeSettings, Settings, SettingsPatch } from '@ootmm/generator';
import { StateCreator } from 'zustand';

import type { Store } from './';
import * as API from '../api';

export interface SettingsSlice {
  settings: Settings;
  itemPool: Items;
  locations: string[];
  setSettings: (newSettings: Settings | ((s: Settings) => Settings)) => void;
  patchSettings: (patch: SettingsPatch) => void;
}

export const createSettingsSlice: StateCreator<Store, [], [], SettingsSlice> = (set, get) => ({
  settings: API.initialSettings(),
  itemPool: {} as Items,
  locations: [] as string[],
  setSettings: (newSettings) => {
    if (typeof newSettings === 'function') {
      newSettings = newSettings(get().settings);
    }
    set({ settings: makeSettings(newSettings) });
  },
  patchSettings: (patch) => {
    set({ settings: mergeSettings(get().settings, patch) });
  }
});
