import { ENEMY_WATER_SURFACE_SLOTS } from './enemy-water-surface-data';
import { ENEMY_GOLD_ACCESS } from './enemy-gold-access';
import { OOT_VANILLA_SLOT_LOGIC } from './enemy-vanilla-slot-data';
import { ENEMY_SOURCE_ADDITIONS } from './enemy-source-additions';
export { ENEMY_SOURCE_ADDITIONS } from './enemy-source-additions';
import { ENEMY_GOLD_SOURCES } from './enemy-gold-data';
export { ENEMY_GOLD_SOURCES } from './enemy-gold-data';
import type { Game, EnemyOotSpawnTemplate } from '@ootmm/data';
import type { Settings } from './settings';
import { ENEMY_ACTOR_CATALOG, MACROS, RAW_GI, FOREIGN_ENEMY_SOULS, ENEMY_LOGIC_MM_ROOMS as mmRoomBindings, ENEMY_LOGIC_OOT_ROOMS as ootRoomBindings, ENEMY_SPAWN_OOT_LOCATIONS, ENEMY_SPAWN_OOT_TARGETS, ENEMY_SPAWN_MM_ACTORS, ENEMY_SPAWN_ACTOR_TYPES, ENEMY_SPAWN_OOT_TEMPLATES, applyEnemyOotSpawnPolicies, applyEnemyMmSpawnPolicies } from '@ootmm/data';
import { enemy_mm_placement_data as mmPlacementData, enemy_placement_data as placementData, enemy_mm_facts as mmFacts, enemy_oot_source_corrections as ootActorCorrections, oot_enemizer as ootEnemizerFacts, enemy_oot_resources as resourceFacts, } from './enemy-data';
import { Random, shuffle } from './random';
import { mmRoomOverlapStates } from './enemy-room-overlap';
import { ENEMY_OOT_POSITIONS } from './enemy-position-data';
import { spawnEncounter, spawnEncounterAllows, validateSpawnRegistry, type SpawnInventoryRow, enemySpawnSourceAllowed, enemySpawnTargetAllowed, enemySpawnMacroMatches, enemySpawnMacroGroup, enemySpawnLimit, type EnemySpawnLocation, type EnemySpawnTarget } from './enemy-spawn-rules';
import { ENEMY_MEMORY_POLICY, OOT_GAMEPLAY_POLICY, enemyMemoryCost, enemyMemoryTotal, makeEnemyMemoryEvaluator, enemyNativeActorFact, enemyObjectBytes, EnemyMemoryAllocationError, enemySceneObjectBounds, enemyScriptedRootOffsets, enemyPinnedMmActors, enemyNativeActorDependencies, enemyNativeDynamicCollisionCount, enemyOriginalRoomDynamicCollisions, enemyScriptedControllerCost, solveWeightedMemory, solveAdaptiveMemory, type AdaptiveMemoryAssessment, type AdaptiveMemoryChoice, type EnemyMemoryCost, type EnemyMemoryOptions } from './enemy-memory';

const spawnActorTypes = new Map(Object.values(ENEMY_SPAWN_ACTOR_TYPES).map(row => [row.game + '/' + row.actorId, row]));

function enemySpawnActorType(game: Game, actorId: number): Pick<MmEnemy, 'type' | 'stationary'> {
  const row = spawnActorTypes.get(game + '/' + actorId);
  if (!row) throw new Error(`Missing enemy spawn type: ${game}/${actorId}`);
  return { type: row.type, stationary: row.stationary };
}

const MOTH_ENEMY: EnemyDefinition = {
  key: 'mm-moth-foreign', name: 'Moths', game: 'mm', actorId: 0x23d, objectId: 1, habitat: 'air',
  foreignOnly: true, params: [], spawnParams: 0x14, killMacro: 'can_kill_moths', logicMacros: [], soulMacro: 'soul_moth', soulItem: 'SOUL_ENEMY_MOTH',
};

const FOREIGN_BOE_ENEMIES: readonly EnemyDefinition[] = [
  0x2800, 0x3200, 0x3205, 0x6404, 0x6405, 0x7800, 0x7804, 0x8c05, 0x9605, 0xc200, 0xfa00, 0xfa01, 0xff05,
].map(params => ({
  key: `mm-boe-foreign-${params.toString(16)}`, name: params & 1 ? 'White Boe' : 'Black Boe',
  game: 'mm', actorId: 0x164, objectId: 0x17a, habitat: 'ground', foreignOnly: true,
  params: [], spawnParams: params, killMacro: 'can_kill_boes', logicMacros: [], soulMacro: 'soul_boe', requiresNoCutscene: true
}));

const FOREIGN_EENO_ENEMIES: readonly EnemyDefinition[] = [1, 0xff00, 0xff01].map(params => ({
  key: `mm-eeno-foreign-${params.toString(16)}`, name: params & 1 ? 'Large Eeno' : 'Eeno', game: 'mm', actorId: 0x1e6, objectId: 0x1c4,
  habitat: 'ground', foreignOnly: true, params: [], spawnParams: params,
  killMacro: 'can_kill_eenos', logicMacros: [], soulMacro: 'soul_eenos', soulItem: 'SOUL_ENEMY_EENO', requiresNoCutscene: true,
}));

const FOREIGN_EXTRA_NATIVE_MM_ENEMIES: readonly EnemyDefinition[] = [
  { key: 'mm-large-dodongo-foreign', name: 'Large Dodongo', game: 'mm', actorId: 0xb, objectId: 0xa, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 1, killMacro: 'can_kill_dodongos', logicMacros: [], soulMacro: 'soul_dodongo' },
  { key: 'mm-mini-baba-foreign', name: 'Mini Baba', game: 'mm', actorId: 0x66, objectId: 0x31, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 2, killMacro: 'can_kill_baba_nuts', logicMacros: [], soulMacro: 'soul_deku_baba', requiresNoCutscene: true },
  { key: 'mm-carnivorous-lily-foreign', name: 'Carnivorous Lily Pad', game: 'mm', actorId: 0x13a, objectId: 0x161, habitat: 'water-surface', foreignOnly: true, params: [], spawnParams: 0x0fff, killMacro: 'can_kill_carnivorous_lilies', logicMacros: [], soulMacro: 'can_kill_carnivorous_lilies' },
  { key: 'mm-wart-foreign', name: 'Wart', game: 'mm', actorId: 0x12c, objectId: 0x15d, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0x807f, killMacro: 'can_kill_warts', logicMacros: [], soulMacro: 'can_kill_warts', soulItem: 'SOUL_ENEMY_WART' },
  { key: 'mm-snapper-gekko-foreign', name: 'Woodfall Gekko', game: 'mm', actorId: 0x7, objectId: 0x128, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0x807f, killMacro: 'can_kill_gekko_wft', logicMacros: [], soulMacro: 'can_kill_gekko_wft', soulItem: 'SOUL_ENEMY_GEKKO' },
  { key: 'mm-big-slime-foreign', name: 'Great Bay Gekko', game: 'mm', actorId: 0x65, objectId: 0x128, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0x807f, killMacro: 'can_kill_gekko_gbt', logicMacros: [], soulMacro: 'can_kill_gekko_gbt', soulItem: 'SOUL_ENEMY_GEKKO' },
  { key: 'mm-ikana-throne-foreign', name: 'Igos du Ikana', game: 'mm', actorId: 0x115, objectId: 0x156, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0x807f, killMacro: 'can_kill_igos_solo', logicMacros: [], soulMacro: 'can_kill_igos_solo' },
  { key: 'mm-ikana-trio-foreign', name: 'Igos du Ikana', game: 'mm', actorId: 0x115, objectId: 0x156, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0x907f, killMacro: 'can_kill_igos_solo', logicMacros: [], soulMacro: 'can_kill_igos_solo' },
  { key: 'mm-ikana-soldier-foreign', name: 'Ikana Soldier', game: 'mm', actorId: 0x115, objectId: 0x156, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0x817f, killMacro: 'can_kill_igos_solo', logicMacros: [], soulMacro: 'can_kill_igos_solo' },
  { key: 'mm-ikana-soldier-wide-foreign', name: 'Ikana Soldier', game: 'mm', actorId: 0x115, objectId: 0x156, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0x827f, killMacro: 'can_kill_igos_solo', logicMacros: [], soulMacro: 'can_kill_igos_solo' },
  { key: 'mm-captain-keeta-foreign', name: 'Captain Keeta', game: 'mm', actorId: 0x28f, objectId: 0x268, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0x807f, killMacro: 'can_kill_keeta', logicMacros: [], soulMacro: 'can_kill_keeta', soulItem: 'SOUL_ENEMY_CAPTAIN_KEETA' },
  { key: 'mm-garo-master-foreign', name: 'Garo Master', game: 'mm', actorId: 0x182, objectId: 0x155, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0x807f, killMacro: 'can_kill_garo_master', logicMacros: [], soulMacro: 'soul_garo', soulItem: 'SOUL_ENEMY_GARO' },
  { key: 'mm-gomess-foreign', name: 'Gomess', game: 'mm', actorId: 0x43, objectId: 0x52, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0x807f, killMacro: 'can_kill_gomess', logicMacros: [], soulMacro: 'soul_gomess', soulItem: 'SOUL_ENEMY_GOMESS' },
  ...[0, 1].map(ice => ({
    key: `mm-wizzrobe-${ice ? 'ice' : 'fire'}-foreign`,
    name: ice ? 'Ice Wizzrobe' : 'Fire Wizzrobe', game: 'mm' as const, actorId: 0x15d, objectId: 0x178,
    habitat: 'ground' as const, foreignOnly: true, params: [],
    spawnParams: 0x807f | (ice << 8), killMacro: 'can_kill_wizzrobe_close', logicMacros: [],
    soulMacro: 'soul_wizzrobe', soulItem: 'SOUL_ENEMY_WIZZROBE' as const,
  })),
  {
    key: 'mm-skullfish-independent-foreign', name: 'Skullfish', game: 'mm', actorId: 0x180, objectId: 0x16b,
    habitat: 'underwater', foreignOnly: true, params: [], spawnParams: 0x192,
    killMacro: 'can_kill_skullfish', logicMacros: [], soulMacro: 'soul_skull_fish', soulItem: 'SOUL_ENEMY_SKULLFISH', requiresNoCutscene: true
  },
  {
    key: 'mm-dexihand-foreign', name: 'Dexihand', game: 'mm', actorId: 0x1d1, objectId: 0x1b5,
    habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0,
    killMacro: 'can_kill_dexihands', logicMacros: [], soulItem: 'SOUL_ENEMY_DEXIHAND', requiresNoCutscene: true
  },
  {
    key: 'mm-death-armos-foreign', name: 'Death Armos', game: 'mm', actorId: 0x2d, objectId: 0x1d,
    habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0xff,
    killMacro: 'can_kill_death_armoses', logicMacros: [], soulMacro: 'soul_armos', soulItem: 'SOUL_ENEMY_ARMOS', requiresNoCutscene: true
  },
  {
    key: 'mm-garahna-foreign', name: 'Garahna', game: 'mm', actorId: 0x23f, objectId: 0x15c,
    habitat: 'underwater', foreignOnly: true, params: [], spawnParams: 0,
    killMacro: 'can_kill_garahnas', logicMacros: [], soulMacro: 'soul_garahna', soulItem: 'SOUL_ENEMY_GARAHNA', requiresNoCutscene: true
  },
  {
    key: 'mm-takkuri-foreign', name: 'Takkuri', game: 'mm', actorId: 0x291, objectId: 0x22,
    habitat: 'air', foreignOnly: true, params: [], spawnParams: 0,
    killMacro: 'can_kill_takkuri', logicMacros: [], soulMacro: 'soul_takkuri', soulItem: 'SOUL_ENEMY_TAKKURI', requiresNoCutscene: true
  },
  {
    key: 'mm-eyegore-foreign', name: 'Eyegore', game: 'mm', actorId: 0x184, objectId: 0x18d,
    habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0x1fc0,
    killMacro: 'can_kill_eyegore_world', logicMacros: [], soulItem: 'SOUL_ENEMY_EYEGORE', miniboss: true, requiresNoCutscene: true
  },
];

const FOREIGN_MM_VARIANT_ENEMIES: readonly EnemyDefinition[] = [
  ...[0x0202, 0x0801, 0x0a01, 0x0c01, 0x1402, 0xff00, 0xff01, 0xff02].map(params => ({
    key: `mm-chuchu-foreign-${params.toString(16)}`, name: ['Blue ChuChu', 'Green ChuChu', 'Yellow ChuChu'][params & 0xff], game: 'mm' as const,
    actorId: 0x14a, objectId: 0x16a, habitat: 'ground' as const, foreignOnly: true, params: [], spawnParams: params,
    killMacro: 'can_kill_chuchu', logicMacros: [], soulItem: 'SOUL_ENEMY_CHUCHU', requiresNoCutscene: true,
  })),
  {
    key: 'mm-desbreko-foreign-f00', name: 'Desbreko school (15)', game: 'mm', actorId: 0x14b, objectId: 0x16b, habitat: 'underwater',
    foreignOnly: true, params: [], spawnParams: 0x0f00, killMacro: 'can_kill_skullfish', logicMacros: [], soulMacro: 'soul_skull_fish', requiresNoCutscene: true
  },
  {
    key: 'mm-masked-hiploop-foreign', name: 'Masked Hiploop', game: 'mm', actorId: 0x1e9, objectId: 0x1c6, habitat: 'ground',
    foreignOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_hiploops_masked', logicMacros: [], soulItem: 'SOUL_ENEMY_HIPLOOP', requiresNoCutscene: true
  },
  ...[0x0102, 0x0103, 0x0134, 0x019f].map(params => ({
    key: `mm-bad-bat-foreign-${params.toString(16)}`, name: params === 0x019f ? 'Perched Bad Bat' : 'Bad Bat swarm', game: 'mm' as const,
    actorId: 0x15b, objectId: 0x172, habitat: 'air' as const, foreignOnly: true, params: [], spawnParams: params,
    killMacro: params === 0x019f ? 'can_kill_bad_bats_perched' : 'can_kill_bad_bats_cluster', logicMacros: [], soulItem: 'SOUL_ENEMY_BAD_BAT', requiresNoCutscene: true,
  })),
  ...[0x8014, 0x8023, 0x8028].map(params => ({
    key: `mm-real-bombchu-foreign-${params.toString(16)}`, name: 'Respawning Real Bombchu', game: 'mm' as const,
    actorId: 0x16f, objectId: 0x181, habitat: 'ground' as const, foreignOnly: true, params: [], spawnParams: params,
    killMacro: 'can_kill_real_bombchu', logicMacros: [], soulMacro: 'soul_real_bombchu', requiresNoCutscene: true,
  })),
  ...[1, 5, 0x28, 0x32, 0x3c, 0x46, 0x8c].map(params => ({
    key: `mm-real-bombchu-foreign-${params.toString(16)}`, name: 'Dungeon Real Bombchu', game: 'mm' as const,
    actorId: 0x16f, objectId: 0x181, habitat: 'ground' as const, foreignOnly: true, params: [], spawnParams: params,
    killMacro: 'can_kill_real_bombchu', logicMacros: [], soulMacro: 'soul_real_bombchu', requiresNoCutscene: true,
  })),
];

const KEETA_GUARDS: EnemyDefinition = { key: 'mm-keeta-guards', name: 'Keeta chase Stalchildren', game: 'mm', actorId: 0xed, objectId: 0x142, habitat: 'ground', nativeOnly: true, targetAllowed: false, params: [], spawnParams: 0, killMacro: 'can_kill_keeta_guards', soulMacro: 'can_kill_keeta_guards', logicMacros: ['can_kill_keeta_guards'] };

const FOREIGN_MM_EXTRA_VARIANT_ENEMIES: readonly EnemyDefinition[] = [
  ...[2, 3].map(params => ({
    key: `mm-dragonfly-foreign-${params}`, name: params === 2 ? 'Temple Dragonfly' : 'Woodfall Dragonfly',
    game: 'mm' as const, actorId: 0x109, objectId: 0x14e, habitat: 'air' as const,
    foreignOnly: true, params: [], spawnParams: params,
    killMacro: 'can_kill_dragonflies_distance', logicMacros: [], soulItem: 'SOUL_ENEMY_DRAGONFLY', requiresNoCutscene: true,
  })),
  ...[1, 4].map(params => ({
    key: `mm-bio-baba-foreign-${params}`, name: params === 1 ? 'Leafless Bio Deku Baba' : 'Walking Bio Deku Baba',
    game: 'mm' as const, actorId: 0x12d, objectId: 0x15e, habitat: 'ground' as const,
    foreignOnly: true, params: [], spawnParams: params,
    killMacro: 'can_kill_bio_babas', logicMacros: [], soulItem: 'SOUL_ENEMY_BIO_BABA', requiresNoCutscene: true,
  })),
];

function additionalForeignEnemyParams(game: Game, actorId: number): readonly number[] {
  if (game === 'oot' && actorId === 0x197)
    return [0];
  if (game === 'mm' && actorId === 0x21d)
    return [0x1fc0];
  if (game === 'mm' && actorId === 0xb) return [1];
  if (game === 'oot' && [0x69, 0x1d, 0x37, 0x8a, 0x1b0].includes(actorId))
    return ({ 0x69: [0xfd, 0x2fc, 0x2fb], 0x1d: [0], 0x37: [1], 0x8a: [0x601], 0x1b0: [5, 10] } as Record<number, number[]>)[actorId];
  if (game === 'oot' && (actorId === 0xa4 || actorId === 0x33))
    return [0];
  if (game === 'mm' && actorId === 0x115)
    return [0x807f, 0x907f, 0x817f, 0x827f];
  if (game === 'mm' && [0x43, 0x182, 0x12c, 0x7, 0x65, 0x28f].includes(actorId))
    return [0x807f];
  if (game === 'mm' && actorId === 0x15d)
    return [0x807f, 0x817f];
  if (game === 'mm' && actorId === 0x23d)
    return [0x14];
  if (game === 'mm' && actorId === 0x184)
    return [0x1fc0];
  return game !== 'mm' ? [] : actorId === 0x113 ? [0] : actorId === 0x180 ? [0x192] : actorId === 0x1d1 ? [0] : actorId === 0x2d ? [0xff] : (actorId === 0x23f || actorId === 0x291) ? [0] : [];
}

export function additionalForeignEnemyTarget(game: Game, actorId: number, params?: number): boolean {
  const forms = additionalForeignEnemyParams(game, actorId);
  return params === undefined ? forms.length > 0 : forms.includes(params);
}

const NATIVE_FLOOR_TILE_ENEMIES: readonly EnemyDefinition[] = [
  {
    key: 'oot-flying-floor-tile-source', name: 'Flying Floor Tile', game: 'oot',
    actorId: 0x6b, objectId: 0x5f, habitat: 'ground', nativeOnly: true,
    params: [0xffff, 3, 7, 9, 13, 17, 21, 25], spawnParams: 0xffff,
    killMacro: 'can_clear_flying_floor_tiles', logicMacros: ['can_clear_flying_floor_tiles'],
    soulMacro: 'can_clear_flying_floor_tiles'
  },
];

const NATIVE_MM_ENEMIES: readonly EnemyDefinition[] = [
  {
    key: 'mm-deku-baba', nativeOnly: true, preserveNativeResource: true,
    name: 'Deku Baba', game: 'mm', actorId: 0x33, objectId: 0x31, habitat: 'ground', params: [0], spawnParams: 0,
    killMacro: 'can_kill_baba_nuts', logicMacros: ['can_kill_baba_nuts', 'can_kill_baba_sticks', 'can_kill_baba_both_sticks'],
    soulMacro: 'soul_deku_baba', requiresNoCutscene: true
  },
  {
    key: 'mm-mini-baba', nativeOnly: true, preserveNativeResource: true, targetAllowed: false,
    name: 'Mini Baba', game: 'mm', actorId: 0x66, objectId: 0x31, habitat: 'ground', params: [1, 2], spawnParams: 1,
    killMacro: 'can_kill_baba_sticks', logicMacros: ['can_kill_baba_nuts', 'can_kill_baba_sticks', 'can_kill_baba_both_sticks'],
    soulMacro: 'soul_deku_baba', requiresNoCutscene: true
  },
  {
    key: 'mm-mad-scrub', nativeOnly: true, preserveNativeResource: true, name: 'Mad Scrub', game: 'mm', actorId: 0x3b,
    objectId: 0x40, habitat: 'ground', params: [0xff00, 0xff02], spawnParams: 0xff00,
    killMacro: 'can_kill_scrubs_anywhere', logicMacros: ['can_kill_scrubs_anywhere', 'can_kill_scrubs_woodfall'],
    soulMacro: 'soul_deku_scrub', requiresNoCutscene: true
  },
  {
    key: 'mm-shabom', nativeOnly: true, name: 'Shabom', game: 'mm', actorId: 0x1d, objectId: 0x0e, habitat: 'air',
    params: [0], spawnParams: 0, killMacro: 'can_kill_shaboms', logicMacros: ['can_kill_shaboms'],
    soulMacro: 'can_kill_shaboms', requiresNoCutscene: true
  },
  {
    key: 'mm-guay', nativeOnly: true, name: 'Guay', game: 'mm', actorId: 0xf1, objectId: 0x06, habitat: 'air',
    params: [0], spawnParams: 0, killMacro: 'can_kill_guays', logicMacros: ['can_kill_guays', 'can_kill_guays_distance'],
    requiresNoCutscene: true
  },
  {
    key: 'mm-bubble', nativeOnly: true, name: 'Blue Bubble', game: 'mm', actorId: 0x3e, objectId: 0x51, habitat: 'air',
    params: [0xffff], spawnParams: 0xffff, killMacro: 'can_kill_bubbles', logicMacros: ['can_kill_bubbles'],
    soulMacro: 'soul_bubble', requiresNoCutscene: true
  },
  {
    key: 'mm-red-bubble', nativeOnly: true, targetAllowed: false, name: 'Red Bubble', game: 'mm', actorId: 0x3c,
    objectId: 0x51, habitat: 'ground', params: [0], spawnParams: 0, killMacro: 'can_kill_bubbles',
    logicMacros: ['can_kill_bubbles'], soulMacro: 'soul_bubble', requiresNoCutscene: true
  },
  {
    key: 'mm-skulltula', nativeOnly: true, targetAllowed: false, name: 'Skulltula', game: 'mm', actorId: 0x24,
    objectId: 0x20, habitat: 'air', params: [0x003f, 0x007f], spawnParams: 0x003f,
    killMacro: 'can_kill_skulltulas', logicMacros: ['can_kill_skulltulas'], soulMacro: 'soul_skulltula',
    requiresNoCutscene: true
  },
];

const FOREIGN_STALFOS_VARIANTS: readonly EnemyDefinition[] = [{ key: 'oot-stalfos-invisible-foreign', name: 'Invisible Stalfos', game: 'oot', actorId: 2, objectId: 0x32, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_invisible_stalfos', logicMacros: [], soulMacro: 'soul_stalfos' }];

const OOT_NATIVE_ENEMIES: readonly EnemyDefinition[] = [
  { key: 'oot-stalfos-invisible-native-target', name: 'Invisible Stalfos', game: 'oot', actorId: 2, objectId: 0x32, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_invisible_stalfos', logicMacros: [], soulMacro: 'soul_stalfos' },
  { key: 'oot-skulltula-invisible-native-target', name: 'Invisible Skulltula', game: 'oot', actorId: 0x37, objectId: 0x24, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 2, killMacro: 'can_kill_invisible_skulltulas', logicMacros: [], soulMacro: 'soul_skulltula' },
  { key: 'oot-red-bubble-native-target', name: 'Red Bubble', game: 'oot', actorId: 0x69, objectId: 0x5d, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0xfffe, killMacro: 'can_kill_red_bubbles', logicMacros: [], soulMacro: 'soul_bubble' },
  { key: 'oot-keese-invisible-native-target', name: 'Invisible Keese', game: 'oot', actorId: 0x13, objectId: 0x0d, habitat: 'air', nativeOnly: true, params: [], spawnParams: 0x8002, killMacro: 'can_kill_invisible_keese', logicMacros: [], soulMacro: 'soul_keese' },
  {
    key: 'oot-shabom', nativeOnly: true, name: 'Shabom', game: 'oot', actorId: 0x2d, objectId: 0x12,
    habitat: 'air', params: [0], spawnParams: 0, killMacro: 'can_kill_shaboms',
    logicMacros: ['can_kill_shaboms'], soulMacro: 'soul_shaboms'
  },
  {
    key: 'oot-skulltula', nativeOnly: true, name: 'Skulltula', game: 'oot', actorId: 0x37, objectId: 0x24,
    habitat: 'ground', params: [0, 1], spawnParams: 0, killMacro: 'can_kill_skulltulas',
    logicMacros: ['can_kill_skulltulas'], soulMacro: 'soul_skulltula'
  },
  {
    key: 'oot-skullwalltula', nativeOnly: true, targetAllowed: false, name: 'Skullwalltula', game: 'oot',
    actorId: 0x95, objectId: 0x24, habitat: 'air', params: [0], spawnParams: 0,
    killMacro: 'can_kill_skullwalltulas', logicMacros: ['can_kill_skullwalltulas'], soulMacro: 'soul_skullwalltula'
  },
  {
    key: 'oot-deku-scrub', nativeOnly: true, name: 'Deku Scrub', game: 'oot', actorId: 0x60, objectId: 0x4a,
    habitat: 'ground', params: Array.from({ length: 256 }, (_, shots) => shots << 8), spawnParams: 0,
    killMacro: 'can_kill_mad_scrubs', logicMacros: ['can_kill_mad_scrubs'], soulMacro: 'soul_deku_scrub'
  },
  {
    key: 'oot-business-scrub-combat', nativeOnly: true, name: 'Business Scrub', game: 'oot',
    actorId: 0x195, objectId: 0x168, habitat: 'ground', params: [], spawnParams: 0x4000,
    killMacro: 'can_kill_mad_scrubs', logicMacros: [], soulMacro: 'soul_deku_scrub'
  },
  {
    key: 'oot-anubis-native', nativeOnly: true, name: 'Anubis', game: 'oot',
    actorId: 0xf6, objectId: 1, habitat: 'ground', params: [0, 1, 2, 3, 0xffff], spawnParams: 3,
    killMacro: 'can_kill_anubis', logicMacros: ['can_kill_anubis'], soulItem: 'SOUL_ENEMY_ANUBIS'
  },
  {
    key: 'oot-flare-dancer-native', nativeOnly: true, name: 'Flare Dancer', game: 'oot', actorId: 0x99,
    objectId: 0x9e, habitat: 'ground', params: [0], spawnParams: 0, killMacro: 'can_kill_flare_dancer',
    logicMacros: ['can_kill_flare_dancer'], soulItem: 'SOUL_ENEMY_FLARE_DANCER', miniboss: true
  },
  {
    key: 'oot-hint-scrub', nativeOnly: true, targetAllowed: false, name: 'Hint Deku Scrub', game: 'oot',
    actorId: 0x192, objectId: 0x164, habitat: 'ground', params: [0x1a00], spawnParams: 0x1a00,
    killMacro: 'can_kill_yellow_scrubs', logicMacros: ['can_kill_yellow_scrubs'], soulMacro: 'soul_deku_scrub'
  },
];

const NATIVE_OOT_RESOURCE_ENEMIES: readonly EnemyDefinition[] = [
  {
    key: 'oot-deku-baba', name: 'Deku Baba', game: 'oot',
    actorId: 0x55, objectId: 0x39, habitat: 'ground', nativeOnly: true,
    preserveNativeResource: true, params: [0, 1], spawnParams: 0,
    killMacro: 'can_kill_baba_nuts', logicMacros: ['can_kill_baba_nuts', 'can_kill_baba_sticks'],
    soulMacro: 'soul_deku_baba'
  },
  {
    key: 'oot-withered-baba', name: 'Withered Deku Baba', game: 'oot',
    actorId: 0xc7, objectId: 0x39, habitat: 'ground', nativeOnly: true,
    preserveNativeResource: true, targetAllowed: false,
    params: [0, 1, 0xffff], spawnParams: 1,
    killMacro: 'can_kill_baba_sticks', logicMacros: ['can_kill_baba_sticks', 'can_kill_baba_nuts'],
    soulMacro: 'soul_deku_baba'
  },
];

const NATIVE_PEAHAT_TARGETS: readonly EnemyDefinition[] = [
  { key: 'oot-peahat-native-ground', name: 'Peahat', game: 'oot', actorId: 0x1d, objectId: 0x18, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0xffff, killMacro: 'can_kill_randomized_peahats_ground', logicMacros: [], soulMacro: 'soul_peahat' },
  { key: 'oot-peahat-native-flying', name: 'Flying Peahat', game: 'oot', actorId: 0x1d, objectId: 0x18, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_randomized_peahats_high', logicMacros: [], soulMacro: 'soul_peahat' },
  { key: 'mm-peahat-native-target', name: 'Peahat', game: 'mm', actorId: 0x14, objectId: 0x14, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_randomized_peahats', logicMacros: [], soulMacro: 'soul_peahat' },
];

const NATIVE_OOT_POE_TARGET: EnemyDefinition = {
  key: 'oot-poe-native-target', name: 'Poe', game: 'oot', actorId: 0x0d, objectId: 1, habitat: 'air',
  params: [], spawnParams: 0, killMacro: 'can_kill_poes', logicMacros: [], soulMacro: 'soul_poe',
  nativeOnly: true,
};

const NATIVE_RENEWABLE_BABA_TARGETS: readonly EnemyDefinition[] = [
  { key: 'oot-withered-baba-native-target', name: 'Withered Deku Baba', game: 'oot', actorId: 0xc7, objectId: 0x39, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 1, killMacro: 'can_kill_baba_sticks', logicMacros: [], soulMacro: 'soul_deku_baba' },
  ...[1, 2].map(params => ({ key: `mm-mini-baba-native-target-${params}`, name: params === 1 ? 'Withered Deku Baba' : 'Mini Baba', game: 'mm' as const, actorId: 0x66, objectId: 0x31, habitat: 'ground' as const, nativeOnly: true, params: [], spawnParams: params, killMacro: params === 1 ? 'can_kill_baba_sticks' : 'can_kill_baba_nuts', logicMacros: [], soulMacro: 'soul_deku_baba', requiresNoCutscene: true })),
];

const NATIVE_SKULL_KID_ENEMIES: readonly EnemyDefinition[] = [
  {
    key: 'oot-skull-kid-combat-source', name: 'Combat Skull Kid', game: 'oot', actorId: 0x115,
    objectId: 0x10a, habitat: 'ground', nativeOnly: true, targetAllowed: false, sourceFormsIndependent: true,
    params: [0xffff], spawnParams: 0xffff, killMacro: 'can_kill_skull_kids',
    logicMacros: ['can_kill_skull_kids'], soulMacro: 'soul_skull_kids'
  },
  {
    key: 'oot-skull-kid-native', name: 'Skull Kid', game: 'oot', actorId: 0x115,
    objectId: 0x10a, habitat: 'ground', nativeOnly: true,
    params: [], spawnParams: 0xffff, killMacro: 'can_kill_skull_kids', logicMacros: [],
    soulMacro: 'soul_skull_kids'
  },
];

const NATIVE_MM_SKULLWALLTULA_TARGETS: readonly EnemyDefinition[] = [
  { key: 'mm-skullwalltula-native-target', name: 'Skullwalltula', game: 'mm', actorId: 0x50, objectId: 0x20, habitat: 'ground', nativeOnly: true, params: [0xfffc], spawnParams: 0xff00, killMacro: 'can_kill_skullwalltulas', logicMacros: [], soulMacro: 'soul_skullwalltula', requiresNoCutscene: true },
];

type NativeTargetOverride = Pick<EnemyDefinition, 'spawnParams'> & Partial<Pick<EnemyDefinition, 'targetAllowed' | 'requiresNoCutscene'>>;

const NATIVE_TARGET_FORM_OVERRIDES: Readonly<Record<string, NativeTargetOverride>> = {
  'oot-stalfos': { spawnParams: 3 },
  'oot-lizalfos': { spawnParams: 0xffff },
  'oot-armos': { spawnParams: 0xffff },
  'oot-beamos': { spawnParams: 0x0500 },
  'oot-redead': { spawnParams: 0x7f02 },
  'oot-tektite': { spawnParams: 0 },
  'oot-iron-knuckle': { spawnParams: 0xff82 },
  'mm-octorok': { spawnParams: 0xff00 },
  'mm-freezard': { spawnParams: 2, requiresNoCutscene: true },
  'mm-wolfos': { spawnParams: 0xff00, requiresNoCutscene: true },
  'mm-keese': { spawnParams: 0 },
  'mm-stalchild': { spawnParams: 0x0022, requiresNoCutscene: true },
};

const NATIVE_SKULLTULA_TARGET_VARIANTS: readonly EnemyDefinition[] = [
  {
    key: 'mm-skulltula-visible-target', name: 'Skulltula', game: 'mm', actorId: 0x24, objectId: 0x20, habitat: 'air',
    params: [], spawnParams: 0x3f, killMacro: 'can_kill_skulltulas', logicMacros: [], soulMacro: 'soul_skulltula',
    nativeOnly: true, requiresNoCutscene: true
  },
  {
    key: 'mm-skulltula-lens-target', name: 'Invisible Skulltula', game: 'mm', actorId: 0x24, objectId: 0x20, habitat: 'air',
    params: [], spawnParams: 0x7f, killMacro: 'can_kill_skulltulas_lens', logicMacros: [], soulMacro: 'soul_skulltula',
    nativeOnly: true, requiresNoCutscene: true
  },
];

const NATIVE_WALLMASTER_TARGETS: readonly EnemyDefinition[] = [
  {
    key: 'oot-wallmaster', nativeOnly: true, sourceFormsIndependent: true, name: 'Wallmaster', game: 'oot',
    actorId: 0x11, objectId: 0xb, habitat: 'ground', params: [1], spawnParams: 0,
    killMacro: 'can_kill_wallmasters', logicMacros: ['can_kill_wallmasters'], soulMacro: 'soul_wallmaster'
  },
  {
    key: 'mm-wallmaster', nativeOnly: true, name: 'Wallmaster', game: 'mm',
    actorId: 0xa, objectId: 9, habitat: 'ground', params: [], spawnParams: 1,
    killMacro: 'can_kill_wallmasters', logicMacros: [], soulMacro: 'soul_wallmaster', requiresNoCutscene: true
  },
];

const MM_NATIVE_EXTRA_VARIANTS: readonly EnemyDefinition[] = [
  { key: 'mm-frozen-wallmaster-native-target', name: 'Frozen Wallmaster', game: 'mm', actorId: 0xa, objectId: 0x9, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x7f81, killMacro: 'can_kill_frozen_wallmasters', logicMacros: [], soulMacro: 'soul_wallmaster', requiresNoCutscene: true },
  { key: 'mm-red-ice-wallmaster-native-target', name: 'Red-Ice Wallmaster', game: 'mm', actorId: 0xa, objectId: 0x9, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x7e81, killMacro: 'can_kill_red_ice_wallmasters', logicMacros: [], soulMacro: 'soul_wallmaster', requiresNoCutscene: true },
  { key: 'mm-frozen-redead-native-target', name: 'Frozen ReDead', game: 'mm', actorId: 0x4c, objectId: 0x75, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x7f04, killMacro: 'can_kill_frozen_redead', logicMacros: [], soulMacro: 'soul_redead_gibdo' },
  { key: 'mm-red-ice-redead-native-target', name: 'Red-Ice ReDead', game: 'mm', actorId: 0x4c, objectId: 0x75, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x7e04, killMacro: 'can_kill_red_ice_redead', logicMacros: [], soulMacro: 'soul_redead_gibdo' },
  { key: 'mm-floormaster-invisible-native-target', name: 'Invisible Floormaster', game: 'mm', actorId: 0x4a, objectId: 0x9, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x8000, killMacro: 'can_kill_floormasters_lens', logicMacros: [], soulMacro: 'soul_floormaster' },
  { key: 'mm-leever-large-native-target', name: 'Large Leever', game: 'mm', actorId: 0x216, objectId: 0x201, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x8000, killMacro: 'can_kill_leevers', logicMacros: [], soulMacro: 'soul_leever' },
  { key: 'mm-guay-mega-native-target', name: 'Mega Guay', game: 'mm', actorId: 0xf1, objectId: 0x6, habitat: 'air', nativeOnly: true, params: [], spawnParams: 0x1, killMacro: 'can_kill_guays', logicMacros: [], soulMacro: 'soul_guay' },
  {
    key: 'mm-business-scrub-combat', name: 'Business Scrub', game: 'mm', actorId: 0x3b, objectId: 0x40,
    habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0xff03,
    killMacro: 'can_kill_scrubs_anywhere', logicMacros: [], soulMacro: 'soul_deku_scrub'
  },
  { ...MOTH_ENEMY, key: 'mm-moth-native-target', foreignOnly: false, nativeOnly: true },
  ...['Meg', 'Joelle', 'Beth', 'Amy'].map((name, type) => ({ key: `mm-poe-sister-native-${type}`, name, game: 'mm' as const, actorId: 0x1e8, objectId: 0x1c5, habitat: 'air' as const, miniboss: type === 0, nativeOnly: true, params: [], spawnParams: type << 8, killMacro: 'can_kill_poe_sisters', logicMacros: [], soulMacro: 'soul_poe' })),
  { key: 'mm-poe-sisters-encounter-native-target', name: 'Meg', game: 'mm', actorId: 0x1e8, objectId: 0x1c5, habitat: 'air', miniboss: true, nativeOnly: true, params: [], spawnParams: 0x4000, killMacro: 'can_kill_poe_sisters', logicMacros: [], soulMacro: 'soul_poe' },
  ...FOREIGN_EXTRA_NATIVE_MM_ENEMIES.map(row => ({ ...row, key: row.key.replace('-foreign', '-native-target'), nativeOnly: true, foreignOnly: false })),
  { key: 'mm-pirate-fighter-native-target', name: 'Pirate Fighter', game: 'mm', actorId: 0x21d, objectId: 0x204, habitat: 'ground', params: [], spawnParams: 0x1fc0, killMacro: 'can_fight_pirates', logicMacros: [], soulMacro: 'soul_thief_fighter' },
  { key: 'mm-big-poe-native-target', name: 'Big Poe', game: 'mm', actorId: 0x208, objectId: 0x1f1, habitat: 'air', nativeOnly: true, params: [], spawnParams: 0xff00, killMacro: 'can_kill_big_poes', logicMacros: [], soulMacro: 'soul_poe' },
  { key: 'mm-flying-pot-native-target', name: 'Flying Pot', game: 'mm', actorId: 0x8d, objectId: 3, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x3fff, killMacro: 'can_kill_flying_pots', logicMacros: [], soulMacro: 'soul_flying_pot' },
  { key: 'mm-leever-native-target', name: 'Leever', game: 'mm', actorId: 0x216, objectId: 0x201, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_leevers', logicMacros: [], soulMacro: 'soul_leever' },
  { key: 'mm-garo-native-target', name: 'Garo', game: 'mm', actorId: 0x113, objectId: 0x155, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_garo', logicMacros: [], soulItem: 'SOUL_ENEMY_GARO' },
  ...[2, 3, 0x8000, 0x8002, 0x8004].map(params => ({ key: `mm-native-keese-${params.toString(16)}`, name: params & 0x8000 ? 'Invisible Keese' : 'Keese', game: 'mm' as const, actorId: 0xc, objectId: 0xb, habitat: 'air' as const, nativeOnly: true, params: [], spawnParams: params, killMacro: params & 0x8000 ? 'can_kill_keese_lens' : 'can_kill_keese_close', logicMacros: [], soulMacro: 'soul_keese' })),
  ...[0x7ffe, 0x7f00, 0x7f01, 0x7f02, 0x7f03].map(params => ({ key: `mm-native-redead-${params.toString(16)}`, name: params === 0x7ffe ? 'Gibdo' : params === 0x7f03 ? 'Invisible ReDead' : 'ReDead', game: 'mm' as const, actorId: 0x4c, objectId: 0x75, habitat: 'ground' as const, nativeOnly: true, params: [], spawnParams: params, killMacro: params === 0x7f03 ? 'can_kill_redead_lens' : 'can_kill_redead_gibdo', logicMacros: [], soulMacro: 'soul_redead_gibdo' })),
  { key: 'mm-native-big-deku-baba', name: 'Large Deku Baba', game: 'mm', actorId: 0x33, objectId: 0x31, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 1, killMacro: 'can_kill_baba_sticks', logicMacros: [], soulMacro: 'soul_deku_baba' },
];

const OOT_NATIVE_EXTRA_VARIANTS: readonly EnemyDefinition[] = [
  { key: 'oot-frozen-wallmaster-native-target', name: 'Frozen Wallmaster', game: 'oot', actorId: 0x11, objectId: 0xb, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x7f80, killMacro: 'can_kill_frozen_wallmasters', logicMacros: [], soulMacro: 'soul_wallmaster' },
  { key: 'oot-red-ice-wallmaster-native-target', name: 'Red-Ice Wallmaster', game: 'oot', actorId: 0x11, objectId: 0xb, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x7e80, killMacro: 'can_kill_red_ice_wallmasters', logicMacros: [], soulMacro: 'soul_wallmaster' },
  { key: 'oot-small-beamos-native-target', name: 'Small Beamos', game: 'oot', actorId: 0x8a, objectId: 0x8b, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x601, killMacro: 'can_kill_beamos', logicMacros: [], soulMacro: 'soul_beamos' },
  { key: 'oot-frozen-redead-native-target', name: 'Frozen ReDead', game: 'oot', actorId: 0x90, objectId: 0x98, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x7f04, killMacro: 'can_kill_frozen_redead', logicMacros: [], soulMacro: 'soul_redead_gibdo' },
  { key: 'oot-red-ice-redead-native-target', name: 'Red-Ice ReDead', game: 'oot', actorId: 0x90, objectId: 0x98, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x7e04, killMacro: 'can_kill_red_ice_redead', logicMacros: [], soulMacro: 'soul_redead_gibdo' },
  { key: 'oot-floormaster-invisible-native-target', name: 'Invisible Floormaster', game: 'oot', actorId: 0x8e, objectId: 0xb, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x8000, killMacro: 'can_kill_floormasters_lens', logicMacros: [], soulMacro: 'soul_floormaster' },
  { key: 'oot-leever-large-native-target', name: 'Large Leever', game: 'oot', actorId: 0x1c, objectId: 0x17, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x1, killMacro: 'can_kill_leevers', logicMacros: [], soulMacro: 'soul_leever' },
  { key: 'oot-guay-mega-native-target', name: 'Mega Guay', game: 'oot', actorId: 0x1c0, objectId: 0x8, habitat: 'air', nativeOnly: true, params: [], spawnParams: 0x1, killMacro: 'can_kill_guays', logicMacros: [], soulMacro: 'soul_guay' },
  { key: 'oot-stalchild-large-5-native-target', name: 'Large Stalchild', game: 'oot', actorId: 0x1b0, objectId: 0x184, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0x5, killMacro: 'can_kill_stalchildren', logicMacros: [], soulMacro: 'soul_stalchild' },
  { key: 'oot-stalchild-large-10-native-target', name: 'Large Stalchild', game: 'oot', actorId: 0x1b0, objectId: 0x184, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0xa, killMacro: 'can_kill_stalchildren', logicMacros: [], soulMacro: 'soul_stalchild' },
  ...[5, 6, 7].map(type => ({
    key: `oot-dancing-redead-${type}`, name: 'Dancing ReDead', game: 'oot' as const,
    actorId: 0x90, objectId: 0x98, habitat: 'ground' as const, nativeOnly: true,
    params: [], spawnParams: 0x7f00 | type,
    killMacro: 'can_kill_redead_gibdo', logicMacros: [], soulMacro: 'soul_redead_gibdo',
  })),
  ...[0x7f00, 0x7f01, 0x7f03].map(params => ({ key: `oot-native-redead-${params.toString(16)}`, name: params === 0x7f03 ? 'Invisible ReDead' : params === 0x7f00 ? 'ReDead (mourns when stationary)' : 'Standing ReDead', game: 'oot' as const, actorId: 0x90, objectId: 0x98, habitat: 'ground' as const, nativeOnly: true, params: [], spawnParams: params, killMacro: params === 0x7f03 ? 'can_kill_redead_lens' : 'can_kill_redead_gibdo', logicMacros: [], soulMacro: 'soul_redead_gibdo' })),
  ...[0x8000, 0x8004].map(params => ({ key: `oot-native-keese-${params.toString(16)}`, name: params === 0x8000 ? 'Invisible Fire Keese' : 'Invisible Ice Keese', game: 'oot' as const, actorId: 0x13, objectId: 0xd, habitat: 'air' as const, nativeOnly: true, params: [], spawnParams: params, killMacro: 'can_kill_keese_lens', logicMacros: [], soulMacro: 'soul_keese' })),
  { key: 'oot-dark-link-native-target', name: 'Dark Link', game: 'oot', actorId: 0x33, objectId: 0x20, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_dark_link', logicMacros: [], soulMacro: 'soul_dark_link', soulItem: 'SOUL_ENEMY_DARK_LINK' },
  { key: 'oot-dead-hand-native-target', name: 'Dead Hand', game: 'oot', actorId: 0xa4, objectId: 0xa6, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_dead_hand', logicMacros: [], soulMacro: 'soul_dead_hand', soulItem: 'SOUL_ENEMY_DEAD_HAND' },
  ...['Meg', 'Joelle', 'Beth', 'Amy'].map((name, type) => ({ key: `oot-poe-sister-native-${type}`, name, game: 'oot' as const, actorId: 0x91, objectId: 0x99, habitat: 'air' as const, miniboss: type === 0, nativeOnly: true, params: [], spawnParams: type << 8, killMacro: type === 0 ? 'can_kill_poe_meg' : 'can_kill_poe_sisters', logicMacros: [], soulMacro: 'soul_poe' })),
  { key: 'oot-poe-sisters-encounter-native-target', name: 'Meg', game: 'oot', actorId: 0x91, objectId: 0x99, habitat: 'air', miniboss: true, nativeOnly: true, params: [], spawnParams: 0x4000, killMacro: 'can_kill_poe_meg', logicMacros: [], soulMacro: 'soul_poe' },
  { key: 'oot-gerudo-fighter-native-target', name: 'Gerudo Fighter', game: 'oot', actorId: 0x197, objectId: 0x169, habitat: 'ground', params: [], spawnParams: 0, killMacro: 'can_kill_gerudo_fighter', logicMacros: [], soulMacro: 'soul_thief_fighter' },
  ...[{ params: 0x00fd, name: 'White Bubble' }, { params: 0x02fc, name: 'Green Bubble' }, { params: 0x02fb, name: 'Big Green Bubble' }].map(row => ({ key: `oot-bubble-patrol-native-${row.params.toString(16)}`, name: row.name, game: 'oot' as const, actorId: 0x69, objectId: 0x5d, habitat: 'air' as const, nativeOnly: true, params: [], spawnParams: row.params, killMacro: 'can_kill_bubbles', logicMacros: [], soulMacro: 'soul_bubble' })),
  { key: 'oot-big-poe-native-target', name: 'Big Poe', game: 'oot', actorId: 0x175, objectId: 0x6d, habitat: 'air', nativeOnly: true, params: [], spawnParams: 1, killMacro: 'can_kill_poes', logicMacros: [], soulMacro: 'soul_poe' },
  { key: 'oot-flying-pot-native-target', name: 'Flying Pot', game: 'oot', actorId: 0x11d, objectId: 3, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0xffff, killMacro: 'can_kill_flying_pots', logicMacros: [], soulMacro: 'soul_flying_pot' },
  { key: 'oot-leever-native-target', name: 'Leever', game: 'oot', actorId: 0x1c, objectId: 0x17, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_leevers', logicMacros: [], soulMacro: 'soul_leever' },
  { key: 'oot-iron-knuckle-silver-native-target', name: 'Silver Iron Knuckle', game: 'oot', actorId: 0x113, objectId: 0x106, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0xff81, killMacro: 'can_kill_iron_knuckles', logicMacros: [], soulMacro: 'soul_iron_knuckle' },
  { key: 'oot-skulltula-large-native-target', name: 'Big Skulltula', game: 'oot', actorId: 0x37, objectId: 0x24, habitat: 'air', nativeOnly: true, params: [], spawnParams: 1, killMacro: 'can_kill_skulltulas', logicMacros: [], soulMacro: 'soul_skulltula' },
];

const FOREIGN_OOT_ROSTER_VARIANTS: readonly EnemyDefinition[] = [
  ...OOT_NATIVE_EXTRA_VARIANTS.filter(e => e.actorId === 0x69 && [0xfd, 0x2fc, 0x2fb].includes(e.spawnParams) || e.actorId === 0x37 && e.spawnParams === 1 || e.actorId === 0x1b0 && [5, 10].includes(e.spawnParams)),
  NATIVE_PEAHAT_TARGETS.find(e => e.key === 'oot-peahat-native-flying')!,
  { key: 'oot-small-beamos', name: 'Small Beamos', game: 'oot', actorId: 0x8a, objectId: 0x8b, habitat: 'ground', params: [], spawnParams: 0x601, killMacro: 'can_kill_beamos', logicMacros: [], soulMacro: 'soul_beamos' } as EnemyDefinition,
].map(e => ({ ...e, key: e.key + '-foreign', nativeOnly: false, foreignOnly: true, params: [], logicMacros: [] }));

const MM_FOREIGN_DEAD_HAND: EnemyDefinition = { key: 'oot-dead-hand-foreign', name: 'Dead Hand', game: 'oot', actorId: 0xa4, objectId: 0xa6, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_dead_hand', logicMacros: [], soulMacro: 'soul_dead_hand', soulItem: 'SOUL_ENEMY_DEAD_HAND' };

const MM_FOREIGN_DARK_LINK: EnemyDefinition = { key: 'oot-dark-link-foreign', name: 'Dark Link', game: 'oot', actorId: 0x33, objectId: 0x20, habitat: 'ground', foreignOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_dark_link', logicMacros: [], soulMacro: 'soul_dark_link', soulItem: 'SOUL_ENEMY_DARK_LINK' };

export const isEnemyEncounter = (actor: typeof ENEMY_ACTOR_CATALOG[number]) =>
  !(actor.game === 'oot' && [0xb6, 0xdf].includes(actor.actorId)) &&
  (actor.role === 'enemy' || actor.role === 'miniboss' || (actor.game === 'mm' && [0x23d, 0x23f].includes(actor.actorId)) || (actor.game === 'oot' && actor.overlay === 'En_Anubice_Tag'));

const SPECIES: Readonly<Record<string, readonly string[]>> = {
  'oot/En_Bigokuta': ['big-octo-jabu'],
  'mm/En_Bigokuta': ['big-octorok-swamp'],
  'mm/En_Tanron1': ['moth'],
  'mm/En_Tanron3': ['garahna'],
  'oot/En_Anubice_Tag': ['anubis'],
  'oot/En_Anubice': ['anubis'],
  'oot/En_Reeba': ['leever'],
  'mm/En_Neo_Reeba': ['leever'],
  'oot/En_Po_Field': ['poe', 'big-poe'],
  'oot/En_Poh': ['poe'],
  'mm/En_Poh': ['poe'],
  'mm/En_Bigpo': ['big-poe'],
  'oot/En_Bb': ['bubble'],
  'mm/En_Bb': ['bubble'],
  'mm/En_Bbfall': ['bubble'],
  'oot/En_Hintnuts': ['mad-scrub'],
  'oot/En_Dekunuts': ['mad-scrub'],
  'mm/En_Dekunuts': ['mad-scrub'],
  'oot/En_GeldB': ['gerudo-fighter'],
  'mm/En_Kaizoku': ['pirate-fighter'],
  'oot/En_Zf': ['lizalfos', 'dinolfos'],
  'mm/En_Dinofos': ['dinolfos'],
};

export function enemyActorSpecies(game: Game, actorId: number, params?: number): readonly string[] {
  const actor = ENEMY_ACTOR_CATALOG.find(e => e.game === game && e.actorId === actorId && isEnemyEncounter(e));
  if (!actor)
    throw new Error(`Unknown enemy encounter: ${game}/${actorId.toString(16)}`);
  if (actor.overlay === 'En_Karebaba')
    return game === 'oot' ? ['withered-deku-baba'] : params === undefined ? ['withered-deku-baba', 'mini-baba'] : [params === 2 ? 'mini-baba' : 'withered-deku-baba'];
  if (game === 'oot' && actor.overlay === 'En_Zf' && params !== undefined)
    return [(params & 0xff) === 0xfe ? 'dinolfos' : 'lizalfos'];
  return SPECIES[`${game}/${actor.overlay}`] ?? [actor.overlay];
}

export function nativeEnemyCounterparts(sourceGame: Game, sourceActorId: number, targetGame: Game, params?: number) {
  if (targetGame !== sourceGame && params !== undefined &&
    ((enemySpawnMacroMatches('oot-foreign-extra-forms', { target: { game: sourceGame, actorId: sourceActorId, params } }) &&
      additionalForeignEnemyTarget(sourceGame, sourceActorId, params)) ||
      enemySpawnMacroMatches('mm-foreign-extra-form', { target: { game: sourceGame, actorId: sourceActorId, params } }))) return [];
  const species = enemyActorSpecies(sourceGame, sourceActorId, params);
  return ENEMY_ACTOR_CATALOG.filter(actor => actor.game === targetGame && isEnemyEncounter(actor) &&
    enemyActorSpecies(targetGame, actor.actorId).some(s => species.includes(s)));
}

export function foreignEnemyForms(sourceGame: Game, sourceActorId: number, targetGame: Game) {
  return [...standaloneEnemyParams(sourceGame, sourceActorId), ...additionalForeignEnemyParams(sourceGame, sourceActorId)]
    .map(params => sourceGame === 'oot' && sourceActorId === 0x115 && params === 0xffff ? 3 : params)
    .filter(params => !nativeEnemyCounterparts(sourceGame, sourceActorId, targetGame, params).length);
}

export function assertForeignEnemyExclusive(sourceGame: Game, sourceActorId: number, targetGame: Game, params?: number): void {
  if (sourceGame === targetGame)
    throw new Error('A foreign enemy must come from the other game');
  const native = nativeEnemyCounterparts(sourceGame, sourceActorId, targetGame, params);
  if (native.length)
    throw new Error(`Enemy already has a native counterpart in ${targetGame}: ${native.map(e => e.overlay).join(', ')}`);
}

const FORMS: Record<Game, Readonly<Record<number, readonly number[]>>> = {
  oot: {
    0x197: [0],
    0x69: [0xfd, 0x2fc, 0x2fb], 0x1d: [0], 0x37: [1], 0x8a: [0x601], 0x1b0: [5, 10],
    0x33: [0], 0xa4: [0], 0x6b: [0xffff],
    0x002: [3, 0], 0x025: [0xff80], 0x02b: [6, 0x4006], 0x02f: [0], 0x034: [0], 0x035: [0xffff],
    0x038: [0], 0x03a: [10], 0x04b: [2, 0xffff], 0x063: [0], 0x099: [0], 0x0ec: [0],
    0x0f6: [3], 0x115: [3], 0x18c: [0],
  },
  mm: {
    0x21d: [0x1fc0],
    0xb: [1], 0x12c: [0x807f], 0x7: [0x807f], 0x65: [0x807f], 0x115: [0x807f, 0x907f, 0x817f, 0x827f], 0x28f: [0x807f], 0x43: [0x807f], 0x182: [0x807f], 0x15d: [0x807f, 0x817f], 0x23d: [0x14],
    0x66: [2], 0x13a: [0x0fff], 0x113: [0], 0x180: [0x192], 0x1d1: [0], 0x2d: [0xff], 0x23f: [0], 0x291: [0], 0x184: [0x1fc0],
    0x14a: [0x0202, 0x0801, 0x0a01, 0x0c01, 0x1402, 0xff00, 0xff01, 0xff02, 0xff03], 0x109: [0, 2, 3], 0x12d: [0, 1, 4], 0x14b: [0x0300, 0x0f00], 0x155: [0], 0x15b: [0x0101, 0x0102, 0x0103, 0x0134, 0x019f],
    0x164: [0x2800, 0x3200, 0x3205, 0x6404, 0x6405, 0x7800, 0x7804, 0x8c05, 0x9605, 0xc200, 0xfa00, 0xfa01, 0xff00, 0xff01, 0xff05], 0x16f: [1, 5, 0x14, 0x28, 0x32, 0x3c, 0x46, 0x8c, 0x8014, 0x8023, 0x8028], 0x1ba: [0], 0x1e6: [0, 1, 0xff00, 0xff01], 0x1e9: [0, 0x0101], 0x204: [0],
  },
};

function foreignEnemyRuntimeForms(game: Game, actorId: number): readonly number[] {
  return FORMS[game][actorId] ?? [];
}

export function foreignEnemyRuntimeReady(game: Game, actorId: number, params: number): boolean {
  return foreignEnemyRuntimeForms(game, actorId).includes(params);
}

const targetParams: Record<Game, Readonly<Record<number, readonly number[]>>> = {
  oot: {
    0x002: [0x0003],
    0x00e: [0x0000],
    0x011: [0x0000],
    0x012: [0x0000],
    0x013: [0x0000, 0x0002, 0x0004],
    0x01b: [0x0000, 0xfffe],
    0x01d: [0x0000, 0xffff],
    0x025: [0xff80, 0xfffe],
    0x02b: [0x0006, 0x4006],
    0x02d: [0x0000],
    0x02f: [0x0000],
    0x034: [0x0000],
    0x035: [0xffff],
    0x037: [0x0000],
    0x038: [0x0000],
    0x03a: [0x000a],
    0x04b: [0x0002, 0xffff],
    0x054: [0xffff],
    0x055: [0x0000, 0x0001],
    0x060: [0x0000],
    0x063: [0x0000],
    0x06b: [0xffff],
    0x069: [0xffff],
    0x08a: [0x0500, 0x0501],
    0x08e: [0x0000],
    0x090: [0x7f02, 0x7ffe],
    0x095: [0x0000],
    0x099: [0x0000],
    0x0c5: [0x0000],
    0x0dd: [0x0000],
    0x0ec: [0x0000],
    0x0f6: [0x0003],
    0x113: [0xff82, 0xff83],
    0x115: [0xffff],
    0x121: [0x0000],
    0x18c: [0x0000],
    0x1af: [0xff00, 0xff01],
    0x1c0: [0x0000],
  },
  mm: {
    0x008: [0xff00],
    0x00a: [0x0001],
    0x00b: [0x0000, 0x0001],
    0x00c: [0x0000, 0x0004, 0x8003],
    0x012: [0xfffd, 0xfffe, 0xffff],
    0x014: [0x0000],
    0x019: [0x0000],
    0x01d: [0x0000],
    0x024: [0x003f, 0x007f],
    0x032: [0xffff],
    0x033: [0x0000],
    0x03b: [0xff00, 0xff02],
    0x03c: [0x0000],
    0x03e: [0xffff],
    0x047: [0x0300, 0x0500, 0x0600, 0x0800, 0xff00],
    0x04a: [0x0000],
    0x04c: [0x7f05, 0x7f06, 0x7f07],
    0x064: [0x0000],
    0x066: [0x0001, 0x0002],
    0x084: [0xff01, 0xff02, 0xff03],
    0x08f: [0x0002, 0x100f, 0x2001, 0x300f],
    0x0ec: [0xff00, 0xff01, 0xff81],
    0x0ed: [0x0022, 0x0032],
    0x0f1: [0x0000],
    0x109: [0x0000, 0x0002, 0x0003],
    0x13a: [0x0fff],
    0x12d: [0x0000, 0x0001, 0x0002, 0x0004],
    0x14a: [0x0202, 0x0801, 0x0a01, 0x0c01, 0x1402, 0xff00, 0xff01, 0xff02, 0xff03],
    0x14b: [0x0300, 0x0f00],
    0x155: [0x0000],
    0x15b: [0x0101, 0x0102, 0x0103, 0x0134, 0x019f],
    0x164: [0x2800, 0x3200, 0x3205, 0x6404, 0x6405, 0x7800, 0x7804, 0x8c05, 0x9605, 0xc200, 0xfa00, 0xfa01, 0xff00, 0xff01, 0xff05],
    0x16f: [0x0001, 0x0005, 0x0014, 0x0028, 0x0032, 0x003c, 0x0046, 0x008c, 0x8014, 0x8023, 0x8028],
    0x1ba: [0x0000],
    0x1e6: [0x0000, 0x0001, 0xff00, 0xff01],
    0x1e9: [0x0000, 0x0101],
    0x1f3: [0x00ff],
    0x204: [0x0000],
  },
};

function standaloneEnemyParams(game: Game, actorId: number): readonly number[] {
  return targetParams[game][actorId] ?? [];
}

export function nativeEnemyVariantTarget(game: Game, actorId: number, params: number): boolean {
  return !enemyNeverShuffled(game, actorId) &&
    (game === 'oot' ? OOT_NATIVE_EXTRA_VARIANTS : MM_NATIVE_EXTRA_VARIANTS)
      .some(enemy => enemy.actorId === actorId && enemy.spawnParams === params && enemy.targetAllowed !== false);
}

export function standaloneEnemyTarget(game: Game, actorId: number, params?: number): boolean {
  const forms = standaloneEnemyParams(game, actorId);
  return params === undefined ? forms.length > 0 : forms.includes(params);
}

function standaloneEnemySource(game: Game, actorId: number): boolean {
  return standaloneEnemyTarget(game, actorId) || enemySpawnMacroMatches('standalone-source-extra', { target: { game, actorId } });
}

export function standaloneEnemyDonorParams(game: Game, actorId: number, params: number): number {
  return game === 'oot' && actorId === 0x115 && params === 3 ? 0xffff : params;
}

export function standaloneEnemyTransfer(source: {
  game: Game;
  actorId: number;
}, target: {
  game: Game;
  actorId: number;
  spawnParams: number;
}) {
  const donorParams = source.game !== target.game
    ? standaloneEnemyDonorParams(target.game, target.actorId, target.spawnParams) : target.spawnParams;
  return standaloneEnemySource(source.game, source.actorId) && (standaloneEnemyTarget(target.game, target.actorId, donorParams) || source.game !== target.game && additionalForeignEnemyTarget(target.game, target.actorId, donorParams));
}

const ARENAS = {
  'oot/En_Torch2': 'oot-dark-link',
  'oot/En_Dh': 'oot-dead-hand',
  'oot/En_Bigokuta': 'oot-big-octo',
  'oot/En_GeldB': 'oot-gerudo-fighter',
  'mm/En_Pametfrog': 'mm-snapper-gekko',
  'mm/En_Bigslime': 'mm-big-slime',
  'mm/En_Death': 'mm-gomess',
  'mm/En_Knight': 'mm-ikana-throne',
  'mm/Boss_04': 'mm-wart',
  'mm/En_Wiz': 'mm-wizrobe',
  'mm/En_Jso2': 'mm-garo-master',
  'mm/En_Egol': 'mm-eyegore',
  'mm/En_Bsb': 'mm-captain-keeta',
  'mm/En_Bigpo': 'mm-big-poe',
  'mm/En_Kaizoku': 'mm-pirate-fighter',
  'mm/En_Dragon': 'mm-deep-python',
} as const;

export type EnemyArena = typeof ARENAS[keyof typeof ARENAS];

const REQUIREMENTS = new Map<string, EnemyArena>(ENEMY_ACTOR_CATALOG.flatMap(actor => {
  const key = `${actor.game}/${actor.overlay}` as keyof typeof ARENAS;
  return Object.hasOwn(ARENAS, key) ? [[`${actor.game}/${actor.actorId}`, ARENAS[key]] as const] : [];
}));

function enemyArenaRequirement(game: Game, actorId: number): EnemyArena | undefined {
  return REQUIREMENTS.get(`${game}/${actorId}`);
}

export function enemyEncounterCompatible(sourceGame: Game, sourceActorId: number, targetGame: Game, targetActorId: number, arenaCapabilities: ReadonlySet<EnemyArena> = new Set(), preservesSourceCompletion = false): boolean {
  const original = enemyArenaRequirement(sourceGame, sourceActorId);
  const required = enemyArenaRequirement(targetGame, targetActorId);
  if (original && !required && !preservesSourceCompletion)
    return false;
  return !required || required === original || arenaCapabilities.has(required);
}

export function nativeResourceTarget(game: Game, actorId: number, params: number): boolean {
  return !enemyNeverShuffled(game, actorId) && !enemyCanRespawn(game, actorId, params) &&
    !enemyIsPerched(game, actorId, params) && !!nativeEnemyOnDemandObjects(game, actorId, params) &&
    [...enemyExclusionDefinitions.values()].some(enemy => enemy.game === game && enemy.actorId === actorId && enemy.spawnParams === params &&
      !enemy.foreignOnly && !enemy.arenaOnly && enemy.targetAllowed !== false);
}

export const nativeGoldTarget = nativeResourceTarget;

export function nativeResourceSource(game: Game, actorId: number, params: number): boolean {
  return enemySpawnMacroMatches('native-resource-source', { target: { game, actorId, params } });
}

class OotEnemyResourceLimit extends EnemyMemoryAllocationError {
}

const OOT_SELECTED_ENEMY_RESOURCE_ENVELOPE = resourceFacts.selectedEnemyEnvelopeBytes;

if (OOT_SELECTED_ENEMY_RESOURCE_ENVELOPE !== OOT_GAMEPLAY_POLICY.selectedEnemyBytes ||
  OOT_SELECTED_ENEMY_RESOURCE_ENVELOPE + OOT_GAMEPLAY_POLICY.nativeReserve > OOT_GAMEPLAY_POLICY.arenaBytes - 96)
  throw Error('OoT enemy budget overlaps the gameplay reserve');

const ENEMY_ROOM_ACTOR_TARGET = 32;

function foreignEnemyExtraActors(game: Game, actorId: number, params: number): number {
  if (game === 'mm' && actorId === 0x23f && params === 0)
    return 10;
  if ((game === 'oot' && actorId === 0x91 || game === 'mm' && actorId === 0x1e8) && ((params >>> 8) & 3) === 0)
    return 3;
  if (game === 'oot' && (actorId === 0x35 || actorId === 0xde))
    return 7;
  if (game === 'mm' && actorId === 0x180)
    return 5;
  if (game === 'mm' && actorId === 0x115 && (params === 0x807f || params === 0x907f))
    return params === 0x907f ? 3 : 2;
  if (game === 'oot' && actorId === 0x63)
    return 3;
  if (game === 'mm' && actorId === 0x14b)
    return Math.min(20, (params >>> 8) & 0xff);
  if (game === 'mm' && actorId === 0x15b)
    return params & 0x80 ? 0 : Math.max(0, ((params & 31) === 31 ? 0 : params & 31) - 1);
  if (game === 'mm' && actorId === 0x1e6 && (params & 0xff) === 1)
    return 3;
  return 0;
}

function foreignEnemyExtraBudget(authoredActors: number): number {
  return Math.max(0, ENEMY_ROOM_ACTOR_TARGET - authoredActors);
}

function enemyPlacementDensityCost(game: Game, actorId: number, params: number, authoredActors: number, native: boolean): number {
  if (actorId === ARWING_ENEMIES[game].actorId)
    return 2;
  if (game === 'oot' && (actorId === 0xa4 || actorId === 0x33))
    return Math.max(1, foreignEnemyExtraBudget(authoredActors));
  if (game === 'mm' && [0x43, 0x15d, 0x182, 0x12c, 0x7, 0x65, 0x28f].includes(actorId))
    return Math.max(1, foreignEnemyExtraBudget(authoredActors));
  if (game === 'mm' && actorId === 0x115)
    return Math.max(2, foreignEnemyExtraBudget(authoredActors));
  if (game === 'mm' && actorId === 0x23d)
    return Math.max(1, foreignEnemyExtraBudget(authoredActors));
  if (game === 'mm' && actorId === 0x184)
    return Math.max(12, foreignEnemyExtraBudget(authoredActors));
  if (native && !(game === 'oot' ? [0x91, 0xde] : [0x1e8, 0x180]).includes(actorId))
    return 0;
  return foreignEnemyExtraActors(game, actorId, params);
}

const CONTRACTS = [
  { game: 'oot', overlay: 'En_Torch2', key: 'oot-dark-link', killMacro: 'can_kill_dark_link', completion: 'room-clear', blockers: ['player-combat state', 'Water Temple arena and completion'] },
  { game: 'oot', overlay: 'En_Dh', key: 'oot-dead-hand', killMacro: 'can_kill_dead_hand', completion: 'room-clear', blockers: ['authored Dead Hand arms', 'burrowing floor and body activation'] },
  { game: 'oot', overlay: 'En_Bigokuta', key: 'oot-big-octo', killMacro: 'can_kill_big_octo', completion: 'room-clear', blockers: ['rotating Jabu platform controller', 'circular chase arena'] },
  { game: 'oot', overlay: 'En_GeldB', key: 'oot-gerudo-fighter', killMacro: 'can_kill_gerudo_fighter', completion: 'collectible', blockers: ['authored key collectible flag', 'capture and escape state'] },
  { game: 'mm', overlay: 'En_Pametfrog', key: 'mm-snapper-gekko', killMacro: 'can_kill_gekko_wft', completion: 'scripted', blockers: ['Snapper mount and wall-climbing arena', 'cutscene and frog reward'] },
  { game: 'mm', overlay: 'En_Bigslime', key: 'mm-big-slime', killMacro: 'can_kill_gekko_gbt', completion: 'scripted', blockers: ['Minislime family and ceiling arena', 'cutscene and frog reward'] },
  { game: 'mm', overlay: 'En_Knight', key: 'mm-ikana-throne', killMacro: 'can_kill_igos', completion: 'scripted', blockers: ['three-knight encounter ownership', 'curtains and reflected sunlight', 'song reward cutscene'] },
  { game: 'mm', overlay: 'Boss_04', key: 'mm-wart', killMacro: 'can_kill_warts', completion: 'scripted', blockers: ['Tanron bubble family', 'arena phase and reward completion'] },
  { game: 'mm', overlay: 'En_Wiz', key: 'mm-wizrobe', killMacro: 'can_kill_wizzrobe_close', completion: 'switch', blockers: ['Wizrobe platform props', 'intro and second-phase cutscene chain'] },
  { game: 'mm', overlay: 'En_Jso2', key: 'mm-garo-master', killMacro: 'can_kill_garo_master', completion: 'scripted', blockers: ['arena collision actor', 'intro, death and reward cutscenes'] },
  { game: 'mm', overlay: 'En_Egol', key: 'mm-eyegore', killMacro: 'can_kill_eyegore', completion: 'switch', blockers: ['encoded path index', 'death switch and rock effects'] },
  { game: 'mm', overlay: 'En_Bigpo', key: 'mm-big-poe', killMacro: 'can_kill_big_poe', completion: 'scripted', blockers: ['regular versus Dampe-summoned forms', 'bottle soul and reward ownership'] },
  { game: 'mm', overlay: 'En_Kaizoku', key: 'mm-pirate-fighter', killMacro: 'can_fight_pirates', completion: 'scripted', blockers: ['capture and scene escape state', 'authored fight completion'] },
  { game: 'mm', overlay: 'En_Death', key: 'mm-gomess', killMacro: 'can_kill_gomess', completion: 'scripted', blockers: ['Minideath bat family', 'intro and reward completion'] },
  { game: 'mm', overlay: 'En_Bsb', key: 'mm-captain-keeta', killMacro: 'can_kill_keeta', completion: 'scripted', blockers: ['race path and child actors', 'graveyard reward and week-event state'] },
] as const;

const ENEMY_ENCOUNTER_CONTRACTS = CONTRACTS.map(contract => {
  const actor = ENEMY_ACTOR_CATALOG.find(actor => actor.game === contract.game && actor.overlay === contract.overlay);
  if (!actor || (actor.role !== 'enemy' && actor.role !== 'miniboss'))
    throw new Error('Missing native encounter contract actor: ' + contract.key);
  const host: Game = contract.game === 'oot' ? 'mm' : 'oot';
  return {
    ...contract, actorId: actor.actorId, objectId: actor.objectId,
    nativeVariantShuffle: contract.key === 'mm-wizrobe',
    foreignStatus: nativeEnemyCounterparts(contract.game, actor.actorId, host).length ? 'native-counterpart' as const : 'missing-runtime-and-arena-adapter' as const
  };
});

const NATIVE_MINIBOSS_VARIANTS = [
  { key: 'mm-wizzrobe-fire', game: 'mm', contract: 'mm-wizrobe', actorId: 0x15d, objectId: 0x178, type: 0, killMacro: 'can_kill_wizzrobe_close' },
  { key: 'mm-wizzrobe-ice', game: 'mm', contract: 'mm-wizrobe', actorId: 0x15d, objectId: 0x178, type: 1, killMacro: 'can_kill_wizzrobe_close' },
] as const;

export type NativeMinibossVariantKey = typeof NATIVE_MINIBOSS_VARIANTS[number]['key'];

export function nativeMinibossVariant(key: string) {
  return NATIVE_MINIBOSS_VARIANTS.find(variant => variant.key === key);
}

export const NATIVE_ARENA_COMBAT_TARGETS = [
  { key: 'oot-arena-torch-slug', game: 'oot', actorId: 0x38, objectId: 0x25, spawnParams: 0, killMacro: 'can_kill_torch_slugs' },
  { key: 'oot-arena-tektite', game: 'oot', actorId: 0x1b, objectId: 0x16, spawnParams: 0xffff, killMacro: 'can_kill_tektites' },
  { key: 'oot-arena-stalfos', game: 'oot', actorId: 2, objectId: 0x32, spawnParams: 3, killMacro: 'can_kill_stalfos' },
  { key: 'oot-arena-iron-knuckle-black', game: 'oot', actorId: 0x113, objectId: 0x106, spawnParams: 0xff82, killMacro: 'can_kill_iron_knuckles' },
  { key: 'oot-arena-iron-knuckle-white', game: 'oot', actorId: 0x113, objectId: 0x106, spawnParams: 0xff83, killMacro: 'can_kill_iron_knuckles' },
] as const;

export const BIG_OCTO_ARENA_COMBAT_TARGETS = [
  { key: 'oot-big-octo-arena-biri', game: 'oot', actorId: 0x34, objectId: 0x21, spawnParams: 0, killMacro: 'can_kill_biri' },
] as const;

export type BigOctoArenaCombatTargetKey = typeof BIG_OCTO_ARENA_COMBAT_TARGETS[number]['key'];

export type NativeArenaCombatTargetKey = typeof NATIVE_ARENA_COMBAT_TARGETS[number]['key'];

export function nativeArenaCombatTarget(key: string) {
  return NATIVE_ARENA_COMBAT_TARGETS.find(target => target.key === key);
}

const NATIVE_ARENA_ENEMIES: readonly EnemyDefinition[] = [
  ...(['oot-dead-hand', 'oot-dark-link', 'oot-big-octo'] as const).map(key => {
    const contract = ENEMY_ENCOUNTER_CONTRACTS.find(contract => contract.key === key)!;
    return {
      key: key + '-encounter', name: key, game: 'oot' as const, actorId: contract.actorId,
      objectId: contract.objectId, habitat: 'ground' as const, params: [], spawnParams: 0,
      targetAllowed: false, nativeOnly: true, arenaOnly: true,
      killMacro: contract.killMacro, logicMacros: [contract.killMacro],
      soulMacro: key === 'oot-dead-hand' ? 'soul_dead_hand' : key === 'oot-dark-link' ? 'soul_dark_link' : 'soul_octorok',
      soulItem: key === 'oot-dead-hand' ? 'SOUL_ENEMY_DEAD_HAND' : key === 'oot-dark-link' ? 'SOUL_ENEMY_DARK_LINK' : 'SOUL_ENEMY_OCTOROK'
    };
  }),
  ...[...NATIVE_ARENA_COMBAT_TARGETS, ...BIG_OCTO_ARENA_COMBAT_TARGETS].map(target => ({
    ...target, name: target.key,
    habitat: 'ground' as const, params: [], nativeOnly: true, arenaOnly: true,
    logicMacros: []
  })),
  {
    key: 'mm-wizzrobe-encounter', name: 'Wizzrobe arena', game: 'mm', actorId: 0x15d, objectId: 0x178,
    habitat: 'ground', params: [], spawnParams: 0, targetAllowed: false,
    nativeOnly: true, arenaOnly: true, killMacro: 'can_kill_wizzrobe_close', logicMacros: [],
    soulMacro: 'can_kill_wizzrobe_close'
  },
  ...NATIVE_MINIBOSS_VARIANTS.map(variant => ({
    key: variant.key, name: variant.key, game: variant.game,
    actorId: variant.actorId, objectId: variant.objectId, habitat: 'ground' as const, params: [],
    spawnParams: variant.type << 8, nativeOnly: true, arenaOnly: true,
    killMacro: variant.killMacro, logicMacros: []
  })),
];

const NATIVE_FIGHT_RESTORATION_SOURCES = [
  {
    key: 'oot-gerudo-fighter-restored', game: 'oot', actorId: 0x197, objectId: 0x169,
    params: [0x0a00, 0x0c00, 0x0e00, 0x0f00], killMacro: 'can_kill_gerudo_fighter', preserves: 'jail-key-collectible'
  },
  {
    key: 'mm-regular-big-poe-restored', game: 'mm', actorId: 0x208, objectId: 0x1f1,
    params: [0xff00], killMacro: 'can_kill_big_poe', soulMacro: 'soul_poe', preserves: 'bottle-soul-and-native-cutscene'
  },
  {
    key: 'mm-pirate-fighter-restored', game: 'mm', actorId: 0x21d, objectId: 0x204,
    params: [0x024b, 0x02cb, 0x020b], killMacro: 'can_fight_pirates', preserves: 'escape-switch-and-native-cutscene'
  },
] as const;

export function nativeFightRestorationSource(key: string) {
  return NATIVE_FIGHT_RESTORATION_SOURCES.find(source => source.key === key);
}

export function nativeFightRestorationRoomAllowed(sourceKey: string, room: number,
  settings: { gerudoFortress?: Settings['gerudoFortress'] }): boolean {
  return enemySpawnMacroMatches('native-fight-room-allowed', { source: { key: sourceKey }, location: { room }, settings });
}

export function nativeFightRestorationCombat(sourceKey: string, target: {
  game: Game;
  killMacro: string;
}): string | undefined {
  const source = nativeFightRestorationSource(sourceKey);
  if (!source || !/^[a-z_][a-z_0-9]*$/.test(target.killMacro))
    return;
  if (source.game === 'oot' && source.actorId === 0x197) return `(${target.killMacro})`;
  const soul = 'soulMacro' in source ? ` && ${source.soulMacro}` : '';
  return `(${target.killMacro}) && (${source.killMacro}${soul})`;
}

const NATIVE_RESTORED_FIGHT_ENEMIES: readonly EnemyDefinition[] = NATIVE_FIGHT_RESTORATION_SOURCES.map(source => ({
  key: source.key, name: source.key, game: source.game, actorId: source.actorId, objectId: source.objectId,
  habitat: source.actorId === 0x208 ? 'air' as const : 'ground' as const,
  params: [...source.params], spawnParams: source.params[0], targetAllowed: false,
  nativeOnly: true, arenaOnly: true, killMacro: source.killMacro, logicMacros: [source.killMacro],
  soulMacro: source.actorId === 0x208 ? 'soul_poe' : 'soul_thief_fighter',
}));

const MINIBOSS_ACTORS = new Set(ENEMY_ACTOR_CATALOG.filter(actor => actor.role === 'miniboss')
  .map(actor => `${actor.game}/${actor.actorId}`));

export function isMinibossEnemy(game: Game, actorId: number, params?: number): boolean {
  return enemySpawnMacroMatches('miniboss', { target: { game, actorId, params },
    facts: { catalogMiniboss: MINIBOSS_ACTORS.has(`${game}/${actorId}`) } });
}

function minibossOnly(settings: Partial<Settings>, game: Game): boolean {
  return !!(game === 'oot' ? settings.enemyShuffleOotMinibossOnly : settings.enemyShuffleMmMinibossOnly);
}

export function enemyKeepsNativeEncounter(game: Game, actorId: number, params: number | undefined,
  target: Pick<EnemyDefinition, 'game' | 'actorId' | 'spawnParams'>): boolean {
  return target.game === game && target.actorId === actorId &&
    (game === 'mm' && actorId === 0x184 || isMinibossEnemy(game, actorId, params) &&
      isMinibossEnemy(target.game, target.actorId, target.spawnParams));
}

export function requiresMinibossReplacement(settings: Partial<Settings>, game: Game, _actorId: number,
  sourceIsMiniboss: boolean): boolean {
  return (minibossOnly(settings, game) || !!settings.enemyShuffleMinibosses && !!settings.enemyShuffleMinibossType &&
    settings.enemyShuffleMinibossType !== 'anywhere') && sourceIsMiniboss;
}

function minibossPlacementAllowed(settings: Settings, host: Game, sourceActorId: number, target: Pick<EnemyDefinition, 'game' | 'actorId'> & {
  spawnParams?: number;
  variables?: readonly number[];
}, sourceIsMiniboss: boolean): boolean {
  const params = 'spawnParams' in target ? target.spawnParams : target.variables?.[0];
  if (target.game === host && target.actorId === (host === 'oot' ? 0x91 : 0x1e8) && (params === 0 || params === 0x4000) &&
    (params === 0x4000) !== sourceIsMiniboss)
    return false;
  if (target.game === 'mm' && target.actorId === 0x115 && (params === 0x807f || params === 0x907f) &&
    (params === 0x907f) !== sourceIsMiniboss)
    return false;
  if (requiresMinibossReplacement(settings, host, sourceActorId, sourceIsMiniboss))
    return isMinibossEnemy(target.game, target.actorId, 'spawnParams' in target ? target.spawnParams : target.variables?.[0]);
  return settings.enemyShuffleMinibossType !== 'rooms' || !isMinibossEnemy(target.game, target.actorId, 'spawnParams' in target ? target.spawnParams : target.variables?.[0]);
}

const SCRIPTED_MM_FAMILIES = [
  'mm-snapper-gekko', 'mm-big-slime', 'mm-ikana-throne', 'mm-wart',
  'mm-garo-master', 'mm-eyegore', 'mm-gomess', 'mm-captain-keeta', 'mm-wizrobe',
] as const;

const SCRIPTED_MM_ENEMIES: readonly EnemyDefinition[] = SCRIPTED_MM_FAMILIES.map(key => {
  const contract = ENEMY_ENCOUNTER_CONTRACTS.find(row => row.key === key)!;
  return {
    key: key + '-scripted', name: key, game: 'mm', actorId: contract.actorId, objectId: contract.objectId,
    habitat: 'ground', params: [], spawnParams: 0, nativeOnly: true, arenaOnly: true,
    targetAllowed: false, killMacro: contract.killMacro,
    logicMacros: key === 'mm-eyegore' ? [contract.killMacro, 'can_kill_eyegore_distance', 'can_kill_eyegore_world'] :
      key === 'mm-garo-master' ? [contract.killMacro, 'can_kill_garo_master_world', 'can_kill_garo_master_shrine'] :
        key === 'mm-wart' ? [contract.killMacro, 'can_kill_warts_shrine'] : [contract.killMacro],
    soulMacro: contract.killMacro
  };
});

export function scriptedMmTargetAllowed(target: EnemyDefinition, crossGame: boolean, source?: EnemyDefinition): boolean {
  if (target.arenaOnly || target.targetAllowed === false || enemyNeverShuffled(target.game, target.actorId) ||
    enemyCanRespawn(target.game, target.actorId, target.spawnParams) || enemyIsPerched(target.game, target.actorId, target.spawnParams))
    return false;
  if (target.game === 'mm' && target.actorId === 0x16f && target.spawnParams !== 0x14)
    return false;
  if (target.game === 'mm')
    return !target.foreignOnly && !!nativeEnemyOnDemandObjects('mm', target.actorId, target.spawnParams);
  return crossGame && !target.nativeOnly && foreignEnemyRuntimeReady('oot', target.actorId, target.spawnParams) &&
    !nativeEnemyCounterparts('oot', target.actorId, 'mm', target.spawnParams).length;
}

function dynamicEnemyPool(settings: Settings, host: Game, sourceId: number, scene: number | readonly number[] | undefined, sourceIsMiniboss: boolean): EnemyDefinition[] {
  const scenes = scene === undefined ? [] : typeof scene === 'number' ? [scene] : scene;
  const keys = new Set(enemyShuffleRoster(host, settings.crossGameEnemies).flatMap(row => row.legacyKeys));
  const forms = new Map<string, EnemyDefinition>();
  for (const key of keys) {
    const enemy = enemyDefinition(key);
    if (enemy.arenaOnly || enemy.targetAllowed === false || enemyNeverShuffled(enemy.game, enemy.actorId) ||
      enemyCanRespawn(enemy.game, enemy.actorId, enemy.spawnParams) || enemyIsPerched(enemy.game, enemy.actorId, enemy.spawnParams) ||
      enemyShuffleExcluded(settings, enemy, host))
      continue;
    if (enemy.game === host) {
      if (enemy.foreignOnly || !nativeEnemyOnDemandObjects(host, enemy.actorId, enemy.spawnParams)) continue;
      if (host === 'oot' && !OOT_ENEMIZER.targets.some(row => row.actorId === enemy.actorId && row.params === enemy.spawnParams) &&
        !OOT_NATIVE_EXTRA_VARIANTS.includes(enemy) && !NATIVE_EXTENSIONS.some(row => row.key === enemy.key) &&
        enemy.key !== ARWING_ENEMIES.oot.key) continue;
      if (host === 'mm' && !MM_NATIVE_ROSTER.some(row => row.actorId === enemy.actorId && row.variables.includes(enemy.spawnParams)) &&
        !MM_NATIVE_EXTRA_VARIANTS.includes(enemy) && !mmNativeCounterpartRoster(ENEMIES).some(row =>
          row.actorId === enemy.actorId && row.variables.includes(enemy.spawnParams)) && enemy.key !== ARWING_ENEMIES.mm.key) continue;
      if (!scenes.every(scene => enemySpawnMacroMatches('native-scene-keep', { target: enemy,
        facts: { sceneKeep: (nativeSceneKeeps.scenes[host] as Record<number, number>)[scene] } }))) continue;
    } else if (!settings.crossGameEnemies || enemy.nativeOnly || !foreignEnemyRuntimeReady(enemy.game, enemy.actorId, enemy.spawnParams) ||
      nativeEnemyCounterparts(enemy.game, enemy.actorId, host, enemy.spawnParams).length) continue;
    /* Supplemental encounters (including Gold Skulltula rewards) place their
     * replacements on the floor. They do not retain an authored wall transform. */
    if (!enemySpawnMacroMatches('dynamic-habitat-allowed', { target: { ...enemy, params: enemy.spawnParams }, settings })) continue;
    if (isMinibossEnemy(enemy.game, enemy.actorId, enemy.spawnParams) && !settings.enemyShuffleMinibosses && !minibossOnly(settings, host) ||
      !minibossPlacementAllowed(settings, host, sourceId, enemy, sourceIsMiniboss)) continue;
    const identity = `${enemy.game}/${enemy.actorId}/${enemy.spawnParams}`;
    if (!forms.has(identity))
      forms.set(identity, enemy);
  }
  return [...forms.values()];
}

export function scriptedMmSource(key: string) {
  return SCRIPTED_MM_ENEMIES.find(row => row.key === key);
}

const ootSpawnFacts = applyEnemyOotSpawnPolicies(ootEnemizerFacts, ENEMY_SOURCE_ADDITIONS.locations, ENEMY_SPAWN_OOT_LOCATIONS, ENEMY_SPAWN_OOT_TARGETS);

export const OOT_ENEMIZER = {
  targets: ootSpawnFacts.targets,
  aliases: [...ootEnemizerFacts.aliases, ...ENEMY_SOURCE_ADDITIONS.aliases] as readonly (readonly [readonly number[], readonly (readonly number[])[]])[],
  locations: ootSpawnFacts.locations.map(location => {
    const row = ootActorCorrections.find(row => row.mq === location.mq && row.scene === location.scene && row.room === location.room &&
      row.setup === location.setup && row.slot === location.slot);
    if (!row)
      return location;
    if (location.actorId !== row.upstreamActorId)
      throw new Error('Stale OoT source actor correction');
    return { ...location, actorId: row.actorId };
  }),
};

type OotEnemizerTarget = typeof ootSpawnFacts.targets[number];

type OotEnemizerLocation = typeof ootSpawnFacts.locations[number] & { fieldSlot?: number };

export type OotEnemizerChoice = {
  location: OotEnemizerLocation;
  target: OotEnemizerTarget;
  targetIndex: number;
  ignoreRestrictions?: boolean;
  foreign?: boolean;
  sourceParams?: number;
  targetParams: number;
  killLogic: string;
  dropLogic: string | null;
  aliases: readonly (readonly number[])[];
};

type OotEnemySlot = Pick<OotEnemizerLocation, 'scene' | 'room' | 'setup' | 'slot'>;

export function ootEnemizerLocationKey(location: OotEnemySlot): string {
  return [location.scene, location.room, location.setup, location.slot].join('/');
}

/* NO_BROKEN_ACTORS patches 31 OoT actor-list entries by ROM address.
 * These 18 entries are the ones also eligible for the ordinary OoT Enemizer
 * source roster. Other removed entries are not shuffle candidates.
 * Keep this table in sync with src/oot/patches/broken_actors.S. The generator
 * independently checks the patched ROM entries and errors on a missed source.
 * Forest Temple room 0: 0x25ce078, 0x25ce088
 * Graveyard room 1 setups 2/3: 0x202f3dc..43c, 0x202f7cc..82c
 * Lost Woods shortcut: 0x2168090, 0x21681c8
 */
const OOT_BROKEN_ENEMIZER_SLOTS = new Set<string>([
  '5/0/0/2', '5/0/0/3',
  ...[2, 3].flatMap(setup => [2, 3, 4, 5, 6, 7, 8].map(slot => `83/1/${setup}/${slot}`)),
  '91/3/0/2', '91/3/2/2',
]);

function ootEnemizerBrokenSlot(location: OotEnemizerLocation): boolean {
  return !location.mq && OOT_BROKEN_ENEMIZER_SLOTS.has(ootEnemizerLocationKey(location));
}

function ootEnemizerLocations(mq: ReadonlySet<string>, settings: Settings): readonly OotEnemizerLocation[] {
  return OOT_ENEMIZER.locations.filter(row =>
    (row.dungeon === null || row.mq === mq.has(row.dungeon)) &&
    (settings.restoreBrokenActors || !ootEnemizerBrokenSlot(row)));
}

function ootSpawnLocations(location: OotEnemizerLocation): EnemySpawnLocation[] {
  return [{ ...location, game: 'oot' }, ...(ootPlacementAliases.get(ootEnemizerLocationKey(location)) ?? [])
    .map(([scene, room, setup, slot]) => ({ game: 'oot' as const, scene, room, setup, slot, mq: false }))];
}

function namedSpawnId(game: Game, key: string): string {
  return `${game}/named/${key}`;
}

function spawnPointId(location: EnemySpawnLocation, grotto?: number): string {
  const layout = location.game === 'oot' ? (location.mq ? 'mq' : 'vanilla') : (grotto === undefined ? 'normal' : `grotto-${grotto}`);
  return `${location.game}/${layout}/${location.scene ?? '-'}/${location.room ?? '-'}/${location.setup ?? '-'}/${location.slot ?? '-'}`;
}

function ootSpawnRegistryId(location: OotEnemizerLocation): string {
  return location.fieldSlot === undefined ? spawnPointId({ ...location, game: 'oot' }) :
    namedSpawnId('oot', OOT_DYNAMIC_PLANNER_SLOTS[location.fieldSlot].key);
}

function ootSpawnIsMinibossEncounter(location: OotEnemizerLocation): boolean {
  return spawnEncounter(ootSpawnRegistryId(location)).minibossEncounter;
}

function ootSpawnSource(defs: readonly EnemyDefinition[], location: OotEnemizerLocation) {
  const key = location.fieldSlot === undefined
    ? location.broken ? undefined : ootEnemizerSourceKey(defs, location)
    : OOT_DYNAMIC_PLANNER_SLOTS[location.fieldSlot].key;
  return { ...location, key, encounterId: ootSpawnRegistryId(location), habitat: key ? enemyDefinition(key).habitat : undefined };
}

const { sceneKeeps: nativeSceneKeeps, roomCounts } = placementData;

const { foreignActorIds, ootObjectIds, nativeCounterparts } = mmPlacementData;

const NATIVE_EXTENSIONS = [
  { key: 'oot-flying-floor-tile-source', actorId: 0x6b, params: 0xffff, name: 'Flying Floor Tile', killLogic: 'can_clear_flying_floor_tiles' },
  { key: 'oot-poe-native-target', actorId: 0x0d, params: 0, name: 'Poe', killLogic: 'can_kill_poes' },
  { key: 'oot-stalchild-native-target', actorId: 0x1b0, params: 0, name: 'Stalchild', killLogic: 'can_kill_stalchildren' },
  { key: 'oot-withered-baba-native-target', actorId: 0xc7, params: 1, name: 'Withered Deku Baba', killLogic: 'can_kill_baba_sticks' },
  { key: 'oot-skulltula-invisible-native-target', actorId: 0x37, params: 2, name: 'Invisible Skulltula', killLogic: 'can_kill_invisible_skulltulas' },
  { key: 'oot-red-bubble-native-target', actorId: 0x69, params: 0xfffe, name: 'Red Bubble', killLogic: 'can_kill_red_bubbles' },
  { key: 'oot-keese-invisible-native-target', actorId: 0x13, params: 0x8002, name: 'Invisible Keese', killLogic: 'can_kill_invisible_keese' },
  { key: 'oot-stalfos-invisible-native-target', actorId: 2, params: 0, name: 'Invisible Stalfos', killLogic: 'can_kill_invisible_stalfos' },
  { key: 'oot-business-scrub-combat', actorId: 0x195, params: 0x4000, name: 'Business Scrub', killLogic: 'can_kill_mad_scrubs' },
] as const;

function ootStandalonePortSlot(location: OotEnemizerLocation): boolean {
  return enemySpawnMacroMatches('oot-standalone-port-slot', { source: location });
}

function ootNativeExtensions(location: OotEnemizerLocation) {
  const allowedActors = location.choices.map(index => OOT_ENEMIZER.targets[index].actorId);
  return NATIVE_EXTENSIONS.filter(form => enemySpawnMacroMatches('oot-native-business-scrub', { target: form, facts: { allowedActors } }));
}

function ootNativeVariantFitsScene(def: EnemyDefinition, scene: number): boolean {
  return enemySpawnMacroMatches('native-scene-keep', { target: def, facts: { sceneKeep: (nativeSceneKeeps.scenes.oot as Record<number, number>)[scene] } });
}

function ootCombinedNativeTargets(defs: readonly EnemyDefinition[], location: OotEnemizerLocation, ignoreRestrictions = false): EnemyDefinition[] {
  if (!enemySpawnMacroMatches('oot-native-extension-source', { source: location, settings: { enemyShuffleIgnoreRestrictions: ignoreRestrictions } }))
    return [];
  const targets = ootNativeExtensions(location).map(form => {
    const rows = defs.filter(def => def.key === form.key && def.game === 'oot' &&
      def.actorId === form.actorId && def.spawnParams === form.params && def.nativeOnly && def.targetAllowed !== false);
    if (rows.length !== 1)
      throw new Error('Missing combined native counterpart: ' + form.key);
    return rows[0];
  });
  return targets.concat(OOT_NATIVE_EXTRA_VARIANTS.filter(def => ootNativeVariantFitsScene(def, location.scene)));
}

export function isOotEnemizerNativeExtensionChoice(choice: OotEnemizerChoice): boolean {
  if (choice.targetIndex !== -1 || choice.foreign !== false)
    return false;
  const locationKey = ootEnemizerLocationKey(choice.location);
  const location = OOT_ENEMIZER.locations.find(candidate => candidate.mq === choice.location.mq && ootEnemizerLocationKey(candidate) === locationKey);
  if (!location || location.broken)
    return false;
  const target = choice.target;
  if (!enemySpawnMacroMatches('oot-native-extension-source', { source: location, settings: { enemyShuffleIgnoreRestrictions: choice.ignoreRestrictions } }) &&
    !(target.actorId === ARWING_ENEMIES.oot.actorId && ootArwingSlot(location)))
    return false;
  const forms = [
    ...ootNativeExtensions(location),
    ...OOT_NATIVE_EXTRA_VARIANTS.filter(def => ootNativeVariantFitsScene(def, location.scene))
      .map(def => ({ actorId: def.actorId, params: def.spawnParams, name: def.name, killLogic: def.killMacro })),
  ];
  if (ootArwingSlot(location)) {
    const arwing = ARWING_ENEMIES.oot;
    forms.push({ actorId: arwing.actorId, params: arwing.spawnParams, name: arwing.name, killLogic: arwing.killMacro });
  }
  const form = forms.find(candidate => candidate.actorId === target.actorId && candidate.params === target.params);
  return !!form && choice.targetParams === form.params && target.name === form.name &&
    target.killLogic === form.killLogic && target.weight === 1 && choice.killLogic === form.killLogic &&
    choice.dropLogic === null && target.dropLogic === null && target.requires.length === 0 && target.restrictions.length === 0;
}

const DUNGEONS: Readonly<Record<string, string>> = { DT: 'Deku Tree', DC: 'Dodongos Cavern', JJ: 'Jabu Jabus Belly', Forest: 'Forest Temple', Fire: 'Fire Temple', Water: 'Water Temple', Spirit: 'Spirit Temple', Shadow: 'Shadow Temple', BotW: 'Bottom of the Well', IC: 'Ice Cavern', GTG: 'Gerudo Training Ground', Ganon: 'Ganons Castle' };

const OOT_NATIVE_ENEMY_KEY_PREFIX = 'ootr-';

export const ootNativeEnemyDefinitionKey = (id: number, params: number) => `${OOT_NATIVE_ENEMY_KEY_PREFIX}native-${id.toString(16)}-${params.toString(16)}`;

function baseDefinition(base: readonly EnemyDefinition[], id: number): EnemyDefinition {
  let best: EnemyDefinition | undefined;
  for (const row of base) {
    if (row.game === 'oot' && row.actorId === id && !row.foreignOnly &&
      (!best || row.logicMacros.length > best.logicMacros.length))
      best = row;
  }
  if (!best)
    throw new Error('Missing native OoT actor definition: ' + id.toString(16));
  return best;
}

function ootEnemizerDefinitions(base: readonly EnemyDefinition[]): EnemyDefinition[] {
  const forms = OOT_ENEMIZER.targets.map(target => ({ target, params: target.params }));
  for (const params of [0xff02, 0xff03]) {
    const target = OOT_ENEMIZER.targets.find(row => row.actorId === 0x113 && (row.params & 0x3f) === (params & 0x3f))!;
    forms.push({ target, params });
  }
  const originals = new Map<number, EnemyDefinition>();
  return forms.map(({ target, params }) => {
    let original = originals.get(target.actorId);
    if (!original) {
      original = baseDefinition(base, target.actorId);
      originals.set(target.actorId, original);
    }
    return {
      ...original, key: ootNativeEnemyDefinitionKey(target.actorId, params), name: target.name,
      soulMacro: original.soulMacro ?? 'soul_' + original.key.split('-')[1], params: [], spawnParams: params,
      nativeOnly: true, foreignOnly: false, targetAllowed: true,
      preserveNativeResource: false, arenaOnly: false, requiresNoCutscene: false, requiresNoScriptedPeers: false,
    };
  });
}

export type OotEnemizerReplacement = EnemyReplacement & {
  ootr: OotEnemizerChoice;
};

function crossRestrictions(target: EnemyDefinition): readonly string[] {
  return enemySpawnMacroGroup('oot-restriction', { target: { ...target, params: target.spawnParams } });
}

function resolvedMq(settings: Settings, provided?: ReadonlySet<string>): ReadonlySet<string> {
  const option = settings.mqDungeons;
  const keys = provided ?? new Set(option.type === 'all' ? Object.keys(DUNGEONS) : option.type === 'specific' ? option.values : []);
  return new Set([...keys].map(key => DUNGEONS[key] ?? key));
}

function ootArwingSlot(location: OotEnemizerLocation): boolean {
  return enemySpawnMacroMatches('oot-arwing-source', { source: location });
}

export function ootNativeChoiceAllows(location: OotEnemizerLocation, index: number, ignoreRestrictions = false): boolean {
  if (location.choices.includes(index))
    return true;
  const target = OOT_ENEMIZER.targets[index];
  return !!target && enemySpawnMacroMatches('oot-relaxed-native-source', {
    source: location, target, settings: { enemyShuffleIgnoreRestrictions: ignoreRestrictions },
  });
}

type OotEnemizerCandidate = {
  index: number;
  def: EnemyDefinition;
  weight: number;
};

function ootForeignTargets(defs: readonly EnemyDefinition[]) {
  return defs.filter(d => d.game === 'mm' && !d.nativeOnly && d.targetAllowed !== false && !d.arenaOnly &&
    (mmFacts.roster.some(row => row.actorId === d.actorId && row.variables.includes(d.spawnParams)) || enemySpawnMacroMatches('oot-foreign-roster-extras', { target: { ...d, params: d.spawnParams } }) || FOREIGN_EXTRA_NATIVE_MM_ENEMIES.some(row => row.key === d.key)) &&
    foreignEnemyRuntimeReady('mm', d.actorId, d.spawnParams) && !nativeEnemyCounterparts('mm', d.actorId, 'oot', d.spawnParams).length);
}

function ootEnemizerCandidatePools(defs: readonly EnemyDefinition[], settings: Settings, locations: readonly OotEnemizerLocation[]): Map<OotEnemizerLocation, OotEnemizerCandidate[]> {
  const forced = settings.plando.enemyTargets?.oot;
  const definitionsByKey = new Map<string, EnemyDefinition>();
  for (const def of defs) {
    if (!definitionsByKey.has(def.key))
      definitionsByKey.set(def.key, def);
  }
  const forcedOotForms = forced ? defs.filter(def => def.key === forced && def.game === 'oot') : [];
  const uniqueCross = new Map<string, EnemyDefinition>();
  if (settings.crossGameEnemies) {
    for (const target of ootForeignTargets(defs)) {
      const key = target.actorId + '/' + target.spawnParams;
      if (!uniqueCross.has(key) || target.key === forced)
        uniqueCross.set(key, target);
    }
  }
  const cross = [...uniqueCross.values()];
  const candidates = new Map<OotEnemizerLocation, OotEnemizerCandidate[]>();
  for (const location of locations) {
    if (location.broken || (!settings.restoreBrokenActors && ootEnemizerBrokenSlot(location)))
      continue;
    const source = ootSpawnSource(defs, location);
    const spawnLocations = ootSpawnLocations(location);
    const ignoreRestrictions = false;
    const nativeIndices = location.choices;
    const native = nativeIndices
      .filter(index => enemySpawnMacroMatches('oot-native-wall-source', { source: location, target: OOT_ENEMIZER.targets[index] }))
      .map(index => {
        const { actorId, params } = OOT_ENEMIZER.targets[index];
        const key = ootNativeEnemyDefinitionKey(actorId, params);
        const def = definitionsByKey.get(key);
        if (!def)
          throw new Error('Unregistered native OoT target form');
        return { index, def };
      });
    const foreign = cross.filter(target => {
      const context = { source: location, target: { ...target, params: target.spawnParams }, settings,
        facts: { capabilities: crossRestrictions(target), allWaterSurfaceSlots: !!ootWaterSurfaceSlot(location) &&
          (ootPlacementAliases.get(ootEnemizerLocationKey(location)) ?? []).every(([scene, room, setup, slot]) =>
            !!ootWaterSurfaceSlot({ mq: false, scene, room, setup, slot })) } };
      return enemySpawnMacroMatches('oot-foreign-standalone-allowed', context) &&
        enemySpawnMacroMatches('oot-foreign-water-surface', context) &&
        enemySpawnMacroMatches('oot-foreign-habitat', context);
    });
    const additions = [
      ...ootCombinedNativeTargets(defs, location, ignoreRestrictions),
      ...foreign,
    ].map(def => ({ index: -1, def }));
    if (ootArwingSlot(location))
      additions.push({ index: -1, def: ARWING_ENEMIES.oot });
    let pool = [...native, ...additions].filter(({ def }) => enemyTargetCompatible(settings, `enemy_oot_${location.scene}_${location.room}_${location.setup}_${location.slot}`, { ...def, spawnParams: (location.varOverrides as Readonly<Record<string, number>>)[def.name] ?? def.spawnParams }) && !enemyShuffleExcluded(settings, def, 'oot') &&
      spawnLocations.every(where => enemySpawnTargetAllowed(where, source,
        { ...def, spawnParams: (location.varOverrides as Readonly<Record<string, number>>)[def.name] ?? def.spawnParams }, settings)) &&
      !enemyCanRespawn(def.game, def.actorId, def.spawnParams) &&
      !enemyIsPerched(def.game, def.actorId, def.spawnParams) &&
      enemySpawnMacroMatches('oot-native-target-source', { location, source: location, target: def }));
    if (!settings.enemyShuffleMinibosses && !minibossOnly(settings, 'oot'))
      pool = pool.filter(({ def }) => !isMinibossEnemy(def.game, def.actorId, def.spawnParams));
    else
      pool = pool.filter(({ def }) => minibossPlacementAllowed(settings, 'oot', location.actorId, def,
        ootSpawnIsMinibossEncounter(location)));
    if (forced) {
      pool = pool.filter(row => row.def.key === forced || (row.index >= 0 &&
        forcedOotForms.some(def => def.actorId === row.def.actorId && def.spawnParams === row.def.spawnParams)));
    }
    if (!pool.length && enemyCompatibilityRule(settings, `enemy_oot_${location.scene}_${location.room}_${location.setup}_${location.slot}`))
      throw new Error(`No compatible weighted enemy for OoT scene ${location.scene}, room ${location.room}, slot ${location.slot}`);
    if (pool.length) {
      const weights = enemyFormWeights(pool, row => row.def, target => enemyWeight(settings, 'oot', target));
      candidates.set(location, pool.map((row, index) => ({ ...row, weight: weights[index] })));
    }
  }
  return candidates;
}

function ootEnemizerModelBank(def: EnemyDefinition): string {
  return `${def.game}/${def.game === 'oot' && def.actorId === 0xd ? 9 : def.objectId}`;
}

function ootGroupedTargetForm(def: EnemyDefinition, location: OotEnemizerLocation): string {
  const overrides = location.varOverrides as Readonly<Record<string, number>>;
  return `${def.game}/${def.actorId}/${overrides[def.name] ?? def.spawnParams}`;
}

function enemyGroupLimit(target: Pick<EnemyDefinition, 'game' | 'actorId'>): number {
  return enemySpawnLimit('group-size', { target });
}

function groupedEnemyPool<T>(pool: readonly T[], copies: number, identity: (value: T) => EnemyShuffleIdentity,
  weight: (target: EnemyShuffleIdentity) => number, next: (max: number) => number): { pool: T[]; miniboss?: T } {
  const miniboss = (value: T) => {
    const target = identity(value);
    return isMinibossEnemy(target.game, target.actorId, target.spawnParams);
  };
  const grouped = pool.filter(value => copies <= enemyGroupLimit(identity(value)));
  const ordinary = grouped.filter(value => !miniboss(value));
  if (copies <= 1 || !ordinary.length || !pool.some(miniboss)) return { pool: grouped };
  const eligible = pool.filter(value => miniboss(value) || copies <= enemyGroupLimit(identity(value)));
  const rolled = pickEnemyWeighted(eligible, enemyFormWeights(eligible, identity, weight), next);
  return { pool: ordinary, miniboss: miniboss(rolled) ? rolled : undefined };
}

function minibossAllocation<T>(pools: readonly (readonly T[])[], identify: (value: T) => EnemyShuffleIdentity,
  copies: readonly number[], fits: (selected: readonly (T | undefined)[]) => boolean, fixed: readonly EnemyShuffleIdentity[] = []) {
  const family = (target: EnemyShuffleIdentity) => isMinibossEnemy(target.game, target.actorId, target.spawnParams)
    ? enemyVarietyFamily(target) : undefined;
  const families = pools.map(pool => new Map(pool.map(value => [value, family(identify(value))])));
  const fixedFamilies = new Set(fixed.flatMap(target => family(target) ?? []));
  const ordinary: (boolean | undefined)[] = [];
  const hasOrdinary = (i: number) => {
    if (ordinary[i] !== undefined) return ordinary[i];
    const partial: (T | undefined)[] = Array(pools.length);
    return ordinary[i] = pools[i].some(value => {
      if (families[i].get(value)) return false;
      partial[i] = value;
      return fits(partial);
    });
  };
  const failures = (selected: readonly (T | undefined)[], stopEarly = false): AdaptiveMemoryAssessment[] => {
    const used = new Map<string, number[]>();
    selected.forEach((value, i) => {
      const key = value === undefined ? undefined : families[i].get(value);
      if (!key) return;
      const slots = used.get(key) ?? [];
      slots.push(i);
      used.set(key, slots);
    });
    const result: AdaptiveMemoryAssessment[] = [];
    for (const [key, slots] of used) {
      if (!fixedFamilies.has(key) && slots.reduce((n, i) => n + copies[i], 0) <= 1) continue;
      const avoidable = slots.some(i => {
        if (hasOrdinary(i)) return true;
        if (copies[i] > 1) return false;
        const proposal = selected.slice();
        return pools[i].some(value => {
          const alternative = families[i].get(value);
          if (!alternative || used.has(alternative) || fixedFamilies.has(alternative)) return false;
          proposal[i] = value;
          return fits(proposal);
        });
      });
      if (avoidable) {
        result.push({ excess: 1, slots, individual: true });
        if (stopEarly) return result;
      }
    }
    return result;
  };
  return { failures, fits: (selected: readonly (T | undefined)[]) => !failures(selected, true).length };
}

function ootEnemizerSourceKey(defs: readonly EnemyDefinition[], location: OotEnemizerLocation): string {
  if (enemySpawnMacroMatches('oot-dinolfos-source', { source: location, location }))
    return 'oot-dinolfos';
  return baseDefinition(defs, location.actorId).key;
}

const ootPlacementAliases = new Map<string, OotEnemizerChoice['aliases']>();

for (const [parts, aliases] of OOT_ENEMIZER.aliases) {
  const key = parts.join('/');
  if (!ootPlacementAliases.has(key))
    ootPlacementAliases.set(key, aliases);
}

export const OOT_FIELD_STALCHILD_SLOTS = Array.from({ length: 2 }, (_, slot) => ({
  key: `oot-field-stalchild-${slot}`, scene: 0x51, room: 0, slot,
}));

export const OOT_FOREST_BOW_SLOTS = Array.from({ length: 3 }, (_, slot) => ({
  key: `oot-forest-bow-${slot}`, scene: 3, room: 6, slot,
}));

export const OOT_BURIAL_ENEMY_SLOTS = [
  ...Array.from({ length: 6 }, (_, slot) => ({
    key: `oot-well-coffin-${slot}`, scene: 8, room: 2, slot, actorId: 0x13, objectId: 0x13,
  })),
  ...Array.from({ length: 15 }, (_, slot) => ({
    key: `oot-graveyard-poe-${slot}`, scene: 0x53, room: 1, slot, actorId: 0xd, objectId: 9,
  })),
];

const OOT_DYNAMIC_PLANNER_SLOTS = [...OOT_FIELD_STALCHILD_SLOTS, ...OOT_FOREST_BOW_SLOTS, ...OOT_BURIAL_ENEMY_SLOTS];

function ootBurialEnemyLocations(mq: ReadonlySet<string>): OotEnemizerLocation[] {
  const first = OOT_FIELD_STALCHILD_SLOTS.length + OOT_FOREST_BOW_SLOTS.length;
  return OOT_BURIAL_ENEMY_SLOTS.flatMap<OotEnemizerLocation>(({ key, scene, room, actorId }, index) => {
    const dungeon = scene === 8 ? 'BotW' : null;
    if (dungeon && mq.has(DUNGEONS[dungeon])) return [];
    return [{ mq: false, dungeon, scene, room, setup: 0, slot: -1 - index,
      fieldSlot: first + index, actorId, ...ootSpawnTemplate(key), switchFlag: -1 }];
  });
}

function ootSpawnTemplate(name: string) {
  const template = (ENEMY_SPAWN_OOT_TEMPLATES as Readonly<Record<string, EnemyOotSpawnTemplate>>)[name];
  if (!template) throw new Error('Missing enemy spawn point: ' + name);
  return { ...template, choices: template.choices.map(name => {
    const index = OOT_ENEMIZER.targets.findIndex(target => target.name === name);
    if (index < 0) throw new Error('Unknown enemy spawn template target: ' + name);
    return index;
  }) };
}

function ootFieldStalchildLocations(): OotEnemizerLocation[] {
  return OOT_FIELD_STALCHILD_SLOTS.map(({ key, scene, room, slot }) => ({
    mq: false, dungeon: null, scene, room, setup: 0, slot: -1 - slot, fieldSlot: slot,
    actorId: 0x1b0,
    ...ootSpawnTemplate(key), switchFlag: -1,
  }));
}

function ootForestBowLocations(): OotEnemizerLocation[] {
  return OOT_FOREST_BOW_SLOTS.map(({ key, scene, room, slot }) => ({
    mq: false, dungeon: 'Forest', scene, room, setup: 0, slot: -1 - slot,
    fieldSlot: OOT_FIELD_STALCHILD_SLOTS.length + slot, actorId: 2,
    ...ootSpawnTemplate(key), switchFlag: -1,
  }));
}

export const OOT_DESERT_LEEVER_SLOTS = [
  ...Array.from({ length: 5 }, (_, slot) => ({ key: `oot-colossus-leever-${slot}`, scene: 0x5c, room: 0, slot })),
  ...Array.from({ length: 3 }, (_, slot) => ({ key: `oot-wasteland-leever-${slot}`, scene: 0x5e, room: 1, slot })),
] as const;

export const OOT_ENCOUNTER_MEMORY_ROOMS: Readonly<Record<string, readonly (readonly [number, number])[]>> = {
  'oot-dead-hand-encounter': [[7, 4], [8, 4]],
  'oot-dark-link-encounter': [[5, 13]],
  'oot-gerudo-fighter-restored': [[12, 1], [12, 2], [12, 4], [12, 5]],
};

function ootSupplementalMemoryRows(plan: EnemyPlan, options: EnemyMemoryOptions, isMq: (scene: number) => boolean) {
  return plan.flatMap(row => {
    if (row.ootr || row.placement) return [];
    const gold = ENEMY_GOLD_SOURCES.find(s => s.key === row.source && s.game === 'oot');
    const desert = [...OOT_DESERT_LEEVER_SLOTS, ...OOT_DYNAMIC_PLANNER_SLOTS].find(s => s.key === row.source);
    const rooms = desert ? [[desert.scene, desert.room] as const] : gold ? [...new Set(gold.slots.map(s => s.room))].map(room => [gold.scene, room] as const) :
      (OOT_ENCOUNTER_MEMORY_ROOMS[row.source] ?? []).filter(([scene, room]) =>
        !isMq(scene) && nativeFightRestorationRoomAllowed(row.source, room, options));
    if (!rooms.length) return [];
    const target = enemyDefinition(row.target);
    return rooms.map(([scene, room]) => ({
      scene, room, source: row.source, target,
      cost: enemyMemoryCost('oot', target.game, target.actorId, target.spawnParams, options)
    }));
  });
}

function ootPreservedSpawnMemoryRows(plannedSources: ReadonlySet<string>, options: EnemyMemoryOptions, scene: number, mq: boolean) {
  if (mq && scene <= 9) return [];
  return [...OOT_DYNAMIC_PLANNER_SLOTS, ...OOT_DESERT_LEEVER_SLOTS]
    .filter(slot => slot.scene === scene && !plannedSources.has(slot.key))
    .map(slot => {
      const target = enemyDefinition(OOT_DESERT_LEEVER_SLOTS.some(source => source.key === slot.key)
        ? 'oot-leever-native-target' : slot.key);
      return {
        scene, room: slot.room, source: slot.key, target,
        cost: enemyMemoryCost('oot', 'oot', target.actorId, target.spawnParams, options),
      };
    });
}

function ootMemoryRooms(scene: number, mq: boolean): number[] {
  return Object.keys(roomCounts).flatMap(key => {
    const [s, room, m] = key.split('/');
    return +s === scene && m === String(mq) ? [+room] : [];
  });
}

function makeOotEnemizerScenePlan(defs: readonly EnemyDefinition[], settings: Settings, random: Random, locations: readonly OotEnemizerLocation[], gold: EnemyPlan = [], onProgress?: (message: string) => void): EnemyPlan {
  const rows: EnemyPlan = [];
  const candidates = ootEnemizerCandidatePools(defs, settings, locations);
  const roomActorCounts: Readonly<Record<string, number>> = roomCounts;
  const units: {
    members: OotEnemizerLocation[];
    choices: Map<OotEnemizerLocation, OotEnemizerCandidate>[];
    miniboss?: { location: OotEnemizerLocation; candidate: OotEnemizerCandidate };
  }[] = [];
  const groups = new Map<string, OotEnemizerLocation[]>();
  for (const location of candidates.keys()) {
    const key = settings.enemyShuffleOotMode === 'grouped' ? `${location.scene}/${ootEnemizerModelBank(baseDefinition(defs, location.actorId))}/${ootSpawnIsMinibossEncounter(location)}` : ootEnemizerLocationKey(location);
    const entries = groups.get(key) ?? [];
    entries.push(location);
    groups.set(key, entries);
  }
  for (const members of groups.values()) {
    const first = candidates.get(members[0])!;
    const keys = [...new Set(first.map(row => ootGroupedTargetForm(row.def, members[0])))];
    const choices = keys.flatMap(key => {
      const selected = new Map<OotEnemizerLocation, OotEnemizerCandidate>();
      for (const location of members) {
        const candidate = candidates.get(location)!.find(row => ootGroupedTargetForm(row.def, location) === key);
        if (!candidate)
          return [];
        selected.set(location, candidate);
      }
      return [selected];
    });
    const allocation = groupedEnemyPool(choices, members.length, value => value.get(members[0])!.def,
      target => enemyWeight(settings, 'oot', target), max => random.next() % max);
    if (!allocation.pool.length)
      throw Error('No shared compatible OoT enemy form for grouped placements');
    const location = allocation.miniboss ? members[random.next() % members.length] : undefined;
    units.push({ members, choices: allocation.pool,
      miniboss: location ? { location, candidate: allocation.miniboss!.get(location)! } : undefined });
  }
  const actualCost = (location: OotEnemizerLocation, row: OotEnemizerCandidate) => enemyMemoryCost('oot', row.def.game, row.def.actorId, (location.varOverrides as Readonly<Record<string, number>>)[row.def.name] ?? row.def.spawnParams, settings);
  const plannedSources = new Set(gold.map(row => row.source));
  for (const location of candidates.keys())
    if (location.fieldSlot !== undefined) plannedSources.add(OOT_DYNAMIC_PLANNER_SLOTS[location.fieldSlot].key);
  const fixedRows = [
    ...ootSupplementalMemoryRows(gold, settings, () => locations[0].mq),
    ...ootPreservedSpawnMemoryRows(plannedSources, settings, locations[0].scene, locations[0].mq),
  ];
  const goldRooms = new Map<number, EnemyMemoryCost[]>();
  for (const { scene, room, cost } of fixedRows) {
    if (scene !== locations[0].scene) continue;
    const costs = goldRooms.get(room) ?? [];
    costs.push(cost);
    goldRooms.set(room, costs);
  }
  const rooms = [...new Set([...ootMemoryRooms(locations[0].scene, locations[0].mq),
  ...[...candidates.keys()].map(l => l.room), ...goldRooms.keys()])];
  const roomBase = new Map(rooms.map(room => {
    const members = [...candidates.keys()].filter(l => l.room === room && l.fieldSlot === undefined);
    return [room, Math.max(0, (roomActorCounts[`${locations[0].scene}/${room}/${locations[0].mq}`] ?? 32) - members.length)];
  }));
  const choiceCosts = new WeakMap<Map<OotEnemizerLocation, OotEnemizerCandidate>, EnemyMemoryCost[]>();
  for (const unit of units) for (const choice of unit.choices)
    choiceCosts.set(choice, unit.members.map(location => actualCost(location, choice.get(location)!)));
  const minimum = new Map(units.flatMap(unit => unit.members.map(location => {
    const costs = unit.choices.map(choice => actualCost(location, choice.get(location)!));
    return [location, { shared: {}, instances: Math.min(...costs.map(c => c.instances)), actors: Math.min(...costs.map(c => c.actors)) }] as const;
  })));
  const states = rooms.flatMap((a, i) => rooms.slice(i).map(b => ({
    a, b,
    slots: units.flatMap((unit, index) => unit.members.some(l => l.room === a || l.room === b) ? [index] : []),
    roomCount: a === b ? 1 : 2,
    baseActors: (roomBase.get(a) ?? 0) + (a === b ? 0 : roomBase.get(b) ?? 0),
    dynamicCollisionAllowance: Math.max(0, ENEMY_MEMORY_POLICY.dynamicCollisionLimit - ENEMY_MEMORY_POLICY.dynamicCollisionReserve -
      enemyOriginalRoomDynamicCollisions('oot', locations[0].mq, locations[0].scene, a) -
      (a === b ? 0 : enemyOriginalRoomDynamicCollisions('oot', locations[0].mq, locations[0].scene, b))),
  })));

  const stateMembers = new Map(states.map(state => [state, units.flatMap((unit, index) =>
    unit.members.flatMap((location, offset) => location.room === state.a || location.room === state.b
      ? [{ index, offset, minimum: minimum.get(location)! }] : []))]));
  const evaluateMemory = makeEnemyMemoryEvaluator();
  const stateTotals = new Map<typeof states[number], {
    choices: (Map<OotEnemizerLocation, OotEnemizerCandidate> | undefined)[];
    value: ReturnType<typeof evaluateMemory>;
  }>();
  const assessMemory = (selected: readonly (Map<OotEnemizerLocation, OotEnemizerCandidate> | undefined)[], stopEarly = false, affected?: ReadonlySet<number>, failures?: AdaptiveMemoryAssessment[]): AdaptiveMemoryAssessment => {
    let worst: AdaptiveMemoryAssessment = { excess: 0, slots: [] };
    for (const state of states) {
      const { a, b, slots, roomCount, baseActors, dynamicCollisionAllowance } = state;
      if (affected && !slots.some(i => affected.has(i)))
        continue;
      const previous = stateTotals.get(state);
      let total: ReturnType<typeof evaluateMemory>;
      if (previous && slots.every((slot, i) => selected[slot] === previous.choices[i])) {
        total = previous.value;
      } else {
        const costs = [...(goldRooms.get(a) ?? []), ...(a === b ? [] : goldRooms.get(b) ?? [])];
        for (const member of stateMembers.get(state)!) {
          const choice = selected[member.index];
          costs.push(choice ? choiceCosts.get(choice)![member.offset] : member.minimum);
        }
        total = evaluateMemory(costs);
        stateTotals.set(state, { choices: slots.map(slot => selected[slot]), value: total });
      }
      const excess = Math.max(0, total.bytes / OOT_SELECTED_ENEMY_RESOURCE_ENVELOPE - 1,
        (total.dynamicCollisions - dynamicCollisionAllowance) / ENEMY_MEMORY_POLICY.dynamicCollisionLimit,
        (total.actors + baseActors) / (OOT_GAMEPLAY_POLICY.actorLimit - OOT_GAMEPLAY_POLICY.actorSlots) - 1,
        total.foreign / 30 - 1, total.models / 80 - 1);
      if (excess > 0)
        failures?.push({ excess, slots, roomCount });
      if (excess > worst.excess) {
        worst = { excess, slots, roomCount };
        if (stopEarly)
          return worst;
      }
    }
    return worst;
  };
  const fixed = [...new Map(fixedRows
    .filter(row => row.scene === locations[0].scene).map(row => [row.source, row.target])).values()];
  const minibosses = minibossAllocation(units.map(unit => unit.choices), value => value.values().next().value!.def,
    units.map(unit => unit.members.length), selected => assessMemory(selected, true).excess === 0, fixed);
  const assess: typeof assessMemory = (selected, stopEarly = false, affected, failures) => {
    const repeated = minibosses.failures(selected, stopEarly);
    failures?.push(...repeated);
    if (stopEarly && repeated.length) return repeated[0];
    const memory = assessMemory(selected, stopEarly, affected, failures);
    return memory.excess > 0 ? memory : repeated[0] ?? memory;
  };
  const fits = (selected: readonly (Map<OotEnemizerLocation, OotEnemizerCandidate> | undefined)[]) => assess(selected, true).excess === 0;
  let selectedUnits: Map<OotEnemizerLocation, OotEnemizerCandidate>[];
  const adaptiveGroups = new Map<OotEnemizerLocation, string>();
  const protectedPicks = new Set<OotEnemizerLocation>();
  try {
    if (settings.enemyShuffleOotMode === 'grouped') {
      selectedUnits = solveWeightedMemory(units.map(unit => {
        const weights = enemyFormWeights(unit.choices, c => c.get(unit.members[0])!.def, target => enemyWeight(settings, 'oot', target));
        return unit.choices.map((value, i) => ({ value, weight: weights[i], cost: enemyMemoryTotal([...value].map(([l, r]) => actualCost(l, r))).bytes }));
      }), max => random.next() % max, fits);
    }
    else {
      const result = solveAdaptiveMemory(units.map(unit => unit.choices.map(value => {
        const location = unit.members[0], row = value.get(location)!;
        const target = { ...row.def, spawnParams: (location.varOverrides as Readonly<Record<string, number>>)[row.def.name] ?? row.def.spawnParams };
        return {
          value, key: ootGroupedTargetForm(row.def, location), family: enemyWeightFamily(target),
          weight: enemyWeight(settings, 'oot', target), maxGroupSize: enemyGroupLimit(target)
        };
      })), max => random.next() % max, fits, assess, {
        onProgress,
        positions: units.map(unit => unit.members.flatMap(location => {
          if (location.fieldSlot !== undefined) return [];
          const keys = [[+location.mq, location.scene, location.room, location.setup, location.slot],
          ...(ootPlacementAliases.get(ootEnemizerLocationKey(location)) ?? []).map(parts => [0, ...parts])];
          return keys.map(parts => {
            const key = parts.join('/');
            const position = ENEMY_OOT_POSITIONS[key] ?? (ENEMY_SOURCE_ADDITIONS.positions as Record<string, readonly number[]>)[key];
            if (!position)
              throw Error('Missing OoT enemy spatial metadata');
            return { x: position[0], y: position[1], z: position[2] };
          });
        })), assessScopes: selected => {
          const failures: AdaptiveMemoryAssessment[] = [];
          assess(selected, false, undefined, failures);
          return failures;
        }, fitsScopes: (selected, affected) => assess(selected, true, affected).excess === 0
      });
      selectedUnits = result.selected;
      for (const index of result.protectedSlots)
        for (const location of units[index].members)
          protectedPicks.add(location);
      for (const group of result.groups)
        if (group.length > 1)
          for (const index of group)
            for (const location of units[index].members)
              adaptiveGroups.set(location, `oot:${location.scene}:${group[0]}`);
    }
  }
  catch (error) {
    if (!(error instanceof EnemyMemoryAllocationError)) throw error;
    if (settings.enemyShuffleOotMode === 'grouped' || !fits(units.map(() => undefined)))
      throw new OotEnemyResourceLimit(error.message, { cause: error });
    onProgress?.('trying weighted fallback for the same scene');
    try {
      selectedUnits = solveWeightedMemory(units.map(unit => {
        const weights = enemyFormWeights(unit.choices, c => c.get(unit.members[0])!.def, target => enemyWeight(settings, 'oot', target));
        return unit.choices.map((value, i) => ({ value, weight: weights[i], cost: enemyMemoryTotal(choiceCosts.get(value)!).bytes }));
      }), max => random.next() % max, fits);
    }
    catch (fallback) {
      if (!(fallback instanceof EnemyMemoryAllocationError)) throw fallback;
      throw new OotEnemyResourceLimit(`adaptive search: ${error.message}; weighted fallback: ${fallback.message}`, { cause: fallback });
    }
    adaptiveGroups.clear();
    protectedPicks.clear();
    for (const unit of units)
      if (unit.choices.length === 1)
        for (const location of unit.members) protectedPicks.add(location);
  }
  const usedMinibosses = new Set([...fixed, ...selectedUnits.flatMap(unit => [...unit.values()].map(row => row.def))]
    .filter(target => isMinibossEnemy(target.game, target.actorId, target.spawnParams)).map(enemyVarietyFamily));
  for (const [i, unit] of units.entries()) {
    if (!unit.miniboss) continue;
    const { location, candidate } = unit.miniboss;
    const family = enemyVarietyFamily(candidate.def);
    if (usedMinibosses.has(family)) continue;
    const previous = selectedUnits[i], proposal = new Map(previous);
    proposal.set(location, candidate);
    choiceCosts.set(proposal, unit.members.map(member => actualCost(member, proposal.get(member)!)));
    selectedUnits[i] = proposal;
    if (assessMemory(selectedUnits, true).excess > 0) selectedUnits[i] = previous;
    else {
      usedMinibosses.add(family);
      protectedPicks.add(location);
    }
  }
  const selections = new Map(selectedUnits.flatMap(unit => [...unit]));
  for (const [location, pool] of candidates) {
    const selected = selections.get(location)!;
    const extra = (row: OotEnemizerCandidate, location: OotEnemizerLocation) => enemyPlacementDensityCost(row.def.game, row.def.actorId, row.def.spawnParams, roomActorCounts[`${location.scene}/${location.room}/${location.mq}`] ?? 32, row.def.game === 'oot');
    const baseCost = actualCost(location, selected);
    const eligible = pool.filter(row => {
      if ((adaptiveGroups.has(location) || protectedPicks.has(location)) && ootGroupedTargetForm(row.def, location) !== ootGroupedTargetForm(selected.def, location))
        return false;
      if (isMinibossEnemy(row.def.game, row.def.actorId, row.def.spawnParams) &&
        ootGroupedTargetForm(row.def, location) !== ootGroupedTargetForm(selected.def, location))
        return false;
      const cost = actualCost(location, row);
      return cost.instances <= baseCost.instances && cost.actors <= baseCost.actors &&
        (cost.dynamicCollisions ?? 0) <= (baseCost.dynamicCollisions ?? 0) && Object.entries(cost.shared).every(([key, size]) => size <= (baseCost.shared[key] ?? 0));
    });
    const source = location.fieldSlot === undefined ? ootEnemizerSourceKey(defs, location) : OOT_DYNAMIC_PLANNER_SLOTS[location.fieldSlot].key;
    const target = selected.def;
    const descriptor = selected.index >= 0 ? OOT_ENEMIZER.targets[selected.index] : {
      name: target.name,
      actorId: target.actorId,
      params: target.spawnParams,
      weight: 1,
      killLogic: target.killMacro,
      dropLogic: null,
      soulName: target.name,
      restrictions: [],
      requires: [],
    };
    const overrides = location.varOverrides as Readonly<Record<string, number>>;
    const logic = (location.logicOverrides as Readonly<Record<string, string>>)[descriptor.name];
    const params = overrides[descriptor.name] ?? descriptor.params;
    const targetKey = selected.index >= 0 ? ootNativeEnemyDefinitionKey(target.actorId, params) : target.key;
    const choice: OotEnemizerReplacement['ootr'] = {
      location,
      target: descriptor,
      targetIndex: selected.index,
      targetParams: params,
      killLogic: logic ?? (target.game === 'oot' && target.actorId === 0x25 ? target.killMacro : descriptor.killLogic),
      dropLogic: logic !== undefined ? null : descriptor.dropLogic,
      aliases: ootPlacementAliases.get(ootEnemizerLocationKey(location)) ?? [],
      foreign: target.game !== 'oot',
    };
    rows.push({
      source, target: targetKey,
      ...(location.fieldSlot === undefined ? { ootr: choice } : { kind: location.scene === 3 ? 'forest-bow' : location.scene === 0x51 ? 'field-spawner' : 'burial-spawner', field: choice }),
      ...(adaptiveGroups.has(location) ? { adaptiveGroup: adaptiveGroups.get(location) } : {}),
      ...(protectedPicks.has(location) ? { protectedPick: true } : {})
    });
  }
  return rows;
}

export function enemyNeverShuffled(game: Game, actorId: number): boolean {
  return enemySpawnMacroMatches('never-shuffled', { target: { game, actorId } });
}

export function enemySourceAllowedBySettings(game: Game, actorId: number, settings: Settings, params: number | undefined,
  sourceIsMiniboss: boolean): boolean {
  /* Wallmaster Shuffle owns natural Wallmaster source slots: they must stay
   * in place so their authored entrance-shuffle warp destinations still work. */
  if (minibossOnly(settings, game) && !sourceIsMiniboss)
    return false;
  if (settings.erWallmasters !== 'none' && (game === 'oot' ? actorId === 0x11 : actorId === 0x0a))
    return false;
  return enemySpawnMacroMatches('source-settings-allowed', { target: { game, actorId, params }, settings, facts: { miniboss: !minibossOnly(settings, game) && sourceIsMiniboss } });
}

function ootResourceExcess(report: ReturnType<typeof enemyOotSceneMemoryReports>[number]): number {
  return Math.max(0, report.bytes / report.budget - 1, report.actors / report.actorBudget - 1,
    report.models / 80 - 1, report.foreign / 30 - 1,
    (report.dynamicCollisions - report.collisionBudget) / ENEMY_MEMORY_POLICY.dynamicCollisionLimit);
}

function validateOotGoldMemory(plan: EnemyPlan, settings: Settings): void {
  const gold = plan.filter(row => ENEMY_GOLD_SOURCES.some(s => s.game === 'oot' && s.key === row.source));
  const plannedSources = new Set([...plan.map(row => row.source), ...OOT_DYNAMIC_PLANNER_SLOTS.map(slot => slot.key)]);
  for (const report of enemyOotSceneMemoryReports(gold, settings, plannedSources))
    if (ootResourceExcess(report) > 0)
      throw new OotEnemyResourceLimit(`OoT scene ${report.scene}: Gold Skulltula replacements exceed the room reserve`);
}

function repairOotGoldMemory(plan: EnemyPlan, pools: ReadonlyMap<string, readonly EnemyDefinition[]>, settings: Settings, random: Random): void {
  const gold = plan.filter(row => pools.has(row.source));
  /* Exact room placements are planned after the supplemental choices. Their
   * dynamic slots must not look preserved while this search is still partial. */
  const plannedSources = new Set([...plan.map(row => row.source), ...OOT_DYNAMIC_PLANNER_SLOTS.map(slot => slot.key)]);
  const scenes = new Set(ootSupplementalMemoryRows(gold, settings, () => false).map(row => row.scene));
  for (const scene of scenes) {
    const rows = gold.filter(row => ENEMY_GOLD_SOURCES.some(s => s.game === 'oot' && s.scene === scene && s.key === row.source) ||
      OOT_DESERT_LEEVER_SLOTS.some(s => s.scene === scene && s.key === row.source));
    const fixedRows = ootSupplementalMemoryRows(plan.filter(row => !rows.includes(row)), settings, () => false)
      .filter(row => row.scene === scene);
    const fixedSources = new Set(fixedRows.map(row => row.source));
    const fixedPlan = plan.filter(row => fixedSources.has(row.source));
    const assess = (selected: readonly (EnemyDefinition | undefined)[]): AdaptiveMemoryAssessment => {
      const selectedPlan = [...fixedPlan,
        ...rows.flatMap((row, i) => selected[i] ? [{ ...row, target: selected[i]!.key }] : [])];
      let worst: AdaptiveMemoryAssessment = { excess: 0, slots: [] };
      for (const report of enemyOotSceneMemoryReports(selectedPlan, settings, plannedSources)) {
        if (report.scene !== scene) continue;
        const excess = ootResourceExcess(report);
        if (excess > worst.excess) worst = {
          excess, roomCount: report.rooms.length,
          slots: rows.flatMap((row, i) => ENEMY_GOLD_SOURCES.some(s => s.key === row.source && s.slots.some(slot => report.rooms.includes(slot.room))) ||
            OOT_DESERT_LEEVER_SLOTS.some(s => s.key === row.source && report.rooms.includes(s.room)) ? [i] : [])
        };
      }
      return worst;
    };

    const units: number[][] = [];
    const desert = rows.flatMap((row, i) => row.kind === 'desert-spawner' ? [i] : []);
    for (let i = 0; i < rows.length; ++i) {
      if (settings.enemyShuffleOotMode === 'grouped' && desert.includes(i)) {
        if (i === desert[0]) units.push(desert);
      } else units.push([i]);
    }
    const expand = (selected: readonly (EnemyDefinition | undefined)[]) =>
      rows.map((_, i) => selected[units.findIndex(unit => unit.includes(i))]);
    const assessUnits = (selected: readonly (EnemyDefinition | undefined)[]): AdaptiveMemoryAssessment => {
      const result = assess(expand(selected));
      return { ...result, slots: units.flatMap((unit, i) => unit.some(slot => result.slots.includes(slot)) ? [i] : []) };
    };
    const candidates = units.map(unit => pools.get(rows[unit[0]].source)!.filter(value => unit.length <= enemyGroupLimit(value)));
    const fixed = [...new Map(fixedRows.map(row => [row.source, row.target])).values()];
    const minibosses = minibossAllocation(candidates, value => value, units.map(unit => unit.length),
      selected => assessUnits(selected).excess === 0, fixed);
    const initial = units.map((unit, i) => candidates[i].find(value => value.key === rows[unit[0]].target)!);
    if (initial.every(Boolean) && minibosses.fits(initial) && assessUnits(initial).excess === 0) continue;
    const assessAllocation = (selected: readonly (EnemyDefinition | undefined)[]) =>
      minibosses.failures(selected, true)[0] ?? assessUnits(selected);
    const result = solveAdaptiveMemory(candidates.map(pool => pool.map(value => ({
      value,
      key: value.key, family: enemyWeightFamily(value), weight: enemyWeight(settings, 'oot', value), maxGroupSize: enemyGroupLimit(value)
    }))),
      max => random.next() % max, selected => minibosses.fits(selected) && assessUnits(selected).excess === 0, assessAllocation,
      { positions: units.map(() => []), canGroup: () => false });
    const selected = expand(result.selected);
    rows.forEach((row, i) => {
      row.target = selected[i]!.key;
    });
  }
}

function makeOotEnemizerPlan(defs: readonly EnemyDefinition[], settings: Settings, random: Random, options: {
  mqDungeons?: ReadonlySet<string>;
} = {}, gold: EnemyPlan = [], onProgress?: EnemyPlanProgress): EnemyPlan {
  if (settings.games === 'mm' || !(settings.enemyShuffleOot || settings.enemyShuffleOotMinibossOnly))
    return [];
  validateOotGoldMemory(gold, settings);
  const scenes = new Map<number, OotEnemizerLocation[]>();
  const mq = resolvedMq(settings, options.mqDungeons);
  for (const location of [...ootEnemizerLocations(mq, settings), ...ootFieldStalchildLocations(), ...(!mq.has(DUNGEONS.Forest) ? ootForestBowLocations() : []), ...ootBurialEnemyLocations(mq)]) {
    if (!spawnEncounterAllows(ootSpawnRegistryId(location), settings, 'oot') ||
      !enemySourceAllowedBySettings('oot', location.actorId, settings, undefined, ootSpawnIsMinibossEncounter(location))) continue;
    const source = ootSpawnSource(defs, location);
    if (!ootSpawnLocations(location).every(where => enemySpawnSourceAllowed(settings, where, source))) continue;
    const rows = scenes.get(location.scene);
    if (rows)
      rows.push(location);
    else
      scenes.set(location.scene, [location]);
  }
  const result: EnemyPlan = [];
  let completed = 0;
  for (const [scene, locations] of scenes) {
    onProgress?.('Placing OoT enemies', completed, scenes.size);
    try {
      result.push(...makeOotEnemizerScenePlan(defs, settings, random, locations, gold,
        message => onProgress?.(`Placing OoT enemies: ${message}`, completed, scenes.size)));
    }
    catch (error) {
      if (!(error instanceof OotEnemyResourceLimit)) throw error;
      const forced = settings.plando.enemyTargets?.oot;
      if (forced)
        throw new OotEnemyResourceLimit(`Forced OoT target cannot fit selected-enemy resource envelope in scene ${scene}: ${forced}; ${error.message}`, { cause: error });
      // The enclosing planner already retries the whole allocation up to 8 times.
      throw new OotEnemyResourceLimit(`OoT scene ${scene}: bounded selected-enemy resource search exhausted; ${error.message}`, { cause: error });
    }
    onProgress?.('Placing OoT enemies', ++completed, scenes.size);
  }
  return result;
}

type MmEnemy = {
  key: string;
  game: 'oot' | 'mm';
  actorId: number;
  objectId: number;
  type: number;
  stationary: boolean;
  variables: number[];
};

type MmActor = {
  slot: number;
  offset: number;
  actorId: number;
  params: number;
  raw: number[];
};

function mmSourceFormAllowed(actor: Pick<MmActor, 'actorId' | 'params'>): boolean {
  return enemySpawnMacroMatches('mm-source-form', { source: actor });
}

type MmMap = {
  room: number;
  setup: number;
  start: number;
  end: number;
  header: number;
  actors: MmActor[];
  objects: {
    offset: number;
    objectId: number;
  }[];
  objectCommand?: number;
};

type MmScene = {
  scene: number;
  grotto?: number;
  maps: MmMap[];
};

function mmSpawnLocations(scene: MmScene, actor: MmActor): EnemySpawnLocation[] {
  return scene.maps.flatMap(map => map.actors.filter(alias => alias.offset === actor.offset)
    .map(alias => ({ game: 'mm' as const, scene: scene.scene, room: map.room, setup: map.setup, slot: alias.slot, grotto: scene.grotto })));
}

function mmSpawnSource(scene: MmScene, actor: MmActor, key?: string) {
  const habitat = ENEMIES.find(enemy => enemy.game === 'mm' && enemy.actorId === actor.actorId)?.habitat;
  return { ...actor, habitat, encounterId: mmSpawnRegistryId(scene, actor), key: actor.actorId === 0x12d && actor.params === 2 ? MM_BIO_BABA_LILY_PAD_KEY : key };
}

function mmSpawnRegistryId(scene: MmScene, actor: MmActor): string {
  const points = mmSpawnLocations(scene, actor).map(where => spawnPointId(where, scene.grotto)).sort();
  if (!points.length) throw new Error('Missing MM physical spawn: ' + actor.offset);
  return points[0];
}

function mmSpawnIsMinibossEncounter(scene: MmScene, actor: MmActor): boolean {
  return spawnEncounter(mmSpawnRegistryId(scene, actor)).minibossEncounter;
}

function mmSpawnSourceAllowed(settings: Settings, scene: MmScene, actor: MmActor, key?: string): boolean {
  const source = mmSpawnSource(scene, actor, key);
  return spawnEncounterAllows(mmSpawnRegistryId(scene, actor), settings, 'mm') &&
    enemySourceAllowedBySettings('mm', actor.actorId, settings, undefined, mmSpawnIsMinibossEncounter(scene, actor)) &&
    mmSpawnLocations(scene, actor).every(where => enemySpawnSourceAllowed(settings, where, source));
}

function mmSpawnLogicName(scene: MmScene, actor: MmActor): string {
  return `enemy_mm_${scene.scene}_${actor.offset}${scene.grotto === undefined ? '' : '_' + scene.grotto}`;
}

function mmSpawnTargetAllowed(scene: MmScene, actor: MmActor, target: EnemySpawnTarget, settings: Settings, key?: string): boolean {
  // The shuffled Dexihand uses zeroed throw parameters. Keep authored throws out of that conversion.
  if (actor.actorId === 0x1d1 && target.game === 'mm' && target.actorId === actor.actorId)
    return false;
  const source = mmSpawnSource(scene, actor, key);
  return enemyTargetCompatible(settings, mmSpawnLogicName(scene, actor), target) &&
    mmSpawnLocations(scene, actor).every(where => enemySpawnTargetAllowed(where, source, target, settings));
}

function mmAquaticSpawnAllowed(actor: MmActor, target: Pick<EnemySpawnTarget, 'game' | 'actorId' | 'spawnParams'>): boolean {
  /* Mounted Bio Babas are authored above their leaves, not at an underwater
   * raycast slot. Their unchanged native form already supports that position. */
  return enemySpawnMacroMatches('mm-aquatic-source', { source: actor, target: { ...target, params: target.spawnParams }, facts: { aquaticSlot: !!mmAquaticSlot(actor.offset) } });
}

type MmActorChange = {
  adaptiveGroup?: string;
  protectedPick?: boolean;
  mapIndex: number;
  room: number;
  setup: number;
  slot: number;
  actorOffset: number;
  sourceActorId: number;
  sourceParams: number;
  targetKey: string;
  targetGame: Game;
  targetActorId: number;
  targetParams: number;
};

const nativeEnemyDemandObjects: Record<Game, Readonly<Record<number, readonly number[]>>> = {
  oot: {
    0x02: [0x32], 0x0d: [0x09, 0x6e], 0x0e: [0x07], 0x11: [0x0b], 0x12: [0x0c], 0x13: [0x0d], 0x1b: [0x16],
    0x1c: [0x17], 0x1d: [0x18], 0x25: [0x1b], 0x2b: [0x1e], 0x2d: [0x12], 0x2f: [0x1f], 0x33: [0x20], 0x34: [0x21], 0x35: [0x22], 0x37: [0x24], 0x38: [0x25], 0x3a: [0x26], 0x4b: [0x30], 0x54: [0x38], 0x55: [0x39], 0x60: [0x4a], 0x63: [0x56], 0x6b: [0x5f], 0x69: [0x5d], 0x8a: [0x8b], 0x8e: [0x0b], 0x90: [0x98], 0x91: [0x99], 0x95: [0x24], 0x99: [0x9e], 0xa4: [0xa6],
    0x73: [0x2026],
    0xc5: [0xb5], 0xc7: [0x39], 0xdd: [0xd4], 0xde: [0xd5], 0xec: [0xe5], 0x113: [0x106], 0x11d: [], 0x115: [0x10a], 0x121: [0x114], 0x175: [0x6d], 0x195: [0x168],
    0x18c: [0x26], 0x197: [0x169], 0x1af: [0x183], 0x1c0: [0x08], 0x1b0: [0x184],
  },
  mm: {
    0x07: [0x128], 0x08: [0x05], 0x0a: [0x09], 0x0b: [0x0a], 0x0c: [0x0b], 0x12: [0x12], 0x14: [0x14], 0x19: [0x17], 0x1d: [0x0e], 0x24: [0x20], 0x2d: [0x1d],
    0x32: [0x30], 0x33: [0x31], 0x3b: [0x40], 0x3c: [0x51], 0x3e: [0x51], 0x43: [0x52], 0x47: [0x6a], 0x50: [0x20],
    0x56: [0x2026],
    0x4a: [0x09], 0x4c: [0x75], 0x64: [0x8e], 0x65: [0x128], 0x66: [0x31], 0x6c: [0xab],
    0x84: [0xd8], 0x8d: [], 0x8f: [0xe4], 0xec: [0x141], 0xed: [0x142], 0xf1: [0x06], 0x109: [0x14e], 0x113: [0x155], 0x115: [0x156], 0x12c: [0x15d], 0x12d: [0x15e], 0x13a: [0x161], 0x14a: [0x16a, 0x167], 0x14b: [0x16b], 0x155: [0x171], 0x15b: [0x172], 0x15d: [0x178], 0x164: [0x17a],
    0x16f: [0x181], 0x180: [0x16b], 0x182: [0x155], 0x184: [0x18d], 0x1ba: [0x1a6], 0x1d1: [0x1b5], 0x1e6: [0x1c4], 0x1e8: [0x1c5], 0x1e9: [0x1c6], 0x1f3: [0x1c3], 0x204: [0x1eb], 0x208: [0x1f1], 0x216: [0x201], 0x21d: [0x204], 0x23f: [0x15c], 0x28f: [0x268], 0x291: [0x22],
  },
};

export function nativeEnemyOnDemandObjects(game: Game, actorId: number, params?: number): readonly number[] | undefined {
  const ids = nativeEnemyDemandObjects[game][actorId];
  const redIce = params === 0x7e04 && (game === 'oot' ? actorId === 0x90 : actorId === 0x4c) ||
    (game === 'oot' ? actorId === 0x11 && params === 0x7e80 : actorId === 0xa && params === 0x7e81);
  return ids && redIce ? [...ids, game === 'oot' ? 0x6b : 0x106b] : ids;
}

function mmSceneSkipped(scene: number): boolean {
  return enemySpawnMacroMatches('mm-skipped-scene', { location: { scene } });
}

type MmTargetGroupAllows = (source: readonly MmEnemy[], target: MmEnemy, actor?: MmActor, members?: readonly MmActor[]) => boolean;

export function mmPlacementTypeAllows(source: Pick<MmEnemy, 'type'> & Partial<Pick<MmEnemy, 'game' | 'actorId'>>, target: Pick<MmEnemy, 'type'> & Partial<Pick<MmEnemy, 'game' | 'actorId'>>): boolean {
  return enemySpawnMacroMatches('mm-placement-type', { source, target });
}

function solveMmScene(scene: MmScene, nativeRoster: readonly MmEnemy[], targetRoster: readonly MmEnemy[], next: (max: number) => number, groupAllows: MmTargetGroupAllows, ignoreRestrictions: boolean, weight: (target: EnemyShuffleIdentity) => number, supplemental: EnemyPlan, memoryOptions: Settings, queue?: MmSceneProblem[]): MmActorChange[] {
  if (mmSceneSkipped(scene.scene))
    return [];
  const objects = new Set(scene.maps.flatMap(map => map.objects.map(o => o.objectId)));
  const groups = new Map<string, {
    actor: MmActor;
    maps: number[];
    source: MmEnemy;
  }[]>();
  scene.maps.forEach((map, mi) => map.actors.forEach(actor => {
    const source = nativeRoster.find(e => e.actorId === actor.actorId && mmSourceFormAllowed(actor) &&
      (e.objectId <= 3 || objects.has(e.objectId)));
    if (!source || !mmSpawnSourceAllowed(memoryOptions, scene, actor, source.key))
      return;
    const groupKey = `${source.objectId}/${mmSpawnIsMinibossEncounter(scene, actor)}`;
    const entries = groups.get(groupKey) ?? [];
    const existing = entries.find(e => e.actor.offset === actor.offset);
    if (existing)
      existing.maps.push(mi);
    else
      entries.push({ actor, maps: [mi], source });
    groups.set(groupKey, entries);
  }));
  const rows = [...groups.values()].map(entries => ({ entries, copies: entries.length }));
  const allocations = rows.map(row => {
    const sources = [...new Set(row.entries.map(e => e.source))];
    const compatible = targetRoster.filter(target => groupAllows(sources, target, undefined, row.entries.map(entry => entry.actor)) &&
      row.entries.every(({ actor, source }) => groupAllows([source], target, actor) &&
        (ignoreRestrictions || mmPlacementTypeAllows(source, target))));
    if (!compatible.length && row.entries.some(entry => enemyCompatibilityRule(memoryOptions, mmSpawnLogicName(scene, entry.actor))))
      throw new Error(`No compatible weighted enemy for MM scene ${scene.scene}`);
    const allocation = groupedEnemyPool(compatible, row.entries.length,
      target => ({ ...target, spawnParams: target.variables[0] }), weight, next);
    if (!allocation.pool.length && compatible.length)
      throw new EnemyMemoryAllocationError(`MM grouped scene ${scene.scene}: no target supports a group of ${row.entries.length}`);
    return allocation;
  });
  const active = rows.flatMap((row, i) => allocations[i].pool.length
    ? [{ ...row, miniboss: allocations[i].miniboss, minibossSlot: allocations[i].miniboss ? next(row.entries.length) : -1 }] : []);
  const eligible = allocations.filter(allocation => allocation.pool.length).map(allocation => allocation.pool);
  const memory = mmSceneMemory(scene, active, eligible, supplemental, undefined, memoryOptions);
  const weighted = eligible.map(pool => {
    const weights = enemyFormWeights(pool, target => ({ ...target, spawnParams: target.variables[0] }), weight);
    return pool.map((value, i) => ({ value, weight: weights[i], cost: enemyMemoryTotal([enemyMemoryCost('mm', value.game, value.actorId, value.variables[0], memoryOptions)]).bytes }));
  });
  const emit = (selected: MmEnemy[]) => {
    const changes: MmActorChange[] = active.flatMap((row, i) => row.entries.map(({ actor, maps }) => {
      const mapIndex = maps[0], map = scene.maps[mapIndex], target = selected[i];
      return {
        mapIndex, room: map.room, setup: map.setup, slot: actor.slot, actorOffset: actor.offset,
        sourceActorId: actor.actorId, sourceParams: actor.params, targetKey: target.key, targetGame: target.game,
        targetActorId: target.actorId, targetParams: target.variables[0]
      };
    }));
    if (!active.some(row => row.miniboss)) return changes;
    const slots = active.flatMap(row => row.entries.map(entry => ({ entries: [entry] })));
    const targets = active.flatMap((row, i) => row.entries.map(() => selected[i]));
    const splitMemory = mmSceneMemory(scene, slots, undefined, supplemental, undefined, memoryOptions);
    const used = new Set([
      ...targets.map(target => ({ ...target, spawnParams: target.variables[0] })),
      ...supplemental.filter(row => scene.maps.some(map => mmEncounterRoots(scene, map, row).length)).map(row => enemyDefinition(row.target)),
    ].filter(target => isMinibossEnemy(target.game, target.actorId, target.spawnParams)).map(enemyVarietyFamily));
    let start = 0;
    for (const row of active) {
      const index = start + row.minibossSlot;
      start += row.entries.length;
      const target = row.miniboss;
      if (!target) continue;
      const family = enemyVarietyFamily({ ...target, spawnParams: target.variables[0] });
      if (used.has(family)) continue;
      const previous = targets[index];
      targets[index] = target;
      if (!splitMemory.fits(targets)) {
        targets[index] = previous;
        continue;
      }
      used.add(family);
      Object.assign(changes[index], {
        targetKey: target.key, targetGame: target.game, targetActorId: target.actorId,
        targetParams: target.variables[0], protectedPick: true,
      });
    }
    return changes;
  };
  if (queue) {
    const result: MmActorChange[] = [];
    queue.push({
      scene, rows: active, pools: eligible.map(pool => pool.map(value => {
        const target = { ...value, spawnParams: value.variables[0] };
        return {
          value, key: `${target.game}/${target.actorId}/${target.spawnParams}`, family: enemyWeightFamily(target),
          variety: enemyVarietyFamily(target), weight: weight(target), maxGroupSize: enemyGroupLimit(target)
        };
      })), individual: false, positions: active.map(() => []), finish: selected => {
        result.push(...emit(selected));
      }
    });
    return result;
  }
  let selected: MmEnemy[];
  try {
    selected = solveWeightedMemory(weighted, next, memory.fits);
  }
  catch (error) {
    if (error instanceof EnemyMemoryAllocationError)
      throw mmAllocationError(`MM grouped scene ${scene.scene}: ${error.message}`, scene, active, eligible, supplemental, memoryOptions);
    throw error;
  }
  return emit(selected);
}

function finalMmActorChanges(changes: readonly MmActorChange[]): MmActorChange[] {
  const byOffset = new Map<number, MmActorChange>();
  for (const change of changes) {
    byOffset.set(change.actorOffset, change);
  }
  return [...byOffset.values()];
}

type MmMemorySlot = {
  entries: readonly {
    actor: MmActor;
    maps: readonly number[];
  }[];
  encounter?: EnemyReplacement;
  copies?: number;
};

type MmEncounterChoice = {
  value: EnemyReplacement;
  weight: number;
  cost: number;
  familyWeight: number;
};

type MmSceneProblem = {
  scene: MmScene;
  rows: readonly MmMemorySlot[];
  pools: readonly (readonly AdaptiveMemoryChoice<MmEnemy>[])[];
  individual: boolean;
  positions: readonly (readonly {
    x: number;
    y: number;
    z: number;
  }[])[];
  finish: (selected: MmEnemy[], groups: number[][], protectedSlots: number[]) => void;
};

function mmEncounterRoots(scene: MmScene, map: MmMap, row: EnemyReplacement): MmActor[] {
  if (!row.kind || row.kind === 'variant')
    return [];
  if (row.kind === 'gold') {
    const source = ENEMY_GOLD_SOURCES.find(s => s.key === row.source)!;
    return source.game !== 'mm' || source.scene !== scene.scene ? [] :
      map.actors.filter(actor => source.slots.some(slot => slot.offset === actor.offset));
  }
  const source = enemyDefinition(row.source);
  if (source.game !== 'mm')
    return [];
  return map.actors.filter(actor => actor.actorId === (row.kind === 'keeta-guards' ? 0x28f : source.actorId) &&
    (row.kind !== 'restore' || source.params.includes(actor.params)) &&
    (row.kind !== 'scripted' || enemyScriptedRootOffsets[scene.scene]?.includes(actor.offset)));
}

export type EnemySceneMemoryReport = {
  scene: number;
  grotto?: number;
  budget: number;
  bytes: number;
  headroom: number;
  fragmentation: number;
  actors: number;
  objectSavings: number;
  scenarios: number;
  resourceExcess: number;
};

export function mmScriptedReleasedObjects(scene: number, map: MmMap, plan: EnemyPlan): number[] {
  const scripted = plan.filter(row => row.kind === 'scripted' &&
    !enemyKeepsNativeEncounter('mm', enemyDefinition(row.source).actorId, enemyDefinition(row.source).spawnParams, enemyDefinition(row.target)));
  const roots = scripted.flatMap(row => mmEncounterRoots({ scene } as MmScene, map, row));
  const offsets = new Set(roots.map(root => root.offset));
  const retained = map.actors.filter(actor => !offsets.has(actor.offset));
  const objects = new Set(roots.filter(root => root.actorId !== 0x115).map(root => enemyNativeActorFact('mm', root.actorId)?.object));
  return map.objects.flatMap(({ objectId }) => {
    if (!objects.has(objectId) || objectId <= 3 || retained.some(actor => {
      const fact = enemyNativeActorFact('mm', actor.actorId);
      return !fact || fact.object === objectId || [...enemyNativeActorDependencies('mm', actor.actorId)].some(id => enemyNativeActorFact('mm', id)?.object === objectId);
    }) || scripted.some(row => enemyMemoryCost('mm', enemyDefinition(row.target).game,
      enemyDefinition(row.target).actorId, enemyDefinition(row.target).spawnParams).shared[`model/mm/${objectId}`] !== undefined))
      return [];
    return [objectId];
  });
}

const SCRIPTED_NATIVE_CHILDREN: Readonly<Record<number, readonly (readonly [number, number])[]>> = {
  7: [[0x144, 1]], 0x65: [[0xd9, 10]], 0x12c: [[0x23e, 83]], 0x43: [[0x44, 20]], 0x115: [[0x115, 2]],
};

function mmSupplementalCosts(scene: MmScene, plan: EnemyPlan, memoryOptions: EnemyMemoryOptions = {}): EnemyMemoryCost[][] {
  return scene.maps.map(map => plan.flatMap(row => {
    if (row.placement?.scene === scene.scene && row.placement.grotto === scene.grotto && map.actors.some(actor => actor.offset === row.placement!.actorOffset))
      return splitMemoryCosts('mm', row, memoryOptions);
    if (!row.kind || row.kind === 'variant')
      return [];
    const target = enemyDefinition(row.target), guards = row.kind === 'keeta-guards';
    if (row.kind === 'scripted' && enemyKeepsNativeEncounter('mm', enemyDefinition(row.source).actorId, enemyDefinition(row.source).spawnParams, target)) return [];
    const roots = mmEncounterRoots(scene, map, row);
    return roots.flatMap(() => {
      const child = enemyMemoryCost('mm', target.game, target.actorId, target.spawnParams, memoryOptions);
      return guards ? [child, child] : row.kind === 'scripted' ? [child, enemyScriptedControllerCost()] : [child];
    });
  }));
}

function mmSceneMemory(scene: MmScene, slots: readonly MmMemorySlot[], pools?: readonly (readonly MmEnemy[])[], supplemental: EnemyPlan = [], dynamicSupplemental?: () => EnemyPlan, memoryOptions: EnemyMemoryOptions = {}) {
  const p = ENEMY_MEMORY_POLICY;
  const variableSources = new Set(slots.flatMap(slot => slot.encounter ? [slot.encounter.source] : []));
  const supplementalCosts = mmSupplementalCosts(scene, supplemental.filter(row => !variableSources.has(row.source)), memoryOptions);
  const keep = (nativeSceneKeeps.scenes.mm as Record<string, number>)[scene.scene];
  let roomBytes = 0, playerSlots = 1, horseSlots = 1;
  for (const map of scene.maps.filter(m => m.setup === 0)) {
    let bytes = 0, players = 1, horses = 1;
    for (const { objectId: id } of map.objects) {
      if ([0x10, 0x11, 0x14c, 0x14d, 0x154].includes(id))
        ++players;
      else if (id === 0x7d)
        ++horses;
      else
        bytes += enemyObjectBytes('mm', id);
    }
    roomBytes = Math.max(roomBytes, bytes);
    playerSlots = Math.max(playerSlots, players);
    horseSlots = Math.max(horseSlots, horses);
  }
  const originalPool = scene.scene === 0x2d ? 1470 * 1024 : 1380 * 1024;
  const catalogBound = enemyObjectBytes('mm', 1) + (keep ? enemyObjectBytes('mm', keep) : 0) + roomBytes +
    playerSlots * p.playerObjectBound + horseSlots * p.horseObjectBound + 0x1000;
  const objectBound = enemySceneObjectBounds[scene.scene];
  const bound = objectBound ? Math.max(catalogBound, objectBound.baseBytes + objectBound.playerSlots * p.playerObjectBound +
    objectBound.horseSlots * p.horseObjectBound + 0x1000) : originalPool;
  const objectSavings = keep && ![0x08, 0x20, 0x24, 0x43, 0x4f, 0x69].includes(scene.scene)
    ? Math.max(0, originalPool - bound) : 0;
  const extra = p.mmRecovered - p.engineAllowance - p.gameplayReserve - p.fragmentationReserve;
  const days = (actor: MmActor) => (((actor.raw[9] & 7) << 7) | (actor.raw[13] & 0x7f)) || 0x3ff;
  const rowByMap = scene.maps.map(() => new Map<number, number>());
  slots.forEach((row, i) => row.entries.forEach(entry => entry.maps.forEach(mi => rowByMap[mi].set(entry.actor.offset, i))));
  type State = {
    indices: number[];
    prepaid: Set<string>;
    budget: number;
    actors: number;
    dynamicCollisionAllowance: number;
    sourceCosts: EnemyMemoryCost[];
    supplemental: EnemyMemoryCost[];
    maps: number[];
  };
  const scripted = [...supplemental.filter(row => !(row.kind === 'scripted' && enemyKeepsNativeEncounter('mm', enemyDefinition(row.source).actorId, enemyDefinition(row.source).spawnParams, enemyDefinition(row.target)))),
    ...slots.flatMap(slot => slot.encounter ? [slot.encounter] : [])].filter(row => row.kind === 'scripted');
  const removedRoots = new Set(scene.maps.flatMap(map => scripted.flatMap(row => mmEncounterRoots(scene, map, row)).map(actor => actor.offset)));
  const releasedObjects = scene.maps.map(map => new Set(mmScriptedReleasedObjects(scene.scene, map, scripted)));
  const states: State[] = [], unique = new Set<string>();
  for (const { setup, maps } of mmRoomOverlapStates(scene.scene, scene.maps)) {
    const [a, b = a] = maps, first = scene.maps[a], second = scene.maps[b];
    for (let day = 0; day < 10; day++) {
      const mask = (1 << day) | (day < 9 ? 1 << (day + 1) : 0);
      const indices: number[] = [], original: MmActor[] = [], retained: MmActor[] = [], flowers: MmActor[] = [];
      for (const mi of maps)
        for (const actor of scene.maps[mi].actors) {
          if (!(days(actor) & mask))
            continue;
          const index = rowByMap[mi].get(actor.offset);
          if (index === undefined) {
            if (removedRoots.has(actor.offset)) original.push(actor);
            else retained.push(actor);
          }
          else {
            indices.push(index);

            if (actor.actorId === 0x3b && [0, 2].includes(actor.params & 0xff)) flowers.push(actor);

            if (actor.actorId === 0x13a || actor.actorId === 0x12d && actor.params === 0) retained.push(actor);
            else original.push(actor);
          }
        }
      const extraCosts = maps.flatMap(mi => supplementalCosts[mi]);
      const sourceCosts: EnemyMemoryCost[] = retained
        .filter(actor => actor.actorId === 0x13a && maps.some(mi => rowByMap[mi].has(actor.offset)))
        .map(() => ({ shared: {}, instances: 128, actors: 0 }));
      /* Scrub flowers exist independently of the replacement, including
       * before it dies. Account for their actor, overlay and dyna slot. */
      sourceCosts.push(...flowers.map(() => enemyMemoryCost('mm', 'mm', 0x183, 0)));
      slots.forEach((slot, index) => {
        if (!slot.encounter)
          return;
        for (const mi of maps)
          for (const _ of mmEncounterRoots(scene, scene.maps[mi], slot.encounter)) {
            indices.push(index);
            if (slot.encounter.kind === 'keeta-guards')
              indices.push(index);

          }
      });
      if (!indices.length && !extraCosts.length)
        continue;
      const prepaid = new Set<string>(enemyPinnedMmActors.map(id => `code/mm/${id}`));
      for (const { objectId: id } of first.objects)
        if ((a === b || second.objects.some(o => o.objectId === id)) &&
          !maps.every(mi => releasedObjects[mi].has(id)))
          prepaid.add(`model/mm/${id}`);
      for (const row of scripted)
        if (enemyDefinition(row.source).actorId === 0x115 && maps.some(mi => mmEncounterRoots(scene, scene.maps[mi], row).length))
          prepaid.add('model/mm/342');
      const liveIds = new Set(retained.map(actor => actor.actorId));
      for (const id of liveIds)
        prepaid.add(`code/mm/${id}`);
      const retainedDependencies = new Set([...liveIds].flatMap(id => [...enemyNativeActorDependencies('mm', id)]));
      let credit = 0;
      const releasedCode = new Set<number>();
      for (const actor of original) {
        const fact = enemyNativeActorFact('mm', actor.actorId);
        if (!fact)
          throw Error('Unprofiled original MM enemy');
        credit += fact.instance;
        if (!retainedDependencies.has(actor.actorId) && !releasedCode.has(actor.actorId)) {
          releasedCode.add(actor.actorId);
          credit += fact.code;
        }
      }
      for (const root of original.filter(actor => removedRoots.has(actor.offset)))
        for (const [child, count] of SCRIPTED_NATIVE_CHILDREN[root.actorId] ?? []) {
          const fact = enemyNativeActorFact('mm', child);
          credit += count * fact.instance;
          if (!retainedDependencies.has(child) && !releasedCode.has(child)) {
            releasedCode.add(child);
            credit += fact.code;
          }
        }
      const budget = extra + credit + (setup === 0 ? objectSavings : 0);
      const dynamicCollisionAllowance = Math.max(0, p.dynamicCollisionLimit - p.dynamicCollisionReserve -
        retained.reduce((count, actor) => count + enemyNativeDynamicCollisionCount('mm', actor.actorId, actor.params), 0));
      const signature = JSON.stringify([indices, budget, retained.length, dynamicCollisionAllowance, sourceCosts, [...prepaid].sort(), extraCosts, dynamicSupplemental ? maps : []]);
      if (unique.has(signature))
        continue;
      unique.add(signature);
      states.push({ indices, prepaid, budget, actors: retained.length, dynamicCollisionAllowance, sourceCosts, supplemental: extraCosts, maps });
    }
  }
  const minimum = pools?.map((pool, i) => {
    const encounter = slots[i].encounter;
    if (encounter?.kind === 'scripted' && pool.some(target => enemyKeepsNativeEncounter('mm',
      enemyDefinition(encounter.source).actorId, enemyDefinition(encounter.source).spawnParams, { ...target, spawnParams: target.variables[0] })))
      return { shared: {}, instances: 0, actors: 0 };
    const costs = pool.map(target => enemyMemoryCost('mm', target.game, target.actorId, target.variables[0], memoryOptions));
    return { shared: {}, instances: Math.min(...costs.map(c => c.instances)), actors: Math.min(...costs.map(c => c.actors)) };
  });
  const coexisting = slots.map(() => new Set<number>());
  for (const state of states)
    for (const a of state.indices)
      for (const b of state.indices)
        coexisting[a].add(b);

  const targetIds = new Map<string, string>();
  const costSignatures = new WeakMap<EnemyMemoryCost, string>();
  const costSignature = (cost: EnemyMemoryCost) => {
    let key = costSignatures.get(cost);
    if (key === undefined) {
      key = JSON.stringify([Object.entries(cost.shared).sort(([a], [b]) => a.localeCompare(b)),
      cost.instances, cost.actors, cost.dynamicCollisions ?? 0,
      cost.preview ? [cost.preview.key, cost.preview.sharedBytes, cost.preview.instances] : null]);
      costSignatures.set(cost, key);
    }
    return key;
  };
  const targetCache = new WeakMap<MmEnemy, { game: string; id: number; params: number; key: string; cost: EnemyMemoryCost }>();
  const targetMemory = (target: MmEnemy) => {
    const params = target.variables[0], old = targetCache.get(target);
    if (old && old.game === target.game && old.id === target.actorId && old.params === params) return old;
    const cost = enemyMemoryCost('mm', target.game, target.actorId, params, memoryOptions);

    const identity = `${target.game}/${target.actorId}/${params}:${costSignature(cost)}`;
    let key = targetIds.get(identity);
    if (key === undefined) {

      const id = targetIds.size + 1;

      key = id < 0xffff ? String.fromCharCode(id) : '\uffff' + id + ';';
      targetIds.set(identity, key);
    }
    const value = {
      game: target.game, id: target.actorId, params,
      key, cost
    };
    targetCache.set(target, value);
    return value;
  };
  const scriptedSources = slots.map(slot => slot.encounter?.kind === 'scripted' ? enemyDefinition(slot.encounter.source) : undefined);

  type Scope = { key: string; states: State[]; indices: number[] };
  const fullScope: Scope = { key: '*', states, indices: [...new Set(states.flatMap(state => state.indices))] };
  const scopeCache = new Map<string, Scope>();
  let lastScopeSlots: number[] = [], lastScopeValue: Scope | undefined;
  const scopeFor = (affected?: ReadonlySet<number>): Scope => {
    if (!affected) return fullScope;
    if (lastScopeValue && affected.size === lastScopeSlots.length && lastScopeSlots.every(i => affected.has(i))) return lastScopeValue;
    lastScopeSlots = [...affected].sort((a, b) => a - b);
    const key = lastScopeSlots.join(',');
    let scope = scopeCache.get(key);
    if (!scope) {
      const subset = states.filter(state => state.indices.some(i => affected.has(i)));
      scope = { key, states: subset, indices: [...new Set(subset.flatMap(state => state.indices))] };
      if (scopeCache.size >= 256) scopeCache.delete(scopeCache.keys().next().value!);
      scopeCache.set(key, scope);
    }
    return lastScopeValue = scope;
  };
  type Total = ReturnType<ReturnType<typeof makeEnemyMemoryEvaluator>>;

  const prepared: (ReturnType<typeof targetMemory> | undefined)[] = [];
  const prepare = (selected: readonly (MmEnemy | undefined)[], scope: Scope) => {
    for (const i of scope.indices) prepared[i] = selected[i] ? targetMemory(selected[i]!) : undefined;
  };
  const stateEvaluators = new Map(states.map(state => [state, {
    evaluate: makeEnemyMemoryEvaluator(state.prepaid),
    cache: new Map<string, Total>(), keys: [] as string[], next: 0,
    lastKey: undefined as string | undefined, last: undefined as Total | undefined,
  }]));
  const evaluate = (selected: readonly (MmEnemy | undefined)[], stopEarly = false, affected?: ReadonlySet<number>, failures?: AdaptiveMemoryAssessment[], preparedScope?: Scope) => {
    let pressure: AdaptiveMemoryAssessment = { excess: 0, slots: [] };
    let worst: EnemySceneMemoryReport = {
      scene: scene.scene, grotto: scene.grotto, budget: extra + objectSavings, bytes: 0,
      headroom: p.gameplayReserve, fragmentation: p.fragmentationReserve, actors: 0, objectSavings, scenarios: states.length, resourceExcess: 0
    };
    const dynamicCosts = dynamicSupplemental ? mmSupplementalCosts(scene, dynamicSupplemental(), memoryOptions) : undefined;
    const scope = preparedScope ?? scopeFor(affected);
    if (!preparedScope) prepare(selected, scope);
    let measured = false;
    for (const state of scope.states) {
      const evaluator = stateEvaluators.get(state)!;
      let key = '';
      if (!dynamicCosts) for (const i of state.indices) key += prepared[i]?.key ?? '\0';
      let total = dynamicCosts ? undefined : key === evaluator.lastKey ? evaluator.last : evaluator.cache.get(key);
      if (!total) {
        const costs: EnemyMemoryCost[] = [];
        for (const i of state.indices) {
          const target = selected[i], source = scriptedSources[i];
          if (source && target && enemyKeepsNativeEncounter('mm', source.actorId, source.spawnParams, { ...target, spawnParams: target.variables[0] })) {
            const shared: Record<string, number> = {};
            let instances = 0, actors = 0;
            for (const [id, count] of [[source.actorId, 1] as const, ...(SCRIPTED_NATIVE_CHILDREN[source.actorId] ?? [])]) {
              const fact = enemyNativeActorFact('mm', id);
              shared[`code/mm/${id}`] = fact.code;
              instances += count * fact.instance;
              actors += count;
            }
            costs.push({ shared, instances, actors });
            continue;
          }
          if (target) costs.push(prepared[i]!.cost);
          else if (minimum) costs.push(minimum[i]);
          if (source && target) costs.push(enemyScriptedControllerCost());
        }
        costs.push(...state.sourceCosts);
        if (dynamicCosts) for (const mi of state.maps) costs.push(...dynamicCosts[mi]);
        else costs.push(...state.supplemental);
        total = evaluator.evaluate(costs);
        if (!dynamicCosts) {

          if (evaluator.keys.length === 256) evaluator.cache.delete(evaluator.keys[evaluator.next]);
          evaluator.keys[evaluator.next] = key;
          evaluator.next = (evaluator.next + 1) % 256;
          evaluator.cache.set(key, total);
        }
      }
      if (!dynamicCosts) {
        evaluator.lastKey = key;
        evaluator.last = total;
      }
      const { foreign, models } = total;
      const actors = state.actors + total.actors;

      if (stopEarly) {
        if (total.bytes > state.budget || actors > p.actorLimit || total.dynamicCollisions > state.dynamicCollisionAllowance || foreign > 30 || models > 80)
          return { report: worst, pressure: { excess: 1, slots: [] } };
        continue;
      }
      if (!measured || total.bytes - state.budget > worst.bytes - worst.budget)
        worst = { ...worst, bytes: total.bytes, budget: state.budget };
      measured = true;
      worst.actors = Math.max(worst.actors, actors);
      const excess = Math.max(0, (total.bytes - state.budget) / Math.max(1, state.budget), actors / p.actorLimit - 1, (total.dynamicCollisions - state.dynamicCollisionAllowance) / p.dynamicCollisionLimit, foreign / 30 - 1, models / 80 - 1);
      if (excess > 0) {
        const scope = { excess, slots: [...new Set(state.indices)], roomCount: new Set(state.maps.map(mi => scene.maps[mi].room)).size };
        failures?.push(scope);
        if (excess > pressure.excess) pressure = scope;
      }
    }
    worst.resourceExcess = pressure.excess;
    return { report: worst, pressure };
  };

  const fitCache = new Map<string, boolean>();
  const fitKeys: string[] = [];
  let nextFitKey = 0, lastResult: boolean | undefined, lastFitKey: string | undefined;
  const fits = (selected: readonly (MmEnemy | undefined)[], affected?: ReadonlySet<number>, fixedScope?: Scope) => {
    if (dynamicSupplemental) return evaluate(selected, true, affected).pressure.excess === 0;
    if (affected && !affected.size) return true;
    const scope = fixedScope ?? scopeFor(affected);
    let cacheKey = scope.key + ':';
    for (const i of scope.indices) {
      const target = selected[i];
      const row = prepared[i] = target ? targetMemory(target) : undefined;
      cacheKey += row?.key ?? '\0';
    }
    if (cacheKey === lastFitKey) return lastResult!;
    lastFitKey = cacheKey;
    const cached = fitCache.get(cacheKey);
    if (cached !== undefined) return lastResult = cached;
    const value = evaluate(selected, true, affected, undefined, scope).pressure.excess === 0;

    if (fitKeys.length === 4096) fitCache.delete(fitKeys[nextFitKey]);
    fitKeys[nextFitKey] = cacheKey;
    nextFitKey = (nextFitKey + 1) % 4096;
    fitCache.set(cacheKey, value);
    return lastResult = value;
  };
  const minibosses = pools && minibossAllocation(pools, target => ({ ...target, spawnParams: target.variables[0] }),
    slots.map(slot => slot.encounter
      ? slot.encounter.kind === 'gold' ? 1 : Math.max(1, new Set(scene.maps.flatMap(map =>
        mmEncounterRoots(scene, map, slot.encounter!).map(actor => actor.raw.join(',')))).size) * (slot.encounter.kind === 'keeta-guards' ? 2 : 1)
      : slot.copies ?? 1),
    selected => fits(selected),
    supplemental.filter(row => !variableSources.has(row.source) &&
      scene.maps.some(map => mmEncounterRoots(scene, map, row).length)).map(row => enemyDefinition(row.target)));
  const uniqueMinibosses = (selected: readonly (MmEnemy | undefined)[]) => !minibosses || minibosses.fits(selected);
  return {
    measure: (selected: readonly (MmEnemy | undefined)[]) => evaluate(selected).report,
    assess: (selected: readonly MmEnemy[]) => minibosses?.failures(selected, true)[0] ?? evaluate(selected).pressure,
    assessScopes: (selected: readonly MmEnemy[]) => {
      const failures: AdaptiveMemoryAssessment[] = minibosses?.failures(selected) ?? [];
      evaluate(selected, false, undefined, failures);
      return failures;
    },
    fitsScopes: (selected: readonly (MmEnemy | undefined)[], affected: ReadonlySet<number>) => uniqueMinibosses(selected) && fits(selected, affected),
    prepareFitsScopes: (affected: ReadonlySet<number>) => {
      const snapshot = new Set(affected), scope = scopeFor(snapshot);
      return (selected: readonly (MmEnemy | undefined)[]) => uniqueMinibosses(selected) && fits(selected, snapshot, scope);
    },
    varietyScopes: states.filter(state => state.maps.length === 1).map(state => state.indices),
    canShare: (first: number, second: number) => coexisting[first].has(second),
    fits: (selected: readonly (MmEnemy | undefined)[]) => uniqueMinibosses(selected) && fits(selected),
  };
}

function solveMmSceneIndividual(scene: MmScene, nativeRoster: readonly MmEnemy[], targets: readonly MmEnemy[], next: (max: number) => number, allows: MmTargetGroupAllows, ignoreRestrictions: boolean, weight: (target: EnemyShuffleIdentity, source: MmEnemy) => number, keepEmpty: (source: MmEnemy, actor: MmActor) => boolean, supplemental: EnemyPlan, memoryOptions: Settings, queue?: MmSceneProblem[]): MmActorChange[] {
  if (mmSceneSkipped(scene.scene))
    return [];
  type Slot = {
    actor: MmActor;
    maps: number[];
    source: MmEnemy;
  };
  const slots = new Map<number, Slot>();
  scene.maps.forEach((map, mi) => map.actors.forEach(actor => {
    const source = nativeRoster.find(e => e.actorId === actor.actorId && mmSourceFormAllowed(actor) && (e.objectId <= 3 || map.objects.some(o => o.objectId === e.objectId)));
    if (!source || !mmSpawnSourceAllowed(memoryOptions, scene, actor, source.key))
      return;
    const old = slots.get(actor.offset);
    if (old) {
      if (old.actor.actorId !== actor.actorId || old.actor.params !== actor.params)
        throw new Error('Conflicting MM physical actor aliases');
      old.maps.push(mi);
    }
    else
      slots.set(actor.offset, { actor, maps: [mi], source });
  }));
  if (!slots.size)
    return [];
  const sameSpawn = new Map<string, Slot[]>();
  for (const slot of slots.values()) {
    const key = `${slot.actor.raw.join(',')}/${mmSpawnIsMinibossEncounter(scene, slot.actor)}`;
    const entries = sameSpawn.get(key);
    if (entries)
      entries.push(slot);
    else
      sameSpawn.set(key, [slot]);
  }
  let rows = [...sameSpawn.values()].flatMap(entries => {
    const maps = entries.flatMap(entry => entry.maps);
    const rooms = new Set(maps.map(index => scene.maps[index].room));
    if (rooms.size > 1 && new Set(maps).size === maps.length)
      return [{ ...entries[0], maps, entries }];
    return entries.map(entry => ({ ...entry, entries: [entry] }));
  });
  const pools = rows.map(row => {
    const pool = targets.filter(target => (ignoreRestrictions || mmPlacementTypeAllows(row.source, target)) &&
      row.entries.every(entry => allows([entry.source], target, entry.actor)));
    if (!pool.length) {
      if (row.entries.some(entry => enemyCompatibilityRule(memoryOptions, mmSpawnLogicName(scene, entry.actor))))
        throw new Error(`No compatible weighted enemy for MM scene ${scene.scene}`);
      if (keepEmpty(row.source, row.actor))
        return [];
      throw new Error('MM individual scene ' + scene.scene + ': empty slot pool');
    }
    return mmWeightedOrder(pool, next, target => weight(target, row.source));
  });
  rows = rows.filter((_, index) => pools[index].length);
  const eligiblePools = pools.filter(pool => pool.length);
  const memory = mmSceneMemory(scene, rows, eligiblePools, supplemental, undefined, memoryOptions);
  const adaptivePools = eligiblePools.map((pool, i) => pool.map(value => {
    const target = { ...value, spawnParams: value.variables[0] };
    return {
      value, key: `${target.game}/${target.actorId}/${target.spawnParams}`, family: enemyWeightFamily(target),
      variety: enemyVarietyFamily(target), weight: weight(target, rows[i].source), maxGroupSize: enemyGroupLimit(target)
    };
  }));
  const positions = rows.map(row => row.entries.map(({ actor }) => {
    const signed = (offset: number) => ((actor.raw[offset] << 8 | actor.raw[offset + 1]) << 16) >> 16;
    return { x: signed(2), y: signed(4), z: signed(6) };
  }));
  const emit = (selected: MmEnemy[], groups: number[][], protectedSlots: number[]) => {
    const adaptiveGroups = new Map<number, string>();
    const protectedPicks = new Set(protectedSlots);
    for (const group of groups)
      if (group.length > 1)
        for (const index of group)
          adaptiveGroups.set(index, `mm:${scene.scene}:${group[0]}`);
    const actorChanges = rows.flatMap((row, i) => {
      const target = selected[i];
      const pick = next(target.variables.length);
      if (!Number.isInteger(pick) || pick < 0 || pick >= target.variables.length)
        throw new Error('Invalid MM random index');
      return row.entries.map(({ actor, maps }) => {
        const mapIndex = maps[0], map = scene.maps[mapIndex];
        return {
          mapIndex, room: map.room, setup: map.setup, slot: actor.slot, actorOffset: actor.offset,
          sourceActorId: actor.actorId, sourceParams: actor.params, targetKey: target.key,
          targetGame: target.game, targetActorId: target.actorId, targetParams: target.variables[pick],
          ...(adaptiveGroups.has(i) ? { adaptiveGroup: adaptiveGroups.get(i) } : {}),
          ...(protectedPicks.has(i) ? { protectedPick: true } : {})
        };
      });
    });
    return actorChanges;
  };
  if (queue) {
    const result: MmActorChange[] = [];
    queue.push({
      scene, rows, pools: adaptivePools, individual: true, positions,
      finish: (selected, groups, protectedSlots) => {
        result.push(...emit(selected, groups, protectedSlots));
      }
    });
    return result;
  }
  try {
    const result = solveAdaptiveMemory(adaptivePools, next, memory.fits, memory.assess, {
      positions, canShare: memory.canShare, assessScopes: memory.assessScopes, fitsScopes: memory.fitsScopes, prepareFitsScopes: memory.prepareFitsScopes, varietyScopes: memory.varietyScopes,
    });
    return emit(result.selected, result.groups, result.protectedSlots);
  }
  catch (error) {
    if (error instanceof EnemyMemoryAllocationError)
      throw mmAllocationError(`MM scene ${scene.scene}: ${error.message}`, scene, rows, eligiblePools, supplemental, memoryOptions);
    throw error;
  }
}

function splitMemoryCosts(host: Game, row: EnemyReplacement, options: EnemyMemoryOptions): EnemyMemoryCost[] {
  const children = row.splitChildren ?? [];
  const costs = children.map(key => {
    const target = enemyDefinition(key);
    return enemyMemoryCost(host, target.game, target.actorId, target.spawnParams,
      { ...options, enemyShuffleBari: false, enemyShuffleEeno: false });
  });
  if (!row.splitPool?.length) return costs;
  const total = enemyMemoryTotal(costs);
  return [{
    shared: Object.fromEntries(total.shared),
    instances: 3 * Math.max(0, ...costs.map(cost => cost.instances)),
    actors: 3 * Math.max(0, ...costs.map(cost => cost.actors)),
    dynamicCollisions: 3 * Math.max(0, ...costs.map(cost => cost.dynamicCollisions ?? 0)),
  }];
}

export function enemyOotSceneMemoryReports(plan: EnemyPlan, options: EnemyMemoryOptions = {},
  plannedSources: ReadonlySet<string> = new Set(plan.map(row => row.source))) {
  if (plan.some(row => row.splitChildren))
    options = { ...options, enemyShuffleBari: false, enemyShuffleEeno: false };
  const layouts = new Map(plan.flatMap(row => row.ootr ? [[row.ootr.location.scene, row.ootr.location.mq] as const] : []));
  for (const row of plan) {
    const gold = ENEMY_GOLD_SOURCES.find(s => s.game === 'oot' && s.key === row.source);
    if (gold && !layouts.has(gold.scene)) layouts.set(gold.scene, gold.slots[0].mq);
  }
  const extra = ootSupplementalMemoryRows(plan, options, scene => layouts.get(scene) ?? false);
  const scenes = new Set([...layouts.keys(), ...extra.map(r => r.scene)]);
  return [...scenes].flatMap(scene => {
    const rows = plan.filter(row => row.ootr?.location.scene === scene);
    const mq = layouts.get(scene) ?? false;
    const byRoom = new Map<number, EnemyMemoryCost[]>();
    const count = new Map<number, number>();
    const add = (room: number, cost: EnemyMemoryCost) => {
      const list = byRoom.get(room) ?? [];
      list.push(cost);
      byRoom.set(room, list);
    };
    for (const row of rows) {
      const choice = row.ootr!, target = enemyDefinition(row.target), room = choice.location.room;
      add(room, enemyMemoryCost('oot', target.game, target.actorId, choice.targetParams, options));
      splitMemoryCosts('oot', row, options).forEach(cost => add(room, cost));
      count.set(room, (count.get(room) ?? 0) + 1);
    }
    for (const row of extra) if (row.scene === scene) add(row.room, row.cost);
    for (const row of ootPreservedSpawnMemoryRows(plannedSources, options, scene, mq)) add(row.room, row.cost);
    const rooms = [...new Set([...ootMemoryRooms(scene, mq), ...byRoom.keys()])];
    const baseline = (room: number) => Math.max(0, (roomCounts as Readonly<Record<string, number>>)[`${scene}/${room}/${mq}`] ?? 32) - (count.get(room) ?? 0);
    return rooms.flatMap((a, i) => rooms.slice(i).map(b => {
      const total = enemyMemoryTotal([...(byRoom.get(a) ?? []), ...(a === b ? [] : byRoom.get(b) ?? [])]);
      return {
        scene, mq, rooms: a === b ? [a] : [a, b], bytes: total.bytes, budget: OOT_SELECTED_ENEMY_RESOURCE_ENVELOPE,
        actors: total.actors + Math.max(0, baseline(a)) + (a === b ? 0 : Math.max(0, baseline(b))), actorBudget: OOT_GAMEPLAY_POLICY.actorLimit - OOT_GAMEPLAY_POLICY.actorSlots,
        models: [...total.shared.keys()].filter(k => k.startsWith('model/')).length,
        foreign: [...total.shared.keys()].filter(k => k.startsWith('foreign/')).length,
        dynamicCollisions: total.dynamicCollisions,
        collisionBudget: Math.max(0, ENEMY_MEMORY_POLICY.dynamicCollisionLimit - ENEMY_MEMORY_POLICY.dynamicCollisionReserve -
          enemyOriginalRoomDynamicCollisions('oot', mq, scene, a) - (a === b ? 0 : enemyOriginalRoomDynamicCollisions('oot', mq, scene, b)))
      };
    }));
  });
}

export function enemySceneMemoryReports(plan: EnemyPlan, memoryOptions: EnemyMemoryOptions = {}): EnemySceneMemoryReport[] {
  if (plan.some(row => row.splitChildren))
    memoryOptions = { ...memoryOptions, enemyShuffleBari: false, enemyShuffleEeno: false };
  const reports: EnemySceneMemoryReport[] = [];
  for (const scene of MM_ENEMY_SCENES) {
    const placements = plan.filter(row => row.placement?.scene === scene.scene && row.placement.grotto === scene.grotto);
    if (!placements.length && !mmSupplementalCosts(scene, plan, memoryOptions).some(costs => costs.length))
      continue;
    const rows = placements.map(row => ({
      entries: scene.maps.flatMap((map, mi) => {
        const actor = map.actors.find(a => a.offset === row.placement!.actorOffset);
        return actor ? [{ actor, maps: [mi] }] : [];
      })
    }));
    const selected = placements.map(row => {
      const target = enemyDefinition(row.target);
      return {
        key: target.key, game: target.game,
        actorId: target.actorId, objectId: target.objectId, variables: [row.placement!.targetParams], type: 0, stationary: false
      };
    });
    reports.push(mmSceneMemory(scene, rows, undefined, plan, undefined, memoryOptions).measure(selected));
  }
  return reports;
}

type MmPlacement = MmActorChange & {
  scene: number;
  grotto?: number;
  roomStart: number;
  roomEnd: number;
  raw: number[];
  dryAquatic?: boolean;
};

type MmEnemyReplacement = EnemyReplacement & {
  placement: MmPlacement;
};

const MM_NATIVE_SOURCE_EXTENSIONS: readonly MmEnemy[] = [
  { key: 'CarnivorousLily', actorId: 0x13a, objectId: 0x161, variables: [0x0fff] },
  { key: 'DeathArmos', actorId: 0x2d, objectId: 0x1d, variables: [0xff] },
  { key: 'FlyingPot', actorId: 0x8d, objectId: 3, variables: [0x3fff] },
  { key: 'Skullfish', actorId: 0x180, objectId: 0x16b, variables: [0x192] },
  { key: 'Dexihand', actorId: 0x1d1, objectId: 0x1b5, variables: [0] },
  { key: 'Moths', actorId: 0x23d, objectId: 1, variables: [0x14] },
  { key: 'Takkuri', actorId: 0x291, objectId: 0x22, variables: [0] },
].map(row => ({ ...row, game: 'mm', ...enemySpawnActorType('mm', row.actorId) }));

const MM_NATIVE_ROSTER: readonly MmEnemy[] = [...applyEnemyMmSpawnPolicies(mmFacts.roster, ENEMY_SPAWN_MM_ACTORS), ...MM_NATIVE_SOURCE_EXTENSIONS]
  .map(row => ({ ...row, game: 'mm' as const, variables: row.variables.filter(params => enemySpawnMacroMatches('mm-native-roster-form', { target: { ...row, game: 'mm', params } })) }));

export const MM_NATIVE_SCENES: readonly MmScene[] = mmFacts.scenes;

export const MM_GENERIC_GROTTOS = [
  'Path to Snowhead', 'Ikana Valley', 'Zora Cape', 'Road to Ikana', 'Great Bay Coast',
  'Ikana Graveyard', 'Twin Islands', 'Termina Field Pillar', 'Mountain Village',
  'Woods of Mystery', 'Southern Swamp', 'Road to Southern Swamp', 'Termina Field Tall Grass',
].map((name, index) => ({ id: 0x13 + index, name }));

export const MM_ENEMY_SCENES: readonly MmScene[] = MM_NATIVE_SCENES.flatMap(scene => scene.scene !== 7 ? [scene] : [
  { ...scene, maps: scene.maps.filter(map => map.room !== 4) },
  ...MM_GENERIC_GROTTOS.map(grotto => ({ ...scene, grotto: grotto.id, maps: scene.maps.filter(map => map.room === 4) })),
]);

const MM_NATIVE_ENEMY_KEY_PREFIX = 'mm-placement-';

const MM_BIO_BABA_LILY_PAD_KEY = mmNativeRosterKey('biobaba-inert');

function mmNativeRosterKey(key: string): string {
  return MM_NATIVE_ENEMY_KEY_PREFIX + key.toLowerCase();
}

function mmEnemyDefinitions(base: readonly EnemyDefinition[]): EnemyDefinition[] {
  const originals = new Map<number, EnemyDefinition>();
  for (const definition of base) {
    if (definition.game !== 'mm')
      continue;
    const existing = originals.get(definition.actorId);
    if (!existing || Number(!!definition.foreignOnly) < Number(!!existing.foreignOnly) ||
      (!!definition.foreignOnly === !!existing.foreignOnly && definition.logicMacros.length > existing.logicMacros.length))
      originals.set(definition.actorId, definition);
  }
  const result = MM_NATIVE_ROSTER.map<EnemyDefinition>(row => {
    const original = originals.get(row.actorId);
    if (!original)
      throw new Error(`Missing MM enemy combat logic: ${row.key}`);
    return {
      ...original, key: mmNativeRosterKey(row.key), name: row.key, game: 'mm', actorId: row.actorId,
      objectId: row.objectId, params: row.variables, spawnParams: row.variables[0],
      soulMacro: original.soulMacro ?? `soul_${original.key.split('-')[1]}`,
      logicMacros: original.logicMacros.length ? original.logicMacros : [original.killMacro],
      nativeOnly: true, foreignOnly: false, targetAllowed: true,
      arenaOnly: false, preserveNativeResource: false, requiresNoCutscene: false, requiresNoScriptedPeers: false
    };
  });
  const bio = result.find(d => d.actorId === 0x12d)!;
  result.push({
    ...bio, key: MM_BIO_BABA_LILY_PAD_KEY, name: 'Bio Baba lily pad', params: [2], spawnParams: 2,
    killMacro: 'no_enemy', logicMacros: [],
    soulMacro: 'no_enemy', soulItem: undefined,
  });
  return result;
}

const OOT_TYPES = new Map(Object.values(ENEMY_SPAWN_ACTOR_TYPES).filter(row => row.game === 'oot')
  .map(row => [row.actorId, [row.type, row.stationary] as const]));

function mmForeignRoster(defs: readonly EnemyDefinition[]): MmEnemy[] {
  const result: MmEnemy[] = [];
  for (const def of defs) {
    if (def.game !== 'oot' || def.nativeOnly || def.targetAllowed === false || def.arenaOnly ||
      !foreignEnemyRuntimeReady('oot', def.actorId, def.spawnParams) ||
      nativeEnemyCounterparts('oot', def.actorId, 'mm', def.spawnParams).length)
      continue;
    const descriptor = OOT_TYPES.get(def.actorId);
    if (!descriptor)
      throw new Error(`Missing MM cross-pool descriptor: ${def.key}`);
    if (!ootObjectIds.includes(def.objectId))
      throw new Error(`Missing MM donor object size: ${def.key}`);
    const previous = result.find(r => r.actorId === def.actorId && r.objectId === def.objectId);
    if (previous) {
      if (!previous.variables.includes(def.spawnParams))
        previous.variables.push(def.spawnParams);
      continue;
    }
    if (!foreignActorIds.includes(def.actorId))
      throw new Error(`Missing MM foreign actor dependencies: ${def.key}`);
    const [type, stationary] = descriptor;
    result.push({
      key: def.key, game: 'oot', actorId: def.actorId, objectId: def.objectId,
      type, stationary, variables: [def.spawnParams],
    });
  }
  return result;
}

function mmNativeCounterpartRoster(defs: readonly EnemyDefinition[]): MmEnemy[] {
  return nativeCounterparts.map(({ actorId, objectId }) => {
    const key = actorId === 0x50 ? 'mm-skullwalltula-native-target' : 'mm-like-like';
    const def = defs.find(d => d.key === key && d.game === 'mm' && d.actorId === actorId && d.objectId === objectId && d.targetAllowed !== false);
    if (!def)
      throw new Error('Missing receiving-native MM counterpart definition: ' + key);
    return {
      key, game: 'mm', actorId, objectId, ...enemySpawnActorType('mm', actorId),
      variables: [def.spawnParams],
    };
  });
}

export function enemyMmCounterpartSlotAllowed(actorId: number, params: number, raw: readonly number[], targetActorId: number): boolean {
  return enemySpawnMacroMatches('mm-native-counterpart-slot', {
    source: { actorId, params }, target: { actorId: targetActorId },
    facts: { rawLength: raw.length, cutscene: ((raw[10] << 8) | raw[11]) & 0x7f,
      rawActorId: ((raw[0] << 8) | raw[1]) & 0xfff, rawParams: (raw[14] << 8) | raw[15] },
  });
}

function mmNativeCounterpartGroupAllows(scene: MmScene, source: readonly MmEnemy[], target: MmEnemy): boolean {
  if (!enemySpawnMacroMatches('mm-native-wall-target', { target }))
    return true;
  return scene.maps.some(map => map.actors.some(actor => source.some(old => old.actorId === actor.actorId) &&
    enemyMmCounterpartSlotAllowed(actor.actorId, actor.params, actor.raw, target.actorId)));
}

function mmGroupedNativeModelAllows(target: MmEnemy): boolean {
  return target.game !== 'mm' || target.objectId <= 3 || !!nativeEnemyOnDemandObjects('mm', target.actorId);
}

function makeMmEnemyPlan(defs: readonly EnemyDefinition[], settings: Settings, random: Random, supplemental: EnemyPlan, encounterRequests: readonly (readonly MmEncounterChoice[])[] = [], onProgress?: EnemyPlanProgress): EnemyPlan {
  if (!(settings.enemyShuffleMm || settings.enemyShuffleMmMinibossOnly) || settings.games === 'oot')
    return [];
  onProgress?.('Preparing MM enemy placements', 0, MM_ENEMY_SCENES.length);
  const scriptedDinolfosScenes = new Set(MM_NATIVE_SCENES
    .filter(scene => scene.maps.some(map => map.actors.some(actor => enemySpawnMacroMatches('mm-scripted-dinolfos-source', { source: actor, facts: { cutscene: ((actor.raw[10] << 8) | actor.raw[11]) & 0x7f } }))))
    .map(scene => scene.scene));
  const counterparts = mmNativeCounterpartRoster(defs);
  const wall = counterparts.find(row => row.actorId === 0x50)!;
  const likeLike = counterparts.find(row => row.actorId === 0x6c)!;
  const leever: MmEnemy = {
    key: 'mm-leever-native-target', game: 'mm', actorId: 0x216, objectId: 0x201,
    ...enemySpawnActorType('mm', 0x216), variables: [0],
  };
  const native = [
    ...MM_NATIVE_ROSTER.map(e => ({ ...e, key: mmNativeRosterKey(e.key) })),
    { ...wall, key: 'mm-skullwalltula-native-target', variables: [0xfffc] },
    leever,
    likeLike,
  ];
  const nativeByActor = new Map<number, MmEnemy>();
  for (const row of native) {
    if (!nativeByActor.has(row.actorId))
      nativeByActor.set(row.actorId, row);
  }
  const sourceKey = ({ sourceActorId, sourceParams }: MmActorChange) => sourceActorId === 0x12d && sourceParams === 2 ? MM_BIO_BABA_LILY_PAD_KEY : nativeByActor.get(sourceActorId)!.key;
  const placementTargetKey = ({ targetGame, targetActorId, targetParams, targetKey }: MmActorChange) => targetGame === 'mm' && targetActorId === 0x12d && targetParams === 2 ? MM_BIO_BABA_LILY_PAD_KEY : targetKey;
  const isDryAquatic = (game: Game, actorId: number, offset: number) => false && mmAquaticTarget(game, actorId) && !mmAquaticSlot(offset);
  const variants = MM_NATIVE_EXTRA_VARIANTS.map(def => {
    const descriptor = nativeByActor.get(def.actorId) ?? {
      game: 'mm' as const, actorId: def.actorId, objectId: def.objectId,
      ...enemySpawnActorType('mm', def.actorId),
    };
    return { ...descriptor, key: def.key, variables: [def.spawnParams] };
  });
  const nativeTargets: MmEnemy[] = native.filter(target => enemySpawnMacroMatches('mm-native-base-target', { target }))
    .map(row => ({ ...row, variables: row.variables.filter(params => enemySpawnMacroMatches('mm-target-form', { target: { ...row, params } })) }));
  for (const row of [...variants, ...counterparts]) {
    const existing = nativeTargets.find(target => target.actorId === row.actorId && target.objectId === row.objectId);
    if (existing) {
      for (const params of row.variables) {
        if (!existing.variables.includes(params))
          existing.variables.push(params);
      }
    }
    else {
      nativeTargets.push({ ...row, variables: [...row.variables] });
    }
  }
  {
    const arwing = ARWING_ENEMIES.mm;
    nativeTargets.push({
      key: arwing.key, game: arwing.game, actorId: arwing.actorId, objectId: arwing.objectId,
      ...enemySpawnActorType(arwing.game, arwing.actorId), variables: [0]
    });
  }
  const targets: readonly MmEnemy[] = [...nativeTargets, ...(settings.crossGameEnemies ? mmForeignRoster(defs) : [])]
    .flatMap(target => target.variables.filter(spawnParams => !enemyCanRespawn(target.game, target.actorId, spawnParams) &&
      !enemyIsPerched(target.game, target.actorId, spawnParams) &&
      (settings.enemyShuffleMinibosses || minibossOnly(settings, 'mm') || !isMinibossEnemy(target.game, target.actorId, spawnParams)) &&
      !enemyShuffleExcluded(settings, { ...target, spawnParams }, 'mm'))
      .map(spawnParams => ({
        ...target,
        key: target.game === 'mm' ? MM_NATIVE_EXTRA_VARIANTS.find(def => def.actorId === target.actorId && def.spawnParams === spawnParams)?.key ?? target.key : target.key,
        variables: [spawnParams]
      })));
  const isGraveyardExpandingTarget = (scene: number, game: Game, id: number, params: number) =>
    !enemySpawnMacroMatches('mm-graveyard-target', { location: { scene }, facts: { extraActors: foreignEnemyExtraActors(game, id, params) } });
  const forced = settings.plando.enemyTargets?.mm;
  if (forced) {
    const selected = defs.find(d => d.key === forced);
    if (!selected)
      throw new Error(`Unknown forced MM target: ${forced}`);
    const target = targets.find(t => t.game === selected.game && t.actorId === selected.actorId && t.variables.includes(selected.spawnParams));
    if (!target && enemyCanRespawn(selected.game, selected.actorId, selected.spawnParams))
      return [];
    if (!target)
      throw new Error(`Forced target outside MM pool: ${forced}`);
    const output: MmEnemyReplacement[] = [];
    for (const scene of MM_ENEMY_SCENES) {
      if (mmSceneSkipped(scene.scene))
        continue;
      if (isGraveyardExpandingTarget(scene.scene, selected.game, selected.actorId, selected.spawnParams))
        continue;
      if (!enemySpawnMacroMatches('mm-scripted-scene-target', { target: selected, facts: { scriptedDinolfos: scriptedDinolfosScenes.has(scene.scene) } }))
        continue;
      if (!enemySpawnMacroMatches('native-scene-keep', { target: selected, facts: { sceneKeep: (nativeSceneKeeps.scenes.mm as Record<number, number>)[scene.scene] } }))
        throw new Error('Forced flying pot requires receiving native dungeon keep');
      const objects = new Set(scene.maps.flatMap(map => map.objects.map(o => o.objectId)));
      const changes: MmActorChange[] = [];
      scene.maps.forEach((map, mapIndex) => map.actors.forEach(actor => {
        const source = native.find(e => e.actorId === actor.actorId && mmSourceFormAllowed(actor) && (e.objectId <= 3 || objects.has(e.objectId)));
        if (!source || !mmSpawnSourceAllowed(settings, scene, actor, source.key) || !mmSpawnTargetAllowed(scene, actor, selected, settings, source.key)) return;
        if (!source || ((settings.enemyShuffleMinibosses || minibossOnly(settings, 'mm')) && !minibossPlacementAllowed(settings, 'mm', actor.actorId, selected, mmSpawnIsMinibossEncounter(scene, actor))) || enemySpawnMacroMatches('mm-native-dinolfos-target', { target: selected }) && !enemyMmCounterpartSlotAllowed(actor.actorId, actor.params, actor.raw, selected.actorId) ||
          (!mmAquaticSpawnAllowed(actor, selected) ||
            (enemyIsCarnivorousLily(selected.game, selected.actorId) && !mmWaterSurfaceSlot(actor.offset))))
          return;
        changes.push({
          mapIndex, room: map.room, setup: map.setup, slot: actor.slot, actorOffset: actor.offset,
          sourceActorId: actor.actorId, sourceParams: actor.params, targetKey: selected.key,
          targetGame: selected.game, targetActorId: selected.actorId, targetParams: selected.spawnParams
        });
      }));
      if (enemySpawnMacroMatches('mm-native-wall-target', { target: selected }) && changes.some(c => !enemyMmCounterpartSlotAllowed(c.sourceActorId, c.sourceParams, scene.maps[c.mapIndex].actors.find(a => a.offset === c.actorOffset)!.raw, selected.actorId)))
        continue;
      const forcedRows = finalMmActorChanges(changes).map(change => ({
        entries: scene.maps.flatMap((map, mi) => {
          const actor = map.actors.find(a => a.offset === change.actorOffset);
          return actor ? [{ actor, maps: [mi] }] : [];
        })
      }));
      const forcedMemory = mmSceneMemory(scene, forcedRows, undefined, supplemental, undefined, settings);
      if (!forcedMemory.fits(forcedRows.map(() => target)))
        throw Error(`Forced MM target exceeds scene ${scene.scene} memory budget: ${forced}`);
      finalMmActorChanges(changes).forEach(change => {
        const map = scene.maps[change.mapIndex], actor = map.actors.find(a => a.offset === change.actorOffset)!;
        output.push({
          source: sourceKey(change),
          target: placementTargetKey(change),
          placement: {
            ...change, scene: scene.scene, ...(scene.grotto !== undefined ? { grotto: scene.grotto } : {}), roomStart: map.start, roomEnd: map.end, raw: actor.raw,
            ...(isDryAquatic(change.targetGame, change.targetActorId, actor.offset) ? { dryAquatic: true } : {})
          }
        });
      });
    }
    return output;
  }
  const output: MmEnemyReplacement[] = [];
  const individual = settings.enemyShuffleMmMode === 'individual';
  const next = (max: number) => random.next() % max;
  const memoryErrors: MmSceneAllocationError[] = [];
  const problems: MmSceneProblem[] = [], emitScenes: (() => void)[] = [];
  let completed = 0;
  for (const scene of MM_ENEMY_SCENES) {
    onProgress?.('Preparing MM enemy placements', completed++, MM_ENEMY_SCENES.length);
    try {
      const sceneIndividual = individual;
      const allows: MmTargetGroupAllows = (source, target, actor, members) => {
        const sourceSlots = () => actor ? [actor] : members ?? [];
        if (!sourceSlots().every(slot => mmSpawnTargetAllowed(scene, slot, { ...enemyDefinition(target.key), spawnParams: target.variables[0] }, settings,
          source.find(row => row.actorId === slot.actorId)?.key)))
          return false;
        if (actor && (settings.enemyShuffleMinibosses || minibossOnly(settings, 'mm')) &&
          !minibossPlacementAllowed(settings, 'mm', actor.actorId, target,
            mmSpawnIsMinibossEncounter(scene, actor)))
          return false;
        if (!enemySpawnMacroMatches('mm-scripted-scene-target', { target, facts: { scriptedDinolfos: scriptedDinolfosScenes.has(scene.scene) } }))
          return false;
        if (!sourceSlots().every(slot => mmAquaticSpawnAllowed(slot, { ...target, spawnParams: target.variables[0] })))
          return false;
        if (!enemySpawnMacroMatches('mm-water-surface-source', { target, settings,
          facts: { allWaterSurfaceSlots: sourceSlots().every(slot => !!mmWaterSurfaceSlot(slot.offset)) } }))
          return false;
        if (!enemySpawnMacroMatches('native-scene-keep', { target, facts: { sceneKeep: (nativeSceneKeeps.scenes.mm as Record<number, number>)[scene.scene] } }))
          return false;
        if (actor)
          return target.game !== 'mm' || enemyMmCounterpartSlotAllowed(actor.actorId, actor.params, actor.raw, target.actorId);
        return (sceneIndividual || mmGroupedNativeModelAllows(target)) && mmNativeCounterpartGroupAllows(scene, source, target);
      };
      const sceneTargets = targets.map(target => ({
        ...target, variables: target.variables.filter(params => !isGraveyardExpandingTarget(scene.scene, target.game, target.actorId, params)),
      })).filter(target => target.variables.length);
      const actorChanges = sceneIndividual
        ? solveMmSceneIndividual(scene, native, sceneTargets, next, allows, false, target => enemyWeight(settings, 'mm', target), (source, actor) => requiresMinibossReplacement(settings, 'mm', source.actorId, mmSpawnIsMinibossEncounter(scene, actor)), supplemental, settings, problems)
        : solveMmScene(scene, native, sceneTargets, next, allows, false, target => enemyWeight(settings, 'mm', target), supplemental, settings, problems);
      emitScenes.push(() => {
        for (const change of actorChanges)
          if (change.targetGame === 'oot' && change.targetActorId === 2) {
            const key = change.targetParams === 0 ? 'oot-stalfos-invisible-foreign' : 'oot-stalfos-foreign';
            const form = defs.find(d => d.key === key && d.game === 'oot' && d.actorId === 2 && d.spawnParams === change.targetParams && d.foreignOnly);
            if (!form)
              throw new Error('Missing selected Stalfos foreign form');
            change.targetKey = form.key;
          }
        for (const change of actorChanges) {
          if (change.targetGame === 'mm' && change.targetActorId === 0x3b && change.targetParams === 0xff03) {
            change.targetKey = 'mm-business-scrub-combat';
            continue;
          }
          if (change.targetGame !== 'mm' || change.targetActorId !== 0x15d)
            continue;
          const form = MM_NATIVE_EXTRA_VARIANTS.find(def => def.actorId === 0x15d && def.spawnParams === change.targetParams);
          if (!form)
            throw new Error('Missing standalone MM Wizzrobe form');
          change.targetKey = form.key;
        }
        const changes = finalMmActorChanges(actorChanges);
        changes.forEach(change => {
          const map = scene.maps[change.mapIndex], actor = map.actors.find(a => a.offset === change.actorOffset)!;
          const placement = {
            ...change, scene: scene.scene, ...(scene.grotto !== undefined ? { grotto: scene.grotto } : {}), roomStart: map.start, roomEnd: map.end, raw: actor.raw,
            dryAquatic: isDryAquatic(change.targetGame, change.targetActorId, actor.offset) || undefined
          };
          output.push({
            source: sourceKey(change),
            target: placementTargetKey(change),
            placement,
            ...(change.adaptiveGroup ? { adaptiveGroup: change.adaptiveGroup } : {}),
            ...(change.protectedPick ? { protectedPick: true } : {})
          });
        });
      });
    }
    catch (error) {
      if (!(error instanceof MmSceneAllocationError))
        throw error;
      memoryErrors.push(error);
    }
  }
  onProgress?.('Preparing MM enemy placements', completed, MM_ENEMY_SCENES.length);
  solveMmJointEncounters(problems, encounterRequests, supplemental, next, settings, onProgress);
  for (const emit of emitScenes)
    emit();
  if (memoryErrors.length)
    throw new MmSceneAllocationError(memoryErrors.map(e => e.message).join('; '), plan => memoryErrors.every(e => e.acceptsSupplemental(plan)));
  return output;
}

function solveMmJointEncounters(input: readonly MmSceneProblem[], requests: readonly (readonly MmEncounterChoice[])[], supplemental: EnemyPlan, next: (max: number) => number, options: EnemyMemoryOptions, onProgress?: EnemyPlanProgress): void {
  const problems = [...input];
  for (const scene of MM_ENEMY_SCENES)
    if (!problems.some(p => p.scene === scene) &&
      requests.some(pool => scene.maps.some(map => mmEncounterRoots(scene, map, pool[0].value).length)))
      problems.push({ scene, rows: [], pools: [], individual: true, positions: [], finish: () => { } });
  const uses = problems.map(problem => requests.flatMap((pool, index) => problem.scene.maps.some(map => mmEncounterRoots(problem.scene, map, pool[0].value).length) ? [index] : []));
  const remaining = new Set(problems.map((_, i) => i));
  let completed = 0;
  const report = (message: string) => onProgress?.(`Planning MM encounters: ${message}`, completed, problems.length);
  while (remaining.size) {
    onProgress?.('Planning MM encounters', completed, problems.length);
    const members = new Set([remaining.values().next().value!]), bindings = new Set<number>();
    let changed = true;
    while (changed) {
      changed = false;
      for (const i of members)
        for (const r of uses[i])
          bindings.add(r);
      for (const i of remaining)
        if (!members.has(i) && uses[i].some(r => bindings.has(r))) {
          members.add(i);
          changed = true;
        }
    }
    const pools: AdaptiveMemoryChoice<MmEnemy>[][] = [], positions: {
      x: number;
      y: number;
      z: number;
    }[][] = [];
    const owner: number[] = [], encounterIndices = new Map<number, number>();
    const contexts = [...members].map(i => {
      remaining.delete(i);
      const problem = problems[i];
      const indices = problem.pools.map((pool) => {
        const index = pools.length;
        pools.push([...pool]);
        owner.push(i);
        return index;
      });
      positions.push(...problem.positions.map(p => [...p]));
      return { i, problem, indices };
    });
    for (const r of bindings) {
      encounterIndices.set(r, pools.length);
      owner.push(-1);
      positions.push([]);
      pools.push(requests[r].map(choice => {
        const target = enemyDefinition(choice.value.target);
        const value: MmEnemy = {
          key: target.key, game: target.game, actorId: target.actorId, objectId: target.objectId,
          variables: [target.spawnParams], type: 0, stationary: false
        };
        return {
          value, key: `${target.game}/${target.actorId}/${target.spawnParams}`, family: enemyWeightFamily(target),
          variety: enemyVarietyFamily(target), weight: choice.familyWeight, maxGroupSize: enemyGroupLimit(target)
        };
      }));
    }
    const memories = contexts.map(({ i, problem, indices }) => {
      const localBindings = uses[i], global = [...indices, ...localBindings.map(r => encounterIndices.get(r)!)];
      const rows = [...problem.rows, ...localBindings.map(r => ({ entries: [], encounter: requests[r][0].value }))];
      const memory = mmSceneMemory(problem.scene, rows, global.map(g => pools[g].map(c => c.value)), supplemental, undefined, options);
      return { global, memory, selected: Array<MmEnemy | undefined>(global.length), affected: new Set<number>() };
    });
    const project = (selected: readonly (MmEnemy | undefined)[], context: typeof memories[number]) => {
      for (let i = 0; i < context.global.length; ++i) context.selected[i] = selected[context.global[i]];
      return context.selected;
    };
    const fits = (selected: readonly (MmEnemy | undefined)[]) => memories.every(context => context.memory.fits(project(selected, context)));

    let previousAffected: number[] = [];
    const fitsScopes = (selected: readonly (MmEnemy | undefined)[], affected: ReadonlySet<number>) => {
      if (affected.size !== previousAffected.length || !previousAffected.every(i => affected.has(i))) {
        previousAffected = [...affected];
        for (const context of memories) {
          context.affected.clear();
          context.global.forEach((g, i) => {
            if (affected.has(g)) context.affected.add(i);
          });
        }
      }
      return memories.every(context => !context.affected.size ||
        context.memory.fitsScopes(project(selected, context), context.affected));
    };
    const prepareFitsScopes = (affected: ReadonlySet<number>) => {
      const scoped = memories.flatMap(context => {
        const local = new Set(context.global.flatMap((g, i) => affected.has(g) ? [i] : []));
        return local.size ? [{ context, fits: context.memory.prepareFitsScopes(local) }] : [];
      });
      return (selected: readonly (MmEnemy | undefined)[]) => scoped.every(({ context, fits }) => fits(project(selected, context)));
    };
    const assessScopes = (selected: readonly MmEnemy[]) => memories.flatMap(({ global, memory }) => memory.assessScopes(global.map(g => selected[g])).map(scope => ({ ...scope, slots: scope.slots.map(i => global[i]) })));
    const coexisting = pools.map(() => new Set<number>());
    for (const { global, memory } of memories)
      for (let i = 0; i < global.length; i++)
        for (let j = 0; j < global.length; j++)
          if (memory.canShare(i, j))
            coexisting[global[i]].add(global[j]);
    const canShare = (a: number, b: number) => coexisting[a].has(b);
    let result: {
      selected: MmEnemy[];
      groups: number[][];
      protectedSlots: number[];
    };

    const solveFactored = (): typeof result => {
      if (contexts.length < 2 || !encounterIndices.size)
        throw new EnemyMemoryAllocationError('No independent MM encounter contexts');
      const bindings = [...encounterIndices.values()];
      const values: (MmEnemy | undefined)[] = Array(pools.length);
      const cache = contexts.map(() => new Map<string, typeof result | null>());
      let resolved: typeof result | undefined;
      const complete = () => {
        for (const context of memories)
          if (!context.memory.fits(project(values, context))) return false;
        if (bindings.some(g => !values[g])) return true;
        const selected = values.slice() as MmEnemy[];
        const groups: number[][] = bindings.map(g => [g]), protectedSlots: number[] = [];
        for (let c = 0; c < contexts.length; ++c) {
          const { problem, indices } = contexts[c], { global, memory } = memories[c];
          const count = indices.length;
          const key = global.slice(count).map(g => values[g]!.key).join(';');
          let local = cache[c].get(key);
          if (local === undefined) {
            const localPools = global.map((g, i) => i < count ? pools[g] : pools[g].filter(p => p.value.key === values[g]!.key));
            try {
              local = solveAdaptiveMemory(localPools, next, memory.fits, memory.assess, {
                onProgress: report,
                positions: [...problem.positions, ...global.slice(count).map(() => [])],
                canShare: memory.canShare,
                canGroup: (a, b) => a < count && b < count && memory.canShare(a, b),
                distinctFamilies: (a, b) => a < count && b < count && memory.canShare(a, b),
                assessScopes: memory.assessScopes, fitsScopes: memory.fitsScopes, prepareFitsScopes: memory.prepareFitsScopes,
                varietyScopes: memory.varietyScopes.map(scope => scope.filter(i => i < count)),
              });
            } catch (error) {
              if (!(error instanceof EnemyMemoryAllocationError)) throw error;
              local = null;
            }
            cache[c].set(key, local);
          }
          if (!local) return false;
          global.forEach((g, i) => {
            selected[g] = local!.selected[i];
          });
          groups.push(...local.groups.map(group => group.filter(i => i < count).map(i => global[i])).filter(group => group.length));
          protectedSlots.push(...local.protectedSlots.filter(i => i < count).map(i => global[i]));
        }
        if (!fits(selected)) throw Error('Factored MM encounter plan lost its memory invariant');
        resolved = { selected, groups, protectedSlots };
        return true;
      };
      for (let attempt = 0; attempt < 8; ++attempt) {
        values.fill(undefined);

        for (const records of cache) for (const [key, value] of records) if (value === null) records.delete(key);
        const order = bindings.map(g => ({ g, priority: next(0x1000000) })).sort((a, b) => a.priority - b.priority).map(x => x.g);
        const preferences = new Map(bindings.map(g => {
          const counts = new Map<string, number>();
          for (const c of pools[g]) counts.set(c.family, (counts.get(c.family) ?? 0) + 1);
          return [g, pools[g].map(c => ({ c, priority: -Math.log((next(0x1000000) + 1) / 0x1000001) / (c.weight / counts.get(c.family)!) }))
            .sort((a, b) => a.priority - b.priority).map(x => x.c.value)] as const;
        }));
        let nodes = 0;
        const search = (depth: number): boolean => {
          if (depth === order.length) return !!resolved;
          const g = order[depth];
          for (const value of preferences.get(g)!) {
            if (++nodes > 4096) break;
            values[g] = value;
            if (complete() && search(depth + 1)) return true;
          }
          values[g] = undefined;
          return false;
        };
        if (search(0)) return resolved!;
      }
      throw new EnemyMemoryAllocationError('No feasible factored MM encounter allocation');
    };
    if (contexts.every(c => c.problem.individual)) {
      const solveJoint = () => solveAdaptiveMemory(pools, next, fits, selected => assessScopes(selected).sort((a, b) => b.excess - a.excess)[0] ?? { excess: 0, slots: [] }, {
        onProgress: report,
        positions, canShare, canGroup: (a, b) => owner[a] >= 0 && owner[a] === owner[b] && canShare(a, b),
        distinctFamilies: (a, b) => owner[a] >= 0 && owner[b] >= 0 && canShare(a, b),
        jointSlots: [...encounterIndices.values()],
        assessScopes,
        fitsScopes, prepareFitsScopes,
        varietyScopes: memories.flatMap(({ global, memory }) => memory.varietyScopes.map(scope => scope.filter(i => owner[global[i]] >= 0).map(i => global[i]))),
      });
      if (contexts.length > 1 && encounterIndices.size) {
        try {
          result = solveFactored();
        }
        catch (error) {
          if (!(error instanceof EnemyMemoryAllocationError)) throw error;
          result = solveJoint();
        }
      } else result = solveJoint();
    }
    else {
      const weighted = pools.map(raw => {
        const pool = [...new Map(raw.map(c => [c.key, c])).values()];
        const counts = new Map<string, Set<string>>();
        for (const c of pool) {
          const forms = counts.get(c.family) ?? new Set<string>();
          forms.add(c.key);
          counts.set(c.family, forms);
        }
        return pool.map(c => ({ value: c.value, weight: c.weight / counts.get(c.family)!.size, cost: 0 }));
      });
      try {
        result = { selected: solveWeightedMemory(weighted, next, fits), groups: pools.map((_, i) => [i]), protectedSlots: [] };
      } catch (error) {
        if (error instanceof EnemyMemoryAllocationError)
          throw new EnemyMemoryAllocationError(`MM scenes ${contexts.map(context => context.problem.scene.scene).join(', ')}: ${error.message}`);
        throw error;
      }
    }
    if (!fits(result.selected))
      throw Error('Joint MM encounter plan lost its memory invariant');
    for (const [r, index] of encounterIndices) {
      const binding = supplemental.find(row => row.source === requests[r][0].value.source)!;
      binding.target = result.selected[index].key;
    }
    for (const { problem, indices } of contexts) {
      const reverse = new Map(indices.map((g, i) => [g, i]));
      problem.finish(indices.map(g => result.selected[g]), result.groups.map(group => group.flatMap(g => reverse.has(g) ? [reverse.get(g)!] : [])).filter(g => g.length), result.protectedSlots.flatMap(g => reverse.has(g) ? [reverse.get(g)!] : []));
    }
    completed += contexts.length;
    onProgress?.('Planning MM encounters', completed, problems.length);
  }
}

type SupplementalMemoryConstraint = (plan: EnemyPlan) => boolean;

class MmSceneAllocationError extends EnemyMemoryAllocationError {
  constructor(message: string, readonly acceptsSupplemental: SupplementalMemoryConstraint) {
    super(message);
  }
}

function mmAllocationError(message: string, scene: MmScene, rows: readonly MmMemorySlot[], pools: readonly (readonly MmEnemy[])[], supplemental: EnemyPlan, memoryOptions: EnemyMemoryOptions): MmSceneAllocationError {
  let current = supplemental;
  const memory = mmSceneMemory(scene, rows, pools, supplemental, () => current, memoryOptions);
  const unassigned = rows.map(() => undefined);
  return new MmSceneAllocationError(message, plan => {
    current = plan;
    return memory.fits(unassigned);
  });
}

export function enemyGoldTargetPool(settings: Settings, source: typeof ENEMY_GOLD_SOURCES[number]): EnemyDefinition[] {
  const locations = source.slots.map(slot => ({ ...slot, game: source.game, scene: source.scene }));
  if (!locations.every(where => enemySpawnSourceAllowed(settings, where, { ...where, key: source.key, encounterId: namedSpawnId(source.game, source.key) }))) return [];
  const id = namedSpawnId(source.game, source.key);
  if (!spawnEncounterAllows(id, settings, source.game)) return [];
  const targets = dynamicEnemyPool(settings, source.game, source.game === 'oot' ? 0x95 : 0x39, source.scene, spawnEncounter(id).minibossEncounter)
    .filter(target => enemyTargetCompatible(settings, source.key.replaceAll('-', '_'), target) && locations.every(where => enemySpawnTargetAllowed(where, { ...where, key: source.key, encounterId: namedSpawnId(source.game, source.key) }, target, settings)));
  if (!targets.length && enemyCompatibilityRule(settings, source.key.replaceAll('-', '_')))
    throw new Error('No compatible weighted enemy for ' + source.location);
  return targets;
}

function supplementalEnemyTargets(settings: Settings, binding: Pick<EnemyReplacement, 'source' | 'kind'>,
  targets: readonly EnemyDefinition[], mq: ReadonlySet<string>): EnemyDefinition[] {
  const source = enemyDefinition(binding.source);
  if (!spawnEncounterAllows(namedSpawnId(source.game, source.key), settings, source.game)) return [];
  const points: { location: EnemySpawnLocation; source: { key: string; actorId: number; params: number; habitat: EnemyDefinition['habitat'] } }[] = source.game === 'mm'
    ? MM_NATIVE_SCENES.flatMap(scene => scene.maps.flatMap(map => (binding.kind === 'keeta-guards' && scene.scene === 0x43
      ? map.actors.filter(actor => actor.actorId === 0x11b || actor.actorId === 0x28f)
      : mmEncounterRoots(scene, map, { ...binding, target: source.key }))
      .map(actor => ({
        location: { game: 'mm' as const, scene: scene.scene, room: map.room, setup: map.setup, slot: actor.slot },
        source: { key: source.key, actorId: source.actorId, habitat: source.habitat, params: binding.kind === 'keeta-guards' ? source.spawnParams : actor.params },
      }))))
    : (OOT_ENCOUNTER_MEMORY_ROOMS[source.key] ?? []).filter(([scene, room]) => {
      const dungeon = ['DT', 'DC', 'JJ', 'Forest', 'Fire', 'Water', 'Spirit', 'Shadow', 'BotW', 'IC'][scene];
      return (!dungeon || !mq.has(DUNGEONS[dungeon])) && nativeFightRestorationRoomAllowed(source.key, room, settings);
    }).map(([scene, room]) => ({
      /* These encounters are keyed by their controller, rather than a normal
       * shuffled actor-list slot. Keep their room selectors as well as the key. */
      location: { game: 'oot' as const, scene, room, setup: 0, mq: false },
      source: { key: source.key, actorId: source.actorId, habitat: source.habitat, params: source.spawnParams },
    }));
  if (!points.length)
    points.push({ location: { game: source.game }, source: { key: source.key, actorId: source.actorId, habitat: source.habitat, params: source.spawnParams } });
  if (!points.every(point => enemySpawnSourceAllowed(settings, point.location, { ...point.source, encounterId: namedSpawnId(point.location.game, point.source.key) }))) return [];
  const name = enemyPlacementLogicName({ ...binding, target: source.key });
  const eligible = targets.filter(target => enemyTargetCompatible(settings, name, target) && points.every(point => enemySpawnTargetAllowed(point.location, { ...point.source, encounterId: namedSpawnId(point.location.game, point.source.key) }, target, settings)));
  if (!eligible.length && enemyCompatibilityRule(settings, name))
    throw new Error('No compatible weighted enemy for ' + binding.source);
  return eligible;
}

function makeSupplementalEnemyPlan(defs: readonly EnemyDefinition[], settings: Settings, random: Random, constraints: readonly SupplementalMemoryConstraint[] = [], jointRequests?: MmEncounterChoice[][], mq: ReadonlySet<string> = new Set()): EnemyPlan {
  const output: EnemyPlan = [];
  const ootGoldPools = new Map<string, readonly EnemyDefinition[]>();
  const requests: MmEncounterChoice[][] = [];
  const choose = (rows: readonly EnemyDefinition[], host: Game, binding?: Pick<EnemyReplacement, 'source' | 'kind'>) => {
    const allowed = rows.filter(row => !enemyCanRespawn(row.game, row.actorId, row.spawnParams) &&
      !enemyShuffleExcluded(settings, row, host));
    if (!allowed.length)
      return undefined;
    const weights = enemyFormWeights(allowed, row => row, target => enemyWeight(settings, host, target));
    if (binding) {
      requests.push(allowed.map((target, i) => ({
        value: { ...binding, target: target.key }, weight: weights[i], familyWeight: enemyWeight(settings, host, target),
        cost: enemyMemoryTotal([enemyMemoryCost(host, target.game, target.actorId, target.spawnParams, settings)]).bytes
      })));
      return undefined;
    }
    return pickEnemyWeighted(allowed, weights, max => random.next() % max);
  };
  const enabled = (game: Game) => (settings.games === 'ootmm' || settings.games === game) &&
    (game === 'oot' ? settings.enemyShuffleOot || settings.enemyShuffleOotMinibossOnly : settings.enemyShuffleMm || settings.enemyShuffleMmMinibossOnly);
  if (enabled('oot')) {
    for (const source of ['oot-dead-hand-encounter', 'oot-dark-link-encounter']) {
      const targets = supplementalEnemyTargets(settings, { source, kind: 'arena' },
        dynamicEnemyPool(settings, 'oot', source === 'oot-dead-hand-encounter' ? 0xa4 : 0x33, OOT_ENCOUNTER_MEMORY_ROOMS[source].map(([scene]) => scene), spawnEncounter(namedSpawnId('oot', source)).minibossEncounter), mq);
      const target = choose(targets, 'oot');
      if (target)
        output.push({ source, target: target.key, kind: 'arena' });
    }
  }
  for (const source of NATIVE_FIGHT_RESTORATION_SOURCES) {
    if (!enabled(source.game) || !enemySourceAllowedBySettings(source.game, source.actorId, settings, source.params[0], spawnEncounter(namedSpawnId(source.game, source.key)).minibossEncounter) ||
      !nativeFightRestorationRoomAllowed(source.key, 2, settings))
      continue;
    const scenes = source.game === 'oot' ? OOT_ENCOUNTER_MEMORY_ROOMS[source.key].map(([scene]) => scene) :
      MM_NATIVE_SCENES.filter(scene => scene.maps.some(map => map.actors.some(actor =>
        actor.actorId === source.actorId && (source.params as readonly number[]).includes(actor.params)))).map(scene => scene.scene);
    let targets = supplementalEnemyTargets(settings, { source: source.key, kind: 'restore' },
      dynamicEnemyPool(settings, source.game, source.actorId, scenes, spawnEncounter(namedSpawnId(source.game, source.key)).minibossEncounter), mq);
    if (source.game === 'oot' && OOT_ENCOUNTER_MEMORY_ROOMS[source.key]
      .filter(([, room]) => nativeFightRestorationRoomAllowed(source.key, room, settings)).length > 1) {
      const ordinary = targets.filter(target => !isMinibossEnemy(target.game, target.actorId, target.spawnParams) &&
        enemyWeight(settings, 'oot', target) > 0);
      if (ordinary.length) targets = ordinary;
    }
    const target = choose(targets, source.game, source.game === 'mm' ? { source: source.key, kind: 'restore' } : undefined);
    if (target)
      output.push({ source: source.key, target: target.key, kind: 'restore' });
  }
  if (enabled('mm')) {
    const guardBinding = { source: KEETA_GUARDS.key, kind: 'keeta-guards' as const };
    const guard = choose(supplementalEnemyTargets(settings, guardBinding, dynamicEnemyPool(settings, 'mm', 0xed, 0x43, spawnEncounter(namedSpawnId('mm', KEETA_GUARDS.key)).minibossEncounter), mq), 'mm', guardBinding);
    if (guard)
      output.push({ source: KEETA_GUARDS.key, target: guard.key, kind: 'keeta-guards' });
    for (const source of SCRIPTED_MM_ENEMIES) {
      if (!enemySourceAllowedBySettings(source.game, source.actorId, settings, source.spawnParams, spawnEncounter(namedSpawnId(source.game, source.key)).minibossEncounter) ||
        !enemySpawnMacroMatches('mm-scripted-source', { source, settings }))
        continue;
      const scenes = MM_NATIVE_SCENES.filter(scene => scene.maps.some(map => map.actors.some(actor =>
        actor.actorId === source.actorId && enemyScriptedRootOffsets[scene.scene]?.includes(actor.offset)))).map(scene => scene.scene);
      const targets = supplementalEnemyTargets(settings, { source: source.key, kind: 'scripted' },
        dynamicEnemyPool(settings, 'mm', source.actorId, scenes, spawnEncounter(namedSpawnId('mm', source.key)).minibossEncounter).filter(enemy => scriptedMmTargetAllowed(enemy, settings.crossGameEnemies, source)), mq);
      const target = choose(targets, source.game, { source: source.key, kind: 'scripted' });
      if (target)
        output.push({ source: source.key, target: target.key, kind: 'scripted' });
    }
  }
  if (settings.enemyShuffleGoldSkulltulas) {
    for (const source of ENEMY_GOLD_SOURCES) {
      if (!enabled(source.game)) continue;
      if (source.game === 'oot' && source.scene <= 9) {
        const dungeon = ['DT', 'DC', 'JJ', 'Forest', 'Fire', 'Water', 'Spirit', 'Shadow', 'BotW', 'IC'][source.scene];
        if (source.slots[0].mq !== mq.has(DUNGEONS[dungeon])) continue;
      }
      const candidates = enemyGoldTargetPool(settings, source);
      const target = choose(candidates, source.game, source.game === 'mm' ? { source: source.key, kind: 'gold' } : undefined);
      if (target) {
        output.push({ source: source.key, target: target.key, kind: 'gold' });
        if (source.game === 'oot') ootGoldPools.set(source.key, candidates.filter(row =>
          !enemyCanRespawn(row.game, row.actorId, row.spawnParams) && !enemyShuffleExcluded(settings, row, 'oot')));
      }
    }
  }
  if (enabled('oot')) {
    for (const scene of [0x5c, 0x5e]) for (const sourceIsMiniboss of [false, true]) {
      const points = OOT_DESERT_LEEVER_SLOTS.filter(slot => slot.scene === scene).map(slot => ({
        slot,
        location: { game: 'oot' as const, scene, room: slot.room, mq: false },
        source: { key: slot.key, actorId: 0x1c, params: 0 },
      })).filter(point => {
        const id = namedSpawnId('oot', point.slot.key);
        return spawnEncounterAllows(id, settings, 'oot') && spawnEncounter(id).minibossEncounter === sourceIsMiniboss &&
          enemySpawnSourceAllowed(settings, point.location, { ...point.source, encounterId: namedSpawnId(point.location.game, point.source.key) });
      });
      if (!points.length) continue;
      const targets = dynamicEnemyPool(settings, 'oot', 0x1c, scene, sourceIsMiniboss);
      const grouped = settings.enemyShuffleOotMode === 'grouped';
      const groupedTargets = grouped ? targets.filter(target => points.every(point =>
        enemySpawnTargetAllowed(point.location, { ...point.source, encounterId: namedSpawnId(point.location.game, point.source.key) }, target, settings))) : [];
      const groupTarget = grouped ? choose(groupedTargets, 'oot') : undefined;
      for (const point of points) {
        const candidates = grouped ? groupedTargets : targets.filter(target =>
          enemySpawnTargetAllowed(point.location, { ...point.source, encounterId: namedSpawnId(point.location.game, point.source.key) }, target, settings));
        const target = grouped ? groupTarget : choose(candidates, 'oot');
        if (!target) continue;
        output.push({ source: point.slot.key, target: target.key, kind: 'desert-spawner' });
        ootGoldPools.set(point.slot.key, candidates);
      }
    }
  }
  repairOotGoldMemory(output, ootGoldPools, settings, random);
  if (jointRequests) {
    jointRequests.push(...requests);
    output.push(...requests.map(pool => pickEnemyWeighted(pool, pool.map(c => c.weight), max => random.next() % max)!.value));
    return output;
  }
  const fitsCache = new Map<string, boolean>();
  let current: EnemyPlan = output;
  const prototype = [...output, ...requests.map(pool => pool[0].value)];
  const scenes = MM_NATIVE_SCENES.filter(scene => mmSupplementalCosts(scene, prototype).some(rows => rows.length))
    .map(scene => mmSceneMemory(scene, [], undefined, prototype, () => current));
  const selected = solveWeightedMemory(requests, max => random.next() % max, selected => {
    const key = selected.map(row => row?.target ?? '').join('|');
    const cached = fitsCache.get(key);
    if (cached !== undefined)
      return cached;
    current = [...output, ...selected.filter((row): row is EnemyReplacement => row !== undefined)];
    const fits = scenes.every(scene => scene.fits([])) && constraints.every(accepts => accepts(current));
    fitsCache.set(key, fits);
    return fits;
  });
  output.push(...selected);
  return output;
}

export type EnemyDefinition = {
  key: string;
  name: string;
  game: Game;
  actorId: number;
  objectId: number;
  habitat: 'ground' | 'air' | 'water-surface' | 'underwater';
  targetAllowed?: boolean;
  nativeOnly?: boolean;
  foreignOnly?: boolean;
  preserveNativeResource?: boolean;
  arenaOnly?: boolean;
  sourceFormsIndependent?: boolean;
  requiresNoCutscene?: boolean;
  requiresNoScriptedPeers?: boolean;
  soulMacro?: string;
  soulItem?: string;
  miniboss?: boolean;
  params: readonly number[];
  spawnParams: number;
  killMacro: string;
  logicMacros: readonly string[];
};

export const ARWING_ENEMIES: Readonly<Record<Game, EnemyDefinition>> = {
  oot: {
    key: 'oot-arwing', name: 'Arwing', game: 'oot', actorId: 0x73, objectId: 1,
    habitat: 'air', nativeOnly: true, params: [], spawnParams: 0,
    killMacro: 'can_kill_arwing', logicMacros: [], soulMacro: 'soul_arwing', soulItem: 'SOUL_ENEMY_ARWING'
  },
  mm: {
    key: 'mm-arwing', name: 'Arwing', game: 'mm', actorId: 0x56, objectId: 1,
    habitat: 'air', nativeOnly: true, params: [], spawnParams: 0,
    killMacro: 'can_kill_arwing', logicMacros: [], soulMacro: 'soul_arwing', soulItem: 'SOUL_ENEMY_ARWING'
  },
};

const ENEMY_DEFINITIONS: readonly EnemyDefinition[] = [
  ...Object.values(ARWING_ENEMIES), ...FOREIGN_OOT_ROSTER_VARIANTS,
  { ...NATIVE_FLOOR_TILE_ENEMIES[0], key: 'oot-flying-floor-tile-foreign', nativeOnly: false, foreignOnly: true, logicMacros: [] },
  ...NATIVE_MM_ENEMIES, ...NATIVE_PEAHAT_TARGETS, ...NATIVE_RENEWABLE_BABA_TARGETS, ...NATIVE_MM_SKULLWALLTULA_TARGETS,
  { key: 'oot-stalchild-native-target', name: 'Stalchild', game: 'oot', actorId: 0x1b0, objectId: 0x184, habitat: 'ground', nativeOnly: true, params: [], spawnParams: 0, killMacro: 'can_kill_stalchildren', logicMacros: [], soulMacro: 'soul_stalchild' }, NATIVE_OOT_POE_TARGET, ...FOREIGN_MM_VARIANT_ENEMIES, ...FOREIGN_MM_EXTRA_VARIANT_ENEMIES, ...FOREIGN_BOE_ENEMIES, ...FOREIGN_EENO_ENEMIES, ...NATIVE_SKULLTULA_TARGET_VARIANTS, ...MM_NATIVE_EXTRA_VARIANTS, ...FOREIGN_EXTRA_NATIVE_MM_ENEMIES, MOTH_ENEMY, ...OOT_NATIVE_EXTRA_VARIANTS, MM_FOREIGN_DARK_LINK, MM_FOREIGN_DEAD_HAND, ...OOT_NATIVE_ENEMIES, ...SCRIPTED_MM_ENEMIES,
  ...NATIVE_ARENA_ENEMIES, ...NATIVE_SKULL_KID_ENEMIES, ...NATIVE_OOT_RESOURCE_ENEMIES, ...NATIVE_RESTORED_FIGHT_ENEMIES, ...NATIVE_WALLMASTER_TARGETS, ...NATIVE_FLOOR_TILE_ENEMIES,
  { key: 'mm-garo', name: 'Garo', game: 'mm', actorId: 0x113, objectId: 0x155, habitat: 'ground', params: [], spawnParams: 0, killMacro: 'can_kill_garo', logicMacros: [], soulMacro: 'soul_garo', foreignOnly: true },
  { key: 'oot-stalfos-foreign', foreignOnly: true, name: 'Stalfos', game: 'oot', actorId: 2, objectId: 0x32, habitat: 'ground', params: [], spawnParams: 3, killMacro: 'can_kill_stalfos', logicMacros: [], soulMacro: 'soul_stalfos' }, ...FOREIGN_STALFOS_VARIANTS,
  { key: 'oot-lizalfos-foreign', foreignOnly: true, name: 'Lizalfos', game: 'oot', actorId: 0x25, objectId: 0x1b, habitat: 'ground', params: [], spawnParams: 0xff80, killMacro: 'can_kill_lizalfos', logicMacros: [], soulMacro: 'soul_lizalfos' },
  { key: 'oot-club-moblin', foreignOnly: true, name: 'Club Moblin', game: 'oot', actorId: 0x4b, objectId: 0x30, habitat: 'ground', params: [], spawnParams: 2, killMacro: 'can_kill_moblins', logicMacros: [], soulMacro: 'soul_moblins' },
  { key: 'oot-skull-kid', foreignOnly: true, name: 'Skull Kid', game: 'oot', actorId: 0x115, objectId: 0x10a, habitat: 'ground', params: [], spawnParams: 3, killMacro: 'can_kill_skull_kids', logicMacros: [], soulMacro: 'soul_skull_kids' },
  { key: 'oot-anubis', foreignOnly: true, name: 'Anubis', game: 'oot', actorId: 0xf6, objectId: 1, habitat: 'ground', params: [], spawnParams: 3, killMacro: 'can_kill_anubis', logicMacros: [], soulItem: 'SOUL_ENEMY_ANUBIS' },
  { key: 'oot-flare-dancer', foreignOnly: true, name: 'Flare Dancer', game: 'oot', actorId: 0x99, objectId: 0x9e, habitat: 'ground', params: [], spawnParams: 0, killMacro: 'can_kill_flare_dancer', logicMacros: [], soulItem: 'SOUL_ENEMY_FLARE_DANCER', miniboss: true },
  { key: 'mm-bio-baba', foreignOnly: true, name: 'Bio Deku Baba', game: 'mm', actorId: 0x12d, objectId: 0x15e, habitat: 'ground', params: [], spawnParams: 0, killMacro: 'can_kill_bio_babas', logicMacros: [], soulItem: 'SOUL_ENEMY_BIO_BABA', requiresNoCutscene: true },
  { key: 'mm-chuchu-foreign', foreignOnly: true, name: 'Red ChuChu', game: 'mm', actorId: 0x14a, objectId: 0x16a, habitat: 'ground', params: [], spawnParams: 0xff03, killMacro: 'can_kill_chuchu', logicMacros: [], soulItem: 'SOUL_ENEMY_CHUCHU', requiresNoCutscene: true },
  { key: 'mm-dragonfly', name: 'Dragonfly', game: 'mm', actorId: 0x109, objectId: 0x14e, habitat: 'air', params: [0, 2, 3], spawnParams: 0, killMacro: 'can_kill_dragonflies_distance', logicMacros: ['can_kill_dragonflies', 'can_kill_dragonflies_distance'], soulItem: 'SOUL_ENEMY_DRAGONFLY', requiresNoCutscene: true },
  { key: 'mm-eeno', name: 'Small Eeno', game: 'mm', actorId: 0x1e6, objectId: 0x1c4, habitat: 'ground', params: [0, 0xff00], spawnParams: 0, killMacro: 'can_kill_eenos', logicMacros: ['can_kill_eenos'], soulMacro: 'soul_eenos', soulItem: 'SOUL_ENEMY_EENO', requiresNoCutscene: true },
  { key: 'mm-hiploop', name: 'Hiploop', game: 'mm', actorId: 0x1e9, objectId: 0x1c6, habitat: 'ground', params: [0, 0x0101], spawnParams: 0x0101, killMacro: 'can_kill_hiploops', logicMacros: ['can_kill_hiploops', 'can_kill_hiploops_masked'], soulItem: 'SOUL_ENEMY_HIPLOOP', requiresNoCutscene: true },
  { key: 'mm-snapper', name: 'Snapper', game: 'mm', actorId: 0x1ba, objectId: 0x1a6, habitat: 'ground', params: [0], spawnParams: 0, killMacro: 'can_kill_snappers_any', logicMacros: ['can_kill_snappers_any', 'can_kill_snappers_flowers'], soulItem: 'SOUL_ENEMY_SNAPPER', requiresNoCutscene: true },
  { key: 'mm-desbreko', name: 'Desbreko', game: 'mm', actorId: 0x14b, objectId: 0x16b, habitat: 'underwater', params: [0x0300, 0x0f00], spawnParams: 0x0300, killMacro: 'can_kill_skullfish', logicMacros: ['can_kill_skullfish'], soulMacro: 'soul_skull_fish', requiresNoCutscene: true },
  { key: 'mm-chuchu', nativeOnly: true, name: 'Red ChuChu', game: 'mm', actorId: 0x14a, objectId: 0x16a, habitat: 'ground', params: [0xff03, 0x0103, 0x0203], spawnParams: 0xff03, killMacro: 'can_kill_chuchu', logicMacros: ['can_kill_chuchu'], soulItem: 'SOUL_ENEMY_CHUCHU', requiresNoCutscene: true, targetAllowed: false },
  { key: 'mm-poe', nativeOnly: true, name: 'Poe', game: 'mm', actorId: 0x1f3, objectId: 0x1c3, habitat: 'air', params: [], spawnParams: 0x00ff, killMacro: 'can_kill_poes', logicMacros: [], soulMacro: 'soul_poe', requiresNoCutscene: true },
  { key: 'mm-stalchild', name: 'Stalchild', game: 'mm', actorId: 0xed, objectId: 0x142, habitat: 'ground', params: [], spawnParams: 0, killMacro: 'can_kill_stalchildren', logicMacros: [], soulMacro: 'soul_stalchild' },
  { key: 'oot-peahat', name: 'Adult Peahat', game: 'oot', actorId: 0x1d, objectId: 0x18, habitat: 'ground', params: [0xffff, 0], spawnParams: 0xffff, killMacro: 'can_kill_peahats_ground', logicMacros: ['can_kill_peahats_ground', 'can_kill_peahats_high'], soulMacro: 'soul_peahat', targetAllowed: false },
  { key: 'mm-peahat', name: 'Adult Peahat', game: 'mm', actorId: 0x14, objectId: 0x14, habitat: 'ground', params: [0], spawnParams: 0, killMacro: 'can_kill_peahats', logicMacros: ['can_kill_peahats'], soulMacro: 'soul_peahat', targetAllowed: false },
  { key: 'oot-lizalfos', name: 'Lizalfos', game: 'oot', actorId: 0x25, objectId: 0x1b, habitat: 'ground', params: [0xffff], spawnParams: 0xffff, killMacro: 'can_kill_lizalfos', logicMacros: ['can_kill_lizalfos'], soulMacro: 'soul_lizalfos' },
  { key: 'oot-dinolfos', name: 'Dinolfos', game: 'oot', actorId: 0x25, objectId: 0x1b, habitat: 'ground', params: [0xfffe], spawnParams: 0xfffe, killMacro: 'can_kill_dinolfos', logicMacros: ['can_kill_dinolfos'], soulMacro: 'soul_dinolfos' },
  { key: 'mm-dinolfos', name: 'Dinolfos', game: 'mm', actorId: 0x19, objectId: 0x17, habitat: 'ground', params: [0, 0xffff], spawnParams: 0, killMacro: 'can_kill_dinolfos', logicMacros: ['can_kill_dinolfos'], soulMacro: 'soul_dinolfos', requiresNoCutscene: true, requiresNoScriptedPeers: true },
  { key: 'oot-moblin', name: 'Spear Moblin', game: 'oot', actorId: 0x4b, objectId: 0x30, habitat: 'ground', params: [], spawnParams: 0xffff, killMacro: 'can_kill_moblins', logicMacros: [], soulMacro: 'soul_moblins' },
  { key: 'oot-octorok', name: 'Octorok', game: 'oot', actorId: 0x0e, objectId: 0x07, habitat: 'water-surface', params: Array.from({ length: 256 }, (_, shots) => shots << 8), spawnParams: 0, killMacro: 'can_kill_octoroks', logicMacros: ['can_kill_octoroks', 'can_kill_octoroks_distance'], soulMacro: 'soul_octorok' },
  { key: 'mm-octorok', name: 'Octorok', game: 'mm', actorId: 0x08, objectId: 0x05, habitat: 'water-surface', params: Array.from({ length: 256 }, (_, shots) => shots << 8), spawnParams: 0, killMacro: 'can_kill_octoroks', logicMacros: ['can_kill_octoroks'], soulMacro: 'soul_octorok' },
  { key: 'mm-skullfish', name: 'Skullfish', game: 'mm', actorId: 0x180, objectId: 0x16b, habitat: 'underwater', params: [0, ...Array.from({ length: 256 }, (_, range) => (range << 4) | 2)], spawnParams: 0x142, killMacro: 'can_kill_skullfish', logicMacros: ['can_kill_skullfish'], soulMacro: 'soul_skull_fish' },
  { key: 'oot-tailpasaran', name: 'Tailpasaran', game: 'oot', actorId: 0x35, objectId: 0x22, habitat: 'ground', params: [0xffff], spawnParams: 0xffff, killMacro: 'can_kill_tailpasarans', logicMacros: ['can_kill_tailpasarans'], soulMacro: 'soul_tailpasarans' },
  { key: 'oot-gohma-larva', name: 'Gohma Larva', game: 'oot', actorId: 0x2b, objectId: 0x1e, habitat: 'ground', params: [], spawnParams: 6, killMacro: 'can_kill_larvae_hatched', logicMacros: [], soulMacro: 'soul_gohma_larvae' },
  { key: 'oot-gohma-larva-hatched', name: 'Hatched Gohma Larva', game: 'oot', actorId: 0x2b, objectId: 0x1e, habitat: 'ground', params: [], spawnParams: 0x4006, killMacro: 'can_kill_larvae_hatched', logicMacros: [], soulMacro: 'soul_gohma_larvae' },
  { key: 'oot-ground-stinger', name: 'Ground Stinger', game: 'oot', actorId: 0x3a, objectId: 0x26, habitat: 'ground', params: [10], spawnParams: 10, killMacro: 'can_kill_stingers', logicMacros: ['can_kill_stingers'], soulMacro: 'soul_stingers' },
  { key: 'oot-water-stinger', name: 'Water Stinger', game: 'oot', actorId: 0x18c, objectId: 0x26, habitat: 'underwater', params: [0], spawnParams: 0, killMacro: 'can_kill_stingers_underwater', logicMacros: ['can_kill_stingers_underwater'], soulMacro: 'soul_stingers' },
  { key: 'oot-underwater-spike', name: 'Underwater Spike', game: 'oot', actorId: 0xec, objectId: 0xe5, habitat: 'underwater', params: [0], spawnParams: 0, killMacro: 'can_kill_spike_enemies_underwater', logicMacros: ['can_kill_spike_enemies', 'can_kill_spike_enemies_underwater'], soulMacro: 'soul_spike_enemy' },
  { key: 'oot-shell-blade', name: 'Shell Blade', game: 'oot', actorId: 0xc5, objectId: 0xb5, habitat: 'underwater', params: [0xffff], spawnParams: 0, killMacro: 'can_kill_shell_blades_underwater', logicMacros: ['can_kill_shell_blades', 'can_kill_shell_blades_underwater'], soulMacro: 'soul_shell_blade' },
  { key: 'mm-shell-blade', name: 'Shell Blade', game: 'mm', actorId: 0x64, objectId: 0x8e, habitat: 'underwater', params: [], spawnParams: 0, killMacro: 'can_kill_shell_blades_underwater', logicMacros: [], soulMacro: 'soul_shell_blade' },
  { key: 'mm-real-bombchu', name: 'Real Bombchu', game: 'mm', actorId: 0x16f, objectId: 0x181, habitat: 'ground', params: [0, 0xff, 0x05, 0x14, 0x28, 0x32, 0x3c, 0x46, 0x8c], spawnParams: 0x14, killMacro: 'can_kill_real_bombchu', logicMacros: ['can_kill_real_bombchu'], soulMacro: 'soul_real_bombchu' },
  { key: 'oot-bari', name: 'Bari', game: 'oot', actorId: 0x63, objectId: 0x56, habitat: 'air', params: [0, 0xffff], spawnParams: 0, killMacro: 'can_kill_bari', logicMacros: ['can_kill_bari'], soulMacro: 'soul_bari', soulItem: 'SOUL_ENEMY_BARI' },
  { key: 'oot-biri', name: 'Biri', game: 'oot', actorId: 0x34, objectId: 0x21, habitat: 'air', params: [0xffff], spawnParams: 0, killMacro: 'can_kill_biri', logicMacros: ['can_kill_biri'], soulMacro: 'soul_biri', soulItem: 'SOUL_ENEMY_BIRI' },
  { key: 'oot-bubble', name: 'Blue Bubble', game: 'oot', actorId: 0x69, objectId: 0x5d, habitat: 'air', params: [0x00ff, 0xffff], spawnParams: 0xffff, killMacro: 'can_kill_bubbles', logicMacros: ['can_kill_bubbles'] },
  { key: 'oot-freezard', name: 'Freezard', game: 'oot', actorId: 0x121, objectId: 0x114, habitat: 'ground', params: [0], spawnParams: 0, killMacro: 'can_kill_freezards', logicMacros: ['can_kill_freezards'] },
  { key: 'mm-freezard', name: 'Freezard', game: 'mm', actorId: 0x8f, objectId: 0xe4, habitat: 'ground', params: [0], spawnParams: 0, killMacro: 'can_kill_freezards', logicMacros: ['can_kill_freezards', 'can_kill_freezards_distance', 'can_kill_freezard_short_range'] },
  { key: 'oot-armos', name: 'Armos', game: 'oot', actorId: 0x54, objectId: 0x38, habitat: 'ground', params: [1], spawnParams: 1, killMacro: 'can_kill_armos', logicMacros: ['can_kill_armos'] },
  { key: 'oot-like-like', name: 'Like Like', game: 'oot', actorId: 0xdd, objectId: 0xd4, habitat: 'ground', params: [0], spawnParams: 0, killMacro: 'can_kill_like_like', logicMacros: ['can_kill_like_like'], soulMacro: 'soul_like_like' },
  { key: 'oot-redead', name: 'ReDead', game: 'oot', actorId: 0x90, objectId: 0x98, habitat: 'ground', params: [0x7f00, 0x7f01, 0x7f02], spawnParams: 0x7f01, killMacro: 'can_kill_redead_gibdo', logicMacros: ['can_kill_redead_gibdo', 'can_kill_redead_gibdo_no_c_button'], soulMacro: 'soul_redead_gibdo' },
  { key: 'oot-iron-knuckle', name: 'Iron Knuckle', game: 'oot', actorId: 0x113, objectId: 0x106, habitat: 'ground', params: [0xff01, 0xff02, 0xff03], spawnParams: 0xff02, killMacro: 'can_kill_iron_knuckles', logicMacros: ['can_kill_iron_knuckles'], soulMacro: 'soul_iron_knuckle' },
  { key: 'oot-floormaster', name: 'Floormaster', game: 'oot', actorId: 0x8e, objectId: 0x0b, habitat: 'ground', params: [0], spawnParams: 0, killMacro: 'can_kill_floormasters', logicMacros: ['can_kill_floormasters'] },
  { key: 'oot-beamos', name: 'Beamos', game: 'oot', actorId: 0x8a, objectId: 0x8b, habitat: 'ground', params: [0x0600], spawnParams: 0x0600, killMacro: 'can_kill_beamos', logicMacros: ['can_kill_beamos'] },
  { key: 'oot-stalfos', name: 'Stalfos', game: 'oot', actorId: 0x02, objectId: 0x32, habitat: 'ground', params: [0xffff], spawnParams: 0xffff, killMacro: 'can_kill_stalfos', logicMacros: ['can_kill_stalfos'] },
  { key: 'mm-armos', name: 'Armos', game: 'mm', actorId: 0x32, objectId: 0x30, habitat: 'ground', params: [0xffff], spawnParams: 0xffff, killMacro: 'can_kill_armos', logicMacros: ['can_kill_armos'] },
  { key: 'mm-like-like', name: 'Like Like', game: 'mm', actorId: 0x6c, objectId: 0xab, habitat: 'ground', params: [0], spawnParams: 0, killMacro: 'can_kill_like_like', logicMacros: ['can_kill_like_like'], soulMacro: 'soul_like_like' },
  { key: 'mm-redead', name: 'ReDead', game: 'mm', actorId: 0x4c, objectId: 0x75, habitat: 'ground', params: [0x7f05, 0x7f06, 0x7f07], spawnParams: 0x7f07, killMacro: 'can_kill_redead_gibdo', logicMacros: ['can_kill_redead_gibdo'], soulMacro: 'soul_redead_gibdo' },
  { key: 'mm-iron-knuckle', name: 'Iron Knuckle', game: 'mm', actorId: 0x84, objectId: 0xd8, habitat: 'ground', params: [0xff01, 0xff02, 0xff03], spawnParams: 0xff02, killMacro: 'can_kill_iron_knuckle', logicMacros: ['can_kill_iron_knuckle'], soulMacro: 'soul_iron_knuckle' },
  { key: 'mm-floormaster', name: 'Floormaster', game: 'mm', actorId: 0x4a, objectId: 0x09, habitat: 'ground', params: [0], spawnParams: 0, killMacro: 'can_kill_floormasters', logicMacros: ['can_kill_floormasters'] },
  { key: 'mm-beamos', name: 'Beamos', game: 'mm', actorId: 0x47, objectId: 0x6a, habitat: 'ground', params: [0x0300, 0x0500, 0x0600, 0x0800, 0xff00], spawnParams: 0x0600, killMacro: 'can_kill_beamos', logicMacros: ['can_kill_beamos'] },
  { key: 'mm-nejiron', name: 'Nejiron', game: 'mm', actorId: 0x155, objectId: 0x171, habitat: 'ground', params: [0], spawnParams: 0, killMacro: 'can_kill_nejirons', logicMacros: ['can_kill_nejirons'], soulMacro: 'soul_nejiron' },
  { key: 'mm-bee', name: 'Giant Bee', game: 'mm', actorId: 0x204, objectId: 0x1eb, habitat: 'air', params: [0], spawnParams: 0, killMacro: 'can_kill_wasp', logicMacros: ['can_kill_wasp'], soulMacro: 'soul_wasp', soulItem: 'SOUL_ENEMY_GIANT_BEE' },
  { key: 'oot-baby-dodongo', name: 'Baby Dodongo', game: 'oot', actorId: 0x2f, objectId: 0x1f, habitat: 'ground', params: [0, 0xffff], spawnParams: 0, killMacro: 'can_kill_baby_dodongos', logicMacros: ['can_kill_baby_dodongos'], soulMacro: 'soul_baby_dodongo' },
  { key: 'oot-torch-slug', name: 'Torch Slug', game: 'oot', actorId: 0x38, objectId: 0x25, habitat: 'ground', params: [0, 0xffff], spawnParams: 0, killMacro: 'can_kill_torch_slugs', logicMacros: ['can_kill_torch_slugs'], soulMacro: 'soul_torch_slug' },
  { key: 'mm-boe', name: 'Black Boe', game: 'mm', actorId: 0x164, objectId: 0x17a, habitat: 'ground', params: [0, 1, 0xff00, 0xff01], spawnParams: 0xff00, killMacro: 'can_kill_boes', logicMacros: ['can_kill_boes'], soulMacro: 'soul_boe' },
  { key: 'mm-white-boe', name: 'White Boe', game: 'mm', actorId: 0x164, objectId: 0x17a, habitat: 'ground', params: [], spawnParams: 0xff01, killMacro: 'can_kill_boes', logicMacros: [], soulMacro: 'soul_boe', requiresNoCutscene: true },
  { key: 'mm-bad-bat', name: 'Bad Bat', game: 'mm', actorId: 0x15b, objectId: 0x172, habitat: 'air', params: [0x0101, 0x0102, 0x0103, 0x0134, 0x019f], spawnParams: 0x0101, killMacro: 'can_kill_bad_bats_single', logicMacros: ['can_kill_bad_bats_single', 'can_kill_bad_bats_cluster'], soulMacro: 'soul_bad_bat', soulItem: 'SOUL_ENEMY_BAD_BAT', requiresNoCutscene: true },
  { key: 'oot-dodongo', name: 'Dodongo', game: 'oot', actorId: 0x12, objectId: 0x0c, habitat: 'ground', params: [0, 1], spawnParams: 0, killMacro: 'can_kill_dodongos', logicMacros: ['can_kill_dodongos'] },
  { key: 'oot-tektite', name: 'Red Tektite', game: 'oot', actorId: 0x1b, objectId: 0x16, habitat: 'ground', params: [0xffff], spawnParams: 0xffff, killMacro: 'can_kill_tektites', logicMacros: ['can_kill_tektites', 'can_kill_tektites_distance'] },
  { key: 'oot-wolfos', name: 'Wolfos', game: 'oot', actorId: 0x1af, objectId: 0x183, habitat: 'ground', params: [0xff00], spawnParams: 0xff00, killMacro: 'can_kill_wolfos', logicMacros: ['can_kill_wolfos'] },
  { key: 'oot-keese', name: 'Keese', game: 'oot', actorId: 0x13, objectId: 0x0d, habitat: 'air', params: [0, 1, 2, 3, 4], spawnParams: 2, killMacro: 'can_kill_keese', logicMacros: ['can_kill_keese'] },
  { key: 'oot-guay', nativeOnly: true, name: 'Guay', game: 'oot', actorId: 0x1c0, objectId: 0x08, habitat: 'air', params: [0], spawnParams: 0, killMacro: 'can_kill_guays', logicMacros: ['can_kill_guays'] },
  { key: 'mm-dodongo', name: 'Dodongo', game: 'mm', actorId: 0x0b, objectId: 0x0a, habitat: 'ground', params: [0, 1], spawnParams: 0, killMacro: 'can_kill_dodongos', logicMacros: ['can_kill_dodongos'] },
  { key: 'mm-tektite', name: 'Red Tektite', game: 'mm', actorId: 0x12, objectId: 0x12, habitat: 'ground', params: [0xffff, 0xfffd], spawnParams: 0xffff, killMacro: 'can_kill_tektites', logicMacros: ['can_kill_tektites'] },
  { key: 'mm-wolfos', name: 'Wolfos', game: 'mm', actorId: 0xec, objectId: 0x141, habitat: 'ground', params: [0, 0xff00, 0xff01], spawnParams: 0, killMacro: 'can_kill_wolfos', logicMacros: ['can_kill_wolfos'] },
  { key: 'mm-keese', name: 'Keese', game: 'mm', actorId: 0x0c, objectId: 0x0b, habitat: 'air', params: [0, 2, 3, 4, 0x8000, 0x8002, 0x8003, 0x8004], spawnParams: 2, killMacro: 'can_kill_keese_close', logicMacros: ['can_kill_keese_close', 'can_kill_keese_distance'] },
];

export const ENEMIES: readonly EnemyDefinition[] = [
  ...OOT_BURIAL_ENEMY_SLOTS.map(slot => ({
    key: slot.key, name: slot.scene === 8 ? 'Coffin Keese' : 'Grave Poe',
    game: 'oot' as const, actorId: slot.actorId, objectId: slot.objectId, habitat: 'air' as const,
    targetAllowed: false, params: [], spawnParams: slot.scene === 8 ? 2 : 1,
    killMacro: slot.scene === 8 ? 'can_kill_keese_close' : 'can_kill_poes', logicMacros: [],
  })),
  ...OOT_FOREST_BOW_SLOTS.map(slot => ({
    key: slot.key, name: 'Forest Bow Stalfos', game: 'oot' as const, actorId: 2, objectId: 0x32,
    habitat: 'ground' as const, targetAllowed: false, params: [], spawnParams: 3,
    killMacro: 'can_kill_stalfos', logicMacros: [], soulMacro: 'soul_stalfos',
  })),
  ...OOT_FIELD_STALCHILD_SLOTS.map(slot => ({
    key: slot.key, name: 'Hyrule Field Stalchild', game: 'oot' as const, actorId: 0x1b0, objectId: 0x184,
    habitat: 'ground' as const, targetAllowed: false, params: [], spawnParams: 0,
    killMacro: 'can_kill_stalchildren', logicMacros: [], soulMacro: 'soul_stalchild'
  })),
  ...OOT_DESERT_LEEVER_SLOTS.map(slot => ({
    key: slot.key, name: 'Leever spawner', game: 'oot' as const, actorId: 0xa7, objectId: 1,
    habitat: 'ground' as const, targetAllowed: false,
    params: [], spawnParams: 0, killMacro: 'can_kill_leevers', logicMacros: [], soulMacro: 'soul_leever',
  })),
  KEETA_GUARDS,
  ...ENEMY_GOLD_SOURCES.map(row => ({
    key: row.key, name: 'Gold Skulltula', game: row.game,
    actorId: row.game === 'oot' ? 0x95 : 0x50, objectId: row.game === 'oot' ? 0x24 : 0x20,
    habitat: 'ground' as const, nativeOnly: true, targetAllowed: false,
    params: [], spawnParams: 0, killMacro: 'can_kill_skulltulas', soulMacro: 'soul_gs', logicMacros: []
  })),
  ...ENEMY_DEFINITIONS.map(enemy => NATIVE_TARGET_FORM_OVERRIDES[enemy.key] ? { ...enemy, ...NATIVE_TARGET_FORM_OVERRIDES[enemy.key], nativeOnly: true } : enemy),
  ...mmEnemyDefinitions(ENEMY_DEFINITIONS),
  ...ootEnemizerDefinitions(ENEMY_DEFINITIONS),
].map(enemy => enemyNeverShuffled(enemy.game, enemy.actorId) ?
  { ...enemy, targetAllowed: false } : enemy);

type EnemyReplacement = {
  source: string;
  target: string;
  adaptiveGroup?: string;
  protectedPick?: boolean;
  kind?: 'arena' | 'variant' | 'restore' | 'scripted' | 'keeta-guards' | 'gold' | 'desert-spawner' | 'field-spawner' | 'forest-bow' | 'burial-spawner';
  placement?: MmPlacement;
  ootr?: OotEnemizerChoice;
  field?: OotEnemizerChoice;
  chaoticOotChoices?: readonly OotEnemizerChoice[];
  chaoticMmChoices?: readonly MmPlacement[];
  splitChildren?: readonly string[];
  splitPool?: readonly string[];
};

export type EnemyPlan = EnemyReplacement[];

export function enemyPlanEncounterId(row: EnemyPlan[number]): string {
  if (row.placement) {
    const p = row.placement;
    const scene = MM_ENEMY_SCENES.find(scene => scene.scene === p.scene && scene.grotto === p.grotto);
    const actor = scene?.maps[p.mapIndex]?.actors.find(actor => actor.offset === p.actorOffset);
    if (!scene || !actor) throw new Error('Missing MM encounter for planned actor ' + p.actorOffset);
    return mmSpawnRegistryId(scene, actor);
  }
  if (row.ootr) return ootSpawnRegistryId(row.ootr.location);
  return namedSpawnId(enemyDefinition(row.source).game, row.source);
}

export function enemyPlanVirtualFloor(row: EnemyPlan[number]): boolean {
  return spawnEncounter(enemyPlanEncounterId(row)).virtualFloor === true;
}

type EnemyShuffleIdentity = Pick<EnemyDefinition, 'key' | 'game' | 'actorId' | 'objectId' | 'spawnParams'>;

function enemyExclusionName(enemy: EnemyDefinition): string {
  const params = enemy.spawnParams;
  if ((enemy.game === 'oot' && enemy.actorId === 0x113) ||
    (enemy.game === 'mm' && enemy.actorId === 0x84)) {
    const armor = ['', 'Silver', 'Black', 'White'][params & 0x7f];
    if (armor)
      return `${armor} Iron Knuckle`;
  }
  if ((enemy.game === 'oot' && enemy.actorId === 0x90) ||
    (enemy.game === 'mm' && enemy.actorId === 0x4c)) {
    const forms: Record<number, string> = {
      0: 'ReDead (mourns when stationary)', 1: 'ReDead', 2: 'ReDead',
      3: 'Invisible ReDead', 4: 'Frozen ReDead',
      5: 'Dancing ReDead', 6: 'Dancing ReDead', 7: 'Dancing ReDead',
    };
    return forms[params & 0xff] ?? enemy.name;
  }
  if (enemy.game === 'oot') {
    if (enemy.actorId === 0x1b)
      return params === 0xfffe ? 'Blue Tektite' : 'Red Tektite';
    if (enemy.actorId === 0x8a)
      return (params & 0xff) === 1 ? 'Small Beamos' : 'Beamos';
    if (enemy.actorId === 0x1af && (params & 0xff) === 1)
      return 'White Wolfos';
    if (enemy.name === 'Deku Baba (Large)')
      return 'Large Deku Baba';
  }
  if (enemy.game === 'mm') {
    if (enemy.actorId === 0x12)
      return [0xfffd, 0xfffe].includes(params) ? 'Blue Tektite' : 'Red Tektite';
    if (enemy.actorId === 0x12d)
      return ['Bio Deku Baba', 'Leafless Bio Deku Baba', 'Bio Deku Baba Lily Pad', '', 'Walking Bio Deku Baba'][params & 0xff] || enemy.name;
    if (enemy.actorId === 0x14a)
      return ['Blue ChuChu', 'Green ChuChu', 'Yellow ChuChu', 'Red ChuChu'][params & 0xff] ?? enemy.name;
    if (enemy.actorId === 0x1e6)
      return params & 1 ? 'Large Eeno' : 'Small Eeno';
    if (enemy.actorId === 0x16f)
      return params & 0x8000 ? 'Respawning Real Bombchu' : 'Real Bombchu';
    if (enemy.actorId === 0x15b && enemy.name === 'Bad Bat swarm')
      return 'Bad Bat Swarm';
    if (enemy.actorId === 0x0f && enemy.name === 'RedBubble')
      return 'Red Bubble';
    if (enemy.actorId === 0x15d && [0, 0x100, 0x807f, 0x817f].includes(enemy.spawnParams))
      return enemy.spawnParams & 0x100 ? 'Ice Wizzrobe' : 'Fire Wizzrobe';
    if (enemy.actorId === 0x3b && [0xff00, 0xff02].includes(params))
      return 'Mad Scrub';
    if (enemy.actorId === 0x0b && enemy.name === 'Dodongo')
      return enemy.spawnParams === 0 ? 'Dodongo' : 'Large Dodongo';
    if (enemy.actorId === 0xec && enemy.name === 'Wolfos')
      return (enemy.spawnParams & 0x3f) === 0 ? 'Wolfos' : 'White Wolfos';
    if (enemy.actorId === 0x0c && ['Keese', 'Invisible Keese'].includes(enemy.name)) {
      const form = ['Fire Keese', undefined, 'Keese', 'Perching Keese', 'Ice Keese'][enemy.spawnParams & 0x7fff];
      if (form)
        return enemy.spawnParams & 0x8000 ? `Invisible ${form}` : form;
    }
  }
  return enemy.name === 'RedBubble' ? 'Red Bubble' : enemy.name;
}

function enemyExclusionIdentity(enemy: EnemyShuffleIdentity) {
  return [enemy.game, enemy.actorId, enemy.objectId, enemy.spawnParams].join('/');
}

function enemyWeightName(enemy: EnemyDefinition): string {
  const name = enemyExclusionName(enemy);
  const family: Record<string, string> = {
    'Hatched Gohma Larva': 'Gohma Larva', 'Big Green Bubble': 'Green Bubble', 'Big Skulltula': 'Skulltula',
    'Small Beamos': 'Beamos', 'Large Dodongo': 'Dodongo', 'Large Leever': 'Leever',
    'Large Stalchild': 'Stalchild', 'Mega Guay': 'Guay', 'Frozen ReDead': 'ReDead', 'Frozen Wallmaster': 'Wallmaster',
    'Red-Ice ReDead': 'ReDead', 'Dancing ReDead': 'ReDead', 'Red-Ice Wallmaster': 'Wallmaster',
    'ReDead (mourns when stationary)': 'ReDead', 'Standing ReDead': 'ReDead',
  };
  return family[name] ?? name;
}

function enemyExclusionGroup(enemy: EnemyDefinition) {
  const form = enemyExclusionForms.get(enemyExclusionIdentity(enemy)) ?? enemy;
  const name = enemyWeightName(form);
  if (['Green Bubble', 'Skulltula', 'Beamos', 'Dodongo', 'Leever', 'Stalchild', 'Guay', 'ReDead'].includes(name))
    return `shared-family/${name}`;
  return [form.game, form.actorId, form.objectId, name].join('/');
}

const enemyExclusionDefinitions = new Map(ENEMIES.map(enemy => [enemy.key, enemy]));

const enemyExclusionForms = new Map<string, EnemyDefinition>();

for (const enemy of ENEMIES) {
  if (enemy.targetAllowed === false || !enemySpawnMacroMatches('combat-definition', { target: enemy }))
    continue;
  const key = enemyExclusionIdentity(enemy);
  const existing = enemyExclusionForms.get(key);
  if (!existing || (existing.arenaOnly && !enemy.arenaOnly) ||
    (!enemy.arenaOnly && (existing.key.startsWith(MM_NATIVE_ENEMY_KEY_PREFIX) || existing.key.startsWith(OOT_NATIVE_ENEMY_KEY_PREFIX))))
    enemyExclusionForms.set(key, enemy);
}

const additionalMmForms: EnemyDefinition[] = [];

for (const row of MM_NATIVE_ROSTER) {
  const base = enemyExclusionDefinitions.get(mmNativeRosterKey(row.key))!;
  for (const spawnParams of row.variables) {
    if (!enemySpawnMacroMatches('active-combat-form', { target: { ...row, params: spawnParams } }))
      continue;
    const key = `${base.key}-${spawnParams.toString(16)}`;
    const form = { ...base, key, spawnParams, params: [] };
    const identity = enemyExclusionIdentity(form);
    const existing = enemyExclusionForms.get(identity);
    const native = ENEMIES.find(enemy => enemyExclusionIdentity(enemy) === identity &&
      !enemy.foreignOnly && enemy.targetAllowed !== false);
    if (native)
      continue;
    if (existing)
      form.name = existing.name;
    enemyExclusionDefinitions.set(key, form);
    if (!existing)
      enemyExclusionForms.set(identity, form);
    additionalMmForms.push(form);
  }
}

const enemyExclusionVariants = [...enemyExclusionForms.values()];

const enemyExclusionGroupByKey = new Map<string, string>();

const enemyExclusionKeysByGroup = new Map<string, string[]>();

for (const enemy of enemyExclusionDefinitions.values()) {
  const group = enemyExclusionGroup(enemy);
  enemyExclusionGroupByKey.set(enemy.key, group);
  const keys = enemyExclusionKeysByGroup.get(group) ?? [];
  keys.push(enemy.key);
  enemyExclusionKeysByGroup.set(group, keys);
}

export function enemyShuffleRoster(game: Game, crossGame: boolean) {
  const foreign = !crossGame ? [] : game === 'oot' ? ootForeignTargets(ENEMIES) :
    mmForeignRoster(ENEMIES).flatMap(row => row.variables.flatMap(params => {
      const enemy = ENEMIES.find(enemy => enemy.game === row.game && enemy.actorId === row.actorId &&
        enemy.objectId === row.objectId && enemy.spawnParams === params && !enemy.nativeOnly &&
        !enemy.arenaOnly && enemy.targetAllowed !== false);
      return enemy ? [enemy] : [];
    }));
  const groups = new Map<string, {
    key: string;
    name: string;
    donor: Game;
    miniboss: boolean;
    legacyKeys: string[];
  }>();
  for (const enemy of [...ENEMIES.filter(enemy => enemy.game === game && !enemy.foreignOnly &&
    enemy.targetAllowed !== false && enemySpawnMacroMatches('active-combat-form', { target: { ...enemy, params: enemy.spawnParams } })),
  ...(game === 'mm' ? additionalMmForms : []), ...foreign]) {
    if (enemyNeverShuffled(enemy.game, enemy.actorId) || enemyCanRespawn(enemy.game, enemy.actorId, enemy.spawnParams) || enemyIsPerched(enemy.game, enemy.actorId, enemy.spawnParams))
      continue;
    const group = enemyExclusionGroup(enemy);
    if (groups.has(group))
      continue;
    groups.set(group, {
      key: `${game}:${enemy.key}`,
      name: enemyWeightName(enemyExclusionForms.get(enemyExclusionIdentity(enemy)) ?? enemy),
      donor: enemy.game,
      miniboss: isMinibossEnemy(enemy.game, enemy.actorId, enemy.spawnParams),
      legacyKeys: [...(enemyExclusionKeysByGroup.get(group) ?? [])],
    });
  }
  return [...groups.values()];
}

export function enemyShuffleTargets(game: Game, crossGame: boolean): EnemyDefinition[] {
  const forms = new Map<string, EnemyDefinition>();
  for (const row of enemyShuffleRoster(game, crossGame)) {
    for (const key of row.legacyKeys) {
      const enemy = enemyDefinition(key);
      if (enemy.targetAllowed === false || enemy.arenaOnly || enemyNeverShuffled(enemy.game, enemy.actorId) ||
        enemyCanRespawn(enemy.game, enemy.actorId, enemy.spawnParams) || enemyIsPerched(enemy.game, enemy.actorId, enemy.spawnParams) ||
        !enemySpawnMacroMatches('active-combat-form', { target: { ...enemy, params: enemy.spawnParams } }) ||
        (enemy.game === game ? enemy.foreignOnly : enemy.nativeOnly || !foreignEnemyRuntimeReady(enemy.game, enemy.actorId, enemy.spawnParams) ||
          nativeEnemyCounterparts(enemy.game, enemy.actorId, game, enemy.spawnParams).length))
        continue;
      const identity = enemyExclusionIdentity(enemy);
      if (!forms.has(identity)) forms.set(identity, {
        ...enemy, name: enemy.key.includes('-red-ice-') ? enemy.name : enemyExclusionName(enemyExclusionForms.get(identity) ?? enemy),
      });
    }
  }
  return [...forms.values()];
}

export function defaultEnemyShuffleWeight(miniboss: boolean, name = ''): number {
  return name.startsWith('Invisible ') ? 0.5 : miniboss ? 0.35 : 1;
}

function enemyWeightFamily(target: EnemyShuffleIdentity): string {
  const identity = enemyExclusionIdentity(target);
  const form = enemyExclusionForms.get(identity);
  return form ? enemyExclusionGroup(form) : enemyExclusionGroupByKey.get(target.key) ?? identity;
}

let enemyVarietyGroups: Map<string, string> | undefined;

function enemyVarietyFamily(target: EnemyShuffleIdentity): string {
  if (!enemyVarietyGroups) {
    const parents = new Map<string, string>();
    const firstActorByFamily = new Map<string, string>();
    const root = (actor: string): string => {
      const parent = parents.get(actor);
      if (!parent || parent === actor) return actor;
      const representative = root(parent);
      parents.set(actor, representative);
      return representative;
    };
    for (const enemy of enemyExclusionVariants) {
      const actor = `${enemy.game}/${enemy.actorId}`, family = enemyWeightFamily(enemy);
      if (!parents.has(actor)) parents.set(actor, actor);
      const first = firstActorByFamily.get(family);
      if (first) parents.set(root(actor), root(first));
      else firstActorByFamily.set(family, actor);
    }
    enemyVarietyGroups = new Map([...parents.keys()].map(actor => [actor, root(actor)]));
  }
  const actor = `${target.game}/${target.actorId}`;
  return enemyVarietyGroups.get(actor) ?? actor;
}

type EnemyWeightGroup = {
  key: string;
  aliases: string[];
  defaultWeight: number;
};

let enemyWeightGroups: Map<string, EnemyWeightGroup> | undefined;

function getEnemyWeightGroups() {
  if (!enemyWeightGroups) {
    enemyWeightGroups = new Map();
    for (const host of ['oot', 'mm'] as const) {
      for (const row of enemyShuffleRoster(host, true)) {
        const family = enemyExclusionGroupByKey.get(row.key.slice(host.length + 1))!;
        enemyWeightGroups.set(`${host}:${family}`, {
          key: row.key,
          defaultWeight: defaultEnemyShuffleWeight(row.miniboss, row.name),
          aliases: row.legacyKeys.map(key => `${host}:${key}`).sort(),
        });
      }
    }
  }
  return enemyWeightGroups;
}

function configuredEnemyWeight(weights: Settings['enemyShuffleWeights'], group: EnemyWeightGroup) {
  if (weights[group.key] !== undefined)
    return weights[group.key];
  for (const key of group.aliases) {
    if (weights[key] !== undefined)
      return weights[key];
  }
  return undefined;
}

export function normalizeEnemyShuffleWeights(weights: Settings['enemyShuffleWeights']) {
  const normalized = { ...weights };
  for (const group of getEnemyWeightGroups().values()) {
    const weight = configuredEnemyWeight(weights, group);
    for (const key of group.aliases)
      delete normalized[key];
    if (weight !== undefined)
      normalized[group.key] = weight;
  }
  return normalized;
}

export function enemyWeight(settings: Settings, host: Game, target: EnemyShuffleIdentity): number {
  const group = getEnemyWeightGroups().get(`${host}:${enemyWeightFamily(target)}`);
  const weight = group && configuredEnemyWeight(settings.enemyShuffleWeights ?? {}, group);
  return weight ?? group?.defaultWeight ?? defaultEnemyShuffleWeight(isMinibossEnemy(target.game, target.actorId, target.spawnParams));
}

function enemyBaseFormIdentity(target: EnemyShuffleIdentity): string {
  const red = target.spawnParams === 0x7e04 && (target.game === 'oot' ? target.actorId === 0x90 : target.actorId === 0x4c) ||
    (target.game === 'oot' ? target.actorId === 0x11 && target.spawnParams === 0x7e80 : target.actorId === 0xa && target.spawnParams === 0x7e81);
  return enemyExclusionIdentity(red ? { ...target, spawnParams: target.spawnParams | 0x100 } : target);
}

export function enemyFormWeights<T>(pool: readonly T[], identity: (entry: T) => EnemyShuffleIdentity, weight: (target: EnemyShuffleIdentity) => number): number[] {
  const forms = new Map<string, Set<string>>();
  const variants = new Map<string, Set<string>>();
  const copies = new Map<string, number>();
  const ids = pool.map(identity);
  for (const target of ids) {
    const family = enemyWeightFamily(target);
    const form = enemyExclusionIdentity(target);
    const base = enemyBaseFormIdentity(target);
    const members = forms.get(family) ?? new Set<string>();
    members.add(base);
    forms.set(family, members);
    const colors = variants.get(base) ?? new Set<string>();
    colors.add(form);
    variants.set(base, colors);
    copies.set(form, (copies.get(form) ?? 0) + 1);
  }
  return ids.map(target => weight(target) / forms.get(enemyWeightFamily(target))!.size /
    variants.get(enemyBaseFormIdentity(target))!.size / copies.get(enemyExclusionIdentity(target))!);
}

function pickEnemyWeighted<T>(pool: readonly T[], weights: readonly number[], next: (max: number) => number): T {
  let roll = next(0x1000000) / 0x1000000 * weights.reduce((sum, weight) => sum + weight, 0);
  for (let i = 0; i < pool.length - 1; i++) {
    roll -= weights[i];
    if (roll < 0)
      return pool[i];
  }
  return pool[pool.length - 1];
}

function mmWeightedOrder(pool: readonly MmEnemy[], next: (max: number) => number, weight: (target: EnemyShuffleIdentity) => number): MmEnemy[] {
  const forms = pool.flatMap(row => row.variables.map(spawnParams => ({ row, spawnParams })));
  const weights = enemyFormWeights(forms, ({ row, spawnParams }) => ({ ...row, spawnParams }), weight);
  return forms.map(({ row, spawnParams }, index) => ({
    row: { ...row, variables: [spawnParams] },
    order: -Math.log((next(0x1000000) + 1) / 0x1000001) / weights[index],
  })).sort((a, b) => a.order - b.order).map(entry => entry.row);
}

export function enemyShuffleExcluded(settings: Settings, target: EnemyShuffleIdentity, host: Game): boolean {
  if (enemyNeverShuffled(target.game, target.actorId)) return true;
  const exclusions = settings.enemyShuffleExcluded;
  if (!exclusions?.length)
    return false;
  const identity = enemyExclusionIdentity(target);
  const targetForm = enemyExclusionForms.get(identity) ?? enemyExclusionVariants.find(enemy => enemy.game === target.game && enemy.actorId === target.actorId && enemy.objectId === target.objectId &&
    enemy.params.includes(target.spawnParams));
  if (!targetForm)
    return false;
  const group = enemyExclusionGroup(targetForm);
  return exclusions.some(key => {
    const scope = key.indexOf(':');
    if (scope >= 0 && key.slice(0, scope) !== host)
      return false;
    return enemyExclusionGroupByKey.get(key.slice(scope + 1)) === group;
  });
}

export function enemyDefinition(key: string): EnemyDefinition {
  const def = enemyExclusionDefinitions.get(key);
  if (!def)
    throw new Error(`Unknown enemy: ${key}`);
  return def;
}

export function enemyIsPerched(game: Game, actorId: number, params: number): boolean {
  return enemySpawnMacroMatches('perched', { target: { game, actorId, params } });
}

export function enemyRequiresWall(game: Game, actorId: number): boolean {
  return enemySpawnMacroMatches('wall-required', { target: { game, actorId } });
}

export function enemyIsElevatedSource(game: Game, actorId: number): boolean {
  return enemySpawnMacroMatches('elevated-source', { target: { game, actorId } });
}

export function enemyIsCeilingSource(game: Game, actorId: number): boolean {
  return enemySpawnMacroMatches('ceiling-source', { target: { game, actorId } });
}

export function enemyPreservesNativeHeight(game: Game, actorId: number): boolean {
  return enemySpawnMacroMatches('preserves-native-height', { target: { game, actorId } });
}

export function enemyRequiresGroundPlacement(game: Game, actorId: number): boolean {
  return enemySpawnMacroMatches('mandatory-ground', { target: { game, actorId } });
}

export function enemyCanRespawn(game: Game, actorId: number, params: number): boolean {
  return enemySpawnMacroMatches('respawning', { target: { game, actorId, params } });
}

export type EnemyPlanCompatibility = ReadonlyMap<string, (target: EnemyDefinition) => boolean>;
const enemyCompatibility = Symbol('enemyCompatibility');

function enemyCompatibilityRule(settings: Settings, name: string | undefined) {
  return settings.logic === 'allLocations' && name
    ? (settings as Settings & { [enemyCompatibility]?: EnemyPlanCompatibility })[enemyCompatibility]?.get(name)
    : undefined;
}

function enemyTargetCompatible(settings: Settings, name: string | undefined, target: EnemySpawnTarget): boolean {
  const rule = enemyCompatibilityRule(settings, name);
  if (!rule) return true;
  const key = (target as Partial<EnemyDefinition>).key;
  const definition = key && enemyDefinition(key).spawnParams === target.spawnParams ? enemyDefinition(key) :
    ENEMIES.find(enemy => enemy.game === target.game && enemy.actorId === target.actorId && enemy.spawnParams === target.spawnParams) ??
    ENEMIES.find(enemy => enemy.game === target.game && enemy.actorId === target.actorId);
  if (!definition) throw new Error('Missing enemy compatibility target');
  return rule({ ...definition, spawnParams: target.spawnParams });
}

export type EnemyPlanProgress = (message: string, completed?: number, total?: number) => void;

function planEnemySplits(plan: EnemyPlan, settings: Settings, random: Random): void {
  const roots = plan.filter(row => {
    if (!row.ootr && !row.placement) return false;
    const target = enemyDefinition(row.target);
    return enemySpawnMacroMatches('split-death-source', { settings,
      target: { ...target, params: row.ootr?.targetParams ?? row.placement?.targetParams ?? target.spawnParams } });
  });
  if (!roots.length) return;
  roots.forEach(row => { row.splitChildren = []; });
  const groups = new Map<string, EnemyReplacement[]>();
  for (const row of roots) {
    const host = row.ootr ? 'oot' : 'mm';
    const scene = row.ootr?.location.scene ?? row.placement!.scene;
    const key = host + '/' + scene + '/' + (row.placement?.grotto ?? '');
    const rows = groups.get(key) ?? [];
    rows.push(row);
    groups.set(key, rows);
  }
  for (const [key, rows] of groups) {
    const [hostText, sceneText, grottoText] = key.split('/');
    const grotto = grottoText ? +grottoText : undefined;
    const host = hostText as Game, scene = +sceneText;
    const scenePlan = plan.filter(row => host === 'oot'
      ? row.ootr?.location.scene === scene || (!row.ootr && !row.placement)
      : row.placement?.scene === scene && row.placement.grotto === grotto || !!row.kind);
    const mmScene = host === 'mm' ? MM_ENEMY_SCENES.find(s => s.scene === scene && s.grotto === grotto) : undefined;
    const pools = rows.flatMap(row => {
      const sourceId = row.ootr?.location.actorId ?? row.placement!.sourceActorId;
      let candidates: EnemyDefinition[];
      if (row.ootr) {
        candidates = (ootEnemizerCandidatePools(ENEMIES, settings, [row.ootr.location]).get(row.ootr.location) ?? [])
          .map(candidate => candidate.def);
      } else {
        const sourceActor = mmScene!.maps[row.placement!.mapIndex].actors.find(actor => actor.offset === row.placement!.actorOffset)!;
        const locations = mmSpawnLocations(mmScene!, sourceActor);
        candidates = dynamicEnemyPool(settings, host, sourceId, scene, mmSpawnIsMinibossEncounter(mmScene!, sourceActor)).filter(target => enemyTargetCompatible(settings, enemyPlacementLogicName(row), target) && locations.every(location =>
          enemySpawnTargetAllowed(location, { ...sourceActor, key: row.source, encounterId: mmSpawnRegistryId(mmScene!, sourceActor) }, target, settings)));
      }
      const unique = [...new Map(candidates.map(enemy => [`${enemy.game}/${enemy.actorId}/${enemy.spawnParams}`, enemy])).values()];
      const choices = unique.map(value => ({
        value, key: `${value.game}/${value.actorId}/${value.spawnParams}`, family: enemyWeightFamily(value),
        weight: enemyWeight(settings, host, value), maxGroupSize: enemyGroupLimit(value),
      }));
      return [choices, choices, choices];
    });
    const apply = (selected: readonly (EnemyDefinition | undefined)[]) => rows.forEach((row, i) => {
      row.splitChildren = selected.slice(i * 3, i * 3 + 3).flatMap(enemy => enemy ? [enemy.key] : []);

    });
    const assess = (selected: readonly (EnemyDefinition | undefined)[]): AdaptiveMemoryAssessment => {
      apply(selected);
      const excess = host === 'oot'
        ? Math.max(0, ...enemyOotSceneMemoryReports(scenePlan, settings).filter(report => report.scene === scene).map(ootResourceExcess))
        : Math.max(0, ...enemySceneMemoryReports(scenePlan, settings).filter(report => report.scene === scene)
          .map(report => report.resourceExcess));
      return { excess, slots: pools.map((_, i) => i) };
    };
    const fixed = host === 'oot'
      ? [...scenePlan.flatMap(row => row.ootr ? [enemyDefinition(row.target)] : []),
        ...new Map(ootSupplementalMemoryRows(scenePlan, settings, () => !!rows[0].ootr?.location.mq)
          .filter(row => row.scene === scene).map(row => [row.source, row.target])).values()]
      : scenePlan.filter(row => row.placement?.scene === scene && row.placement.grotto === grotto ||
        mmScene?.maps.some(map => mmEncounterRoots(mmScene, map, row).length))
        .map(row => enemyDefinition(row.target));
    const minibosses = minibossAllocation(pools.map(pool => pool.map(c => c.value)), value => value,
      pools.map(() => 1), selected => assess(selected).excess <= 0, fixed);
    const selected = solveAdaptiveMemory<EnemyDefinition>(pools, max => random.next() % max,
      selected => minibosses.fits(selected) && assess(selected).excess <= 0,
      selected => minibosses.failures(selected, true)[0] ?? assess(selected)).selected;
    apply(selected);
  }
}

export function enemySpawnInventory(): SpawnInventoryRow[] {
  const rows = new Map<string, SpawnInventoryRow>();
  const add = (id: string, game: Game, actorId: number, sourceKey: string | undefined,
    points: readonly EnemySpawnLocation[], sharedController = false, grotto?: number) => {
    const locations = [...new Set(points.map(point => spawnPointId(point, grotto)))].sort();
    const row: SpawnInventoryRow = { id, game, actorId, ...(sourceKey === undefined ? {} : { sourceKey }), locations, sharedController };
    const previous = rows.get(id);
    if (previous && JSON.stringify(previous) !== JSON.stringify(row))
      throw new Error('Conflicting physical enemy spawn inventory: ' + id);
    rows.set(id, row);
  };
  const ootLocations: readonly OotEnemizerLocation[] = [...OOT_ENEMIZER.locations, ...ootFieldStalchildLocations(), ...ootForestBowLocations(), ...ootBurialEnemyLocations(new Set())];
  for (const location of ootLocations) {
    const points = ootSpawnLocations(location);
    const source = enemyNeverShuffled('oot', location.actorId) && location.fieldSlot === undefined ?
      { actorId: location.actorId, key: undefined } : ootSpawnSource(ENEMIES, location);
    add(ootSpawnRegistryId(location), 'oot', location.actorId, source.key, points);
  }
  const mmRoster = new Map<number, string>();
  for (const source of MM_NATIVE_ROSTER) if (!mmRoster.has(source.actorId))
    mmRoster.set(source.actorId, mmNativeRosterKey(source.key));
  for (const source of mmNativeCounterpartRoster(ENEMIES)) if (!mmRoster.has(source.actorId))
    mmRoster.set(source.actorId, source.actorId === 0x50 ? 'mm-skullwalltula-native-target' : source.key);
  if (!mmRoster.has(0x216)) mmRoster.set(0x216, 'mm-leever-native-target');
  for (const scene of MM_ENEMY_SCENES) {
    const seen = new Set<number>();
    for (const map of scene.maps) for (const actor of map.actors) {
      const key = mmRoster.get(actor.actorId);
      if (key === undefined || seen.has(actor.offset)) continue;
      seen.add(actor.offset);
      const points = mmSpawnLocations(scene, actor), source = mmSpawnSource(scene, actor, key);
      add(mmSpawnRegistryId(scene, actor), 'mm', actor.actorId, source.key, points, false, scene.grotto);
    }
  }
  const named = (source: EnemyDefinition, kind: EnemyReplacement['kind']) => {
    const points: EnemySpawnLocation[] = source.game === 'oot' ? (OOT_ENCOUNTER_MEMORY_ROOMS[source.key] ?? []).map(([scene, room]) =>
      ({ game: 'oot', scene, room, setup: 0, mq: false })) : MM_NATIVE_SCENES.flatMap(scene => scene.maps.flatMap(map =>
      mmEncounterRoots(scene, map, { source: source.key, target: source.key, kind }).map(actor =>
        ({ game: 'mm' as const, scene: scene.scene, room: map.room, setup: map.setup, slot: actor.slot }))));
    if (!points.length) points.push({ game: source.game });
    add(namedSpawnId(source.game, source.key), source.game, source.actorId, source.key, points,
      kind === 'arena' || kind === 'restore' || kind === 'scripted' || kind === 'keeta-guards');
  };
  for (const key of ['oot-dead-hand-encounter', 'oot-dark-link-encounter']) named(enemyDefinition(key), 'arena');
  for (const source of NATIVE_FIGHT_RESTORATION_SOURCES) named(enemyDefinition(source.key), 'restore');
  for (const source of SCRIPTED_MM_ENEMIES) named(source, 'scripted');
  named(KEETA_GUARDS, 'keeta-guards');
  for (const source of ENEMY_GOLD_SOURCES) {
    const actorId = source.game === 'oot' ? 0x95 : 0x39;
    const points = source.slots.map(slot => ({ ...slot, game: source.game, scene: source.scene }));
    add(namedSpawnId(source.game, source.key), source.game, actorId, source.key, points);
  }
  for (const slot of OOT_DESERT_LEEVER_SLOTS) {
    const points: EnemySpawnLocation[] = [{ game: 'oot', scene: slot.scene, room: slot.room, mq: false }];
    add(namedSpawnId('oot', slot.key), 'oot', 0x1c, slot.key, points);
  }
  return [...rows.values()].sort((a, b) => a.id.localeCompare(b.id));
}

let spawnRegistryValidated = false;

export function makeEnemyPlan(settings: Settings, random: Random, layouts: {
  mqDungeons?: ReadonlySet<string>;
  compatibility?: EnemyPlanCompatibility;
} = {}, onProgress?: EnemyPlanProgress): EnemyPlan {
  if (!spawnRegistryValidated && (settings.enemyShuffleOot || settings.enemyShuffleMm || settings.enemyShuffleOotMinibossOnly || settings.enemyShuffleMmMinibossOnly)) {
    validateSpawnRegistry(enemySpawnInventory());
    spawnRegistryValidated = true;
  }
  if (settings.enemyShuffleOotMinibossOnly || settings.enemyShuffleMmMinibossOnly) {
    return (['oot', 'mm'] as const).flatMap(game => {
      if (settings.games !== 'ootmm' && settings.games !== game)
        return [];
      const only = minibossOnly(settings, game);
      if (!only && !(game === 'oot' ? settings.enemyShuffleOot : settings.enemyShuffleMm))
        return [];
      const perGame = { ...settings, crossGameEnemies: only || settings.crossGameEnemies,
        enemyShuffleOot: game === 'oot', enemyShuffleMm: game === 'mm',
        enemyShuffleOotMinibossOnly: false, enemyShuffleMmMinibossOnly: false,
      };
      return makeEnemyPlanInternal(perGame, random, layouts, onProgress, only ? game : undefined);
    });
  }
  return makeEnemyPlanInternal(settings, random, layouts, onProgress);
}

function limitRoomRedIce(plan: EnemyPlan, settings: Settings, random: Random): void {
  const used = new Set<string>();
  const entries = plan.flatMap(row => [
    { row, finish: () => {} },
    ...(row.splitChildren ?? []).map((target, index) => {
      const child = { ...row, target, ootr: row.ootr && { ...row.ootr }, field: row.field && { ...row.field }, placement: row.placement && { ...row.placement } };
      return { row: child, finish: () => { row.splitChildren = row.splitChildren!.map((value, i) => i === index ? child.target : value); } };
    }),
  ]).filter(({ row }) => enemyDefinition(row.target).key.includes('-red-ice-'));
  for (const { row, finish } of shuffle(random, entries)) {
    const source = enemyDefinition(row.source), target = enemyDefinition(row.target);
    const rooms = new Map<string, number>();
    const add = (game: Game, scene: number, room: number, count = 1, grotto?: number) => {
      const key = `${game}/${scene}/${room}/${grotto ?? ''}`;
      rooms.set(key, Math.max(rooms.get(key) ?? 0, count));
    };
    if (row.ootr) {
      const counts = new Map<string, Set<number>>();
      const locations = [row.ootr.location, ...row.ootr.aliases.map(([scene, room, setup, slot]) => ({ scene, room, setup, slot }))];
      for (const location of locations) {
        const key = `${location.scene}/${location.room}/${location.setup}`;
        const slots = counts.get(key) ?? new Set<number>();
        slots.add(location.slot);
        counts.set(key, slots);
        add('oot', location.scene, location.room, slots.size);
      }
    } else if (source.game === 'oot') {
      for (const location of ootSupplementalMemoryRows([row], settings, () => false))
        add('oot', location.scene, location.room);
    } else {
      for (const scene of MM_ENEMY_SCENES) {
        if (row.placement && (row.placement.scene !== scene.scene || row.placement.grotto !== scene.grotto)) continue;
        for (const map of scene.maps) {
          const count = row.placement ? +map.actors.some(actor => actor.offset === row.placement!.actorOffset) :
            mmEncounterRoots(scene, map, row).length;
          if (count) add('mm', scene.scene, map.room, count, scene.grotto);
        }
      }
    }
    const gold = ENEMY_GOLD_SOURCES.find(value => value.key === row.source);
    if (gold) {
      const counts = new Map<string, number>();
      for (const slot of gold.slots) {
        const key = `${slot.room}/${slot.setup}/${slot.mq}`;
        const count = (counts.get(key) ?? 0) + 1;
        counts.set(key, count);
        add(gold.game, gold.scene, slot.room, count);
      }
    }
    if ([...rooms].every(([key, count]) => count === 1 && !used.has(key))) {
      for (const key of rooms.keys()) used.add(key);
      continue;
    }
    const host = source.game;
    const original = enemyMemoryCost(host, target.game, target.actorId, target.spawnParams, settings);
    const choice = row.ootr ?? row.field;
    const nativeCandidates = choice ? ootEnemizerCandidatePools(ENEMIES, settings, [choice.location]).get(choice.location) ?? [] : undefined;
    const available = ENEMIES.filter(candidate => candidate.game === target.game && candidate.actorId === target.actorId &&
      candidate.targetAllowed !== false && !candidate.key.includes('-red-ice-') &&
      (!nativeCandidates || nativeCandidates.some(value => value.def.key === candidate.key)) &&
      enemyWeightFamily(candidate) === enemyWeightFamily(target) && !enemyCanRespawn(candidate.game, candidate.actorId, candidate.spawnParams) &&
      !enemyShuffleExcluded(settings, candidate, host) && enemyTargetCompatible(settings, enemyPlacementLogicName(row), candidate));
    const saved = { ...row, ootr: row.ootr && { ...row.ootr }, field: row.field && { ...row.field },
      placement: row.placement && { ...row.placement } };
    const apply = (replacement: EnemyDefinition) => {
      row.target = replacement.key;
      for (const choice of [row.ootr, row.field]) if (choice) {
        const candidate = nativeCandidates!.find(value => value.def.key === replacement.key)!;
        choice.targetIndex = candidate.index;
        choice.target = candidate.index >= 0 ? OOT_ENEMIZER.targets[candidate.index] : {
          ...choice.target, name: replacement.name, params: replacement.spawnParams, killLogic: replacement.killMacro, dropLogic: null,
        };
        choice.targetParams = replacement.spawnParams;
        choice.killLogic = choice.target.killLogic;
        choice.dropLogic = choice.target.dropLogic;
      }
      if (row.placement) Object.assign(row.placement, {
        targetKey: replacement.key, targetGame: replacement.game,
        targetActorId: replacement.actorId, targetParams: replacement.spawnParams,
      });
      finish();
    };
    let candidates = available.filter(candidate => {
      const cost = enemyMemoryCost(host, candidate.game, candidate.actorId, candidate.spawnParams, settings);
      return cost.instances <= original.instances && cost.actors <= original.actors &&
        (cost.dynamicCollisions ?? 0) <= (original.dynamicCollisions ?? 0) &&
        Object.entries(cost.shared).every(([key, bytes]) => bytes <= (original.shared[key] ?? 0));
    });
    if (!candidates.length) {
      candidates = available.filter(candidate => {
        apply(candidate);
        const fits = enemyOotSceneMemoryReports(plan, settings).every(report => ootResourceExcess(report) <= 0) &&
          enemySceneMemoryReports(plan, settings).every(report => report.resourceExcess <= 0);
        Object.assign(row, saved);
        finish();
        return fits;
      });
    }
    if (!candidates.length) throw new EnemyMemoryAllocationError(`No safe non-red-ice variant for ${row.source}`);
    const weights = enemyFormWeights(candidates, value => value, value => enemyWeight(settings, host, value));
    apply(pickEnemyWeighted(candidates, weights, max => random.next() % max));
  }
}

function makeEnemyPlanInternal(settings: Settings, random: Random, layouts: {
  mqDungeons?: ReadonlySet<string>;
  compatibility?: EnemyPlanCompatibility;
}, onProgress?: EnemyPlanProgress, only?: Game): EnemyPlan {
  settings = Object.assign({}, settings, { [enemyCompatibility]: layouts.compatibility,
    enemyShuffleOotMinibossOnly: only === 'oot',
    enemyShuffleMmMinibossOnly: only === 'mm',
  });
  const progress = settings.enemyShuffleOot || settings.enemyShuffleMm || settings.enemyShuffleOotMinibossOnly || settings.enemyShuffleMmMinibossOnly ? onProgress : undefined;
  const forced = settings.plando.enemyTargets ?? {};
  for (const [game, key] of Object.entries(forced)) {
    const target = enemyDefinition(key!);
    if (enemyCanRespawn(target.game, target.actorId, target.spawnParams) || enemyIsPerched(target.game, target.actorId, target.spawnParams) || target.targetAllowed === false || target.game !== game && !settings.crossGameEnemies ||
      enemyShuffleExcluded(settings, target, game as Game))
      throw new Error(`Unavailable forced enemy target for ${game}: ${key}`);
  }
  let plan: EnemyPlan = [];
  const constraints: SupplementalMemoryConstraint[] = [];
  for (let attempt = 0; attempt < 8; attempt++) {
    try {
      progress?.(attempt ? `Retrying enemy placement (attempt ${attempt + 1} of 8)` : 'Choosing miniboss and reward enemies');
      const encounterRequests: MmEncounterChoice[][] = [];
      const supplemental = makeSupplementalEnemyPlan(ENEMIES, settings, random, constraints, forced.mm ? undefined : encounterRequests, resolvedMq(settings, layouts.mqDungeons));
      plan = [...makeOotEnemizerPlan(ENEMIES, settings, random, layouts, supplemental, progress),
      ...makeMmEnemyPlan(ENEMIES, settings, random, supplemental, encounterRequests, progress), ...supplemental];
      planEnemySplits(plan, settings, random);
      limitRoomRedIce(plan, settings, random);
      break;
    }
    catch (error) {
      if (!(error instanceof EnemyMemoryAllocationError) || attempt === 7 || Object.keys(forced).length)
        throw error;
      if (error instanceof MmSceneAllocationError)
        constraints.push(error.acceptsSupplemental);
    }
  }
  plan = plan.filter(row => {
    if (row.kind !== 'scripted' && row.kind !== 'arena') return true;
    const source = enemyDefinition(row.source);
    return !enemyKeepsNativeEncounter(source.game, source.actorId, source.spawnParams, enemyDefinition(row.target));
  });
  progress?.('Checking enemy memory limits');
  for (const report of enemyOotSceneMemoryReports(plan, settings))
    if (ootResourceExcess(report) > 0)
      throw new OotEnemyResourceLimit(`OoT scene ${report.scene}, rooms ${report.rooms}: gameplay headroom exceeded (bytes ${report.bytes}/${report.budget}, actors ${report.actors}/${report.actorBudget}, models ${report.models}/80, foreign ${report.foreign}/30, collision ${report.dynamicCollisions}/${report.collisionBudget})`);
  for (const report of enemySceneMemoryReports(plan, settings))
    if (report.resourceExcess > 0)
      throw Error(`MM scene ${report.scene}: scripted and placed enemies exceed the memory budget`);

  return plan;
}

export function enemyLogicMacros(macros: Readonly<Record<string, string>>, plan: EnemyPlan, game: Game): Record<string, string> {
  const pairs = plan.filter(p => p.kind !== 'variant').map(p => ({ pair: p, source: enemyDefinition(p.source), target: enemyDefinition(p.target) }))
    .filter(p => p.source.game === game);
  if (!pairs.length)
    return { ...macros };
  const nativeMacros = { ...macros };
  for (const enemy of pairs.flatMap(({ source, target }) => [source, target])) {
    if (enemy.game === game && enemy.soulItem) {
      const alias = enemy.soulMacro ?? `soul_${enemy.key.split('-')[1]}`;
      nativeMacros[alias] ??= `soul_enemy(${enemy.soulItem})`;
    }
  }
  macros = nativeMacros;
  const result = { ...macros };
  const names = new Set(Object.keys(macros).map(k => k.split('(')[0]));
  const snapshot = (name: string) => `base_${name}`;
  const rewrite = (text: string, args: ReadonlySet<string>) => text.replace(/\b(?:age|has|renewable|license|event|setting|trick|special|oot_time|mm_time|price|has_pond_fish|flag_on|flag_off|_song_event_oot|_song_event_mm)\s*\([^)]*\)|\b[A-Za-z_][A-Za-z_0-9]*\b/g, token => names.has(token) && !args.has(token) ? snapshot(token) : token);
  for (const [name, body] of Object.entries(macros)) {
    const base = name.split('(')[0];
    const args = new Set(name.slice(base.length).replace(/[(),]/g, ' ').split(/\s+/).filter(Boolean));
    result[snapshot(base) + name.slice(base.length)] = rewrite(body, args);
  }
  addEnemyPlacementLogic(result, macros, plan, game, enemyDefinition);
  const requirements = new Map<string, Set<string>>();
  for (const { pair, source, target } of pairs) {
    if (pair.ootr || pair.placement || pair.kind === 'gold' || pair.kind === 'burial-spawner' || scopedEnemyEncounter(pair))
      continue;
    const combat = enemyPlacementCombat(result, macros, target, game);
    if (target.game === game && macros[target.killMacro] === undefined)
      throw new Error(`Missing enemy macro: ${target.killMacro}`);
    const soulMacro = source.soulMacro ?? `soul_${source.key.split('-')[1]}`;
    for (const name of [...source.logicMacros, soulMacro]) {
      if (macros[name] === undefined)
        throw new Error(`Missing enemy macro: ${name}`);
      if (!requirements.has(name))
        requirements.set(name, new Set());
      requirements.get(name)!.add(combat);
    }
  }
  for (const [name, targets] of requirements) {
    const combat = [...targets].join(' && ');
    result[name] = name === 'can_kill_keeta_guards' && pairs.some(p => p.pair.kind === 'keeta-guards')
      ? combat : `${snapshot(name)} && ${combat}`;
  }
  return result;
}

export function enemySoulSpecializations(macros: Readonly<Record<string, string>>, plan: EnemyPlan, game: Game) {
  const requirements = new Map<string, Set<string>>();
  for (const pair of plan) {
    if (pair.kind === 'variant' || pair.kind === 'gold' || pair.kind === 'burial-spawner' || scopedEnemyEncounter(pair) || pair.ootr || pair.placement)
      continue;
    const source = enemyDefinition(pair.source), target = enemyDefinition(pair.target);
    if (source.game !== game)
      continue;
    const alias = source.soulMacro ?? `soul_${source.key.split('-')[1]}`;
    const body = macros[alias] ?? source.soulItem;
    if (body === undefined)
      throw new Error(`Missing enemy soul macro: ${alias}`);
    for (const match of body.matchAll(/\b(?:SHARED_)?SOUL_ENEMY_[A-Z0-9_]+\b/g)) {
      const item = match[0];
      if (!requirements.has(item))
        requirements.set(item, new Set());
      requirements.get(item)!.add(enemyCombatMacroName(target, game));
    }
  }
  return [...requirements].map(([item, targets]) => ({
    name: 'soul_enemy', args: [item],
    buffer: `base_soul_enemy(${item}) && ${[...targets].join(' && ')}`
  }));
}

function scopedEnemyEncounter(pair: EnemyReplacement): boolean {
  return pair.kind === 'forest-bow' || pair.kind === 'scripted' || pair.kind === 'arena' || pair.kind === 'restore' && pair.source === 'oot-gerudo-fighter-restored';
}

export function enemyPlacementLogicName(pair: EnemyReplacement): string | undefined {
  if (pair.kind === 'gold') return pair.source.replaceAll('-', '_');
  if (scopedEnemyEncounter(pair)) return 'fight_' + pair.source.replace(/-(?:scripted|arena|restored)$/, '').replaceAll('-', '_');
  if (pair.ootr) {
    const l = pair.ootr.location;
    return `enemy_oot_${l.scene}_${l.room}_${l.setup}_${l.slot}`;
  }
  if (pair.placement)
    return `enemy_mm_${pair.placement.scene}_${pair.placement.actorOffset}${pair.placement.grotto === undefined ? '' : '_' + pair.placement.grotto}`;
  return undefined;
}

const OOT_OVERRIDE_ATOMS: Readonly<Record<string, string>> = {
  Bow: 'can_use_bow', Bombs: 'has_bombs', Nuts: 'has_nuts', Hookshot: 'can_hookshot',
  Longshot: 'can_longshot', Megaton_Hammer: 'can_hammer', Biggoron_Sword: 'can_use_sword_biggoron',
  Iron_Boots: 'has_iron_boots', Dins_Fire: 'can_use_din', Fire_Arrows: 'has_fire_arrows',
  at_day: 'is_day', at_night: 'is_night', is_adult: 'is_adult',
  has_explosives: 'has_explosives', has_bombchus: 'has_bombchu',
  can_kill_tailsparan: 'can_kill_tailpasarans',
};

function translateOotEnemyLocationLogic(text: string, native: Readonly<Record<string, string>>): string {
  if (text === 'at(\'DMC Central Local\', (is_adult and Nuts and Bow)) or Bombs')
    return 'base_dmc_stinger_or_bombs';
  const rewritten = text.replace(/can_use\(([A-Za-z_]+)\)/g, '$1')
    .replace(/\b[A-Za-z_][A-Za-z_0-9]*\b/g, word => {
      if (word === 'and')
        return '&&';
      if (word === 'or')
        return '||';
      if (word === 'not')
        return '!';
      const name = OOT_OVERRIDE_ATOMS[word];
      if (!name || native[name] === undefined)
        throw new Error(`Unreviewed enemy location predicate: ${word} (${name ?? 'unknown'})`);
      return `base_${name}`;
    });
  if (/[^A-Za-z_0-9\s()!&|]/.test(rewritten))
    throw new Error('Invalid enemy location combat expression');
  const tokens = rewritten.match(/base_[A-Za-z_0-9]+|&&|\|\||[()!]/g)!;
  let cursor = 0;
  const primary = (): string => {
    const token = tokens[cursor++];
    if (token === '!')
      return `!(${primary()})`;
    if (token === '(') {
      const inner = disjunction();
      if (tokens[cursor++] !== ')')
        throw new Error('Unbalanced enemy location combat expression');
      return inner;
    }
    if (!token?.startsWith('base_'))
      throw new Error('Invalid enemy location combat atom');
    return token;
  };
  const conjunction = (): string => {
    const terms = [primary()];
    while (tokens[cursor] === '&&') {
      cursor++;
      terms.push(primary());
    }
    return terms.length === 1 ? terms[0] : `(${terms.join(' && ')})`;
  };
  const disjunction = (): string => {
    const terms = [conjunction()];
    while (tokens[cursor] === '||') {
      cursor++;
      terms.push(conjunction());
    }
    return terms.length === 1 ? terms[0] : `(${terms.join(' || ')})`;
  };
  const result = disjunction();
  if (cursor !== tokens.length)
    throw new Error('Trailing enemy location combat expression');
  return result;
}

const ENEMY_COMBAT_OVERRIDES: Readonly<Record<Game, Readonly<Record<string, string>>>> = {
  oot: {
    'mm-pirate-fighter-native-target': 'can_kill_gerudo_fighter',
    'mm-pirate-fighter-restored': 'can_kill_gerudo_fighter',
    'mm-carnivorous-lily-foreign': 'can_kill_carnivorous_lilies',
    'mm-placement-carnivorouslily': 'can_kill_carnivorous_lilies',
    'mm-placement-biobaba-inert': 'true',
  },
  mm: {
    'oot-gerudo-fighter-native-target': 'can_fight_pirates',
    'oot-flying-floor-tile-foreign': 'true',
    'oot-flying-floor-tile-source': 'true',
  },
};

function enemyCombatRule(target: EnemyDefinition, host: Game) {
  const local = ENEMY_COMBAT_OVERRIDES[host][target.key];
  return {
    game: local ? host : target.game,
    name: local ?? (target.key === 'mm-placement-wallmaster' ? 'can_kill_wallmasters' : target.killMacro),
  };
}

function enemyCombatMacroName(target: EnemyDefinition, host: Game): string {
  const { game, name } = enemyCombatRule(target, host);
  return name === 'true' ? name : game === host ? 'base_' + name : foreignEnemyCombatName(game, name);
}

function enemyPlacementCombat(output: Record<string, string>, native: Readonly<Record<string, string>>, target: EnemyDefinition, host: Game): string {
  const { game, name } = enemyCombatRule(target, host);
  if (name === 'true') return name;
  if (game !== host)
    return importForeignEnemyCombat(output, native, game, host, name);
  if (!native[name])
    throw Error('Missing enemy combat macro: ' + host + '/' + name);
  return 'base_' + name;
}

function addEnemyPlacementLogic(output: Record<string, string>, native: Readonly<Record<string, string>>, plan: EnemyPlan, game: Game, definition: (key: string) => EnemyDefinition): void {
  for (const pair of plan) {
    const name = enemyPlacementLogicName(pair);
    if (!name || definition(pair.source).game !== game)
      continue;
    const target = definition(pair.target);
    let combat = enemyPlacementCombat(output, native, target, game);
    if (target.game === game && native[target.killMacro] === undefined)
      throw new Error(`Missing enemy macro: ${target.killMacro}`);
    const ootPlacement = pair.ootr;
    if (ootPlacement && !ootPlacement.foreign) {
      const underwater = (ootPlacement.location.restrictions as readonly string[]).includes('UNDERWATER');
      if (target.actorId === 0xc5)
        combat = `base_can_kill_shell_blades${underwater ? '_underwater' : ''}`;
      if (target.actorId === 0xec)
        combat = `base_can_kill_spike_enemies${underwater ? '_underwater' : ''}`;
      if (target.actorId === 0x1d)
        combat = `base_can_kill_shuffled_peahat_${ootPlacement.targetParams === 0 ? 'flying' : 'ground'}`;
    }
    if (ootPlacement && target.game === 'mm' && game === 'oot' && target.actorId === 0x14b &&
      !(ootPlacement.location.restrictions as readonly string[]).includes('UNDERWATER')) {
      const soul = foreignEnemyCombatName('mm', 'soul_skull_fish');
      if (output[soul] === undefined)
        throw new Error('Missing imported Desbreko soul requirement');
      combat = `base_can_kill_desbreko_surface(${soul})`;
    }
    if (pair.placement?.targetGame === 'mm' && pair.placement.targetActorId === 0x14 &&
      pair.placement.targetParams === 0 && pair.placement.sourceActorId !== 0x14) {
      combat = 'base_can_kill_randomized_peahats';
    }
    const mmTarget = pair.placement;
    if (game === 'mm' && mmTarget?.targetGame === 'mm' &&
      ((mmTarget.targetActorId === 0x24 && (mmTarget.targetParams & 0x1c0) === 0x40) ||
        (mmTarget.targetActorId === 0x0c && (mmTarget.targetParams & 0x8000) !== 0) ||
        (mmTarget.targetActorId === 0x4c && (mmTarget.targetParams & 0xff) === 3))) {
      if (native.can_use_lens === undefined)
        throw new Error('Missing native invisible-enemy Lens requirement');
      combat = `base_can_use_lens && (${combat})`;
    }
    let body = combat;
    if (pair.kind === 'scripted') {

      const source = definition(pair.source);
      const souls: Readonly<Record<number, string>> = {
        0x7: 'GEKKO', 0x65: 'GEKKO', 0x12c: 'WART', 0x182: 'GARO',
        0x184: 'EYEGORE', 0x43: 'GOMESS', 0x15d: 'WIZZROBE',
      };
      const soul = souls[source.actorId];
      const gate = source.actorId === 0x115 ? 'base_soul_boss(SOUL_BOSS_IGOS)' :
        soul ? `base_shared_soul_enemy(SOUL_ENEMY_${soul}, SHARED_SOUL_ENEMY_${soul})` : undefined;
      if (!gate) throw Error('Missing scripted enemy intro requirement: ' + pair.source);
      body = `${gate} && (${body})`;
    }
    if (ootPlacement && !ootPlacement.foreign && ootPlacement.killLogic !== ootPlacement.target.killLogic) {
      const soul = target.soulMacro ?? `soul_${target.key.split('-')[1]}`;
      const soulRequirement = target.soulItem ? `base_soul_enemy(${target.soulItem})` : `base_${soul}`;
      if (!target.soulItem && native[soul] === undefined)
        throw new Error(`Missing enemy soul macro: ${soul}`);
      body = `${soulRequirement} && (${translateOotEnemyLocationLogic(ootPlacement.killLogic, native)})`;
    }
    if (target.soulItem && ['SOUL_ENEMY_GIANT_BEE', 'SOUL_ENEMY_MOTH', 'SOUL_ENEMY_GARAHNA'].includes(target.soulItem)) {
      const soul = target.soulItem;
      const item = enemySharedSoulIds.has(`SHARED_${soul}`)
        ? `cond(setting(sharedSoulsEnemy), base_soul_enemy(SHARED_${soul}), base_soul_enemy(${soul}))`
        : `base_soul_enemy(${soul})`;
      body = `${item} && (${body})`;
    }
    for (const child of new Set(pair.splitChildren ?? [])) {
      const enemy = definition(child);
      if (enemy.game === game && native[enemy.killMacro] === undefined)
        throw new Error(`Missing split child macro: ${enemy.killMacro}`);
      const childCombat = enemyPlacementCombat(output, native, enemy, game);
      if (childCombat !== combat)
        body += ` && (${childCombat})`;
    }
    if (output[name] !== undefined && output[name] !== body)
      throw new Error(`Conflicting enemy location logic: ${name}`);
    output[name] = body;
  }
}

export function enemyPlacementAreaEvents(game: Game, area: string, plan: EnemyPlan): Record<string, string> {
  if (game === 'oot' && area === 'Death Mountain Crater Warp' && plan.some(p => p.ootr?.killLogic === 'at(\'DMC Central Local\', (is_adult and Nuts and Bow)) or Bombs')) {
    return { ENEMY_DMC_CENTRAL_STINGER: 'base_dmc_central_stinger' };
  }
  return {};
}

type EnemyRoomSlots = {
  scene: number;
  room: number;
  setup: number;
  slots: readonly number[];
};

type MmRoomBinding = {
  rooms: readonly EnemyRoomSlots[];
  clearChecks?: Readonly<Record<string, readonly string[]>>;
  encounters?: readonly {
    source: string;
    room: EnemyRoomSlots;
    rules: readonly { section: string; key: string; expectedRule: string; template: string }[];
  }[];
  rules?: readonly {
    section: string;
    key: string;
    expectedRule: string;
    template: string;
    selectors: Readonly<Record<string, EnemyRoomSlots & { vanilla?: string; bioBabaPad?: boolean }>>;
  }[];
};

const MM_ROOM_BINDINGS: Readonly<Record<string, MmRoomBinding>> = mmRoomBindings;

const roomKey = ({ scene, room, setup }: EnemyRoomSlots | MmPlacement) => [scene, room, setup].join('/');

const matchesRoomSlot = (binding: EnemyRoomSlots, placement: MmPlacement) => roomKey(binding) === roomKey(placement) && binding.slots.includes(placement.slot);

const conjunction = (rows: readonly EnemyReplacement[]) => [...new Set(rows.map(enemyPlacementLogicName))].join(' && ') || 'true';

function mmNativeActor(room: EnemyRoomSlots, slot: number): MmActor {
  const actor = MM_NATIVE_SCENES.find(scene => scene.scene === room.scene)?.maps
    .find(map => map.room === room.room && map.setup === room.setup)?.actors.find(actor => actor.slot === slot);
  if (!actor) throw Error('Missing native MM logic slot: ' + roomKey(room) + '/' + slot);
  return actor;
}

function mmNativeDefinition(actor: MmActor): EnemyDefinition {
  const row = MM_NATIVE_ROSTER.find(row => row.actorId === actor.actorId && row.variables.includes(actor.params)) ??
    MM_NATIVE_ROSTER.find(row => row.actorId === actor.actorId);
  if (!row) throw Error('Missing native MM logic actor: ' + actor.actorId);
  return enemyDefinition(mmNativeRosterKey(row.key));
}

function mmNativeCombat(actor: MmActor): string {
  if (actor.actorId === 0x12d && (actor.params & 0xff) === 2) return 'true';
  const native = mmNativeDefinition(actor);
  let combat = 'base_' + native.killMacro;
  if (actor.actorId === 0x24 && (actor.params & 0x1c0) === 0x40 ||
    actor.actorId === 0xc && actor.params & 0x8000 || actor.actorId === 0x4c && (actor.params & 0xff) === 3)
    combat = 'base_can_use_lens && ' + combat;
  return combat;
}

function mmSlotReplacement(room: EnemyRoomSlots, slot: number, plan: EnemyPlan): EnemyReplacement | undefined {
  const actor = mmNativeActor(room, slot);

  return plan.find(row => row.placement?.scene === room.scene && row.placement.actorOffset === actor.offset);
}

function mmRoomRequirement(room: EnemyRoomSlots & { vanilla?: string; bioBabaPad?: boolean }, plan: EnemyPlan) {
  let changed = false;
  const requirements = room.slots.map(slot => {
    const replacement = mmSlotReplacement(room, slot, plan);
    if (replacement) {
      changed = true;
      if (room.bioBabaPad && replacement.placement?.sourceActorId === 0x12d && replacement.placement.sourceParams === 0)
        return 'soul_enemy(SOUL_ENEMY_BIO_BABA)';
      return enemyPlacementLogicName(replacement)!;
    }
    return room.vanilla ?? mmNativeCombat(mmNativeActor(room, slot));
  });
  return { changed, expression: [...new Set(requirements)].map(rule => '(' + rule + ')').join(' && ') || 'true' };
}

export function mmEnemyAreaLogic(areaName: string, area: any, plan: EnemyPlan, areaSetName: string, layout = 'mm'): any {
  const candidates = plan.filter(row => row.placement);
  if (!candidates.length && !plan.some(row => row.kind === 'scripted'))
    return area;
  const areaKey = areaSetName + '/' + areaName;
  const areaBinding = MM_ROOM_BINDINGS[layout + '/' + areaKey] ?? MM_ROOM_BINDINGS['mm/' + areaKey];
  const local = candidates.filter(row => areaBinding?.rooms.some(room => matchesRoomSlot(room, row.placement!)));
  const eventBindings = area.enemy_clear_events as Record<string, {
    scene: number;
    room: number;
    setup: number;
    slots: number[];
    vanilla: string;
  }> | undefined;
  const matches = (row: EnemyReplacement, predicate: string) => {
    const source = enemyDefinition(row.source);
    return source.killMacro === predicate || source.soulMacro === predicate || source.logicMacros.includes(predicate);
  };
  const transform = (expr: string) => {
    return expr.replace(/\bsoul_enemy\(\s*(SOUL_ENEMY_[A-Z0-9_]+)\s*\)|\b(?:can_kill_|soul_)[A-Za-z0-9_]+\b/g, (token, item) => {
      const affected = local.filter(row => item ? enemyDefinition(row.source).soulItem === item : matches(row, token));
      if (!affected.length) return token;
      const remains = areaBinding?.rooms.some(room => room.slots.some(slot => {
        const actor = mmNativeActor(room, slot);
        if (mmSlotReplacement(room, slot, plan)) return false;
        const source = mmNativeDefinition(actor);
        return item ? source.soulItem === item : source.killMacro === token || source.soulMacro === token || source.logicMacros.includes(token);
      }));
      return '(' + (remains ? token + ' && ' : '') + conjunction(affected) + ')';
    });
  };
  const out = { ...area };
  for (const section of ['locations', 'events', 'exits', 'gossip']) {
    if (!area[section])
      continue;
    out[section] = Object.fromEntries(Object.entries(area[section]).map(([name, raw]) => {
      for (const encounter of areaBinding?.encounters ?? []) {
        const row = plan.find(row => row.kind === 'scripted' && row.source === encounter.source);
        const rule = encounter.rules.find(rule => rule.section === section && rule.key === name);
        if (!row || !rule) continue;
        const source = enemyDefinition(row.source), target = enemyDefinition(row.target);
        if (!encounter.room.slots.every(slot => {
          const actor = mmNativeActor(encounter.room, slot);
          return actor.actorId === source.actorId && enemyScriptedRootOffsets[encounter.room.scene]?.includes(actor.offset);
        })) throw Error('Unverified scripted enemy logic binding: ' + areaKey);
        if (raw !== rule.expectedRule) throw Error('Scripted enemy world rule drift: ' + areaKey + '/' + name);

        if (source.actorId === 0x15d && target.game === 'mm' && target.actorId === 0x15d) return [name, raw];
        return [name, rule.template.split('$ENEMY').join('(' + enemyPlacementLogicName(row) + ')')];
      }
      const exact = areaBinding?.rules?.find(rule => rule.section === section && rule.key === name);
      if (exact) {
        if (raw !== exact.expectedRule) throw Error('MM enemy world rule drift: ' + areaKey + '/' + section + '/' + name);
        let changed = false;
        let expr = exact.template;
        for (const [key, selector] of Object.entries(exact.selectors)) {
          const compiled = mmRoomRequirement(selector, plan);
          changed ||= compiled.changed;
          expr = expr.split('$' + key).join('(' + compiled.expression + ')');
        }
        if (/\$[A-Z_]+/.test(expr)) throw Error('Unresolved MM enemy world dependency: ' + areaKey);
        if (changed) return [name, expr];
      }
      const binding = section === 'events' ? eventBindings?.[name] : undefined;
      if (binding) {
        const replacements = candidates.filter(({ placement }) => matchesRoomSlot(binding, placement!));
        if (!replacements.length)
          return [name, raw];
        const vanilla = raw as string;
        if (!vanilla.includes(binding.vanilla))
          throw Error('Enemy clear event rule drift: ' + areaName + '/' + name);

        const nativeKinds = new Set(binding.slots.map(slot => mmNativeActor(binding, slot).actorId));
        const requirement = mmRoomRequirement({ ...binding, vanilla: nativeKinds.size === 1 ? binding.vanilla : undefined }, plan);
        return [name, vanilla.replace(binding.vanilla, '(' + requirement.expression + ')')];
      }
      let expr = transform(raw as string);
      if (section === 'locations') {
        const required = areaBinding?.clearChecks?.[name];
        if (required?.length)
          expr = `(${expr}) && (${conjunction(local.filter(row => required.includes(roomKey(row.placement!))))})`;
      }
      return [name, expr];
    }));
  }
  return out;
}

export function enemySoulDependencies(key: string): ReadonlySet<string> {
  const enemy = enemyDefinition(key);
  const macros = new Map(Object.entries({ ...MACROS.common, ...MACROS[enemy.game] })
    .map(([name, body]) => [name.split('(')[0], body]));
  const souls = new Set<string>(enemy.soulItem ? [enemy.soulItem] : []);
  const visited = new Set<string>();
  const visit = (name: string) => {
    if (visited.has(name)) return;
    visited.add(name);
    const body = macros.get(name);
    if (!body) return;
    for (const token of body.match(/\b[A-Za-z_][A-Za-z_0-9]*\b/g) ?? []) {
      const soul = token.match(/^(?:(?:OOT_|MM_|SHARED_))?(SOUL_ENEMY_[A-Z0-9_]+)$/);
      if (soul) souls.add(soul[1]);
      else if (macros.has(token)) visit(token);
    }
  };
  visit(enemy.killMacro);
  if (enemy.soulMacro) visit(enemy.soulMacro);
  return souls;
}

const foreignEnemyCombatName = (game: Game, name: string) => `${game}_${name}`;

const COMBAT = new Set([
  'can_hammer', 'has_nuts', 'has_sticks', 'can_use_sticks', 'has_explosives',
  'can_use_slingshot', 'can_use_din', 'can_use_light_arrows', 'can_use_ice_arrows',
  'can_use_fire_arrows', 'has_magic', 'can_swim', 'has_chateau', 'has_iron_boots',
  'can_use_sword_master', 'can_use_sword_biggoron', 'can_use_sword_kokiri',
  'can_use_sword_goron', 'can_use_sword_razor', 'can_use_sword_gilded',
  'has_bomb_bag', 'has_bombs', 'has_bombchu', 'has_arrows', 'has_sword_kokiri', 'has_sword_master',
  'has_spin_upgrade', 'has_mask_blast', 'has_mirror_shield', 'is_child', 'is_day', 'is_night', 'can_hookshot',
]);

const ootEnemySword = 'can_use_sword || can_use_sword_razor || can_use_sword_gilded || can_use_sword_biggoron';
const ootEnemyWeapon = `${ootEnemySword} || can_hammer`;
const COMBAT_ADAPTERS: Readonly<Record<Game, Readonly<Record<string, string>>>> = {
  oot: {
    has_weapon_range: 'has_ranged_weapon',
    has_weapon: ootEnemyWeapon,
    can_fight: ootEnemyWeapon,
    can_use_ice_arrows: 'can_use_ice_arrow',
    can_use_fire_arrows: 'has_fire_arrows',
    can_use_fire_short_range: 'has_fire',
    has_sword: ootEnemySword,
    has_arrows: 'can_use_bow',
    has_boomerang: 'can_boomerang',
    can_use_zora_boomerang: 'can_boomerang',
    can_hookshot_short: 'can_hookshot',
    can_use_deku_bubble: 'can_use_slingshot',
    deku_stick_fighting: 'can_use_sticks',
    can_use_lens: 'has_lens',
  },
  mm: {
    has_lens: 'can_use_lens',
    has_shield_for_scrubs: 'has_shield_deku || (is_adult && has_shield_hylian) || has_shield_hero',
    has_fire: 'can_use_fire_short_range',
    has_fire_arrows: 'can_use_fire_arrows',
    has_ranged_weapon: 'has_arrows || can_use_slingshot || can_hookshot_short || has_boomerang',
    can_use_sword: 'has_sword',
    can_use_sword_or_sticks: 'has_sword || can_use_sticks',
    can_use_bow: 'has_arrows',
    can_boomerang: 'has_boomerang',
    can_hookshot: 'can_hookshot_short',
    can_use_mask_blast: 'has_mask_blast',
  },
};

const enemyNativeSoulIds = new Set(RAW_GI.map(row => row.id));

const enemySharedSoulIds = new Set(FOREIGN_ENEMY_SOULS.map(row => `SHARED_${row.soul}`));

export function importForeignEnemyCombat(output: Record<string, string>, native: Readonly<Record<string, string>>, source: Game, host: Game, root: string): string {
  if (source === host)
    throw new Error('Foreign combat logic requires different games');
  const definitions = new Map(Object.entries({ ...MACROS.common, ...MACROS[source] }).map(([key, body]) => {
    const [name, ...args] = key.replace(/[(),]/g, ' ').split(/\s+/).filter(Boolean);
    return [name, { key, body, args }] as const;
  }));
  const igos = source === 'mm' && ['can_kill_igos', 'can_kill_igos_solo'].includes(root);
  const nativeNames = new Set(Object.keys(native).map(key => key.split('(')[0]));
  const staged: Record<string, string> = {};
  const active = new Set<string>();
  const completed = new Set(Object.keys(output).map(key => key.split('(')[0]));
  const soulAlias = foreignEnemyCombatName(source, 'soul_enemy');
  const sourceId = source.toUpperCase();
  const hostId = host.toUpperCase();
  const donorSoulPattern = new RegExp(`\\b${soulAlias}\\(\\s*(${sourceId}_SOUL_ENEMY_[A-Z0-9_]+)\\s*\\)`, 'g');
  const sharedSoulPattern = new RegExp(`\\b${soulAlias}\\(\\s*(SOUL_ENEMY_[A-Z0-9_]+)\\s*\\)`, 'g');
  const importableMacro = /^(?:can_kill_|soul_|shared_soul_enemy$|can_damage_skulltulas$)/;
  const visit = (name: string): string => {
    const alias = foreignEnemyCombatName(source, name);
    if (active.has(name))
      throw new Error(`Recursive foreign enemy combat macro: ${name}`);
    if (completed.has(alias))
      return alias;
    const definition = definitions.get(name);
    if (!definition || !importableMacro.test(name))
      throw new Error(`Unreviewed foreign enemy combat macro: ${source}/${name}`);
    if (name !== 'soul_enemy' && !(igos && name === 'soul_boss') && /\b(?:has|renewable|license|event)\s*\(/.test(definition.body))
      throw new Error(`Foreign enemy combat requires an explicit inventory adapter: ${source}/${name}`);
    if (name === 'soul_enemy') {
      const setting = host === 'oot' ? 'soulsEnemyOot' : 'soulsEnemyMm';
      staged[alias + definition.key.slice(name.length)] = `!setting(${setting}) || has(x)`;
      const donorSetting = source === 'oot' ? 'soulsEnemyOot' : 'soulsEnemyMm';
      const prefix = `${sourceId}_SOUL_ENEMY_`;
      for (const gi of enemyNativeSoulIds) {
        if (!gi.startsWith(prefix) || enemyNativeSoulIds.has(`${hostId}_${gi.slice(source.length + 1)}`))
          continue;
        staged[`${alias}_donor(${definition.args.join(', ')})`] = `!setting(${donorSetting}) || has(x)`;
        break;
      }
      completed.add(alias);
      return alias;
    }
    active.add(name);
    const body = definition.body.replace(/\b[A-Za-z_][A-Za-z_0-9]*\b/g, token => {
      if (definition.args.includes(token))
        return token;
      if (/^SOUL_ENEMY_/.test(token))
        return enemyNativeSoulIds.has(`${hostId}_${token}`)
          ? token : `${sourceId}_${token}`;
      if (igos && /^SOUL_BOSS_/.test(token))
        return `${sourceId}_${token}`;
      if (host === 'oot' && /^(?:has|can_use)_mask_(?:deku|goron|zora|fierce_deity|giant)(?:_|$)/.test(token))
        return 'false';
      if (!definitions.has(token))
        return token;
      if (importableMacro.test(token))
        return visit(token);
      const binding = COMBAT_ADAPTERS[host][token] ?? (COMBAT.has(token) ? token : undefined);
      if (!binding)
        throw new Error(`Unreviewed receiving-game enemy combat predicate: ${source}/${token} -> ${host}`);
      const translated = binding.replace(/\b[A-Za-z_][A-Za-z_0-9]*\b/g, name => {
        if (!nativeNames.has(name))
          throw new Error(`Missing receiving-game enemy combat predicate: ${host}/${name}`);
        return `base_${name}`;
      });
      return `(${translated})`;
    });
    active.delete(name);
    staged[alias + definition.key.slice(name.length)] = body.replace(donorSoulPattern, (_call, soul: string) => `${soulAlias}_donor(${soul})`).replace(sharedSoulPattern, (call, soul: string) => enemySharedSoulIds.has(`SHARED_${soul}`)
      ? `cond(setting(sharedSoulsEnemy), ${soulAlias}(SHARED_${soul}), ${call})`
      : call);
    completed.add(alias);
    return alias;
  };
  const result = visit(root);
  Object.assign(output, staged);
  return result;
}

const facts = placementData.tentacle;

export function tentacleSlot(game: Game, key: OotEnemySlot & {
  mq?: boolean;
}) {
  if (key.mq)
    return undefined;
  return facts.slots[game].find(row => row.scene === key.scene && row.room === key.room && row.setup === key.setup && row.slot === key.slot);
}

export function tentacleSlotAt(game: Game, offset: number) {
  return facts.slots[game].find(row => row.offset === offset);
}

export function enemyIsCarnivorousLily(game: string, actorId: number): boolean {
  return enemySpawnMacroMatches('carnivorous-lily', { target: { game, actorId } });
}

type WaterSurfaceSlot = { original: string; surfaceY: number; collisionRanges: readonly (readonly number[])[] };

export function ootWaterSurfaceSlot(key: OotEnemySlot & { mq?: boolean }): WaterSurfaceSlot | undefined {
  return (ENEMY_WATER_SURFACE_SLOTS.oot as Readonly<Record<string, WaterSurfaceSlot>>)[[+!!key.mq, key.scene, key.room, key.setup, key.slot].join('/')];
}

export function mmWaterSurfaceSlot(offset: number): WaterSurfaceSlot | undefined {
  return (ENEMY_WATER_SURFACE_SLOTS.mm as Readonly<Record<string, WaterSurfaceSlot>>)[String(offset)];
}

const aquaticFacts = placementData.aquatic;

function rootedBioBaba(game: string, actorId: number, params?: number): boolean {
  return enemySpawnMacroMatches('rooted-bio-baba', { target: { game, actorId, params } });
}

export function mmAquaticTarget(game: string, actorId: number, params?: number): boolean {
  return enemySpawnMacroMatches('mm-aquatic', { target: { game, actorId, params } });
}

export function mmAquaticSlot(offset: number) {
  if (aquaticFacts.rejected.includes(offset))
    return undefined;
  let match: (typeof aquaticFacts.slots)[number] | undefined;
  for (const row of aquaticFacts.slots) {
    if (row.offset !== offset)
      continue;
    if (match && match.spawnY !== row.spawnY)
      return undefined;
    match ??= row;
  }
  return match;
}

type OotEnemyRule = {
  section: string;
  key: string;
  expectedRule: string;
  enemyRule?: string;
  template: string;
  requirements?: Readonly<Record<string, string>>;
  vanillaRequirements?: Readonly<Record<string, string>>;
  selectors: Readonly<Record<string, EnemyRoomSlots>>;
};

const OOT_ROOM_BINDINGS = ootRoomBindings as unknown as Readonly<Record<string, {
  rules?: readonly OotEnemyRule[];
  encounters?: readonly {
    source: string; rules: readonly {
      section: string; key: string; expectedRule: string; template: string;
    }[]
  }[];
}>>;

function selectorPairs(selector: readonly number[], plan: EnemyPlan, mq: boolean): EnemyReplacement[] {
  const matches = (position: readonly number[]) => selector.every((value, index) => value === position[index]);
  return plan.filter(({ ootr: ootPlacement }) => {
    if (!ootPlacement || ootPlacement.location.mq !== mq)
      return false;
    const { scene, room, setup, slot } = ootPlacement.location;
    return matches([scene, room, setup, slot]) || ootPlacement.aliases.some(matches);
  });
}

function ootEnemizerEnemyDependency(rule: string, plan: EnemyPlan, mq: boolean, selectors: OotEnemyRule['selectors']): {
  expression: string;
  changed: boolean;
} {
  let changed = false;
  const select = (selector: readonly number[]) => {
    const names = selectorPairs(selector, plan, mq)
      .map(enemyPlacementLogicName).filter((name): name is string => !!name);
    if (names.length) {
      changed = true;
      return `(${[...new Set(names)].join(' && ')})`;
    }

    const key = [+mq, ...selector].join('/');
    const native = OOT_VANILLA_SLOT_LOGIC[key];
    if (!native) throw Error('Missing native room-clear slot logic: ' + key);
    return `(${native})`;
  };
  const expression = rule.replace(/can_(?:clear_room|kill_named)\(\s*(['"]?)([^()'"]+)\1\s*\)/g, (_, _quote, name: string) => {
    const room = selectors[name.trim()];
    if (!room)
      throw Error('Missing enemy slot selector: ' + name);
    return '(' + (room.slots.map(slot => select([room.scene, room.room, room.setup, slot])).join(' && ') || 'true') + ')';
  }).replace(/\band\b/g, '&&').replace(/\bor\b/g, '||').replace(/\bnot\b/g, '!');
  if (/\bcan_(clear_room|kill_named)\b/.test(expression) || /[^A-Za-z_0-9\s()!&,|]/.test(expression))
    throw Error('Invalid upstream enemy dependency: ' + rule);
  return { expression, changed };
}

export function ootEnemizerWorldLogic<T extends Record<string, any>>(game: Game, areaName: string, mq: boolean, rawArea: T, plan: EnemyPlan): T {
  if (game !== 'oot' || !plan.some(row => row.ootr || scopedEnemyEncounter(row)))
    return rawArea;
  const name = areaName.startsWith('OOT ') ? areaName.slice(4) : areaName;
  let result = rawArea;
  if (!mq && name === 'Forest Temple Mini-Boss 2') {
    const fight = plan.filter(row => row.kind === 'forest-bow');
    if (fight.length) {
      const combat = conjunction(fight);
      const rule = fight.length === OOT_FOREST_BOW_SLOTS.length ? combat :
        `(${combat}) && can_use_sword && soul_enemy(SOUL_ENEMY_STALFOS)`;
      result = {
        ...rawArea,
        exits: { ...rawArea.exits, 'Forest Temple Poe 2': rule },
        locations: { ...rawArea.locations, 'Forest Temple Bow': rule },
      };
    }
  }
  for (const binding of OOT_ROOM_BINDINGS[(mq ? 'mq/' : 'oot/') + name]?.rules ?? []) {
    const old = rawArea[binding.section]?.[binding.key];
    if (old === undefined)
      continue;
    if (old !== binding.expectedRule)
      throw Error('Enemy world rule drift: ' + name + '/' + binding.section + '/' + binding.key);
    const requirements = binding.requirements ?? { ENEMY: binding.enemyRule! };
    let changed = false;
    let replacement = binding.template;
    for (const [key, rule] of Object.entries(requirements)) {
      const compiled = ootEnemizerEnemyDependency(rule, plan, mq, binding.selectors);
      const vanilla = binding.vanillaRequirements?.[key];
      changed ||= compiled.changed;
      const expression = !compiled.changed && vanilla !== undefined ? vanilla : compiled.expression;
      replacement = replacement.split('$' + key).join(`(${expression})`);
    }
    if (!changed)
      continue;
    if (/\$[A-Z_]+/.test(replacement))
      throw Error('Unresolved enemy world dependency placeholder');
    if (result === rawArea)
      result = { ...rawArea };
    (result as Record<string, any>)[binding.section] = { ...result[binding.section], [binding.key]: replacement };
  }
  if (!mq) for (const encounter of OOT_ROOM_BINDINGS['oot/' + name]?.encounters ?? []) {
    const row = plan.find(row => scopedEnemyEncounter(row) && row.source === encounter.source);
    if (!row) continue;
    for (const rule of encounter.rules) {
      const old = rawArea[rule.section]?.[rule.key];
      if (old !== rule.expectedRule) throw Error('OoT encounter rule drift: ' + name + '/' + rule.key);
      const replacement = rule.template.split('$ENEMY').join('(' + enemyPlacementLogicName(row) + ')');
      if (result === rawArea) result = { ...rawArea };
      (result as Record<string, any>)[rule.section] = { ...result[rule.section], [rule.key]: replacement };
    }
  }
  return result;
}

export function enemyGoldLocationLogic(game: Game, locations: Readonly<Record<string, string>>, plan: EnemyPlan,
  originalLocations: Readonly<Record<string, string>> = locations): Record<string, string> {
  const result = { ...locations };
  for (const row of plan) {
    if (row.kind !== 'gold') continue;
    const source = ENEMY_GOLD_SOURCES.find(s => s.key === row.source)!;
    if (source.game !== game || result[source.location] === undefined) continue;
    const original = originalLocations[source.location];
    const binding = ENEMY_GOLD_ACCESS[game + '/' + source.location]?.find(rule => rule.expectedRule === original);
    if (!binding) throw Error('Gold Skulltula access rule drift: ' + game + '/' + source.location);
    if (binding.access !== '$RULE' && result[source.location] !== original)
      throw Error('Unreviewed combined enemy/Gold rule: ' + game + '/' + source.location);
    const access = binding.access === '$RULE' ? result[source.location] : binding.access;
    result[source.location] = `(${access}) && ${enemyPlacementLogicName(row)}`;
  }
  return result;
}
