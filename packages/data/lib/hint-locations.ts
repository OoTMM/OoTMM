import { CHECKS } from './checks';

const hintToLocations = new Map<string, string[]>();

for (const check of CHECKS) {
  if (!check.hint)
    continue;
  if (!hintToLocations.has(check.hint)) {
    hintToLocations.set(check.hint, []);
  }
  hintToLocations.get(check.hint)!.push(check.location);
}

export function hintLocations(hint: string): string[] {
  return hintToLocations.get(hint) || [];
}
