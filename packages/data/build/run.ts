import { parseChecks } from './checks';
import { parseDrawGi, parseEntrances, parseGi, parseHints, parseLinkAnimations, parseNpcs, parseRegions, parseScenes } from './common';
import { parseFiles } from './files';
import { parseGossips } from './gossips';
import { emit, emitJson } from './helpers';
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
  const xflagsCount = Array.from(checks.checks.filter((c: any) => c.ov === 'xflag')).length;
  const xflagsCountIds = Object.keys(checks.matches).length;

  await Promise.all([
    emitJson('lib/data-files.json', files),
    emitJson('lib/data-scenes.json', scenes),
    emitJson('lib/data-npc.json', npcs),
    emitJson('lib/data-checks.json', checks.checks),
    emitJson('lib/data-checks-xflags.json', checks.matches),
    emitJson('lib/data-macros.json', macros),
    emitJson('lib/data-world.json', world),
    emitJson('lib/data-regions.json', regions),
    emitJson('lib/data-hints.json', hints),
    emitJson('lib/data-entrances.json', entrances),
    emitJson('lib/data-gi.json', gi),
    emitJson('lib/data-drawgi.json', drawGi),
    emitJson('lib/data-link-animations.json', linkAnimations),
    emitJson('lib/data-gossips.json', gossips),
    emit('include/combo/data/xflags.h', `#define XFLAGS_COUNT ${xflagsCount}\n#define XFLAGS_COUNT_IDS ${xflagsCountIds}\n`),
  ]);
}

run().catch((err) => {
  console.error(err);
  process.exit(1);
});
