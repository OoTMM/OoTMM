import type { Settings } from '@ootmm/core';

import { makeCosmetics, makeRandomSettings, Monitor, DEFAULT_SETTINGS } from '@ootmm/core';
import { worldState, makeLocation, isShuffled } from '@ootmm/logic';

export async function locationList(aSettings: Partial<Settings>) {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const cosmetics = makeCosmetics({});
  const monitor = new Monitor({ onLog: () => {} });
  const random = makeRandomSettings({});
  const { worlds, fixedLocations } = await worldState(monitor, { settings, cosmetics, seed: "--- INTERNAL ---", random, mode: 'create' });

  // Precalculate this to avoid doing it more than once in the gui
  const dungeonLocations = Object.values(worlds[0].dungeons).reduce((acc, x) => new Set([...acc, ...x]));

  const locations: string[] = [];
  for (const loc of worlds[0].locations) {
    const pl = makeLocation(loc, 0);
    if (fixedLocations.has(pl) || !isShuffled(settings, worlds[0], loc, dungeonLocations)) {
      continue;
    }
    locations.push(loc);
  }

  return locations.sort();
}
