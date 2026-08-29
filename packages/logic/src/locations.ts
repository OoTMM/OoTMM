import type { Settings } from '@ootmm/core';
import type { PlayerItem } from './items';
import type { ItemPlacement, Location } from './types';
import type { ResolvedWorldFlags, World } from './world';

import { CHECKS, CHECKS_BY_LOCATION } from '@ootmm/core';
import { ItemHelpers } from './items';

type LocationDescriptor = {
  id: string;
  world: null | number;
}
const locationRegistry: {[k: Location]: LocationDescriptor} = {};

export function makeLocation(id: string, world?: null | number): Location {
  const w = world === undefined ? null : world;
  const x = (id + ((w === null) ? '' : `@${world}`)) as Location;
  const data = locationRegistry[x];
  if (!data) {
    locationRegistry[x] = { id, world: w };
  }
  return x;
}

export function locationData(location: Location): LocationDescriptor {
  return locationRegistry[location];
}

export function makePlayerLocations(settings: Settings, locs: string[]) {
  const result: Location[] = [];
  for (let i = 0; i < settings.players; ++i) {
    result.push(...locs.map(x => makeLocation(x, i)));
  }
  return result;
}

export const ONE_TIME_SHOP_CHECKS = [
  'MM Bomb Shop Bomb Bag',
  'MM Bomb Shop Bomb Bag 2',
  'MM Curiosity Shop All-Night Mask',
];

export const MM_SCRUBS = [
  'MM Southern Swamp Scrub Shop',
  'MM Zora Hall Scrub Shop',
  'MM Ikana Valley Scrub Shop',
];

export const OOT_MERCHANTS = [
  'OOT Haunted Wasteland Carpet Merchant',
  'OOT Kakariko Potion Shop Buy Blue Potion',
  'OOT Lon Lon Ranch Talon Buy Milk',
];

export const MM_MERCHANTS = [
  'MM Gorman Track Milk Purchase',
  'MM Milk Bar Purchase Milk',
  'MM Milk Bar Purchase Chateau',
];

export const OOT_ONE_TIME_SCRUBS = [
  'OOT Lost Woods Scrub Sticks Upgrade',
  'OOT Lost Woods Grotto Scrub Nuts Upgrade',
  'OOT Hyrule Field Grotto Scrub HP',
];

export const OOT_FROGS = [
  'OOT Zora River Frogs Zeldas Lullaby',
  'OOT Zora River Frogs Eponas Song',
  'OOT Zora River Frogs Sarias Song',
  'OOT Zora River Frogs Suns Song',
  'OOT Zora River Frogs Song of Time'
];

export const MM_LOTTERY = [
  'MM Lottery Prize Night 1',
  'MM Lottery Prize Night 2',
  'MM Lottery Prize Night 3',
];

const MM_TINGLE = [
  'MM Tingle Map Clock Town',
  'MM Tingle Map Woodfall',
  'MM Tingle Map Snowhead',
  'MM Tingle Map Ranch',
  'MM Tingle Map Great Bay',
  'MM Tingle Map Ikana',
];

const RENEWABLE_LOCATIONS = new Set([
  ...MM_SCRUBS,
  ...MM_MERCHANTS,
  ...MM_TINGLE,
  ...OOT_MERCHANTS,
  ...CHECKS.filter(x => ['shop', 'cow', 'scrub', 'fairy', 'fish', 'fairy_spot'].includes(x.type) && !ONE_TIME_SHOP_CHECKS.includes(x.location)).map(x => x.location),
]);

export function locationsZelda(settings: Settings) {
  const locs = ['OOT Zelda\'s Letter', 'OOT Zelda\'s Song'];
  if (settings.shuffleWonderItemsOot === 'all' || settings.shuffleWonderItemsOot === 'overworld') {
    locs.push('OOT Castle Courtyard Wonder Item');
  }
  return locs;
}

export function isLocationRenewable(loc: Location) {
  const locationId = locationData(loc).id;
  return RENEWABLE_LOCATIONS.has(locationId);
}

export function isLocationLicenseGranting(world: World, loc: Location) {
  const locationId = locationData(loc).id;
  if (ONE_TIME_SHOP_CHECKS.includes(locationId) || MM_MERCHANTS.includes(locationId) || OOT_MERCHANTS.includes(locationId)) {
    return false;
  }
  const check = CHECKS_BY_LOCATION[locationId];
  if (['cow'].includes(check.type))
    return true;
  if (isLocationRenewable(loc))
    return false;
  return true;
}

