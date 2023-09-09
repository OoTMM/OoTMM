import { Buffer } from 'buffer';

import { options, OptionsInput } from './options';
import { Generator, GeneratorOutput } from './generator';
import { Monitor, MonitorCallbacks } from './monitor';
import { worldState } from './logic';
import { itemName } from './names';
import { isShuffled } from './logic/is-shuffled';
import { DEFAULT_SETTINGS, DUNGEONS, GLITCHES, makeSettings, mergeSettings, SettingCategory, SETTINGS, Settings, SETTINGS_CATEGORIES, SPECIAL_CONDS, SPECIAL_CONDS_FIELDS, TRICKS } from './settings';
import { SettingsPatch } from './settings/patch';
import { makeCosmetics } from './cosmetics';
import { makeRandomSettings } from './settings/random';
import { ItemHelpers, ItemsCount, makePlayerItem } from './items';
import { makeLocation } from './logic/locations';

export { Presets, PRESETS } from './presets';
export { COSMETICS, makeCosmetics, Cosmetics } from './cosmetics';
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

export { SETTINGS, DEFAULT_SETTINGS, SETTINGS_CATEGORIES, TRICKS, GLITCHES, itemName, DUNGEONS, mergeSettings, makeSettings, SPECIAL_CONDS, SPECIAL_CONDS_FIELDS };

export const itemPool = (aSettings: Partial<Settings>): {[k: string]: number} => {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const cosmetics = makeCosmetics({});
  const monitor = new Monitor({ onLog: () => {} });
  const random = makeRandomSettings({});
  const { pool, worlds } = worldState(monitor, { settings, cosmetics, debug: false, seed: "--- INTERNAL ---", random });

  /* Extract relevant items from the pool */
  for (const pi of pool.keys()) {
    if (pi.player !== 0 || ItemHelpers.isJunk(pi.item) || ItemHelpers.isDungeonItem(pi.item) || ItemHelpers.isToken(pi.item) || ItemHelpers.isStrayFairy(pi.item)) {
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
  const { worlds, fixedLocations } = worldState(monitor, { settings, cosmetics, debug: false, seed: "--- INTERNAL ---", random });

  // Precalculate this to avoid doing it more than once in the gui
  const dungeonLocations = Object.values(worlds[0].dungeons).reduce((acc, x) => new Set([...acc, ...x]));

  /* Everywhere below Check.type is a placeholder for Check.flags that I am going to add to the item tables. */
  const locations: LocInfo = {};
  for (const loc in worlds[0].checks) {
    const pl = makeLocation(loc, 0);
    if (fixedLocations.has(pl) || !isShuffled(settings, worlds[0], loc, dungeonLocations)) {
      continue;
    }
    locations[loc] = [worlds[0].checks[loc].type];
  }

  return locations;
}
