import { loadYaml } from './helpers';

type GameWorldDefinition = {[k in string]: string};

const DATA_OOT = {
  overworld: 'world/oot/overworld/**/*.yml',
  boss: 'world/oot/boss/**/*.yml',
  DT: 'world/oot/dungeons/deku_tree.yml',
  DC: 'world/oot/dungeons/dodongo_cavern.yml',
  JJ: 'world/oot/dungeons/jabu_jabu.yml',
  Forest: 'world/oot/dungeons/forest_temple.yml',
  Fire: 'world/oot/dungeons/fire_temple.yml',
  Water: 'world/oot/dungeons/water_temple.yml',
  Spirit: 'world/oot/dungeons/spirit_temple.yml',
  Shadow: 'world/oot/dungeons/shadow_temple.yml',
  BotW: 'world/oot/dungeons/bottom_of_the_well.yml',
  IC: 'world/oot/dungeons/ice_cavern.yml',
  Gerudo: 'world/oot/dungeons/thieves_hideout.yml',
  GTG: 'world/oot/dungeons/gerudo_training_grounds.yml',
  Ganon: 'world/oot/dungeons/ganon_castle.yml',
  Ganon2: 'world/oot/dungeons/ganon_tower.yml',
  TCG: 'world/oot/dungeons/treasure_chest_game.yml',
};

const DATA_MQ = {
  DT: 'world/oot/dungeons_mq/deku_tree_mq.yml',
  DC: 'world/oot/dungeons_mq/dodongo_cavern_mq.yml',
  JJ: 'world/oot/dungeons_mq/jabu_jabu_mq.yml',
  Forest: 'world/oot/dungeons_mq/forest_temple_mq.yml',
  Fire: 'world/oot/dungeons_mq/fire_temple_mq.yml',
  Water: 'world/oot/dungeons_mq/water_temple_mq.yml',
  Spirit: 'world/oot/dungeons_mq/spirit_temple_mq.yml',
  Shadow: 'world/oot/dungeons_mq/shadow_temple_mq.yml',
  BotW: 'world/oot/dungeons_mq/bottom_of_the_well_mq.yml',
  IC: 'world/oot/dungeons_mq/ice_cavern_mq.yml',
  GTG: 'world/oot/dungeons_mq/gerudo_training_grounds_mq.yml',
  Ganon: 'world/oot/dungeons_mq/ganon_castle_mq.yml',
};

const DATA_MM = {
  overworld: 'world/mm/overworld/**/*.yml',
  boss: 'world/mm/boss/**/*.yml',
  WF: 'world/mm/dungeons/woodfall_temple.yml',
  SH: 'world/mm/dungeons/snowhead_temple.yml',
  GB: 'world/mm/dungeons/great_bay_temple.yml',
  ST: 'world/mm/dungeons/stone_tower_temple.yml',
  IST: 'world/mm/dungeons/stone_tower_temple_inverted.yml',
  PF: 'world/mm/dungeons/pirate_fortress.yml',
  ACoI: 'world/mm/dungeons/ancient_castle_of_ikana.yml',
  BtW: 'world/mm/dungeons/beneath_the_well.yml',
  SS: 'world/mm/dungeons/secret_shrine.yml',
  SSH: 'world/mm/dungeons/swamp_spider_house.yml',
  OSH: 'world/mm/dungeons/ocean_spider_house.yml',
  Moon: 'world/mm/dungeons/moon.yml',
};

const DATA_MM_US = {
  DekuPalace: 'world/mm/us/deku_palace_us.yml',
};

const DATA_MM_JP = {
  DekuPalace: 'world/mm/jp/deku_palace_jp.yml',
};

async function parseDefinition(def: GameWorldDefinition) {
  const keys = Object.keys(def);
  const results = await Promise.all(keys.map(k => loadYaml(def[k])));
  return Object.fromEntries(keys.map((k, i) => [k, results[i]]));
}

export async function parseWorld() {
  const [oot, mq, mm, mm_us, mm_jp] = await Promise.all([
    parseDefinition(DATA_OOT),
    parseDefinition(DATA_MQ),
    parseDefinition(DATA_MM),
    parseDefinition(DATA_MM_US),
    parseDefinition(DATA_MM_JP),
  ]);

  const data = {
    oot,
    mq,
    mm,
    mm_us,
    mm_jp,
  };

  data.mm.DekuPalace = {};
  return data;
}
