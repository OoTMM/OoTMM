import type { GossipDefinition } from '../src/gossips';

import { gameId } from '../src/util';
import { GAMES } from '../src/defines';
import { loadCsv, loadTxt, loadYaml, emit } from './helpers';

function buildGossips() {
  const raw = {
    oot: loadCsv('gossips/gossips_oot.csv'),
    mm: loadCsv('gossips/gossips_mm.csv'),
  };

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

  emit('data-gossips', result);
}

async function run() {
  buildGossips();
}

run().then(() => {

}).catch((err) => {
  console.error(err);
  process.exit(1);
});
