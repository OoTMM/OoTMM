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
  reverse?: Entrance;
  type: 'none' | 'boss' | 'dungeon' | 'dungeon-exit' | 'region' | 'region-extra' | 'overworld' | 'one-way-statue' | 'wallmaster' | 'spawn';
  from: string;
  to: string;
  flags: string[];
  debug?: any;
  fromMap: string;
  toMap: string;
  fromSubmap: string;
  toSubmap: string;
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
  if (data.maps) {
    data.fromMap = data.maps[0];
    data.toMap = data.maps[1];
    delete data.maps;
  } else {
    data.fromMap = 'NONE';
    data.toMap = 'NONE';
  }

  if (data.submaps) {
    data.fromSubmap = data.submaps[0];
    data.toSubmap = data.submaps[1];
    delete data.submaps;
  } else {
    data.fromSubmap = 'NONE';
    data.toSubmap = 'NONE';
  }
  return [k, data];
})) as Record<Entrance, EntranceData>;

/* Safety */
for (const i1 of Object.keys(ENTRANCES) as Entrance[]) {
  for (const i2 of Object.keys(ENTRANCES) as Entrance[]) {
    if (i1 === i2) {
      continue;
    }
    const e1 = ENTRANCES[i1];
    const e2 = ENTRANCES[i2];
    if (e1.game !== e2.game) {
      continue;
    }
    if (e1.reverse === i2 && e2.reverse !== i1) {
      throw new Error(`Entrance ${i2} reverse should be ${i1}`);
    }
    if (e1.id === e2.id) {
      throw new Error(`Duplicate entrance id: ${i1} and ${i2} (${e1.id.toString(16)})`);
    }
  }
}
