import { create } from 'zustand';
import { createSettingsSlice, SettingsSlice } from './settings';
import * as API from '../api';
import { isEqual } from 'lodash';
import { makeSettings } from '@ootmm/core';
import { localStoragePrefixedSet } from '../util';
import { createRandomSettingsSlice, RandomSettingsSlice } from './randomSettings';

export type Store = SettingsSlice & RandomSettingsSlice;

export const useStore = create<Store>((...a) => ({
  ...createSettingsSlice(...a),
  ...createRandomSettingsSlice(...a),
}));

let settingsUpdateTicket = 0;

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

useStore.subscribe((state, prevState) => {
  if (state.settings !== prevState.settings) {
    onSettingsUpdate();
  }
  if (state.randomSettings !== prevState.randomSettings) {
    onRandomSettingsUpdate();
  }
});
onSettingsUpdate();
onRandomSettingsUpdate();
