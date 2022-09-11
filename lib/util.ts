import fs from 'fs/promises';

export const fileExists = async (path: string) => {
  try {
    await fs.access(path, fs.constants.F_OK);
    return true;
  } catch {
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
