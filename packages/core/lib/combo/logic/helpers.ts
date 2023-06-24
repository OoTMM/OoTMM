import { Settings } from "../settings";

export function isEntranceShuffle(settings: Settings) {
  if (settings.erBoss !== 'none')
    return true;
  if (settings.erDungeons !== 'none')
    return true;
  if (settings.erRegions)
    return true;
  return false;
}
