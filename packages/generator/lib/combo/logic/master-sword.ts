import { Settings } from '@ootmm/core';

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
