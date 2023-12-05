import { isEqual } from 'lodash';
import { deflate, inflate } from 'pako';

import { DEFAULT_SETTINGS, SETTINGS, Settings, makeSettings } from '../settings';
import { DEFAULT_DUNGEONS } from './dungeons';
import { DEFAULT_SPECIAL_CONDS } from './special-conds';
import { SETTINGS_DEFAULT_HINTS } from './hints';

export function exportSettings(settings: Settings): string {
  const diff: any = {};

  /* Normal settings & fallback */
  for (let k in settings) {
    const v = settings[k as keyof typeof settings] as any;
    const data = SETTINGS.find(s => s.key === k);
    if (!data) {
      diff[k] = v;
      continue;
    }

    let def: any = null;
    switch (data.type) {
    case 'set':
      def = { type: data.default };
      break;
    default:
      def = data.default;
      break;
    }

    if (!isEqual(v, def)) {
      diff[k] = v;
    }
  }

  for (const k of ['specialConds', 'dungeon', 'hints', 'junkLocations', 'tricks', 'glitches', 'plando', 'startingItems']) {
    if (isEqual(settings[k as keyof typeof settings], DEFAULT_SETTINGS[k as keyof typeof DEFAULT_SETTINGS])) {
      delete diff[k];
    }
  }

  const j = JSON.stringify(diff);
  const compressed = deflate(j);
  const str = Buffer.from(compressed).toString('base64');
  return `v1.${str}`;
}

export function importSettings(str: string): Settings {
  let data: any;

  if (str.startsWith('v1.')) {
    data = importSettingsV1(str);
  } else {
    data = importSettingsV0(str);
  }

  return makeSettings(data);
}

function importSettingsV1(str: string): any {
  const data = str.slice(3);
  const buf = Buffer.from(data, 'base64');
  const decompressed = inflate(buf, { to: 'string' });
  const partial = JSON.parse(decompressed);
  return partial;
}

function importSettingsV0(str: string): any {
  const buf = Buffer.from(str, 'base64');
  const partial = JSON.parse(buf.toString());
  return partial;
}
