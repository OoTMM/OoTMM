import type { PartialDeep } from 'type-fest';
import type { Settings } from './type';

import { isEqual } from 'lodash-es';
import { deflateRaw, inflate, inflateRaw } from 'pako';
import { uint8ArrayToBase64, base64ToUint8Array } from 'uint8array-extras';

import { DEFAULT_SETTINGS, SETTINGS, makeSettings } from './';

export function exportSettings(settings: Settings): string {
  const diff: any = {};

  /* Normal settings & fallback */
  for (const k in settings) {
    if (k === 'enemyShuffleExcluded') continue;
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

  for (const k of ['specialConds', 'dungeon', 'hints', 'junkLocations', 'enemyShuffleWeights', 'tricks', 'plando', 'startingItems']) {
    if (isEqual(settings[k as keyof typeof settings], DEFAULT_SETTINGS[k as keyof typeof DEFAULT_SETTINGS])) {
      delete diff[k];
    }
  }

  if (settings.enemyShuffleExcluded?.length) {
    diff.enemyShuffleExcluded = settings.enemyShuffleExcluded;
  }

  const j = JSON.stringify(diff);
  const compressed = deflateRaw(j, { level: 9 });
  const str = uint8ArrayToBase64(compressed, { urlSafe: true });
  return `v2.${str}`;
}

export function importSettingsRaw(str: string): PartialDeep<Settings> {
  let data: any;

  if (str.startsWith('v2.')) {
    data = importSettingsV2(str);
  } else if (str.startsWith('v1.')) {
    data = importSettingsV1(str);
  } else {
    data = importSettingsV0(str);
  }

  return data;
}

export function importSettings(str: string): Settings {
  return makeSettings(importSettingsRaw(str));
}

function importSettingsV2(str: string): PartialDeep<Settings> {
  return decodeSettings(inflateRaw(base64ToUint8Array(str.slice(3))));
}

function importSettingsV1(str: string): PartialDeep<Settings> {
  return decodeSettings(inflate(base64ToUint8Array(str.slice(3))));
}

function importSettingsV0(str: string): PartialDeep<Settings> {
  return decodeSettings(base64ToUint8Array(str));
}

function decodeSettings(data: Uint8Array): PartialDeep<Settings> {
  return JSON.parse(new TextDecoder().decode(data));
}
