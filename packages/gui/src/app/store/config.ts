import { StateCreator } from 'zustand';

import type { Store } from './';
import { saveFile } from '../db';

export interface ConfigSlice {
  config: {
    mode: 'create' | 'random' | 'patch',
    files: {
      oot: File | null;
      mm: File | null;
      patch: File | null;
    },
    seed: string;
  },
  setRomConfigFile: (key: keyof ConfigSlice['config']['files'], file: File | null) => void;
  setSeed: (seed: string) => void;
  setMode: (mode: 'create' | 'random' | 'patch') => void;
}

export const createConfigSlice: StateCreator<Store, [], [], ConfigSlice> = (set, get) => ({
  config: {
    mode: 'create',
    files: {
      oot: null,
      mm: null,
      patch: null,
    },
    seed: '',
  },
  setRomConfigFile: (key, file) => {
    set({ config: { ...get().config, files: { ...get().config.files, [key]: file } } });
    if (key !== 'patch') {
      saveFile(key, file).catch(console.error);
    }
  },
  setSeed: (seed) => {
    set({ config: { ...get().config, seed } });
  },
  setMode: (mode) => {
    set({ config: { ...get().config, mode } });
  }
});
