import { Settings } from "../settings";

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
  return false;
}
