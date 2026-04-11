import type { Cosmetics } from './type';
import { COSMETICS } from './data';

export const DEFAULT_COSMETICS: Cosmetics = { ...COSMETICS.map(c => {
  return c.type === 'boolean' ? {[c.key]: c.default} : c.type === 'file' ? {[c.key]: null} : {[c.key]: 'default'} as any;
}).reduce((a, b) => ({...a, ...b} as any), {}) } as Cosmetics;

export function makeCosmetics(data: Partial<Cosmetics>) {
  return {
    ...DEFAULT_COSMETICS,
    ...data,
  };
}
