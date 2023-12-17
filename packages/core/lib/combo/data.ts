import { isArray } from 'lodash';

export { default as DATA_SCENES } from '../../data/scenes.yml';
export { default as DATA_NPC } from '../../data/npc.yml';
export { default as DATA_REGIONS } from '../../data/regions.yml';
export { default as DATA_HINTS } from '../../data/hints.yml';
export { default as DATA_ENTRANCES } from '../../data/entrances.yml';

import poolOot from '../../data/oot/pool.csv';
import poolMm from '../../data/mm/pool.csv';

export const DATA_POOL = {
  oot: poolOot,
  mm: poolMm,
};

import macrosOot from '../../data/oot/macros.yml';
import macrosMm from '../../data/mm/macros.yml';
import macrosCommon from '../../data/macros.yml';

export const DATA_MACROS = {
  oot: macrosOot,
  mm: macrosMm,
  common: macrosCommon,
};

import entrancesOot from '../../data/oot/entrances.csv';
import entrancesMm from '../../data/mm/entrances.csv';

export const DATA_ENTRANCES_POOL = {
  oot: entrancesOot,
  mm: entrancesMm,
};

import rawGi from '../../data/gi.yml';

type DataGi = {
  index: number;
  id: string;
  item: string;
  flags: number;
  draw: string;
  drawParam: number;
  addFunc: string;
  addParam: string;
  object: { type: 'oot' | 'mm', id: number } | { type: 'custom', id: string } | null;
  name: string;
};

export let DATA_GI: {[k: string]: DataGi} = {};
for (let i = 0; i < rawGi.length; ++i) {
  const v = rawGi[i];
  const index = i + 1;
  const id = v.id;
  const item = `ITEM_${v.item || 'NONE'}`;
  const flags = v.flags;
  let draw: string;
  let drawParam: number;
  if (v.draw) {
    if (isArray(v.draw)) {
      draw = v.draw[0];
      drawParam = v.draw[1];
    } else {
      draw = v.draw;
      drawParam = 0;
    }
  } else {
    draw = 'NONE';
    drawParam = 0;
  }
  draw = `DRAWGI_${draw}`;
  let object: DataGi['object'];
  if (v.object) {
    if (isArray(v.object)) {
      object = { type: v.object[0], id: v.object[1] };
    } else {
      object = { type: 'custom', id: v.object };
    }
  } else {
    object = null;
  }
  const name = v.name || "";
  let addFunc: string;
  let addParam: string;
  if (v.add) {
    if (isArray(v.add)) {
      addFunc = v.add[0];
      addParam = v.add[1].toString();
    } else {
      addFunc = v.add;
      addParam = '0';
    }
  } else {
    addFunc = 'NONE';
    addParam = '0';
  }
  DATA_GI[id] = { index, id, item, flags, draw, drawParam, object, name, addFunc, addParam };
}

import worldOotOverworld from '../../data/oot/world/overworld.yml';
import worldOotBoss from '../../data/oot/world/boss.yml';
import worldOotDekuTree from '../../data/oot/world/deku_tree.yml';
import worldOotDodongoCavern from '../../data/oot/world/dodongo_cavern.yml';
import worldOotJabuJabu from '../../data/oot/world/jabu_jabu.yml';
import worldOotForestTemple from '../../data/oot/world/forest_temple.yml';
import worldOotFireTemple from '../../data/oot/world/fire_temple.yml';
import worldOotWaterTemple from '../../data/oot/world/water_temple.yml';
import worldOotSpiritTemple from '../../data/oot/world/spirit_temple.yml';
import worldOotShadowTemple from '../../data/oot/world/shadow_temple.yml';
import worldOotBottomOfTheWell from '../../data/oot/world/bottom_of_the_well.yml';
import worldOotIceCavern from '../../data/oot/world/ice_cavern.yml';
import worldOotGerudoFortress from '../../data/oot/world/gerudo_fortress.yml';
import worldOotGerudoTrainingGrounds from '../../data/oot/world/gerudo_training_grounds.yml';
import worldOotGanonCastle from '../../data/oot/world/ganon_castle.yml';
import worldOotGanonTower from '../../data/oot/world/ganon_tower.yml';
import worldOotTreasureChestGame from '../../data/oot/world/treasure_chest_game.yml';

