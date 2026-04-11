import type { Settings } from '@ootmm/core';
import type { World } from './world'

import { ItemHelpers } from '@ootmm/core';
import type { LogicResultWorld } from './pipeline';

export function isEntranceShuffle(settings: Settings) {
  if (settings.erBoss !== 'none')
    return true;
  if (settings.erMajorDungeons)
    return true;
  if (settings.erMinorDungeons)
    return true;
  if (settings.erSpiderHouses)
    return true;
  if (settings.erPirateFortress)
    return true;
  if (settings.erBeneathWell)
    return true;
  if (settings.erIkanaCastle)
    return true;
  if (settings.erSecretShrine)
    return true;
  if (settings.erGanonCastle)
    return true;
  if (settings.erGanonTower)
    return true;
  if (settings.erMoon)
    return true;
  if (settings.erRegions !== 'none')
    return true;
  if (settings.erIndoorsMajor)
    return true;
  if (settings.erIndoorsExtra)
    return true;
  if (settings.erWarps !== 'none')
    return true;
  if (settings.erOneWaysMajor)
    return true;
  if (settings.erOneWaysIkana)
    return true;
  if (settings.erOneWaysSongs)
    return true;
  if (settings.erOneWaysStatues)
    return true;
  if (settings.erOneWaysOwls)
    return true;
  if (settings.erGrottos !== 'none')
    return true;
  if (settings.erWallmasters !== 'none')
    return true;
  if (settings.erOverworld !== 'none')
    return true;
  return false;
}

export function mustStartWithMasterSword(settings: Settings) {
  /* Child start - doesn't matter */
  if (settings.startingAge === 'child')
    return false;

  /* Swordless disallowed - must start with MS */
  if (!settings.swordlessAdult)
    return true;

  /* Master Sword not shuffled and no time travel otherwise - must start with it to not be stuck as adult forever */
  if (!settings.shuffleMasterSword && settings.timeTravelSword)
    return true;

  /* Fine */
  return false;
}

// When adding new settings, add in order of how many checks there are to minimize average runtime
export const isShuffled = (settings: Settings, world: LogicResultWorld, loc: string, dungLocations?: Set<string>) => {
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
