import { Cosmetics } from '@ootmm/core';
import { StateCreator } from 'zustand';

import type { Store } from './';
import * as API from '../api';

export interface CosmeticsSlice {
  cosmetics: Cosmetics;
  setCosmetic: (key: keyof Cosmetics, value: any) => void;
}

export const createCosmeticsSlice: StateCreator<Store, [], [], CosmeticsSlice> = (set, get) => ({
  cosmetics: API.initialCosmetics(),
  setCosmetic: (key, value) => {
    set({ cosmetics: { ...get().cosmetics, [key]: value } });
  }
});
