import { Buffer } from 'buffer';

import { options, OptionsInput } from './options';
import { Generator, GeneratorOutput } from './generator';
import { Monitor, MonitorCallbacks } from './monitor';
import { worldState } from './logic';
import { itemName } from './names';
import { isDungeonItem, isItemTriforce, isItemUnlimitedStarting, isJunk, isStrayFairy, isToken, Items } from './logic/items';
import { isShuffled } from './logic/is-shuffled';
import { DEFAULT_SETTINGS, DUNGEONS, makeSettings, mergeSettings, SettingCategory, SETTINGS, Settings, SETTINGS_CATEGORIES, SPECIAL_CONDS, SPECIAL_CONDS_KEYS, TRICKS } from './settings';
import { SettingsPatch } from './settings/patch';
import { makeCosmetics } from './cosmetics';
import { makeRandomSettings } from './settings/random';

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

export type { GeneratorOutput, Settings, OptionsInput, Items, SettingCategory, SettingsPatch };

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
  const cosmetics = makeCosmetics({});
  const monitor = new Monitor({ onLog: () => {} });
  const random = makeRandomSettings({});
  const { pool, world } = worldState(monitor, { settings, cosmetics, debug: false, seed: "--- INTERNAL ---", random });

  /* Extract relevant items from the pool */
  for (const item of Object.keys(pool)) {
    if (isJunk(item) || isDungeonItem(item) || isToken(item) || isStrayFairy(item) || isItemTriforce(item)) {
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
