import React, { createContext, useContext, useState } from 'react';
import { GeneratorOutput, Items, Settings, itemPool, OptionsInput, mergeSettings, makeSettings, SettingsPatch, Cosmetics } from '@ootmm/core';
import { Buffer } from 'buffer';
import { merge } from 'lodash';

import * as API from '../api';

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
    error: string | null;
    result: GeneratorOutput | null;
  },
  isPatch: boolean;
  settings: Settings;
  cosmetics: Cosmetics;
  itemPool: Items;
}

type GeneratorContext = {
  state: GeneratorState;
  setState: React.Dispatch<React.SetStateAction<GeneratorState>>;
  setFile: (key: keyof GeneratorState['romConfig']['files'], file: File) => void;
  setSeed: (seed: string) => void;
  setIsPatch: (isPatch: boolean) => void;
  setSettings: (settings: SettingsPatch) => Settings;
  setCosmetics: (cosmetics: Partial<Cosmetics>) => Cosmetics;
  overrideSettings: (settings: Settings) => Settings;
}

export const GeneratorContext = createContext<GeneratorContext>(null as any);

function createState(): GeneratorState {
  const settings = API.initialSettings();
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

  const setFile = (key: keyof GeneratorState['romConfig']['files'], file: File) => {
    setState({ ...state, romConfig: { ...state.romConfig, files: { ...state.romConfig.files, [key]: file } }});
  };

  const setSeed = (seed: string) => {
    setState({ ...state, romConfig: { ...state.romConfig, seed } });
  };

  const setIsPatch = (isPatch: boolean) => {
    setState({ ...state, isPatch });
  };

  const setSettings = (patch: SettingsPatch) => {
    const newSettings = mergeSettings(state.settings, patch);
    setState({ ...state, settings: newSettings });
    return newSettings;
  };

  const overrideSettings = (settings: Settings) => {
    setState({ ...state, settings });
    return settings;
  };

  const setCosmetics = (patch: Partial<Cosmetics>) => {
    const cosmetics = merge({}, state.cosmetics, patch);
    setState({ ...state, cosmetics });
    return cosmetics;
  }

  return (
    <GeneratorContext.Provider value={{ state, setState, setFile, setSeed, setIsPatch, setSettings, overrideSettings, setCosmetics }}>
      {children}
    </GeneratorContext.Provider>
  );
}

export const useRomConfig = () => {
  const { state, setFile, setSeed } = useContext(GeneratorContext);
  const { romConfig } = state;
  return { romConfig, setFile, setSeed };
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
    const [oot, mm] = (await Promise.all([state.romConfig.files.oot, state.romConfig.files.mm].map((file) => file!.arrayBuffer()))).map(x => Buffer.from(x));
    let patch: Buffer | undefined;
    if (state.isPatch) {
      patch = Buffer.from(await state.romConfig.files.patch!.arrayBuffer());
    }
    const options: OptionsInput = { seed: state.romConfig.seed, settings: state.settings, cosmetics: state.cosmetics };
    try {
      const result = await API.generate({ oot, mm, patch }, options, (message) => {
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
      const count = (newStartingItems[item] || 0) + value;
      if (count <= 0) {
        delete newStartingItems[item];
      } else if (count > state.itemPool[item]) {
        newStartingItems[item] = state.itemPool[item];
      } else {
        newStartingItems[item] = count;
      }
      const newSettings = { ...state.settings, startingItems: newStartingItems };
      localStorage.setItem('settings', JSON.stringify(newSettings));
      return { ...state, settings: newSettings };
    });
  };

  const reset = () => {
    const newSettings = { ...state.settings, startingItems: {} };
    localStorage.setItem('settings', JSON.stringify(newSettings));
    setState({ ...state, settings: newSettings });
  };

  const incr = (item: string) => alterItem(item, 1);
  const decr = (item: string) => alterItem(item, -1);

  return { startingItems, itemPool, incr, decr, reset };
}

export function useCosmetics() {
  const ctx = useContext(GeneratorContext);
  const { cosmetics } = ctx.state;
  const setCosmetics = (patch: Partial<Cosmetics>) => {
    const newCosmetics = ctx.setCosmetics(patch);
    localStorage.setItem('cosmetics', JSON.stringify(newCosmetics));
    return newCosmetics;
  }
  return [cosmetics, setCosmetics] as const;
}
