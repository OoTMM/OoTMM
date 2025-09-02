import { isEqual } from 'lodash';
import { deflateRaw, inflate, inflateRaw } from 'pako';
import { PartialDeep } from 'type-fest';
import { uint8ArrayToBase64, base64ToUint8Array } from 'uint8array-extras';

import { DEFAULT_SETTINGS, SETTINGS, Settings, makeSettings } from '../settings';

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

  for (const k of ['specialConds', 'dungeon', 'hints', 'junkLocations', 'tricks', 'plando', 'startingItems']) {
    if (isEqual(settings[k as keyof typeof settings], DEFAULT_SETTINGS[k as keyof typeof DEFAULT_SETTINGS])) {
      delete diff[k];
    }
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

function importSettingsV2(str: string): any {
  const data = str.slice(3);
  const buf = base64ToUint8Array(data);
  const decompressed = inflateRaw(buf, { to: 'string' });
  const partial = JSON.parse(decompressed);
  return partial;
}

function importSettingsV1(str: string): any {
  const data = str.slice(3);
  const buf = base64ToUint8Array(data);
  const decompressed = inflate(buf, { to: 'string' });
  const partial = JSON.parse(decompressed);
  return partial;
}

function importSettingsV0(str: string): any {
  const bufBinary = base64ToUint8Array(str);
  const buf = new TextDecoder().decode(bufBinary);
  const partial = JSON.parse(buf);
  return partial;
}
