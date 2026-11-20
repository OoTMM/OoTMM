export type * from './types';

export * from './game';
export * from './checks';
export * from './hint-locations';
export * from './animations';
export * from './gossips';
export * from './entrances';

export { default as SCENES } from '../dist/lib/data-scenes.json';
export { default as NPC } from '../dist/lib/data-npc.json';
export { default as MACROS } from '../dist/lib/data-macros.json';
export { default as WORLD } from '../dist/lib/data-world.json';
export { default as REGIONS } from '../dist/lib/data-regions.json';
export { default as HINTS } from '../dist/lib/data-hints.json';
export { default as RAW_GI } from '../dist/lib/data-gi.json';
export { default as RAW_DRAWGI } from '../dist/lib/data-drawgi.json';
export { default as FILES } from '../dist/lib/data-files.json';

export * from './enemy-catalog';
export { ENEMY_SOUNDS_OOT, ENEMY_SOUNDS_MM, SFX_IDS } from './enemy-audio';
export * from './engine-actors';

export * from './foreign-enemy-souls';
export { default as ENEMY_LOGIC_MM_ROOMS } from '../dist/lib/enemy-logic-mm-rooms.json';
export { default as ENEMY_LOGIC_OOT_ROOMS } from '../dist/lib/enemy-logic-oot-rooms.json';
export * from './enemy-spawn-rules';
export { default as ENEMY_SPAWN_RULES } from '../dist/lib/enemy-spawn-rules.json';
export { default as ENEMY_SPAWN_OOT_TARGETS } from '../dist/lib/enemy-spawn-oot-targets.json';
export { default as ENEMY_SPAWN_OOT_LOCATIONS } from '../dist/lib/enemy-spawn-oot-locations.json';
export { default as ENEMY_SPAWN_OOT_TEMPLATES } from '../dist/lib/enemy-spawn-oot-templates.json';
export { default as ENEMY_SPAWN_MM_ACTORS } from '../dist/lib/enemy-spawn-mm-actors.json';
export { default as ENEMY_SPAWN_ACTOR_TYPES } from '../dist/lib/enemy-spawn-actor-types.json';
