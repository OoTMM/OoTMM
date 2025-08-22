import { readFileSync, writeFileSync, mkdirSync, globSync } from 'fs';
import { join } from 'path';
import { parse as parseYaml } from 'yaml';
import * as CSV from 'csv/sync';

function emit(filename: string, data: any) {
  const dir = join(__dirname, '..', 'dist');
  mkdirSync(dir, { recursive: true });
  writeFileSync(join(dir, `${filename}.json`), JSON.stringify(data));
}

function loadYaml(patterns: string | string[]): any {
  patterns = Array.isArray(patterns) ? patterns : [patterns];
  let data: any = null;

  for (const p of patterns) {
    const matchedFiles = globSync(p, { cwd: __dirname });
    if (matchedFiles.length === 0) {
      throw new Error(`No files matched the pattern: ${p}`);
    }
    for (const name of matchedFiles) {
      const file = readFileSync(join(__dirname, name), 'utf8');
      const parsed = parseYaml(file);

      if (data === null) {
        data = parsed;
      } else if (Array.isArray(data) && Array.isArray(parsed)) {
        data.push(...parsed);
      } else if (typeof data === 'object' && typeof parsed === 'object') {
        data = { ...data, ...parsed };
      } else {
        throw new Error(`Incompatible YAML structures in files: existing type ${typeof data}, new type ${typeof parsed}`);
      }
    }
  }
  return data;
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
    overworld: loadYaml('world/oot/overworld/**/*.yml'),
    boss: loadYaml('world/oot/boss.yml'),
    DT: loadYaml('world/oot/dungeons/deku_tree.yml'),
    DC: loadYaml('world/oot/dungeons/dodongo_cavern.yml'),
    JJ: loadYaml('world/oot/dungeons/jabu_jabu.yml'),
    Forest: loadYaml('world/oot/dungeons/forest_temple.yml'),
    Fire: loadYaml('world/oot/dungeons/fire_temple.yml'),
    Water: loadYaml('world/oot/dungeons/water_temple.yml'),
    Spirit: loadYaml('world/oot/dungeons/spirit_temple.yml'),
    Shadow: loadYaml('world/oot/dungeons/shadow_temple.yml'),
    BotW: loadYaml('world/oot/dungeons/bottom_of_the_well.yml'),
    IC: loadYaml('world/oot/dungeons/ice_cavern.yml'),
    Gerudo: loadYaml('world/oot/dungeons/gerudo_fortress.yml'),
    GTG: loadYaml('world/oot/dungeons/gerudo_training_grounds.yml'),
    Ganon: loadYaml('world/oot/dungeons/ganon_castle.yml'),
    Ganon2: loadYaml('world/oot/dungeons/ganon_tower.yml'),
    TCG: loadYaml('world/oot/dungeons/treasure_chest_game.yml'),
  },
  mq: {
    DT: loadYaml('world/oot/dungeons_mq/deku_tree_mq.yml'),
    DC: loadYaml('world/oot/dungeons_mq/dodongo_cavern_mq.yml'),
    JJ: loadYaml('world/oot/dungeons_mq/jabu_jabu_mq.yml'),
    Forest: loadYaml('world/oot/dungeons_mq/forest_temple_mq.yml'),
    Fire: loadYaml('world/oot/dungeons_mq/fire_temple_mq.yml'),
    Water: loadYaml('world/oot/dungeons_mq/water_temple_mq.yml'),
    Spirit: loadYaml('world/oot/dungeons_mq/spirit_temple_mq.yml'),
    Shadow: loadYaml('world/oot/dungeons_mq/shadow_temple_mq.yml'),
    BotW: loadYaml('world/oot/dungeons_mq/bottom_of_the_well_mq.yml'),
    IC: loadYaml('world/oot/dungeons_mq/ice_cavern_mq.yml'),
    GTG: loadYaml('world/oot/dungeons_mq/gerudo_training_grounds_mq.yml'),
    Ganon: loadYaml('world/oot/dungeons_mq/ganon_castle_mq.yml'),
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
    DekuPalace: {},
  },
  mm_us: {
    DekuPalace: loadYaml('world/mm_us/overworld_deku_palace_us.yml'),
  },
  mm_jp: {
    DekuPalace: loadYaml('world/mm_jp/overworld_deku_palace_jp.yml'),
  },
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

emit('data-world', DATA_WORLD);
emit('data-scenes', loadYaml('defs/scenes.yml'));
emit('data-npc', loadYaml('defs/npc.yml'));
emit('data-regions', loadYaml('defs/regions.yml'));
emit('data-hints', loadYaml('defs/hints.yml'));
emit('data-entrances', loadYaml('defs/entrances.yml'));
emit('data-gi', loadYaml('defs/gi.yml'));
emit('data-drawgi', loadYaml('defs/drawgi.yml'));
emit('data-files', DATA_FILES);
emit('data-macros', MACROS);
emit('data-pool', POOL);
emit('data-hints-raw', HINTS_DATA);
