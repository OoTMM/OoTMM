import { Buffer } from 'buffer';

import { options, OptionsInput } from './options';
import { Generator, GeneratorOutput } from './generator';
import { Monitor, MonitorCallbacks } from './monitor';
import { worldState } from './logic';
import { itemName } from './names';
import { isShuffled } from './logic/is-shuffled';
import { DEFAULT_SETTINGS, DUNGEONS, makeSettings, mergeSettings, SettingCategory, SETTINGS, Settings, SETTINGS_CATEGORIES, SPECIAL_CONDS, SPECIAL_CONDS_KEYS, TRICKS } from './settings';
import { SettingsPatch } from './settings/patch';
import { makeCosmetics } from './cosmetics';
import { makeRandomSettings } from './settings/random';
import { CountMap } from './util';
import { ItemHelpers, ItemsCount } from './items';

export { Presets, PRESETS } from './presets';
export { Cosmetics, makeCosmetics, COSMETIC_NAMES } from './cosmetics';
export { COLORS } from './cosmetics/color';
export { OptionRandomSettings, makeRandomSettings } from './settings/random';
export { SettingHint, SettingHintType, SETTINGS_DEFAULT_HINTS, HINT_TYPES } from './settings/hints';

export type GeneratorParams = {
  oot: Buffer,
  mm: Buffer,
  opts?: OptionsInput,
  monitor?: MonitorCallbacks
};

export type { GeneratorOutput, Settings, OptionsInput, SettingCategory, SettingsPatch };

type LocInfo = {
  [k: string]: string[]
}

export const generate = (params: GeneratorParams): Generator => {
  const opts = options(params.opts || {});
  return new Generator(params.oot, params.mm, opts, params.monitor || {});
};

export { SETTINGS, DEFAULT_SETTINGS, SETTINGS_CATEGORIES, TRICKS, itemName, DUNGEONS, mergeSettings, makeSettings, SPECIAL_CONDS, SPECIAL_CONDS_KEYS };

export const itemPool = (aSettings: Partial<Settings>): {[k: string]: number} => {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const cosmetics = makeCosmetics({});
  const monitor = new Monitor({ onLog: () => {} });
  const random = makeRandomSettings({});
  const { pool, world } = worldState(monitor, { settings, cosmetics, debug: false, seed: "--- INTERNAL ---", random });

  /* Extract relevant items from the pool */
  for (const item of pool.keys()) {
    if (ItemHelpers.isJunk(item) || ItemHelpers.isDungeonItem(item) || ItemHelpers.isToken(item) || ItemHelpers.isStrayFairy(item) || ItemHelpers.isItemTriforce(item)) {
      pool.delete(item);
    }
  }

  /* Add unlimited consumables */
  for (const loc in world.checks) {
    const check = world.checks[loc];
    const { item } = check;
    if (ItemHelpers.isItemUnlimitedStarting(item)) {
      pool.set(item, 999);
    }
  }

  /* Sort items */
  const items: ItemsCount = new Map;
  const sortedItems = [...pool.keys()].sort((a, b) => itemName(a.id).localeCompare(itemName(b.id)));
  for (const item of sortedItems) {
    items.set(item, pool.get(item)!);
  }

  /* Make the item pool */
  const itemPool: {[k: string]: number} = {};
  for (const [item, count] of items) {
    itemPool[item.id] = count;
  }
  return itemPool;
}

export const locationList = (aSettings: Partial<Settings>) => {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const cosmetics = makeCosmetics({});
  const monitor = new Monitor({ onLog: () => {} });
  const random = makeRandomSettings({});
  const { world, fixedLocations } = worldState(monitor, { settings, cosmetics, debug: false, seed: "--- INTERNAL ---", random });

  // Precalculate this to avoid doing it more than once in the gui
  const dungeonLocations = Object.values(world.dungeons).reduce((acc, x) => new Set([...acc, ...x]));

  /* Everywhere below Check.type is a placeholder for Check.flags that I am going to add to the item tables. */
  const locations: LocInfo = {};
  for (const loc in world.checks) {
    if (fixedLocations.has(loc) || !isShuffled(settings, world, loc, dungeonLocations)) {
      continue;
    }
    locations[loc] = [world.checks[loc].type];
  }

  return locations;
}