export function isLocationChestFairy(world: World, loc: Location) {
  const locD = locationData(loc);
  const check = CHECKS_BY_LOCATION[locD.id];
  const item = world.checkItems.get(locD.id)!;
  return check.type !== 'sf' && ItemHelpers.isDungeonStrayFairy(item);
}

export function isLocationOtherFairy(world: World, loc: Location) {
  const locD = locationData(loc);
  const check = CHECKS_BY_LOCATION[locD.id];
  const item = world.checkItems.get(locD.id)!;
  return (check.type === 'sf') && ItemHelpers.isDungeonStrayFairy(item);
}

type IsLocationFullyShuffledOptions = {
  noPlando?: boolean;
  songs?: boolean;
  rewards?: boolean;
};

const DUNGEON_SCENES = [
  'OOT_DEKU_TREE', 'OOT_DEKU_TREE_MQ', 'OOT_LAIR_GOHMA',
  'OOT_DODONGO_CAVERN', 'OOT_DODONGO_CAVERN_MQ', 'OOT_LAIR_KING_DODONGO',
  'OOT_INSIDE_JABU_JABU', 'OOT_INSIDE_JABU_JABU_MQ', 'OOT_LAIR_BARINADE',
  'OOT_TEMPLE_FOREST', 'OOT_TEMPLE_FOREST_MQ', 'OOT_LAIR_PHANTOM_GANON',
  'OOT_TEMPLE_FIRE', 'OOT_TEMPLE_FIRE_MQ', 'OOT_LAIR_VOLVAGIA',
  'OOT_TEMPLE_WATER', 'OOT_TEMPLE_WATER_MQ', 'OOT_LAIR_MORPHA',
  'OOT_TEMPLE_SPIRIT', 'OOT_TEMPLE_SPIRIT_MQ', 'OOT_LAIR_TWINROVA',
  'OOT_TEMPLE_SHADOW', 'OOT_TEMPLE_SHADOW_MQ', 'OOT_LAIR_BONGO_BONGO',
  'OOT_GANON_TOWER', 'OOT_LAIR_GANONDORF',
  'OOT_BOTTOM_OF_THE_WELL', 'OOT_BOTTOM_OF_THE_WELL_MQ',
  'OOT_ICE_CAVERN', 'OOT_ICE_CAVERN_MQ',
  'OOT_GERUDO_TRAINING_GROUND', 'OOT_GERUDO_TRAINING_GROUND_MQ', 'OOT_THIEVES_HIDEOUT',
  'OOT_INSIDE_GANON_CASTLE', 'OOT_INSIDE_GANON_CASTLE_MQ',

  'MM_PIRATE_FORTRESS_EXTERIOR', 'MM_PIRATE_FORTRESS_INTERIOR', 'MM_PIRATE_FORTRESS_ENTRANCE',
  'MM_TEMPLE_STONE_TOWER', 'MM_TEMPLE_STONE_TOWER_INVERTED', 'MM_LAIR_TWINMOLD',
  'MM_TEMPLE_WOODFALL', 'MM_LAIR_ODOLWA',
  'MM_CASTLE_IKANA', 'MM_LAIR_IKANA',
  'MM_TEMPLE_SNOWHEAD', 'MM_LAIR_GOHT',
  'MM_SPIDER_HOUSE_SWAMP', 'MM_SPIDER_HOUSE_OCEAN',
  'MM_MOON', 'MM_LAIR_MAJORA', 'MM_MOON_DEKU', 'MM_MOON_GORON', 'MM_MOON_ZORA', 'MM_MOON_LINK',
  'MM_TEMPLE_GREAT_BAY', 'MM_LAIR_GYORG',
  'MM_BENEATH_THE_WELL', 'MM_SECRET_SHRINE',
]

export function isLocationInDungeon(scene: string) {
  return DUNGEON_SCENES.includes(scene);
}

