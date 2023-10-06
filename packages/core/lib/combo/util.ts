import fs from 'fs/promises';
import { Game } from './config';

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

export const fileExists = async (path: string) => {
  if (!process.env.ROLLUP) {
    try {
      await fs.access(path, fs.constants.F_OK);
      return true;
    } catch {
      return false;
    }
  } else {
    return false;
  }
}

export const arrayToIndexMap = (arr: string[]) => {
  const map: {[k: string]: number} = {};
  for (let i = 0; i < arr.length; ++i) {
    map[arr[i]] = i;
  }
  return map;
};

export const align = (n: number, alignment: number) => {
  const missing = (alignment - (n % alignment)) % alignment;
  return n + missing;
}

export const gameId = (game: Game | 'shared', id: string, char: string) => {
  const prefixes = ['OOT', 'MM', 'SHARED'];
  if (!char) {
    char = '_';
  }
  for (const p of prefixes) {
    const pp = p + char;
    if (id.startsWith(pp)) {
      return id;
    }
  }
  return [game.toUpperCase(), id].join(char);
}

export const toU8Buffer = (data: number[]) => {
  const buf = Buffer.alloc(data.length);
  for (let i = 0; i < data.length; ++i) {
    buf.writeUInt8(data[i], i);
  }
  return buf;
};

export const toI8Buffer = (data: number[]) => {
  const buf = Buffer.alloc(data.length);
  for (let i = 0; i < data.length; ++i) {
    buf.writeInt8(data[i], i);
  }
  return buf;
};

export const toU16Buffer = (data: number[]) => {
  const buf = Buffer.alloc(data.length * 2);
  for (let i = 0; i < data.length; ++i) {
    buf.writeUInt16BE(data[i], i * 2);
  }
  return buf;
};

export const toU32Buffer = (data: number[]) => {
  const buf = Buffer.alloc(data.length * 4);
  for (let i = 0; i < data.length; ++i) {
    buf.writeUInt32BE(data[i], i * 4);
  }
  return buf;
};

export const toU32BufferLE = (data: number[]) => {
  const buf = Buffer.alloc(data.length * 4);
  for (let i = 0; i < data.length; ++i) {
    buf.writeUInt32LE(data[i], i * 4);
  }
  return buf;
};

export function padBuffer16(data: Buffer, fill = 0xff) {
  const len = data.length;
  const newLen = align(len, 16);
  if (newLen === len) {
    return data;
  }
  const buf = Buffer.alloc(newLen, fill);
  data.copy(buf);
  return buf;
}
