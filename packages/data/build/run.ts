import { parseChecks } from './checks';
import { parseNpcs, parseScenes } from './common';
import { emit } from './helpers';
import { parseMacros } from './macros';

async function run() {
  const [scenes, npcs, macros] = await Promise.all([
    parseScenes(),
    parseNpcs(),
    parseMacros(),
  ]);
  const checks = await parseChecks({ scenes, npcs });

  await Promise.all([
    emit('data-scenes', scenes),
    emit('data-npc', npcs),
    emit('data-checks', checks),
    emit('data-macros', macros),
  ]);
}

run().catch((err) => {
  console.error(err);
  process.exit(1);
});
