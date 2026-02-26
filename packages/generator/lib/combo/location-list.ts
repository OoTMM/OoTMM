import { Settings, DEFAULT_SETTINGS } from '@ootmm/core';

import { makeRandomSettings } from './random-settings';
import { makeCosmetics } from './cosmetics';
import { worldState } from '../../../logic/src';
import { isShuffled } from '../../../logic/src/is-shuffled';
import { makeLocation } from '../../../logic/src/locations';
import { Monitor } from '@ootmm/core/src/monitor';

export async function locationList(aSettings: Partial<Settings>) {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const cosmetics = makeCosmetics({});
  const monitor = new Monitor({ onLog: () => {} });
  const random = makeRandomSettings({});
  const { worlds, fixedLocations } = await worldState(monitor, { settings, cosmetics, seed: "--- INTERNAL ---", random, mode: 'create' });

  // Precalculate this to avoid doing it more than once in the gui
  const dungeonLocations = Object.values(worlds[0].dungeons).reduce((acc, x) => new Set([...acc, ...x]));

  const locations: string[] = [];
  for (const loc in worlds[0].checks) {
    const pl = makeLocation(loc, 0);
    if (fixedLocations.has(pl) || !isShuffled(settings, worlds[0], loc, dungeonLocations)) {
      continue;
    }
    locations.push(loc);
  }

  return locations.sort();
}
