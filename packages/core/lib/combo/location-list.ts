import { makeCosmetics } from './cosmetics';
import { worldState } from './logic';
import { isShuffled } from './logic/is-shuffled';
import { makeLocation } from './logic/locations';
import { Monitor } from './monitor';
import { Settings, DEFAULT_SETTINGS } from './settings';
import { makeRandomSettings } from './settings/random';

export async function locationList(aSettings: Partial<Settings>) {
  const settings: Settings = { ...DEFAULT_SETTINGS, ...aSettings };
  const cosmetics = makeCosmetics({});
  const monitor = new Monitor({ onLog: () => {} });
  const random = makeRandomSettings({});
  const { worlds, fixedLocations } = await worldState(monitor, { settings, cosmetics, seed: "--- INTERNAL ---", random });

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