export function isLocationFullyShuffled(settings: Settings, fixedLocations: Set<Location>, items: ItemPlacement, plandoLocations: Map<Location, PlayerItem>, loc: Location, opts: IsLocationFullyShuffledOptions): boolean {
  if (fixedLocations.has(loc)) {
    return false;
  }

  const item = items.get(loc)!;

  /* Plando */
  if (opts.noPlando && plandoLocations.has(loc)) {
    return false;
  }

  /* Non-shuffled hideout keys */
  if ((ItemHelpers.isSmallKeyHideout(item.item) || ItemHelpers.isKeyRingHideout(item.item)) && settings.smallKeyShuffleHideout !== 'anywhere') {
    return false;
  }

  /* Non-shuffled TCG keys */
  if ((ItemHelpers.isSmallKeyTCG(item.item) || ItemHelpers.isKeyRingTCG(item.item)) && settings.smallKeyShuffleChestGame !== 'anywhere') {
    return false;
  }

  /* Non-shuffled regular keys */
  if ((ItemHelpers.isSmallKeyRegularOot(item.item) || ItemHelpers.isKeyRingRegularOot(item.item)) && settings.smallKeyShuffleOot !== 'anywhere') {
    return false;
  }
  if ((ItemHelpers.isSmallKeyRegularMm(item.item) || ItemHelpers.isKeyRingRegularMm(item.item)) && settings.smallKeyShuffleMm !== 'anywhere') {
    return false;
  }

  /* Non-shuffled silver rupees */
  if (ItemHelpers.isSilverRupee(item.item) && settings.silverRupeeShuffle !== 'anywhere') {
    return false;
  }

  /* Non-shuffled Ganon BK (doesn't really matter) */
  if (ItemHelpers.isGanonBossKey(item.item) && settings.ganonBossKey !== 'anywhere') {
    return false;
  }

  /* Non shuffled boss keys */
  if (ItemHelpers.isRegularBossKeyOot(item.item) && settings.bossKeyShuffleOot !== 'anywhere') {
    return false;
  }

  if (ItemHelpers.isRegularBossKeyMm(item.item) && settings.bossKeyShuffleMm !== 'anywhere') {
    return false;
  }

  /* Non shuffled town fairy */
  if (ItemHelpers.isTownStrayFairy(item.item) && settings.townFairyShuffle === 'vanilla') {
    return false;
  }

  /* Non shuffled dungeon stray fairy */
  if (ItemHelpers.isDungeonStrayFairy(item.item) && settings.strayFairyChestShuffle !== 'anywhere' && settings.strayFairyOtherShuffle !== 'anywhere') {
    return false;
  }

  /* Non-shuffled map/compass (doesn't really matter) */
  if (ItemHelpers.isMapCompass(item.item) && settings.mapCompassShuffle !== 'anywhere') {
    return false;
  }

  /* Non-shuffled dungeon reward */
  if (ItemHelpers.isDungeonReward(item.item) && settings.dungeonRewardShuffle === 'dungeonBlueWarps') {
    return false;
  }

  /* Songs */
  if (opts.songs) {
    if (ItemHelpers.isSong(item.item) && settings.songs !== 'anywhere') {
      return false;
    }
  }

  /* Rewards */
  if (opts.rewards) {
    if (ItemHelpers.isDungeonReward(item.item) && settings.dungeonRewardShuffle !== 'anywhere') {
      return false;
    }
  }

  return true;
}

export function getPreActivatedOwlsLocations(flags: ResolvedWorldFlags): string[] {
  let preActivatedOwls: string[] = [];
  if (flags.mmPreActivatedOwls.has('clocktown')) preActivatedOwls.push('MM Clock Town Owl Statue');
  if (flags.mmPreActivatedOwls.has('milkroad'))  preActivatedOwls.push('MM Milk Road Owl Statue');
  if (flags.mmPreActivatedOwls.has('swamp'))     preActivatedOwls.push('MM Southern Swamp Owl Statue');
  if (flags.mmPreActivatedOwls.has('woodfall'))  preActivatedOwls.push('MM Woodfall Owl Statue');
  if (flags.mmPreActivatedOwls.has('mountain'))  preActivatedOwls.push('MM Mountain Village Owl Statue');
  if (flags.mmPreActivatedOwls.has('snowhead'))  preActivatedOwls.push('MM Snowhead Owl Statue');
  if (flags.mmPreActivatedOwls.has('greatbay'))  preActivatedOwls.push('MM Great Bay Coast Owl Statue');
  if (flags.mmPreActivatedOwls.has('zoracape'))  preActivatedOwls.push('MM Zora Cape Owl Statue');
  if (flags.mmPreActivatedOwls.has('canyon'))    preActivatedOwls.push('MM Ikana Canyon Owl Statue');
  if (flags.mmPreActivatedOwls.has('tower'))     preActivatedOwls.push('MM Stone Tower Owl Statue');
  return preActivatedOwls;
}
