import type { GossipDefinition } from '../lib/types';

import { gameId, GAMES } from '../lib/game';
import { loadCsv } from './helpers';

export async function parseGossips() {
  const [oot, mm] = await Promise.all([
    loadCsv('gossips/gossips_oot.csv'),
    loadCsv('gossips/gossips_mm.csv'),
  ]);

  const raw = { oot, mm };
  let result: GossipDefinition[] = [];
  for (const game of GAMES) {
    const hints = raw[game];
    for (const hint of hints) {
      const location = gameId(game, hint.location, ' ');
      const id = parseInt(hint.id);
      if (isNaN(id)) {
        throw new Error(`Invalid hint ID for ${location}: ${hint.id}`);
      }
      result.push({
        game,
        location,
        type: hint.type,
        id,
      });
    }
  }

  return result;
}
