import { Settings } from "../settings";

export function isEntranceShuffle(settings: Settings) {
  if (settings.erBoss)
    return true;
  if (settings.erDungeons)
    return true;
  if (settings.erOverworld)
    return true;
  return false;
}
