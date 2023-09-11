import React, { createContext, useContext, useState } from 'react';
import { GeneratorOutput, Items, Settings, itemPool, OptionsInput, mergeSettings, makeSettings, SettingsPatch, Cosmetics, OptionRandomSettings } from '@ootmm/core';
import { Buffer } from 'buffer';
import { merge } from 'lodash';

import * as API from '../api';

type GeneratorState = {
  romConfig: {
    files: {
      oot: Buffer | null;
      mm: Buffer | null;
      patch: Buffer | null;
    },
    seed: string;
  },
  files: {[k: string]: File | null},
  generator: {
    isGenerating: boolean;
    message: string | null;
    error: string | null;
    result: GeneratorOutput | null;
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
  setFile: (key: string, file: File) => void;
  setFileBuffer: (key: keyof GeneratorState['romConfig']['files'], file: Buffer) => void;
  setSeed: (seed: string) => void;
  setIsPatch: (isPatch: boolean) => void;
  setSettings: (settings: SettingsPatch) => Settings;
  setCosmetics: (cosmetics: Partial<Cosmetics>) => Cosmetics;
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
    files: {},
    isPatch: false,
    settings,
    random,
    cosmetics,
    itemPool: ip,
    generator: {
      isGenerating: false,
      message: null,
      error: null,
      result: null,
    }
  };
}

export function GeneratorContextProvider({ children }: { children: React.ReactNode }) {
  const [state, setState] = useState(createState);

  const setFileBuffer = (key: keyof GeneratorState['romConfig']['files'], file: Buffer) => {
    setState(state => ({ ...state, romConfig: { ...state.romConfig, files: { ...state.romConfig.files, [key]: file } }}));
  };

  const setFile = (key: string, file: File) => {
    setState(state => ({ ...state, files: { ...state.files, [key]: file }}));
  };

  const setSeed = (seed: string) => {
    setState(state => ({ ...state, romConfig: { ...state.romConfig, seed } }));
  };

  const setIsPatch = (isPatch: boolean) => {
    setState(state => ({ ...state, isPatch }));
  };

  const setSettings = (patch: SettingsPatch) => {
    const newSettings = mergeSettings(state.settings, patch);
    setState(state => ({ ...state, settings: newSettings }));
    return newSettings;
  };

  const overrideSettings = (settings: Settings) => {
    setState(state => ({ ...state, settings }));
    return settings;
  };

  const setCosmetics = (patch: Partial<Cosmetics>) => {
    const cosmetics = merge({}, state.cosmetics, patch);
    setState(state => ({ ...state, cosmetics }));
    return cosmetics;
  };

  const setRandomSettings = (patch: Partial<OptionRandomSettings>) => {
    const random = merge({}, state.random, patch);
    setState(state => ({ ...state, random }));
    return random;
  };

  return (
    <GeneratorContext.Provider value={{ state, setState, setFileBuffer, setFile, setSeed, setIsPatch, setSettings, overrideSettings, setCosmetics, setRandomSettings }}>
      {children}
    </GeneratorContext.Provider>
  );
}

export const useRomConfig = () => {
  const { state, setFileBuffer, setSeed } = useContext(GeneratorContext);
  const { romConfig } = state;
  return { romConfig, setFileBuffer, setSeed };
}

export const useFiles = () => {
  const { state, setFile } = useContext(GeneratorContext);
  const { files } = state;
  return [files, setFile] as const;
}

export function useIsPatch() {
  const { state, setIsPatch } = useContext(GeneratorContext);
  return [state.isPatch, setIsPatch] as const;
}

export function useGenerator() {
  const { state, setState } = useContext(GeneratorContext);
  const { generator } = state;
  const { isGenerating, message, error, result } = generator;

  const generate = async () => {
    setState((state) => ({ ...state, generator: { ...state.generator, isGenerating: true } }));
    const { oot, mm, patch } = state.romConfig.files;
    const options: OptionsInput = { seed: state.romConfig.seed, settings: state.settings, cosmetics: state.cosmetics, random: state.random };
    try {
      const result = await API.generate({ oot: oot!, mm: mm!, patch: patch ? patch : undefined }, options, (message) => {
        console.log(message);
        setState((state) => ({ ...state, generator: { ...state.generator, message } }));
      });
      setState((state) => ({ ...state, generator: { ...state.generator, isGenerating: false, result } }));
    } catch (e: any) {
      setState((state) => ({ ...state, generator: { ...state.generator, isGenerating: false, error: e.toString() } }));
    }
  };

  return { isGenerating, message, error, result, generate };
}

export function useSettings() {
  const ctx = useContext(GeneratorContext);
  const setSettings = (patch: SettingsPatch) => {
    const newSettings = ctx.setSettings(patch);
    localStorage.setItem('settings', JSON.stringify(newSettings));
    API.itemPoolFromSettings(newSettings).then((itemPool) => {
      ctx.setState((state) => {
        const startingItems = API.restrictItemsByPool(state.settings.startingItems, itemPool);
        const newSettings = { ...state.settings, startingItems };
        localStorage.setItem('settings', JSON.stringify(newSettings));
        return { ...state, settings: makeSettings(newSettings), itemPool };
      });
    });
  };
  return [ctx.state.settings, setSettings] as const;
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
  const setCosmetics = (patch: Partial<Cosmetics>) => {
    const newCosmetics = ctx.setCosmetics(patch);
    const savedCosmetics = { ...newCosmetics };
    for (const key of Object.keys(savedCosmetics)) {
      const v = (savedCosmetics as any)[key];
      if (Buffer.isBuffer(v)) {
        delete (savedCosmetics as any)[key];
      }
    }
    localStorage.setItem('cosmetics', JSON.stringify(savedCosmetics));
    return newCosmetics;
  }
  return [cosmetics, setCosmetics] as const;
}
