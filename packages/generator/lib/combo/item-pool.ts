import { Monitor, Settings, ItemHelpers, ItemsCount, makePlayerItem } from '@ootmm/core';

import { makeCosmetics } from './cosmetics';
import { itemName } from './names';
import { makeRandomSettings } from './random-settings';

/* TODO: Fix this */
/* This file should likely be in logic too */
import { worldState } from '../../../logic/src';

export type Items = {[k: string]: number};

export async function itemPool(settings: Settings): Promise<Items> {
  const cosmetics = makeCosmetics({});
  const monitor = new Monitor({ onLog: () => {} });
  const random = makeRandomSettings({});
  const { pool, worlds, itemProperties } = await worldState(monitor, { settings, cosmetics, seed: "--- INTERNAL ---", random, mode: 'create' });

  /* Extract relevant items from the pool */
  for (const pi of pool.keys()) {
    if (pi.player !== 0 || itemProperties.junk.has(pi.item) || ItemHelpers.isDungeonItem(pi.item) || ItemHelpers.isToken(pi.item) || ItemHelpers.isStrayFairy(pi.item)) {
      pool.delete(pi);
    }
  }

  /* Add unlimited consumables */
  for (const loc in worlds[0].checks) {
    const check = worlds[0].checks[loc];
    const { item } = check;
    const pi = makePlayerItem(item, 0);
    if (ItemHelpers.isItemUnlimitedStarting(item)) {
      pool.set(pi, 999);
    }
  }

  /* Sort items */
  const items: ItemsCount = new Map;
  const sortedItems = [...pool.keys()].sort((a, b) => itemName(a.item.id).localeCompare(itemName(b.item.id)));
  for (const item of sortedItems) {
    items.set(item.item, pool.get(item)!);
  }

  /* Make the item pool */
  const itemPool: Items = {};
  for (const [item, count] of items) {
    itemPool[item.id] = count;
  }
  return itemPool;
}
