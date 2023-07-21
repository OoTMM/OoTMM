import { COSMETICS } from './data';
import { Cosmetics } from './type';

export const DEFAULT_COSMETICS: Cosmetics = { ...COSMETICS.map(c => {
  return c.type === 'zobj' ? {[c.key]: null} : {[c.key]: c.default};
}).reduce((a, b) => ({...a, ...b}), {}) } as Cosmetics;

export function makeCosmetics(data: Partial<Cosmetics>) {
  return {
    ...DEFAULT_COSMETICS,
    ...data,
  };
}
