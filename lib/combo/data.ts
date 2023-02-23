export { default as DATA_GI } from '../../data/gi.yml';
export { default as DATA_SCENES } from '../../data/scenes.yml';
export { default as DATA_NPC } from '../../data/npc.yml';
export { default as DATA_REGIONS } from '../../data/regions.yml';
export { default as DATA_CONFIG } from '../../data/config.yml';
export { default as DATA_HINTS } from '../../data/hints.yml';

import poolOot from '../../data/oot/pool.csv';
import poolMm from '../../data/mm/pool.csv';

export const DATA_POOL = {
  oot: poolOot,
  mm: poolMm,
};

import macrosOot from '../../data/oot/macros.yml';
import macrosMm from '../../data/mm/macros.yml';

export const DATA_MACROS = {
  oot: macrosOot,
  mm: macrosMm,
};

import entrancesOot from '../../data/oot/entrances.csv';
import entrancesMm from '../../data/mm/entrances.csv';

export const DATA_ENTRANCES = {
  oot: entrancesOot,
  mm: entrancesMm,
};

import worldOotOverworld from '../../data/oot/world/overworld.yml';
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

const worldOot = {
  ...worldOotOverworld,
  ...worldOotDekuTree,
  ...worldOotDodongoCavern,
  ...worldOotJabuJabu,
  ...worldOotForestTemple,
  ...worldOotFireTemple,
  ...worldOotWaterTemple,
  ...worldOotSpiritTemple,
  ...worldOotShadowTemple,
  ...worldOotBottomOfTheWell,
  ...worldOotIceCavern,
  ...worldOotGerudoFortress,
  ...worldOotGerudoTrainingGrounds,
  ...worldOotGanonCastle,
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
  ...worldMmOverworld,
  ...worldMmWoodfallTemple,
  ...worldMmSnowheadTemple,
  ...worldMmGreatBayTemple,
  ...worldMmStoneTowerTemple,
  ...worldMmStoneTowerTempleInverted,
  ...worldMmPirateFortress,
  ...worldMmAncientCastleOfIkana,
  ...worldMmBeneathTheWell,
  ...worldMmSecretShrine,
  ...worldMmSwampSpiderHouse,
  ...worldMmOceanSpiderHouse,
  ...worldMmMoon,
};

export const DATA_WORLD = {
  oot: worldOot,
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
