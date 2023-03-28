import { World } from './world';

export const ONE_TIME_SHOP_CHECKS = [
  'MM Bomb Shop Bomb Bag',
  'MM Bomb Shop Bomb Bag 2',
  'MM Curiosity Shop All-Night Mask',
];

export const OOT_ONE_TIME_SCRUBS = [
  'OOT Lost Woods Scrub Sticks Upgrade',
  'OOT Lost Woods Grotto Scrub Nuts Upgrade',
  'OOT Hyrule Field Grotto Scrub HP',
];

export function isLocationRenewable(world: World, loc: string) {
  if (ONE_TIME_SHOP_CHECKS.includes(loc))
    return false;
  const check = world.checks[loc];
  return ['shop', 'cow', 'scrub'].includes(check.type);
}
