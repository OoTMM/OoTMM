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
  isPatch: boolean;
}

type GeneratorContext = {
  state: GeneratorState;
  setState: Dispatch<StateUpdater<GeneratorState>>;
  setRomConfigFile: (key: keyof GeneratorState['romConfig']['files'], file: File | null) => void;
  setSeed: (seed: string) => void;
  setIsPatch: (isPatch: boolean) => void;
}

export const GeneratorContext = createContext<GeneratorContext>(null as any);

function createState(): GeneratorState {
  const settings = API.initialSettings();

  return {
    romConfig: {
      files: {
        oot: null,
        mm: null,
        patch: null,
      },
      seed: '',
    },
    isPatch: false,
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
    <GeneratorContext.Provider value={{ state, setState, setRomConfigFile, setSeed, setIsPatch }}>
      {children}
    </GeneratorContext.Provider>
  );
}

export function useRomConfig() {
  const { state, setRomConfigFile, setSeed } = useContext(GeneratorContext);
  const { romConfig } = state;
  return { romConfig, setRomConfigFile, setSeed };
}

export function useIsPatch() {
  const { state, setIsPatch } = useContext(GeneratorContext);
  return [state.isPatch, setIsPatch] as const;
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
    const { oot, mm, patch } = state.romConfig.files;
    const options: OptionsInput = { seed: state.romConfig.seed, settings, random, cosmetics };
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
