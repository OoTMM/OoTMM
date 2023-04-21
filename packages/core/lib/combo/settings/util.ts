import { cloneDeep } from 'lodash';
import type { PartialDeep } from 'type-fest';
import type { Settings, SettingsBase } from './type';
import { SETTINGS } from './data';
import { DEFAULT_TRICKS, TRICKS } from './tricks';
import { DEFAULT_DUNGEONS } from './dungeons';
import { DEFAULT_SPECIAL_COND, DEFAULT_SPECIAL_CONDS, SPECIAL_CONDS } from './special-conds';
import { SettingsPatch, patchArray } from './patch';

export const DEFAULT_SETTINGS: Settings = { ...SETTINGS.map(s => {
  return {[s.key]: s.default};
}).reduce((a, b) => ({...a, ...b}), {}),
  startingItems: {},
  junkLocations: [] as string[],
  tricks: [ ...DEFAULT_TRICKS ],
  dungeon: { ...DEFAULT_DUNGEONS },
  specialConds: { ...DEFAULT_SPECIAL_CONDS },
  plando: { locations: {} },
} as Settings;

const sortUnique = <T>(arr: T[]): T[] => {
  return [ ...new Set(arr.sort()) ];
};

export function validateSettings(settings: Settings): Settings {
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
    if (!cond.cond) break;
    if (!cond.cond(s)) {
      s.specialConds[key] = { ...DEFAULT_SPECIAL_COND };
    }
  }

  return s;
};

function applyBaseSettings(dest: SettingsBase, src: Partial<SettingsBase>) {
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
      case 'boolean':
        if (typeof newValue === 'boolean') {
          (dest as any)[key] = newValue;
        }
        break;
      }
    }
  }
}

export function makeSettings(arg: PartialDeep<Settings>): Settings {
  /* Clone the base setting to avoid mutating it */
  const result = cloneDeep(DEFAULT_SETTINGS);

  /* Apply the base settings */
  applyBaseSettings(result, arg);

  /* Apply the starting items */
  if (arg.startingItems !== undefined && typeof arg.startingItems === 'object') {
    result.startingItems = { ...arg.startingItems } as Settings['startingItems'];
  }

  /* Apply the junkLocations */
  if (arg.junkLocations !== undefined && Array.isArray(arg.junkLocations)) {
    result.junkLocations = [ ...arg.junkLocations ];
  }

  /* Apply the tricks */
  if (arg.tricks !== undefined && Array.isArray(arg.tricks)) {
    result.tricks = [ ...arg.tricks ];
  }

  /* Apply dungeon settings */
  if (arg.dungeon !== undefined) {
    result.dungeon = { ...DEFAULT_DUNGEONS, ...arg.dungeon };
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
    result.plando = { ...arg.plando } as Settings['plando'];
  }

  return validateSettings(result);
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

  /* Apply dungeons */
  if (patch.dungeon) {
    s.dungeon = { ...s.dungeon, ...patch.dungeon };
  }

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
    const p = patch.plando;
    if (p.locations !== undefined) {
      if (p.locations === null) {
        s.plando.locations = {};
      } else {
        for (const loc in p.locations) {
          const item = p.locations[loc];
          if (item === null) {
            delete s.plando.locations[loc];
          } else {
            s.plando.locations[loc] = item;
          }
        }
      }
    }
  }

  return validateSettings(s);
}
