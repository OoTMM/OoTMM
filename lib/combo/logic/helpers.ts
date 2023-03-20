import { World } from './world';

export const ONE_TIME_SHOP_CHECKS = [
  'MM Bomb Shop Bomb Bag',
  'MM Bomb Shop Bomb Bag 2',
  'MM Curiosity Shop All-Night Mask',
];

export function isLocationRenewable(world: World, loc: string) {
  if (ONE_TIME_SHOP_CHECKS.includes(loc))
    return false;
  const check = world.checks[loc];
  return ['shop', 'cow'].includes(check.type);
}
