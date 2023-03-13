import { Settings } from "../settings";
import { isDungeonStrayFairy, isGoldToken, isHouseToken, isTownStrayFairy } from "./items";
import { World } from "./world";

// When adding new settings, add in order of how many checks there are to minimize average runtime
export const isShuffled = (settings: Settings, world: World, loc: string, dungLocations?: Set<string>) => {

  const check = world.checks[loc];
  // Gold Skulltulas - 100
  if (isGoldToken(check.item)) {
    if (settings.goldSkulltulaTokens === 'none') {
      return false;
    }
    const dungeonLocations = dungLocations ?? Object.values(world.dungeons).reduce((acc, x) => new Set([...acc, ...x]));
    if (settings.goldSkulltulaTokens === 'overworld' && dungeonLocations.has(loc)) {
      return false;
    } else if (settings.goldSkulltulaTokens === 'dungeons' && !dungeonLocations.has(loc)) {
      return false;
    }
    return true;
  }

  // House Skulltulas - 60
  if (isHouseToken(check.item)) {
    if (settings.housesSkulltulaTokens === 'none') {
      return false;
    }
    return true;
  }

  // Stray Fairy Shuffle - 60
  if (isDungeonStrayFairy(check.item)) {
    if (settings.strayFairyShuffle === 'vanilla') {
      return false;
    }
    return true;
  }

  // Single Item shuffles - 1 each
  if (isTownStrayFairy(check.item) && settings.townFairyShuffle === 'vanilla') {
    return false;
  }

  return true;
};
