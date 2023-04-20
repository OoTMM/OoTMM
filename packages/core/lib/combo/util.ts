import fs from 'fs/promises';
import { Game } from './config';

export type Optional<T> = T extends {} ? {[K in keyof T]?: T[K]} : T | undefined;

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
