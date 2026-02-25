import { OptionRandomSettings } from '@ootmm/generator';
import { StateCreator } from 'zustand';

import type { Store } from './';
import * as API from '../api';

export interface RandomSettingsSlice {
  randomSettings: OptionRandomSettings;
  patchRandomSettings: (random: Partial<OptionRandomSettings>) => void;
}

export const createRandomSettingsSlice: StateCreator<Store, [], [], RandomSettingsSlice> = (set, get) => ({
  randomSettings: API.initialRandomSettings(),
  patchRandomSettings: (random) => {
    set({ randomSettings: { ...get().randomSettings, ...random } });
  }
});
