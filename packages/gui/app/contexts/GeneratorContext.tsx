import React, { createContext, useContext, useEffect, useState } from 'react';
import { GeneratorOutput, Items, Settings, itemPool, OptionsInput, mergeSettings, makeSettings, SettingsPatch, Cosmetics, OptionRandomSettings, COSMETICS } from '@ootmm/core';
import { merge } from 'lodash';

import * as API from '../api';
import { loadFile, saveFile } from '../db';

let settingsTicket = 0;

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
  settings: Settings;
  random: OptionRandomSettings;
  cosmetics: Cosmetics;
  itemPool: Items;
}

type GeneratorContext = {
  state: GeneratorState;
  setState: React.Dispatch<React.SetStateAction<GeneratorState>>;
  setRomConfigFile: (key: keyof GeneratorState['romConfig']['files'], file: File | null) => void;
  setSeed: (seed: string) => void;
  setIsPatch: (isPatch: boolean) => void;
  setSettings: (settings: SettingsPatch) => Settings;
  setCosmetic: (key: string, value: any) => Cosmetics;
  overrideSettings: (settings: Settings) => Settings;
  setRandomSettings: (random: Partial<OptionRandomSettings>) => OptionRandomSettings;
}

export const GeneratorContext = createContext<GeneratorContext>(null as any);

function createState(): GeneratorState {
  const settings = API.initialSettings();
  const random = API.initialRandomSettings();
  const cosmetics = API.initialCosmetics();
  const ip = itemPool(settings);
  settings.startingItems = API.restrictItemsByPool(settings.startingItems, ip);
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
    settings,
    random,
    cosmetics,
    itemPool: ip,
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

export function GeneratorContextProvider({ children }: { children: React.ReactNode }) {
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

  const overrideSettings = (settings: Settings) => {
    const ticket = ++settingsTicket;
    setState(state => ({ ...state, settings }));
    localStorage.setItem('settings', JSON.stringify(settings));
    API.itemPoolFromSettings(settings).then((itemPool) => {
      if (ticket !== settingsTicket) {
        return;
      }
      setState((state) => {
        const startingItems = API.restrictItemsByPool(state.settings.startingItems, itemPool);
        const newSettings = { ...state.settings, startingItems };
        localStorage.setItem('settings', JSON.stringify(newSettings));
        return { ...state, settings: makeSettings(newSettings), itemPool };
      });
    });
    return settings;
  };

  const setSettings = (patch: SettingsPatch) => {
    const newSettings = mergeSettings(state.settings, patch);
    return overrideSettings(newSettings);
  };

  const setCosmeticRaw = (key: string, value: any) => {
    const newCosmetics = merge({}, state.cosmetics, { [key]: value });
    setState(state => ({ ...state, cosmetics: newCosmetics }));
    return newCosmetics;
  };

  const setCosmetic = (key: string, value: any) => {
    const newCosmetics = setCosmeticRaw(key, value);

    /* Save to localStorage */
    const savedCosmetics = { ...newCosmetics };
    for (const key of Object.keys(savedCosmetics)) {
      const v = (savedCosmetics as any)[key];
      if (v instanceof File) {
        delete (savedCosmetics as any)[key];
      }
    }
    localStorage.setItem('cosmetics', JSON.stringify(savedCosmetics));

    /* Save new file */
    const cosmeticData = COSMETICS.find(c => c.key === key);
    if (cosmeticData && (cosmeticData.type === 'file')) {
      saveFile(`cosmetics:${key}`, value).catch(console.error);
    }

    return newCosmetics;
  };

  const setRandomSettings = (patch: Partial<OptionRandomSettings>) => {
    const random = merge({}, state.random, patch);
    setState(state => ({ ...state, random }));
    return random;
  };

  /* Async file load */
  useEffect(() => {
    /* Roms */
    loadFile('oot').then(x => setRomConfigFileRaw('oot', x)).catch(console.error);
    loadFile('mm').then(x => setRomConfigFileRaw('mm', x)).catch(console.error);

    /* Cosmetics */
    for (const c of COSMETICS) {
      if (c.type === 'file') {
        loadFile(`cosmetics:${c.key}`).then(x => setCosmeticRaw(c.key, x)).catch(console.error);
      }
    }
  }, []);

  return (
    <GeneratorContext.Provider value={{ state, setState, setRomConfigFile, setSeed, setIsPatch, setSettings, overrideSettings, setCosmetic, setRandomSettings }}>
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
  const { state, setState } = useContext(GeneratorContext);
  const { generator } = state;
  const { isGenerating, message, progress, error, result, archive, warnings } = generator;

  const generate = async () => {
    setState((state) => ({ ...state, generator: { ...state.generator, isGenerating: true, archive: null, result: null, error: null, warnings: [] } }));
    const { oot, mm, patch } = state.romConfig.files;
    const options: OptionsInput = { seed: state.romConfig.seed, settings: state.settings, cosmetics: state.cosmetics, random: state.random };
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

export function useSettings() {
  const ctx = useContext(GeneratorContext);
  return [ctx.state.settings, ctx.setSettings] as const;
}

export function useRandomSettings() {
  const ctx = useContext(GeneratorContext);
  const setRandomSettings = (patch: Partial<OptionRandomSettings>) => {
    const newRandomSettings = ctx.setRandomSettings(patch);
    localStorage.setItem('randomSettings', JSON.stringify(newRandomSettings));
    return newRandomSettings;
  };
  return [ctx.state.random, setRandomSettings] as const;
}

export function useOverrideSettings() {
  const ctx = useContext(GeneratorContext);
  return ctx.overrideSettings;
}

export function useItemPool() {
  const { state } = useContext(GeneratorContext);
  return state.itemPool;
}

export function useStartingItems() {
  const { state, setState } = useContext(GeneratorContext);
  const { itemPool, settings } = state;
  const { startingItems } = settings;

  const alterItem = (item: string, value: number) => {
    setState((state) => {
      const newStartingItems = { ...state.settings.startingItems };
      if (value <= 0) {
        delete newStartingItems[item];
      } else if (value > state.itemPool[item]) {
        newStartingItems[item] = state.itemPool[item];
      } else {
        newStartingItems[item] = value;
      }
      const newSettings = { ...state.settings, startingItems: newStartingItems };
      localStorage.setItem('settings', JSON.stringify(newSettings));
      return { ...state, settings: newSettings };
    });
  };

  const reset = () => {
    const newSettings = { ...state.settings, startingItems: {} };
    localStorage.setItem('settings', JSON.stringify(newSettings));
    setState(state => ({ ...state, settings: newSettings }));
  };


  return { startingItems, itemPool, alterItem, reset };
}

export function useCosmetics() {
  const ctx = useContext(GeneratorContext);
  const { cosmetics } = ctx.state;
  const { setCosmetic } = ctx;
  return [cosmetics, setCosmetic] as const;
}
