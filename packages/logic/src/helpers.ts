import type { Settings } from '@ootmm/core';
import type { LogicResultWorld } from './pipeline';

import { ItemHelpers } from '@ootmm/core';

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

export const isShuffled = (settings: Settings, world: LogicResultWorld, loc: string, dungLocations?: Set<string>) => {
  const check = world.checks[loc];
  const item = check.item;

  // Single Item shuffles - 1 each
  if (ItemHelpers.isTownStrayFairy(item) && settings.townFairyShuffle === 'vanilla') {
    return false;
  }

  return true;
};
