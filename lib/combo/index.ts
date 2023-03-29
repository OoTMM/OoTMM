import { Buffer } from 'buffer';

import { options, OptionsInput } from './options';
import { Generator, GeneratorOutput } from './generator';
import { Monitor, MonitorCallbacks } from './monitor';
import { SETTINGS, DEFAULT_SETTINGS, SETTINGS_CATEGORIES, Settings, TRICKS, DUNGEONS, SPECIAL_CONDS, makeSettings, mergeSettings, SPECIAL_CONDS_KEYS, SettingCategory } from './settings';
import { worldState } from './logic';
import { itemName } from './names';
import { isDungeonItem, isDungeonReward, isItemUnlimitedStarting, isJunk, isStrayFairy, isToken, Items } from './logic/items';
import { isShuffled } from './logic/is-shuffled';

export type GeneratorParams = {
  oot: Buffer,
  mm: Buffer,
  opts?: OptionsInput,
  monitor?: MonitorCallbacks
};

export type { GeneratorOutput, Settings, OptionsInput, Items, SettingCategory };

type LocInfo = {
  [k: string]: string[]
}

export const generate = (params: GeneratorParams): Generator => {
  const opts = options(params.opts || {});
  return new Generator(params.oot, params.mm, opts, params.monitor || {});
};

export { SETTINGS, DEFAULT_SETTINGS, SETTINGS_CATEGORIES, TRICKS, itemName, DUNGEONS, mergeSettings, makeSettings, SPECIAL_CONDS, SPECIAL_CONDS_KEYS };

export const itemPool = (aSettings: Partial<Settings>) => {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const monitor = new Monitor({ onLog: () => {} });
  const { pool, world } = worldState(monitor, { settings, debug: false, seed: "--- INTERNAL ---" });

  /* Extract relevant items from the pool */
  for (const item of Object.keys(pool)) {
    if (isJunk(item) || isDungeonItem(item) || isToken(item) || isStrayFairy(item)) {
      delete pool[item];
    }
  }

  /* Add unlimited consumables */
  for (const loc in world.checks) {
    const check = world.checks[loc];
    const { item } = check;
    if (isItemUnlimitedStarting(item)) {
      pool[item] = 999;
    }
  }

  /* Sort items */
  const items: Items = {};
  const sortedItems = Object.keys(pool).sort((a, b) => itemName(a).localeCompare(itemName(b)));
  for (const item of sortedItems) {
    items[item] = pool[item];
  }

  return items;
}

export const locationList = (aSettings: Partial<Settings>) => {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const monitor = new Monitor({ onLog: () => {} });
  const { world, fixedLocations } = worldState(monitor, { settings, debug: false, seed: "--- INTERNAL ---" });

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
