import { merge } from 'lodash';
import type { PartialDeep } from 'type-fest';
import type { Settings } from './type';
import { SETTINGS } from './data';
import { DEFAULT_TRICKS } from './tricks';
import { DEFAULT_DUNGEONS } from './dungeons';
import { DEFAULT_SPECIAL_CONDS } from './special-conds';

export const DEFAULT_SETTINGS: Settings = { ...SETTINGS.map(s => {
  return {[s.key]: s.default};
}).reduce((a, b) => ({...a, ...b}), {}),
  startingItems: {},
  junkLocations: [] as string[],
  tricks: { ...DEFAULT_TRICKS },
  dungeon: { ...DEFAULT_DUNGEONS },
  specialConds: { ...DEFAULT_SPECIAL_CONDS },
  plando: { locations: {} },
} as Settings;

export function mergeSettings(base: Settings, arg: PartialDeep<Settings>): Settings {
  return merge({}, base, arg);
}

export function makeSettings(arg: PartialDeep<Settings>): Settings {
  return mergeSettings(DEFAULT_SETTINGS, arg);
}
