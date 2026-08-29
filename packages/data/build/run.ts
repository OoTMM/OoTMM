import { parseChecks } from './checks';
import { parseNpcs, parseScenes } from './common';
import { emit } from './helpers';

async function run() {
  const [scenes, npcs] = await Promise.all([
    parseScenes(),
    parseNpcs(),
  ]);
  const checks = await parseChecks({ scenes, npcs });

  await Promise.all([
    emit('data-scenes', scenes),
    emit('data-npc', npcs),
    emit('data-checks', checks),
  ]);
}

run().catch((err) => {
  console.error(err);
  process.exit(1);
});
