import { create } from 'zustand';

import type { Store } from './types';
import { createSettingsSlice } from './settings';
import * as API from '../api';
import { isEqual } from 'lodash';
import { makeSettings } from '@ootmm/core';
import { localStoragePrefixedSet } from '../util';

export const useStore = create<Store>((...a) => ({
  settings: createSettingsSlice(...a),
}));

let settingsUpdateTicket = 0;

function onSettingsUpdate() {
  localStoragePrefixedSet('settings', useStore.getState().settings.settings);
  const currentTicket = ++settingsUpdateTicket;
  let state = useStore.getState();
  Promise.all([
    API.itemPool(state.settings.settings),
    API.locationList(state.settings.settings),
  ]).then(([newItemPool, newLocations]) => {
    if (settingsUpdateTicket !== currentTicket) return;

    /* Update item pool and locations in store */
    state = useStore.getState();
    useStore.setState({
      settings: {
        ...state.settings,
        itemPool: newItemPool,
        locations: newLocations,
      }
    });

    /* Update starting items if needed */
    const startingItems = API.restrictItemsByPool(state.settings.settings.startingItems, newItemPool);
    if (isEqual(state.settings.settings.startingItems, startingItems)) return;
    state.settings.setSettings(state => makeSettings({ ...state, startingItems }));
  });
}

useStore.subscribe((state, prevState) => {
  if (state.settings.settings !== prevState.settings.settings) {
    onSettingsUpdate();
  }
});
onSettingsUpdate();
