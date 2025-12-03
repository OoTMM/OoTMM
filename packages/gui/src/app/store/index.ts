import { create } from 'zustand';
import { createSettingsSlice, SettingsSlice } from './settings';
import * as API from '../api';
import { isEqual } from 'lodash';
import { Cosmetics, COSMETICS, makeSettings } from '@ootmm/core';
import { localStoragePrefixedSet } from '../util';
import { createRandomSettingsSlice, RandomSettingsSlice } from './randomSettings';
import { CosmeticsSlice, createCosmeticsSlice } from './cosmetics';
import { loadFile, loadFileLocal, saveFileLocal } from '../db';
import { ConfigSlice, createConfigSlice } from './config';
import { createGeneratorSlice, GeneratorSlice } from './generator';

export type Store = SettingsSlice & RandomSettingsSlice & CosmeticsSlice & ConfigSlice & GeneratorSlice;

export const useStore = create<Store>((...a) => ({
  ...createSettingsSlice(...a),
  ...createRandomSettingsSlice(...a),
  ...createCosmeticsSlice(...a),
  ...createConfigSlice(...a),
  ...createGeneratorSlice(...a),
}));

let settingsUpdateTicket = 0;
let cosmeticFilesLoaded = false;

function onSettingsUpdate() {
  localStoragePrefixedSet('settings', useStore.getState().settings);
  const currentTicket = ++settingsUpdateTicket;
  let state = useStore.getState();
  Promise.all([
    API.itemPool(state.settings),
    API.locationList(state.settings),
  ]).then(([newItemPool, newLocations]) => {
    if (settingsUpdateTicket !== currentTicket) return;

    /* Update item pool and locations in store */
    state = useStore.getState();
    useStore.setState({
      itemPool: newItemPool,
      locations: newLocations,
    });

    /* Update starting items if needed */
    const startingItems = API.restrictItemsByPool(state.settings.startingItems, newItemPool);
    if (isEqual(state.settings.startingItems, startingItems)) return;
    state.setSettings(state => makeSettings({ ...state, startingItems }));
  });
}

function onRandomSettingsUpdate() {
  localStoragePrefixedSet('randomSettings', useStore.getState().randomSettings);
}

const COSMETICS_FILE_KEYS = COSMETICS.filter(c => c.type === 'file').map(c => c.key);

function onCosmeticsUpdate(prev: Cosmetics, curr: Cosmetics) {
  const state = useStore.getState();
  const savedCosmetics = { ...state.cosmetics };
  for (const c of COSMETICS_FILE_KEYS) {
    delete savedCosmetics[c];
  }
  localStoragePrefixedSet('cosmetics', savedCosmetics);

  if (cosmeticFilesLoaded) {
    for (const c of COSMETICS_FILE_KEYS) {
      const data = curr[c];
      const prevData = prev[c];
      if (prevData !== data) {
        saveFileLocal(`cosmetics:${c}`, data as File).catch(console.error);
      }
    }
  }
}

useStore.subscribe((state, prevState) => {
  if (state.settings !== prevState.settings) {
    onSettingsUpdate();
  }
  if (state.randomSettings !== prevState.randomSettings) {
    onRandomSettingsUpdate();
  }
  if (state.cosmetics !== prevState.cosmetics) {
    onCosmeticsUpdate(prevState.cosmetics, state.cosmetics);
  }
});

onSettingsUpdate();
onRandomSettingsUpdate();
onCosmeticsUpdate(useStore.getState().cosmetics, useStore.getState().cosmetics);

/* Initial load of cosmetic files */
const cosmeticsFiles = COSMETICS_FILE_KEYS.map(c => loadFileLocal(`cosmetics:${c}`).then(x => useStore.getState().setCosmetic(c, x)).catch(console.error));
Promise.allSettled(cosmeticsFiles).finally(() => cosmeticFilesLoaded = true);

/* Initial load of config */
loadFile('oot').then(x => useStore.getState().setRomConfigFile('oot', x)).catch(console.error);
loadFile('mm').then(x => useStore.getState().setRomConfigFile('mm', x)).catch(console.error);
