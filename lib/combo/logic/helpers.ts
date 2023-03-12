import { World } from './world';

export function isLocationRenewable(world: World, loc: string) {
  const check = world.checks[loc];
  return ['shop', 'cow'].includes(check.type);
}
