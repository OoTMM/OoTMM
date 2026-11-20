import { parseChecks } from './checks';
import { parseDrawGi, parseEntrances, parseGi, parseHints, parseLinkAnimations, parseNpcs, parseRegions, parseScenes } from './common';
import { parseFiles } from './files';
import { parseGossips } from './gossips';
import { emit, emitJson, loadYaml } from './helpers';
import { parseMacros } from './macros';
import { parseWorld } from './world';
import { compileEnemySpawnPolicies } from '../lib/enemy-spawn-rules';

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

  const entrancesData = Object.entries(entrances).map(([sym, data]) => ({ key: (((data as any).game === 'mm' ? 0x80000000 : 0) | (data as any).id) >>> 0, sym })).sort((a, b) => a.key - b.key);

  const manifestItems = { version: 1, items: gi.map((item: any, index: number) => ({ id: index + 1, sym: item.id })) };
  const manifestLocations = { version: 1, locations: checks.checks.map((check: any, index: number) => ({ key: check.key, location: check.location })) };
  const manifestEntrances = { version: 1, entrances: entrancesData };

  const [enemySpawnMacros, enemySpawnPlacements] = await Promise.all([
    loadYaml('enemy-logic/spawn-macros.yml'),
    loadYaml('enemy-logic/spawn-rules.yml'),
  ]);
  const enemySpawn = compileEnemySpawnPolicies(enemySpawnMacros, enemySpawnPlacements);
  const enemyRuntimeHeader = [
    '#ifndef COMBO_DATA_ENEMY_SPAWN_H', '#define COMBO_DATA_ENEMY_SPAWN_H', '',
    'typedef struct { s16 scene; s16 room; f32 maxHeightDifference; } EnemyOotRuntimeSpawnRule;',
    'static const EnemyOotRuntimeSpawnRule kEnemyOotRuntimeSpawnRules[] = {',
    ...enemySpawn.runtime.map(rule => `    { ${rule.scene}, ${rule.room}, ${rule.maxHeightDifference}${Number.isInteger(rule.maxHeightDifference) ? '.0' : ''}f },`),
    '};', '', '#endif', '',
  ].join('\n');

  await Promise.all([
    emitJson('lib/data-files.json', files),
    emitJson('lib/data-scenes.json', scenes),
    emitJson('lib/data-npc.json', npcs),
    emitJson('lib/data-checks.json', checks.checks),
    emitJson('lib/data-checks-xflags.json', checks.matches),
    emitJson('lib/data-macros.json', macros),
    ...['mm-rooms', 'oot-rooms'].map(async name =>
      emitJson(`lib/enemy-logic-${name}.json`, await loadYaml(`enemy-logic/${name}.yml`))),
    emitJson('lib/enemy-spawn-rules.json', enemySpawn.rules),
    emitJson('lib/enemy-spawn-oot-targets.json', enemySpawn.targets),
    emitJson('lib/enemy-spawn-oot-locations.json', enemySpawn.locations),
    emitJson('lib/enemy-spawn-oot-templates.json', enemySpawn.templates),
    emitJson('lib/enemy-spawn-mm-actors.json', enemySpawn.mmActors),
    emitJson('lib/enemy-spawn-actor-types.json', enemySpawn.actorTypes),
    emit('include/combo/data/enemy-spawn.h', enemyRuntimeHeader),
    emitJson('lib/data-world.json', world),
    emitJson('lib/data-regions.json', regions),
    emitJson('lib/data-hints.json', hints),
    emitJson('lib/data-entrances.json', entrances),
    emitJson('lib/data-gi.json', gi),
    emitJson('lib/data-drawgi.json', drawGi),
    emitJson('lib/data-link-animations.json', linkAnimations),
    emitJson('lib/data-gossips.json', gossips),
    emitJson('manifests/items.json', manifestItems),
    emitJson('manifests/locations.json', manifestLocations),
    emitJson('manifests/entrances.json', manifestEntrances),
    emit('include/combo/data/xflags.h', `#define XFLAGS_COUNT ${xflagsCount}\n#define XFLAGS_COUNT_IDS ${xflagsCountIds}\n`),
  ]);
}

run().catch((err) => {
  console.error(err);
  process.exit(1);
});
