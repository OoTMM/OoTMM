import { default as DATA_ENTRANCES } from '../dist/data-entrances.json';

export { default as WORLD } from '../dist/data-world.json';
export { default as SCENES } from '../dist/data-scenes.json';
export { default as NPC } from '../dist/data-npc.json';
export { default as REGIONS } from '../dist/data-regions.json';
export { default as HINTS } from '../dist/data-hints.json';
export { default as RAW_GI } from '../dist/data-gi.json';
export { default as RAW_DRAWGI } from '../dist/data-drawgi.json';
export { default as FILES } from '../dist/data-files.json';
export { default as MACROS } from '../dist/data-macros.json';
export { default as POOL } from '../dist/data-pool.json';
export { default as RAW_HINTS_DATA } from '../dist/data-hints-raw.json';

export type EntranceData = {
  game: 'oot' | 'mm';
  id: number;
  type: 'none' | 'boss' | 'dungeon' | 'region' | 'region-extra' | 'overworld' | 'one-way-statue' | 'wallmaster';
  from: string;
  to: string;
  flags: string[];
  debug?: any;
};

export type Entrance = keyof typeof DATA_ENTRANCES;

export const ENTRANCES = Object.fromEntries(Object.entries(DATA_ENTRANCES).map(([k, e]) => {
  const data = Object.assign({}, e) as any;
  if (data.flags === undefined) {
    data.flags = [];
  }
  if (data.areas) {
    data.from = data.areas[0];
    data.to = data.areas[1];
    delete data.areas;
  } else {
    data.from = 'NONE';
    data.to = 'NONE';
  }
  return [k, data];
})) as Record<Entrance, EntranceData>;
