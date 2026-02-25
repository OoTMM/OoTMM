import { StateCreator } from 'zustand';
import { GeneratorOutput, OptionsInput } from '@ootmm/generator';

import type { Store } from './';
import * as API from '../api';

export interface GeneratorSlice {
  generator: {
    isGenerating: boolean;
    message: string | null;
    progress: number | null;
    error: string | null;
    result: GeneratorOutput | null;
    archive: API.ResultFile | null;
    warnings: string[];
  },
  generate: () => Promise<void>;
}

export const createGeneratorSlice: StateCreator<Store, [], [], GeneratorSlice> = (set, get) => ({
  generator: {
    isGenerating: false,
    message: null,
    progress: null,
    error: null,
    result: null,
    archive: null,
    warnings: [],
  },
  generate: async () => {
    set(state => ({ generator: { ...state.generator, isGenerating: true, archive: null, result: null, error: null, warnings: [] } }));
    const state = get();
    const { config } = state;
    const { oot, mm } = config.files;
    let { patch } = config.files;
    if (config.mode !== 'patch') {
      patch = null;
    }
    const options: OptionsInput = { mode: config.mode, seed: config.seed, settings: state.settings, random: state.randomSettings, cosmetics: state.cosmetics };
    try {
      const onMessage = (message: string) => {
        console.log(message);
        set(state => ({ generator: { ...state.generator, message, progress: null } }));
      };

      const onProgress = (progress: number, total: number) => {
        let newProgress: number | null;
        if (total === 0) {
          newProgress = null;
        } else {
          newProgress = Math.floor(progress * 1000 / total) * 0.1;
        }

        set(state => ({ generator: { ...state.generator, progress: newProgress } }));
      };

      const { data, warnings } = await API.generate({ oot: oot!, mm: mm!, patch: patch ? patch : undefined }, options, onMessage, onProgress);
      set(state => ({ generator: { ...state.generator, isGenerating: false, result: data, warnings } }));
      const archive = await API.archive(data);
      set(state => ({ generator: { ...state.generator, archive } }));
    } catch (e: any) {
      set(state => ({ generator: { ...state.generator, isGenerating: false, error: e.toString() } }));
    }
  }
});
