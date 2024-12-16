import { ItemHelpers } from '../items';
import { isDungeonStrayFairy } from '../items/helpers';
import { Settings } from '../settings';
import { ItemPlacement } from './solve';
import { World } from './world';

export type Location = string & { __brand: 'Location' };
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

export function locationsZelda(settings: Settings) {
  const locs = ['OOT Zelda\'s Letter', 'OOT Zelda\'s Song'];
  if (settings.shuffleWonderItemsOot === 'all' || settings.shuffleWonderItemsOot === 'overworld') {
    locs.push('OOT Castle Courtyard Wonder Item');
  }
  return locs;
}

export function isLocationRenewable(world: World, loc: Location) {
  const locationId = locationData(loc).id;
  if (MM_SCRUBS.includes(locationId) || MM_MERCHANTS.includes(locationId))
    return true;
  if (ONE_TIME_SHOP_CHECKS.includes(locationId))
    return false;
  const check = world.checks[locationId];
  if (!check) {
    throw new Error(`Unknown location ${locationId}@${locationData(loc).world}`);
  }
  if (['shop', 'cow', 'scrub', 'fairy', 'fish', 'fairy_spot'].includes(check.type))
    return true;
  if (ItemHelpers.isTingleMap(check.item))
    return true;
  return false;
}

export function isLocationLicenseGranting(world: World, loc: Location) {
  const locationId = locationData(loc).id;
  if (ONE_TIME_SHOP_CHECKS.includes(locationId) || MM_MERCHANTS.includes(locationId)) {
    return false;
  }
  const check = world.checks[locationId];
  if (['cow'].includes(check.type))
    return true;
  if (isLocationRenewable(world, loc))
    return false;
  return true;
}

export function isLocationChestFairy(world: World, loc: Location) {
  const locD = locationData(loc);
  const check = world.checks[locD.id];
  return check.type !== 'sf' && ItemHelpers.isDungeonStrayFairy(check.item);
}

export function isLocationOtherFairy(world: World, loc: Location) {
  const locD = locationData(loc);
  const check = world.checks[locD.id];
  return (check.type === 'sf') && ItemHelpers.isDungeonStrayFairy(check.item);
}

type IsLocationFullyShuffledOptions = {
  noPlando?: boolean;
  songs?: boolean;
  rewards?: boolean;
};

const DUNGEON_SCENES = [
  'DEKU_TREE',              'LAIR_GOHMA',
  'DODONGO_CAVERN',         'LAIR_KING_DODONGO',
  'INSIDE_JABU_JABU',       'LAIR_BARINADE',
  'TEMPLE_FOREST',          'LAIR_PHANTOM_GANON',
  'TEMPLE_FIRE',            'LAIR_VOLVAGIA',
  'TEMPLE_WATER',           'LAIR_MORPHA',
  'TEMPLE_SPIRIT',          'LAIR_TWINROVA',
  'TEMPLE_SHADOW',          'LAIR_BONGO_BONGO',
  'GANON_TOWER',            'LAIR_GANONDORF',
  'BOTTOM_OF_THE_WELL',     'ICE_CAVERN',
  'GERUDO_TRAINING_GROUND', 'THIEVES_HIDEOUT',
  'INSIDE_GANON_CASTLE',

  'PIRATE_FORTRESS_EXTERIOR', 'PIRATE_FORTRESS_INTERIOR',    'PIRATE_FORTRESS_ENTRANCE',
  'TEMPLE_STONE_TOWER',       'TEMPLE_STONE_TOWER_INVERTED', 'LAIR_TWINMOLD',
  'TEMPLE_WOODFALL',          'LAIR_ODOLWA',
  'CASTLE_IKANA',             'LAIR_IKANA',
  'TEMPLE_SNOWHEAD',          'LAIR_GOHT',
  'SPIDER_HOUSE_SWAMP',       'SPIDER_HOUSE_OCEAN',
  'MOON', 'LAIR_MAJORA',      'MOON_DEKU', 'MOON_GORON', 'MOON_ZORA', 'MOON_LINK',
  'TEMPLE_GREAT_BAY',         'LAIR_GYORG',
  'BENEATH_THE_WELL',         'SECRET_SHRINE',
]

export function isLocationInDungeon(scene: string) {
  return DUNGEON_SCENES.includes(scene);
}

export function isLocationFullyShuffled(settings: Settings, fixedLocations: Set<Location>, items: ItemPlacement,  loc: Location, opts: IsLocationFullyShuffledOptions): boolean {
  if (fixedLocations.has(loc)) {
    return false;
  }

  const locD = locationData(loc);
  const item = items.get(loc)!;

  /* Plando */
  if (opts.noPlando && settings.plando.locations[locD.id]) {
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
  if (isDungeonStrayFairy(item.item) && settings.strayFairyChestShuffle !== 'anywhere' && settings.strayFairyOtherShuffle !== 'anywhere') {
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

  /* Non shuffled GS token */
  /* TODO: Handle dungeon/overworld better */
  if (ItemHelpers.isGoldToken(item.item) && settings.goldSkulltulaTokens === 'none') {
    return false;
  }

  /* Non shuffled House tokens */
  if (ItemHelpers.isHouseToken(item.item) && settings.housesSkulltulaTokens === 'none') {
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

export function getPreActivatedOwlsLocations(world: World): string[] {
  let preActivatedOwls: string[] = [];
  if (world.resolvedFlags.mmPreActivatedOwls.has('clocktown')) preActivatedOwls.push('MM Clock Town Owl Statue');
  if (world.resolvedFlags.mmPreActivatedOwls.has('milkroad'))  preActivatedOwls.push('MM Milk Road Owl Statue');
  if (world.resolvedFlags.mmPreActivatedOwls.has('swamp'))     preActivatedOwls.push('MM Southern Swamp Owl Statue');
  if (world.resolvedFlags.mmPreActivatedOwls.has('woodfall'))  preActivatedOwls.push('MM Woodfall Owl Statue');
  if (world.resolvedFlags.mmPreActivatedOwls.has('mountain'))  preActivatedOwls.push('MM Mountain Village Owl Statue');
  if (world.resolvedFlags.mmPreActivatedOwls.has('snowhead'))  preActivatedOwls.push('MM Snowhead Owl Statue');
  if (world.resolvedFlags.mmPreActivatedOwls.has('greatbay'))  preActivatedOwls.push('MM Great Bay Coast Owl Statue');
  if (world.resolvedFlags.mmPreActivatedOwls.has('zoracape'))  preActivatedOwls.push('MM Zora Cape Owl Statue');
  if (world.resolvedFlags.mmPreActivatedOwls.has('canyon'))    preActivatedOwls.push('MM Ikana Canyon Owl Statue');
  if (world.resolvedFlags.mmPreActivatedOwls.has('tower'))     preActivatedOwls.push('MM Stone Tower Owl Statue');
  return preActivatedOwls;
}
