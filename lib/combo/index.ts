import { Buffer } from 'buffer';

import { options, OptionsInput } from './options';
import { Generator, GeneratorOutput } from './generator';
import { Monitor, MonitorCallbacks } from './monitor';
import { SETTINGS, DEFAULT_SETTINGS, SETTINGS_CATEGORIES, Settings, TRICKS } from './settings';
import { worldState } from './logic';
import { itemName } from './names';
import { addItem, isDungeonItem, isDungeonReward, isItemUnlimitedStarting, isJunk, isStrayFairy, isToken, Items } from './logic/items';
import { EXTRA_ITEMS } from './logic/solve';
import { isShuffled } from './logic/is-shuffled';

export type GeneratorParams = {
  oot: Buffer,
  mm: Buffer,
  opts?: OptionsInput,
  monitor?: MonitorCallbacks
};

export type { GeneratorOutput, Settings, OptionsInput, Items };

type LocInfo = {
  [k: string]: string[]
}

export const generate = (params: GeneratorParams): Generator => {
  const opts = options(params.opts || {});
  return new Generator(params.oot, params.mm, opts, params.monitor || {});
};

export { SETTINGS, DEFAULT_SETTINGS, SETTINGS_CATEGORIES, TRICKS, itemName };

export const itemPool = (aSettings: Partial<Settings>) => {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const monitor = new Monitor({ onLog: () => {} });
  const { world, fixedLocations } = worldState(monitor, settings);

  /* Extract relevant items from the world */
  const items: Items = {};
  const rawItems: string[] = [];
  for (const loc in world.checks) {
    const check = world.checks[loc];
    const item = check.item;
    if (!fixedLocations.has(loc) && !isJunk(item) && !isDungeonReward(item) && !isDungeonItem(item) && !isToken(item) && !isStrayFairy(item)) {
      rawItems.push(item);
    }
  }
  for (const extraItem of EXTRA_ITEMS) {
    rawItems.push(extraItem);
  }
  rawItems.sort();
  for (const item of rawItems) {
    addItem(items, item);
  }

  /* Add unlimited consumables */
  for (const loc in world.checks) {
    const check = world.checks[loc];
    const item = check.item;
    if (isItemUnlimitedStarting(item)) {
      items[item] = 999;
    }
  }

  return items;
}

export const locationList = (aSettings: Partial<Settings>) => {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const monitor = new Monitor({ onLog: () => {} });
  const { world, fixedLocations } = worldState(monitor, settings);

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
