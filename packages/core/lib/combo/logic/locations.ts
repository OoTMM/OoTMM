import { ItemHelpers } from '../items';
import { Settings } from '../settings';
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
  locationRegistry[x] = { id, world: w };
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

export const LOCATIONS_ZELDA = [
  'OOT Zelda\'s Letter',
  'OOT Zelda\'s Song',
];

export function isLocationRenewable(world: World, loc: Location) {
  const locationId = locationData(loc).id;
  if (MM_SCRUBS.includes(locationId) || MM_MERCHANTS.includes(locationId))
    return true;
  if (ONE_TIME_SHOP_CHECKS.includes(locationId))
    return false;
  const check = world.checks[locationId];
  if (['shop', 'cow', 'scrub'].includes(check.type))
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
