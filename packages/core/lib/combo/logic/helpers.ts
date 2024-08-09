import { Settings } from "../settings";
import { World } from '../logic/world';

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
export function isPreactivatedOwls(world: World) {
  if (world.resolvedFlags.mmPreActivatedOwls.has('clocktown'))
    return true;
  if (world.resolvedFlags.mmPreActivatedOwls.has('milkroad'))
    return true;
  if (world.resolvedFlags.mmPreActivatedOwls.has('swamp'))
    return true;
  if (world.resolvedFlags.mmPreActivatedOwls.has('woodfall'))
    return true;
  if (world.resolvedFlags.mmPreActivatedOwls.has('mountain'))
    return true;
  if (world.resolvedFlags.mmPreActivatedOwls.has('snowhead'))
    return true;
  if (world.resolvedFlags.mmPreActivatedOwls.has('greatbay'))
    return true;
  if (world.resolvedFlags.mmPreActivatedOwls.has('zoracape'))
    return true;
  if (world.resolvedFlags.mmPreActivatedOwls.has('canyon'))
    return true;
  if (world.resolvedFlags.mmPreActivatedOwls.has('tower'))
    return true;
  return false;
}