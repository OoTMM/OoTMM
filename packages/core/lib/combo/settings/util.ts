import { cloneDeep, isEqual } from 'lodash';
import type { PartialDeep } from 'type-fest';
import type { Settings, SettingsBase } from './type';
import { SETTINGS } from './data';
import { DEFAULT_TRICKS, TRICKS } from './tricks';
import { DEFAULT_DUNGEONS } from './dungeons';
import { DEFAULT_SPECIAL_COND, DEFAULT_SPECIAL_CONDS, SPECIAL_CONDS, SPECIAL_CONDS_FIELDS } from './special-conds';
import { SettingsPatch, patchArray } from './patch';
import { SETTINGS_DEFAULT_HINTS } from './hints';
import { DEFAULT_GLITCHES, GLITCHES } from './glitches';

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
  glitches: [ ...DEFAULT_GLITCHES ],
  dungeon: { ...DEFAULT_DUNGEONS },
  specialConds: { ...DEFAULT_SPECIAL_CONDS },
  plando: { locations: {} },
  hints: [ ...SETTINGS_DEFAULT_HINTS ],
} as Settings;

const sortUnique = <T>(arr: T[]): T[] => {
  return [ ...new Set(arr.sort()) ];
};

function validateSettingsStep(settings: Settings): Settings {
  const s = cloneDeep(settings);

  /* Validate tricks */
  s.tricks = sortUnique(s.tricks.filter(t => TRICKS.hasOwnProperty(t)));

  /* Validate glitches */
  s.glitches = sortUnique(s.glitches.filter(g => GLITCHES.hasOwnProperty(g)));

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

  /* Specific validation */
  for (const data of SETTINGS) {
    const key = data.key;
    const cond = (data as any).cond;
    const min = (data as any).min;
    const max = (data as any).max;

    if (cond && !cond(s)) {
      (s as any)[key] = data.default;
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
        if (newValue instanceof Object && (['all', 'none', 'random', 'specific'].includes(newValue.type!))) {
          (dest as any)[key] = { ...newValue };
          if (newValue.type === 'specific') {
            (dest[key] as any).values = Array.from(new Set(Array.from(newValue.values || []).filter(x => setting.values.some(v => v.value === x))));
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

  /* Apply the glitches */
  if (arg.glitches !== undefined && Array.isArray(arg.glitches)) {
    result.glitches = [ ...arg.glitches ];
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

  if (arg.hints !== undefined) {
    result.hints = [ ...arg.hints ];
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

  /* Apply glitches */
  if (patch.glitches !== undefined)
    s.glitches = patchArray(s.glitches, patch.glitches);

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

  /* Apply hints */
  if (patch.hints !== undefined) {
    s.hints = patchArray(s.hints, patch.hints);
  }

  return validateSettings(s);
}
