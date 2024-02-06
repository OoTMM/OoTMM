import { readFileSync, writeFileSync, mkdirSync } from 'fs';
import { join } from 'path';
import { parse as parseYaml } from 'yaml';
import * as CSV from 'csv/sync';

function emit(name: string, filename: string, data: any) {
  const dir = join(__dirname, '..', 'dist');
  mkdirSync(dir, { recursive: true });
  writeFileSync(join(dir, `${filename}.json`), JSON.stringify(data));
}

function loadYaml(name: string): any {
  const file = readFileSync(join(__dirname, name), 'utf8');
  return parseYaml(file);
}

function loadTxt(name: string): string {
  return readFileSync(join(__dirname, name), 'utf8');
}

function loadCsv(name: string): any[] {
  const content = readFileSync(join(__dirname, name), "utf8");
  return CSV.parse(content, { columns: true, skip_empty_lines: true, trim: true });
}

const DATA_WORLD = {
  oot: {
    overworld: loadYaml('world/oot/overworld.yml'),
    boss: loadYaml('world/oot/boss.yml'),
    DT: loadYaml('world/oot/deku_tree.yml'),
    DC: loadYaml('world/oot/dodongo_cavern.yml'),
    JJ: loadYaml('world/oot/jabu_jabu.yml'),
    Forest: loadYaml('world/oot/forest_temple.yml'),
    Fire: loadYaml('world/oot/fire_temple.yml'),
    Water: loadYaml('world/oot/water_temple.yml'),
    Spirit: loadYaml('world/oot/spirit_temple.yml'),
    Shadow: loadYaml('world/oot/shadow_temple.yml'),
    BotW: loadYaml('world/oot/bottom_of_the_well.yml'),
    IC: loadYaml('world/oot/ice_cavern.yml'),
    Gerudo: loadYaml('world/oot/gerudo_fortress.yml'),
    GTG: loadYaml('world/oot/gerudo_training_grounds.yml'),
    Ganon: loadYaml('world/oot/ganon_castle.yml'),
    Ganon2: loadYaml('world/oot/ganon_tower.yml'),
    TCG: loadYaml('world/oot/treasure_chest_game.yml'),
  },
  mq: {
    DT: loadYaml('world/mq/deku_tree_mq.yml'),
    DC: loadYaml('world/mq/dodongo_cavern_mq.yml'),
    JJ: loadYaml('world/mq/jabu_jabu_mq.yml'),
    Forest: loadYaml('world/mq/forest_temple_mq.yml'),
    Fire: loadYaml('world/mq/fire_temple_mq.yml'),
    Water: loadYaml('world/mq/water_temple_mq.yml'),
    Spirit: loadYaml('world/mq/spirit_temple_mq.yml'),
    Shadow: loadYaml('world/mq/shadow_temple_mq.yml'),
    BotW: loadYaml('world/mq/bottom_of_the_well_mq.yml'),
    IC: loadYaml('world/mq/ice_cavern_mq.yml'),
    GTG: loadYaml('world/mq/gerudo_training_grounds_mq.yml'),
    Ganon: loadYaml('world/mq/ganon_castle_mq.yml'),
  },
  mm: {
    overworld: loadYaml('world/mm/overworld.yml'),
    WF: loadYaml('world/mm/woodfall_temple.yml'),
    SH: loadYaml('world/mm/snowhead_temple.yml'),
    GB: loadYaml('world/mm/great_bay_temple.yml'),
    ST: loadYaml('world/mm/stone_tower_temple.yml'),
    IST: loadYaml('world/mm/stone_tower_temple_inverted.yml'),
    PF: loadYaml('world/mm/pirate_fortress.yml'),
    ACoI: loadYaml('world/mm/ancient_castle_of_ikana.yml'),
    BtW: loadYaml('world/mm/beneath_the_well.yml'),
    SS: loadYaml('world/mm/secret_shrine.yml'),
    SSH: loadYaml('world/mm/swamp_spider_house.yml'),
    OSH: loadYaml('world/mm/ocean_spider_house.yml'),
    Moon: loadYaml('world/mm/moon.yml'),
  }
};

const DATA_FILES = {
  oot: loadTxt('files/files-oot.txt').trim().split('\n'),
  mm: loadTxt('files/files-mm.txt').trim().split('\n'),
};

const MACROS = {
  common: loadYaml('macros/macros_common.yml'),
  oot: loadYaml('macros/macros_oot.yml'),
  mm: loadYaml('macros/macros_mm.yml'),
};

const POOL = {
  oot: loadCsv('pool/pool_oot.csv'),
  mm: loadCsv('pool/pool_mm.csv'),
};

const HINTS_DATA = {
  oot: loadCsv('hints/hints_oot.csv'),
  mm: loadCsv('hints/hints_mm.csv'),
};

emit('WORLD', 'data-world', DATA_WORLD);
emit('SCENES', 'data-scenes', loadYaml('defs/scenes.yml'));
emit('NPC', 'data-npc', loadYaml('defs/npc.yml'));
emit('REGIONS', 'data-regions', loadYaml('defs/regions.yml'));
emit('HINTS', 'data-hints', loadYaml('defs/hints.yml'));
emit('ENTRANCES', 'data-entrances', loadYaml('defs/entrances.yml'));
emit('RAW_GI', 'data-gi', loadYaml('defs/gi.yml'));
emit('RAW_DRAWGI', 'data-drawgi', loadYaml('defs/drawgi.yml'));
emit('FILES', 'data-files', DATA_FILES);
emit('MACROS', 'data-macros', MACROS);
emit('POOL', 'data-pool', POOL);
emit('RAW_HINTS_DATA', 'data-hints-raw', HINTS_DATA);
