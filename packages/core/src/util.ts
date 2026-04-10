
export type Optional<T> = T extends {} ? {[K in keyof T]?: T[K]} : T | undefined;

export type CountMap<T> = Map<T, number>;

export function countMapAdd<T>(map: CountMap<T>, key: T, count: number = 1) {
  const v = map.get(key) || 0;
  const newV = v + count;
  if (newV > 0) {
    map.set(key, newV);
  } else {
    map.delete(key);
  }
}

export function countMapRemove<T>(map: CountMap<T>, key: T, count: number = 1) {
  countMapAdd(map, key, -count);
}

export function countMapCombine<T>(...maps: CountMap<T>[]) {
  const result = new Map<T, number>();
  for (const map of maps) {
    for (const [key, value] of map.entries()) {
      countMapAdd(result, key, value);
    }
  }
  return result;
}

export function countMapArray<T>(map: CountMap<T>) {
  const result: T[] = [];
  for (const [key, value] of map.entries()) {
    for (let i = 0; i < value; ++i) {
      result.push(key);
    }
  }
  return result;
}

export function createMemo<T>() {
  const cache = new Map<string, T>();
  return (key: string, fn: () => T) => {
    const v = cache.get(key);
    if (v !== undefined) {
      return v;
    }
    const value = fn();
    cache.set(key, value);
    return value;
  }
}
