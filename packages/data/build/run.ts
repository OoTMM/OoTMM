import { parseChecks } from './checks';
import { parseDrawGi, parseEntrances, parseGi, parseHints, parseLinkAnimations, parseNpcs, parseRegions, parseScenes } from './common';
import { parseFiles } from './files';
import { parseGossips } from './gossips';
import { emit } from './helpers';
import { parseMacros } from './macros';
import { parseWorld } from './world';

async function run() {
  const [
    files,
    scenes,
    npcs,
    macros,
    world,
    regions,
    hints,
    entrances,
    gi,
    drawGi,
    linkAnimations,
    gossips,
  ] = await Promise.all([
    parseFiles(),
    parseScenes(),
    parseNpcs(),
    parseMacros(),
    parseWorld(),
    parseRegions(),
    parseHints(),
    parseEntrances(),
    parseGi(),
    parseDrawGi(),
    parseLinkAnimations(),
    parseGossips(),
  ]);
  const checks = await parseChecks({ scenes, npcs });

  await Promise.all([
    emit('data-files', files),
    emit('data-scenes', scenes),
    emit('data-npc', npcs),
    emit('data-checks', checks),
    emit('data-macros', macros),
    emit('data-world', world),
    emit('data-regions', regions),
    emit('data-hints', hints),
    emit('data-entrances', entrances),
    emit('data-gi', gi),
    emit('data-drawgi', drawGi),
    emit('data-link-animations', linkAnimations),
    emit('data-gossips', gossips),
  ]);
}

run().catch((err) => {
  console.error(err);
  process.exit(1);
});
