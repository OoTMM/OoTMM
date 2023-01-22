import { Buffer } from 'buffer';

import { options, OptionsInput } from './options';
import { Generator } from './generator';
import { MonitorCallbacks } from './monitor';
import { SETTINGS, DEFAULT_SETTINGS, SETTINGS_CATEGORIES, Settings, TRICKS } from './settings';
import { createWorld } from './logic/world';
import { alterWorld, configFromSettings } from './logic/settings';
import { Items } from './logic/state';
import { addItem, isDungeonItem, isDungeonReward, isJunk, isStrayFairy, isToken } from './logic/items';
import { EXTRA_ITEMS } from './logic/solve';

type GeneratorParams = {
  oot: Buffer,
  mm: Buffer,
  opts?: OptionsInput,
  monitor?: MonitorCallbacks
};

export const generate = (params: GeneratorParams): Generator => {
  const opts = options(params.opts || {});
  return new Generator(params.oot, params.mm, opts, params.monitor || {});
};

export { SETTINGS, DEFAULT_SETTINGS, SETTINGS_CATEGORIES, TRICKS };

export const itemPool = (aSettings: Partial<Settings>) => {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const world = createWorld(settings);
  const config = configFromSettings(settings);
  alterWorld(world, settings, config);

  /* Extract relevant items from the world */
  const items: Items = {};
  const rawItems: string[] = [];
  for (const loc in world.checks) {
    const check = world.checks[loc];
    const item = check.item;
    if (!isJunk(item) && !isDungeonReward(item) && !isDungeonItem(item) && !isToken(item) && !isStrayFairy(item)) {
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
  return items;
}
