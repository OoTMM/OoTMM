import type { GossipDefinition } from '../src/gossips';

import { gameId } from '../src/util';
import { GAMES } from '../src/defines';
import { loadCsv, loadTxt, loadYaml, emit } from './helpers';

const DATA_WORLD = {
  oot: {
    overworld: loadYaml('world/oot/overworld/**/*.yml'),
    boss: loadYaml('world/oot/boss/**/*.yml'),
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
    overworld: loadYaml('world/mm/overworld/**/*.yml'),
    boss: loadYaml('world/mm/boss/**/*.yml'),
    WF: loadYaml('world/mm/dungeons/woodfall_temple.yml'),
    SH: loadYaml('world/mm/dungeons/snowhead_temple.yml'),
    GB: loadYaml('world/mm/dungeons/great_bay_temple.yml'),
    ST: loadYaml('world/mm/dungeons/stone_tower_temple.yml'),
    IST: loadYaml('world/mm/dungeons/stone_tower_temple_inverted.yml'),
    PF: loadYaml('world/mm/dungeons/pirate_fortress.yml'),
    ACoI: loadYaml('world/mm/dungeons/ancient_castle_of_ikana.yml'),
    BtW: loadYaml('world/mm/dungeons/beneath_the_well.yml'),
    SS: loadYaml('world/mm/dungeons/secret_shrine.yml'),
    SSH: loadYaml('world/mm/dungeons/swamp_spider_house.yml'),
    OSH: loadYaml('world/mm/dungeons/ocean_spider_house.yml'),
    Moon: loadYaml('world/mm/dungeons/moon.yml'),
    DekuPalace: {},
  },
  mm_us: {
    DekuPalace: loadYaml('world/mm/us/deku_palace_us.yml'),
  },
  mm_jp: {
    DekuPalace: loadYaml('world/mm/jp/deku_palace_jp.yml'),
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

function buildGossips() {
  const raw = {
    oot: loadCsv('gossips/gossips_oot.csv'),
    mm: loadCsv('gossips/gossips_mm.csv'),
  };

  let result: GossipDefinition[] = [];
  for (const game of GAMES) {
    const hints = raw[game];
    for (const hint of hints) {
      const location = gameId(game, hint.location, ' ');
      const id = parseInt(hint.id);
      if (isNaN(id)) {
        throw new Error(`Invalid hint ID for ${location}: ${hint.id}`);
      }
      result.push({
        game,
        location,
        type: hint.type,
        id,
      });
    }
  }

  emit('data-gossips', result);
}

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

buildGossips();
