import { GAMES } from '../defines';
import { gameId } from '../util';
import { POOL } from './data';

const hintToLocations = new Map<string, string[]>();

for (const game of GAMES) {
  for (const record of POOL[game]) {
    if (record.hint === 'NONE')
      continue;
    const hint = gameId(game, record.hint, '_');
    const loc = gameId(game, record.location, ' ');
    if (!hintToLocations.has(hint)) {
      hintToLocations.set(hint, []);
    }
    hintToLocations.get(hint)!.push(loc);
  }
}

export function hintLocations(hint: string): string[] {
  return hintToLocations.get(hint) || [];
}
