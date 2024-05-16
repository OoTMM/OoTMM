import { COSMETICS } from './data';
import { Cosmetics } from './type';

export const DEFAULT_COSMETICS: Cosmetics = { ...COSMETICS.map(c => {
  return c.type === 'boolean' ? false : c.type === 'file' ? {[c.key]: null} : {[c.key]: 'default'};
}).reduce((a, b) => ({...a, ...b} as any), {}) } as Cosmetics;

export function makeCosmetics(data: Partial<Cosmetics>) {
  return {
    ...DEFAULT_COSMETICS,
    ...data,
  };
}
