import type { Settings } from '@ootmm/core';
import type {Item, ItemsCount} from '@ootmm/logic';

import { makeRandomSettings, makeCosmetics, Monitor } from '@ootmm/core';
import { worldState, ItemGroups, ItemHelpers, Items, makePlayerItem } from '@ootmm/logic';

import { itemName } from './names';

export type Items = {[k: string]: number};

function collapseBottleGroup(itemPool: Items, group: Set<Item>, replacement: Item, maxCount?: number) {
  let count = 0;
  for (const item of group) {
    count += itemPool[item.id] || 0;
    delete itemPool[item.id];
  }
  if (maxCount !== undefined)
    count = Math.min(count, maxCount);
  if (count > 0)
    itemPool[replacement.id] = count;
}

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
  for (const loc of worlds[0].locations) {
    const item = worlds[0].checkItems.get(loc)!;
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
  if (settings.bottleContentShuffle) {
    if (settings.sharedBottles) {
      collapseBottleGroup(itemPool, new Set([...ItemGroups.BOTTLES_OOT, ...ItemGroups.BOTTLES_MM, ...ItemGroups.BOTTLES_SHARED]), Items.SHARED_BOTTLE_RANDOM);
    } else {
      collapseBottleGroup(itemPool, ItemGroups.BOTTLES_OOT, Items.OOT_BOTTLE_RANDOM, settings.extraBottlesOot ? 6 : 4);
      collapseBottleGroup(itemPool, ItemGroups.BOTTLES_MM, Items.MM_BOTTLE_RANDOM);
    }
  }
  return itemPool;
}
