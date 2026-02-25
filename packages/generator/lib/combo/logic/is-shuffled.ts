import { ItemHelpers } from "../items";
import { Settings } from "../settings";
import { World } from "./world";

// When adding new settings, add in order of how many checks there are to minimize average runtime
export const isShuffled = (settings: Settings, world: World, loc: string, dungLocations?: Set<string>) => {
  const check = world.checks[loc];
  const item = check.item;

  // Gold Skulltulas - 100
  if (ItemHelpers.isGoldToken(item)) {
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
  if (ItemHelpers.isHouseToken(item)) {
    if (settings.housesSkulltulaTokens === 'none') {
      return false;
    }
    return true;
  }

  // Single Item shuffles - 1 each
  if (ItemHelpers.isTownStrayFairy(item) && settings.townFairyShuffle === 'vanilla') {
    return false;
  }

  return true;
};
