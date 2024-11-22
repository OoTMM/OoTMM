import { cloneDeep, isEqual } from 'lodash';
import type { PartialDeep } from 'type-fest';
import type { Settings, SettingsBase } from './type';
import { SETTINGS } from './data';
import { DEFAULT_TRICKS, TRICKS } from './tricks';
import { DEFAULT_SPECIAL_COND, DEFAULT_SPECIAL_CONDS, SPECIAL_CONDS, SPECIAL_CONDS_FIELDS } from './special-conds';
import { SettingsPatch, patchArray } from './patch';
import { SETTINGS_DEFAULT_HINTS } from './hints';

export const DEFAULT_SETTINGS: Settings = { ...SETTINGS.map(s => {
  if (s.type === 'set') {
    return {[s.key]: { type: s.default }};
  } else {
    return {[s.key]: s.default};
  }
}).reduce((a, b) => ({...a, ...b} as any), {}),
  startingItems: {},
  junkLocations: [] as string[],
  tricks: [ ...DEFAULT_TRICKS ],
  specialConds: { ...DEFAULT_SPECIAL_CONDS },
  plando: { locations: {}, entrances: {} },
  hints: [ ...SETTINGS_DEFAULT_HINTS ],
} as Settings;

const sortUnique = <T>(arr: T[]): T[] => {
  return [ ...new Set(arr.sort()) ];
};

function validateSettingsStep(settings: Settings): Settings {
  const s = cloneDeep(settings);

  /* Validate tricks */
  s.tricks = sortUnique(s.tricks.filter(t => TRICKS.hasOwnProperty(t)));

  /* Validate junk locations */
  s.junkLocations = sortUnique(s.junkLocations);

  /* Validate starting items */
  const newStartingItems: Settings['startingItems'] = {};
  for (const key in s.startingItems) {
    const value = s.startingItems[key];
    if (value > 0) {
      newStartingItems[key] = value;
    }
  }
  s.startingItems = newStartingItems;

  /* Filter special conds */
  for (const key in SPECIAL_CONDS) {
    const cond = SPECIAL_CONDS[key];
    for (const field of Object.keys(SPECIAL_CONDS_FIELDS)) {
      const f = SPECIAL_CONDS_FIELDS[field as keyof typeof SPECIAL_CONDS_FIELDS];
      const fcond = (f as  any).cond;
      if (fcond && !fcond(s)) {
        (s.specialConds as any)[key][field] = false;
      }
    }
    if (!cond.cond) break;
    if (!cond.cond(s)) {
      s.specialConds[key] = { ...DEFAULT_SPECIAL_COND };
    }
  }

  /* Hardcoded game checks */
  if (s.goal === 'both') {
    if (s.games === 'oot') s.goal = 'ganon';
    if (s.games === 'mm') s.goal = 'majora';
  }

  /* Specific validation */
  for (const data of SETTINGS) {
    const key = data.key;
    const cond = (data as any).cond;
    const min = (data as any).min;
    const max = (data as any).max;
    const defaultV = data.default;

    if (cond && !cond(s)) {
      if (data.type === 'set') {
        (s as any)[key] = { type: defaultV };
      } else {
        (s as any)[key] = defaultV;
      }
    }

    if (data.type === 'enum') {
      const curV = (s as any)[key];
      const enumD = (data as any).values.find((x: any) => x.value === curV);
      if (enumD) {
        if (enumD.cond && !enumD.cond(s)) {
          (s as any)[key] = defaultV;
        }
      }
    }

    if (min !== undefined) {
      const v = typeof min === 'function' ? min(s) : min;
      if ((s as any)[key] < v) {
        (s as any)[key] = v;
      }
    }
    if (max !== undefined) {
      const v = typeof max === 'function' ? max(s) : max;
      if ((s as any)[key] > v) {
        (s as any)[key] = v;
      }
    }
  }

  return s;
}

export function validateSettings(settings: Settings) {
  for (;;) {
    const s = validateSettingsStep(settings);
    if (isEqual(s, settings)) {
      return s;
    }
    settings = s;
  }
}