const worldOot = {
  overworld: worldOotOverworld,
  boss: worldOotBoss,
  DT: worldOotDekuTree,
  DC: worldOotDodongoCavern,
  JJ: worldOotJabuJabu,
  Forest: worldOotForestTemple,
  Fire: worldOotFireTemple,
  Water: worldOotWaterTemple,
  Spirit: worldOotSpiritTemple,
  Shadow: worldOotShadowTemple,
  BotW: worldOotBottomOfTheWell,
  IC: worldOotIceCavern,
  Gerudo: worldOotGerudoFortress,
  GTG: worldOotGerudoTrainingGrounds,
  Ganon: worldOotGanonCastle,
  Ganon2: worldOotGanonTower,
  TCG: worldOotTreasureChestGame,
};

import worldMqDekuTree from '../../data/oot/world_mq/deku_tree_mq.yml';
import worldMqDodongoCavern from '../../data/oot/world_mq/dodongo_cavern_mq.yml';
import worldMqJabuJabu from '../../data/oot/world_mq/jabu_jabu_mq.yml';
import worldMqForestTemple from '../../data/oot/world_mq/forest_temple_mq.yml';
import worldMqFireTemple from '../../data/oot/world_mq/fire_temple_mq.yml';
import worldMqWaterTemple from '../../data/oot/world_mq/water_temple_mq.yml';
import worldMqSpiritTemple from '../../data/oot/world_mq/spirit_temple_mq.yml';
import worldMqShadowTemple from '../../data/oot/world_mq/shadow_temple_mq.yml';
import worldMqBottomOfTheWell from '../../data/oot/world_mq/bottom_of_the_well_mq.yml';
import worldMqIceCavern from '../../data/oot/world_mq/ice_cavern_mq.yml';
import worldMqGerudoTrainingGrounds from '../../data/oot/world_mq/gerudo_training_grounds_mq.yml';
import worldMqGanonCastle from '../../data/oot/world_mq/ganon_castle_mq.yml';

const worldMq = {
  DT: worldMqDekuTree,
  DC: worldMqDodongoCavern,
  JJ: worldMqJabuJabu,
  Forest: worldMqForestTemple,
  Fire: worldMqFireTemple,
  Water: worldMqWaterTemple,
  Spirit: worldMqSpiritTemple,
  Shadow: worldMqShadowTemple,
  BotW: worldMqBottomOfTheWell,
  IC: worldMqIceCavern,
  GTG: worldMqGerudoTrainingGrounds,
  Ganon: worldMqGanonCastle,
};

import worldMmOverworld from '../../data/mm/world/overworld.yml';
import worldMmSwampSpiderHouse from '../../data/mm/world/swamp_spider_house.yml';
import worldMmOceanSpiderHouse from '../../data/mm/world/ocean_spider_house.yml';
import worldMmWoodfallTemple from '../../data/mm/world/woodfall_temple.yml';
import worldMmSnowheadTemple from '../../data/mm/world/snowhead_temple.yml';
import worldMmGreatBayTemple from '../../data/mm/world/great_bay_temple.yml';
import worldMmStoneTowerTemple from '../../data/mm/world/stone_tower_temple.yml';
import worldMmStoneTowerTempleInverted from '../../data/mm/world/stone_tower_temple_inverted.yml';
import worldMmPirateFortress from '../../data/mm/world/pirate_fortress.yml';
import worldMmAncientCastleOfIkana from '../../data/mm/world/ancient_castle_of_ikana.yml';
import worldMmBeneathTheWell from '../../data/mm/world/beneath_the_well.yml';
import worldMmSecretShrine from '../../data/mm/world/secret_shrine.yml';
import worldMmMoon from '../../data/mm/world/moon.yml';

const worldMm = {
  overworld: worldMmOverworld,
  WF: worldMmWoodfallTemple,
  SH: worldMmSnowheadTemple,
  GB: worldMmGreatBayTemple,
  ST: worldMmStoneTowerTemple,
  IST: worldMmStoneTowerTempleInverted,
  PF: worldMmPirateFortress,
  ACoI: worldMmAncientCastleOfIkana,
  BtW: worldMmBeneathTheWell,
  SS: worldMmSecretShrine,
  SSH: worldMmSwampSpiderHouse,
  OSH: worldMmOceanSpiderHouse,
  Moon: worldMmMoon,
};

export const DATA_WORLD = {
  oot: worldOot,
  mq: worldMq,
  mm: worldMm,
};

const mapGossip = (game: Game, data: any[]) => {
  const result: {[k: string]: any} = {};
  for (const v of data) {
    const key = gameId(game, v.location, ' ');
    result[key] = { type: v.type, id: parseInt(v.id) }
  }
  return result;
};

import hintsOot from '../../data/oot/hints.csv';
import hintsMm from '../../data/mm/hints.csv';
import { Game } from './config';
import { gameId } from './util';

export const DATA_HINTS_POOL = {
  oot: mapGossip('oot', hintsOot),
  mm: mapGossip('mm', hintsMm),
};
