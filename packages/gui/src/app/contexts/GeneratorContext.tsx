import { ComponentChildren, createContext } from 'preact';
import { useContext, useEffect, useState, StateUpdater, Dispatch } from 'preact/hooks';
import type { GeneratorOutput, OptionsInput } from '@ootmm/core';

import * as API from '../api';
import { loadFile, saveFile } from '../db';
import { useStore } from '../store';

type GeneratorState = {
  generator: {
    isGenerating: boolean;
    message: string | null;
    progress: number | null;
    error: string | null;
    result: GeneratorOutput | null;
    archive: API.ResultFile | null;
    warnings: string[];
  },
}

type GeneratorContext = {
  state: GeneratorState;
  setState: Dispatch<StateUpdater<GeneratorState>>;
}

export const GeneratorContext = createContext<GeneratorContext>(null as any);

function createState(): GeneratorState {
  return {
    generator: {
      isGenerating: false,
      message: null,
      progress: null,
      error: null,
      result: null,
      archive: null,
      warnings: [],
    }
  };
}

export function GeneratorContextProvider({ children }: { children: ComponentChildren }) {
  const [state, setState] = useState(createState);

  return (
    <GeneratorContext.Provider value={{ state, setState }}>
      {children}
    </GeneratorContext.Provider>
  );
}

export function useGenerator() {
  const settings = useStore(state => state.settings);
  const cosmetics = useStore(state => state.cosmetics);
  const random = useStore(state => state.randomSettings);
  const config = useStore(state => state.config);
  const { state, setState } = useContext(GeneratorContext);
  const { generator } = state;
  const { isGenerating, message, progress, error, result, archive, warnings } = generator;

  const generate = async () => {
    setState((state) => ({ ...state, generator: { ...state.generator, isGenerating: true, archive: null, result: null, error: null, warnings: [] } }));
    const { oot, mm } = config.files;
    let { patch } = config.files;
    if (config.mode !== 'patch') {
      patch = null;
    }
    const options: OptionsInput = { mode: config.mode, seed: config.seed, settings, random, cosmetics };
    try {
      const onMessage = (message: string) => {
        console.log(message);
        setState((state) => ({ ...state, generator: { ...state.generator, message, progress: null } }));
      };

      const onProgress = (progress: number, total: number) => {
        let newProgress: number | null;
        if (total === 0) {
          newProgress = null;
        } else {
          newProgress = Math.floor(progress * 1000 / total) * 0.1;
        }

        setState((state) => ({ ...state, generator: { ...state.generator, progress: newProgress } }));
      };

      const { data, warnings } = await API.generate({ oot: oot!, mm: mm!, patch: patch ? patch : undefined }, options, onMessage, onProgress);
      setState((state) => ({ ...state, generator: { ...state.generator, isGenerating: false, result: data, warnings } }));
      const archive = await API.archive(data);
      setState((state) => ({ ...state, generator: { ...state.generator, archive } }));
    } catch (e: any) {
      setState((state) => ({ ...state, generator: { ...state.generator, isGenerating: false, error: e.toString() } }));
    }
  };

  return { isGenerating, message, error, result, archive, generate, progress, warnings };
}
