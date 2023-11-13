import { COSMETICS } from './data';
import { Cosmetics } from './type';

export const DEFAULT_COSMETICS: Cosmetics = { ...COSMETICS.map(c => {
  return c.type === 'zobj' ? {[c.key]: null} : c.type === 'zip' ? {[c.key]: null} : {[c.key]: 'default'};
}).reduce((a, b) => ({...a, ...b} as any), {}) } as Cosmetics;

export function makeCosmetics(data: Partial<Cosmetics>) {
  return {
    ...DEFAULT_COSMETICS,
    ...data,
  };
}
