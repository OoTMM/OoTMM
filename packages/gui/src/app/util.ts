export const VERSION = process.env.VERSION || 'XXX';
export const ENV_KEYS = (process.env.ENV_KEYS || 'dev').split(',');

function prefix(env: string): string {
  return `generator:${env}:`;
}

export function localStoragePrefixedGet(key: string, defaultValue: any = {}): any {
  for (const e of ENV_KEYS) {
    const data = localStorage.getItem(prefix(e) + key);
    if (data) {
      return JSON.parse(data);
    }
  }
  return defaultValue;
}

export function localStoragePrefixedSet(key: string, value: any): void {
  for (const e of ENV_KEYS) {
    localStorage.setItem(prefix(e) + key, JSON.stringify(value));
  }
}

export function localStorageExport() {
  const unprefixed = new Set<string>();

  /* Build a list of candidate keys */
  for (let i = 0; i < localStorage.length; i++) {
    const key = localStorage.key(i)!;
    for (const e of ENV_KEYS) {
      if (key.startsWith(prefix(e))) {
        const unprefixedKey = key.slice(prefix(e).length);
        unprefixed.add(unprefixedKey);
        break;
      }
    }
  }

  const data = Object.fromEntries([...unprefixed].map(k => [k, localStoragePrefixedGet(k)]));

  return {
    type: "ootmm-web-export",
    version: 1,
    data
  };
}

export function localStorageImport(data: any) {
  if (data.type !== "ootmm-web-export") {
    throw new Error("Invalid data");
  }

  /* Remove existing keys */
  for (let i = 0; i < localStorage.length; i++) {
    const key = localStorage.key(i)!;
    for (const e of ENV_KEYS) {
      if (key.startsWith(prefix(e))) {
        localStorage.removeItem(key);
      }
    }
  }

  for (const [k, v] of Object.entries(data.data)) {
    localStoragePrefixedSet(k, v);
  }
}
