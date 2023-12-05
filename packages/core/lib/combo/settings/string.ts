import { Settings, makeSettings } from '../settings';

export function exportSettings(settings: Settings): string {
  const buf = Buffer.from(JSON.stringify(settings));
  const str = buf.toString('base64');
  return str;
}

export function importSettings(str: string): Settings {
  let data: any;
  data = importSettingsV0(str);
  return makeSettings(data);
}

function importSettingsV0(str: string): any {
  const buf = Buffer.from(str, 'base64');
  const partial = JSON.parse(buf.toString());
  return partial;
}
