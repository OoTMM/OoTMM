import { Glitch } from './glitches';
import { Trick } from './tricks';
import { Settings } from './type';
import { PartialDeep } from 'type-fest';

type SettingsArrayAdd<T> = { add: T[] };
type SettingsArrayRemove<T> = { remove: T[] };
type SettingsArraySet<T> = { set: T[] };
type SettingsArrayPatch<T> = SettingsArrayAdd<T> | SettingsArrayRemove<T> | SettingsArraySet<T>;

export type SettingsPatch = PartialDeep<Omit<Settings, 'junkLocations' | 'tricks' | 'glitches' | 'plando' | 'hints'>> & {
  junkLocations?: SettingsArrayPatch<string>;
  tricks?: SettingsArrayPatch<Trick>;
  glitches?: SettingsArrayPatch<Glitch>;
  plando?: {
    locations?: null | {[k: string]: string | null};
  };
  hints?: SettingsArrayPatch<Settings['hints'][number]>;
};

export function patchArray<T>(arr: T[], patch: SettingsArrayPatch<T>): T[] {
  if ('add' in patch) {
    return [...arr, ...patch.add];
  } else if ('remove' in patch) {
    return arr.filter(x => !patch.remove.includes(x));
  } else if ('set' in patch) {
    return patch.set;
  } else {
    throw new Error('Invalid patch');
  }
}