function applyBaseSettings(dest: SettingsBase, src: PartialDeep<SettingsBase>) {
  for (const setting of SETTINGS) {
    const { key } = setting;
    if (src.hasOwnProperty(key)) {
      const newValue = src[setting.key];
      switch (setting.type) {
      case 'enum':
        if (setting.values.some(v => v.value === newValue)) {
          (dest as any)[key] = newValue;
        }
        break;
      case 'set':
        if (newValue instanceof Object && (['all', 'none', 'random', 'specific', 'random-mixed'].includes(newValue.type!))) {
          (dest as any)[key] = { ...newValue };
          if (newValue.type === 'specific') {
            (dest[key] as any).values = Array.from(new Set(Array.from(newValue.values || []).filter(x => setting.values.some(v => v.value === x))));
          } else if (newValue.type === 'random-mixed') {
            const set = Array.from(new Set(Array.from(newValue.set || []).filter(x => setting.values.some(v => v.value === x))));
            const unset = Array.from(new Set(Array.from(newValue.unset || []).filter(x => setting.values.some(v => v.value === x) && !set.includes(x))));
            (dest[key] as any).set = set;
            (dest[key] as any).unset = unset;
          }
        }
        break;
      case 'boolean':
        if (typeof newValue === 'boolean') {
          (dest as any)[key] = newValue;
        }
        break;
      case 'number':
        if (typeof newValue === 'number') {
          (dest as any)[key] = newValue;
        }
        break;
      }
    }
  }
}

function sortCopyArray<T>(arr: T[]): T[] {
  return [ ...arr ].sort();
}

function sortCopyObject<T extends {[k: string]: any}>(obj: T): T {
  const result = {} as {[k: string]: any};
  for (const key of Object.keys(obj).sort()) {
    const data = obj[key];
    result[key] = data;
  }
  return result as T;
}

export function makeSettings(arg: PartialDeep<Settings>): Settings {
  /* Clone the base setting to avoid mutating it */
  const result = cloneDeep(DEFAULT_SETTINGS);

  /* Apply the base settings */
  applyBaseSettings(result, arg);

  /* Apply the starting items */
  if (arg.startingItems !== undefined && typeof arg.startingItems === 'object') {
    result.startingItems = sortCopyObject(arg.startingItems) as Settings['startingItems'];
  }

  /* Apply the junkLocations */
  if (arg.junkLocations !== undefined && Array.isArray(arg.junkLocations)) {
    result.junkLocations = sortCopyArray(arg.junkLocations);
  }

  /* Apply the tricks */
  if (arg.tricks !== undefined && Array.isArray(arg.tricks)) {
    result.tricks = sortCopyArray(arg.tricks);
  }

  /* Apply special conds */
  if (arg.specialConds !== undefined) {
    for (const k in arg.specialConds) {
      const cond = arg.specialConds[k];
      if (cond !== undefined) {
        result.specialConds[k] = { ...DEFAULT_SPECIAL_CONDS[k], ...cond };
      }
    }
  }

  /* Apply plando */
  if (arg.plando !== undefined) {
    if (arg.plando.locations !== undefined) {
      result.plando.locations = sortCopyObject({ ...arg.plando.locations }) as Settings['plando']['locations'];
    }

    if (arg.plando.entrances !== undefined) {
      result.plando.entrances = sortCopyObject({ ...arg.plando.entrances }) as Settings['plando']['entrances'];
    }
  }

  if (arg.hints !== undefined) {
    result.hints = [ ...arg.hints ];
  }

  return validateSettings(result);
}

function applyKeyValue<K extends string, V>(data: Record<K, V>, patch: undefined | null | Record<K, V | null>): Record<K, V> {
  if (patch === undefined)
    return data;
  if (patch === null)
    return {} as Record<K, V>;
  const result = { ...data };
  for (const key in patch) {
    const value = patch[key];
    if (value === null) {
      delete result[key];
    } else {
      result[key] = value;
    }
  }
  return result;
}

export function mergeSettings(settings: Settings, patch: SettingsPatch): Settings {
  const s = cloneDeep(settings);

  /* Apply the base settings */
  applyBaseSettings(s, patch);

  /* Apply the starting items */
  if (patch.startingItems !== undefined)
    s.startingItems = { ...s.startingItems, ...patch.startingItems } as Settings['startingItems'];

  /* Apply junk locations */
  if (patch.junkLocations !== undefined)
    s.junkLocations = patchArray(s.junkLocations, patch.junkLocations);

  /* Apply tricks */
  if (patch.tricks !== undefined)
    s.tricks = patchArray(s.tricks, patch.tricks);

  /* Apply special conds */
  if (patch.specialConds !== undefined) {
    for (const key in patch.specialConds) {
      const cond = patch.specialConds[key];
      if (cond !== undefined) {
        s.specialConds[key] = { ...s.specialConds[key], ...cond };
      }
    }
  }

  /* Apply plando */
  if (patch.plando !== undefined) {
    s.plando.locations = applyKeyValue(s.plando.locations, patch.plando.locations);
    s.plando.entrances = applyKeyValue(s.plando.entrances, patch.plando.entrances);
  }

  /* Apply hints */
  if (patch.hints !== undefined) {
    s.hints = patchArray(s.hints, patch.hints);
  }

  return validateSettings(s);
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
