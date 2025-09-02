import { ComponentChildren, createContext } from 'preact';
import { useContext, useEffect, useState, StateUpdater, Dispatch } from 'preact/hooks';
import type { GeneratorOutput, OptionsInput } from '@ootmm/core';

import * as API from '../api';
import { loadFile, saveFile } from '../db';
import { useCosmetics } from './CosmeticsContext';
import { useRandomSettings } from './RandomSettingsContext';
import { useSettings } from './SettingsContext';

type GeneratorState = {
  romConfig: {
    mode: 'create' | 'random' | 'patch',
    files: {
      oot: File | null;
      mm: File | null;
      patch: File | null;
    },
    seed: string;
  },
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
  setRomConfigFile: (key: keyof GeneratorState['romConfig']['files'], file: File | null) => void;
  setSeed: (seed: string) => void;
  setMode: (mode: 'create' | 'random' | 'patch') => void;
}

export const GeneratorContext = createContext<GeneratorContext>(null as any);

function createState(): GeneratorState {
  return {
    romConfig: {
      mode: 'create',
      files: {
        oot: null,
        mm: null,
        patch: null,
      },
      seed: '',
    },
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

  const setRomConfigFileRaw = (key: keyof GeneratorState['romConfig']['files'], file: File | null) => {
    setState(state => ({ ...state, romConfig: { ...state.romConfig, files: { ...state.romConfig.files, [key]: file } }}));
  }

  const setRomConfigFile = (key: keyof GeneratorState['romConfig']['files'], file: File | null) => {
    setRomConfigFileRaw(key, file);
    if (key !== 'patch') {
      saveFile(key, file).catch(console.error);
    }
  };

  const setSeed = (seed: string) => {
    setState(state => ({ ...state, romConfig: { ...state.romConfig, seed } }));
  };

  const setMode = (mode: 'create' | 'random' | 'patch') => {
    setState(state => ({ ...state, romConfig: { ...state.romConfig, mode } }));
  };

  const setIsPatch = (isPatch: boolean) => {
    setState(state => ({ ...state, isPatch }));
  };

  /* Async init */
  useEffect(() => {
    /* Roms */
    loadFile('oot').then(x => setRomConfigFileRaw('oot', x)).catch(console.error);
    loadFile('mm').then(x => setRomConfigFileRaw('mm', x)).catch(console.error);
  }, []);

  return (
    <GeneratorContext.Provider value={{ state, setState, setRomConfigFile, setSeed, setMode }}>
      {children}
    </GeneratorContext.Provider>
  );
}

export function useRomConfig() {
  const { state, setRomConfigFile, setSeed, setMode } = useContext(GeneratorContext);
  const { romConfig } = state;
  return { romConfig, setRomConfigFile, setSeed, setMode };
}

export function useGenerator() {
  const settings = useSettings();
  const cosmetics = useCosmetics();
  const random = useRandomSettings();
  const { state, setState } = useContext(GeneratorContext);
  const { generator } = state;
  const { isGenerating, message, progress, error, result, archive, warnings } = generator;

  const generate = async () => {
    setState((state) => ({ ...state, generator: { ...state.generator, isGenerating: true, archive: null, result: null, error: null, warnings: [] } }));
    const { oot, mm } = state.romConfig.files;
    let { patch } = state.romConfig.files;
    if (state.romConfig.mode !== 'patch') {
      patch = null;
    }
    const options: OptionsInput = { mode: state.romConfig.mode, seed: state.romConfig.seed, settings, random, cosmetics };
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
