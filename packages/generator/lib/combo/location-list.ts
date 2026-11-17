import type { Settings } from '@ootmm/core';

import { DEFAULT_SETTINGS } from '@ootmm/core';
import { makeLocation } from '@ootmm/logic';
import { settingsWorld } from './item-pool';

export async function locationList(aSettings: Partial<Settings>) {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const { worlds, fixedLocations } = await settingsWorld(settings);

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
