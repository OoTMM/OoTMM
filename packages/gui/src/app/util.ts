export const VERSION = process.env.VERSION || 'XXX';
export const ENV_KEYS = (process.env.ENV_KEYS || 'dev').split(',');

export function localStoragePrefixedGet(key: string, defaultValue: any = {}): any {
  for (const e of ENV_KEYS) {
    const data = localStorage.getItem('generator:' + e + ':' + key);
    if (data) {
      return JSON.parse(data);
    }
  }
  return defaultValue;
}

export function localStoragePrefixedSet(key: string, value: any): void {
  for (const e of ENV_KEYS) {
    localStorage.setItem('generator:' + e + ':' + key, JSON.stringify(value));
  }
}
