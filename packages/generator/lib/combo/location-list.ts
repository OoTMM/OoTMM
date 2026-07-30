import type { Settings } from '@ootmm/core';

import { makeCosmetics, makeRandomSettings, Monitor, DEFAULT_SETTINGS } from '@ootmm/core';
import { worldState, makeLocation } from '@ootmm/logic';

export async function locationList(aSettings: Partial<Settings>) {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const cosmetics = makeCosmetics({});
  const monitor = new Monitor({ onLog: () => {} });
  const random = makeRandomSettings({});
  const { worlds, fixedLocations } = await worldState(monitor, { settings, cosmetics, seed: "--- INTERNAL ---", random, mode: 'create' });

  const locations: string[] = [];
  for (const loc of worlds[0].locations) {
    const pl = makeLocation(loc, 0);
    if (fixedLocations.has(pl)) {
      continue;
    }
    locations.push(loc);
  }

  return locations.sort();
}
