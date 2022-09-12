import { Random } from '../random';
import { pathfind } from './pathfind';
import { Items } from './state';
import { World } from './world';

const sample = <T>(random: Random, arr: T[]): T => {
  /* Biased towards low values, but that's fine */
  const index = random.next() % arr.length;
  return arr[index];
};

const shuffle = <T>(random: Random, arr: T[]): T[] => {
  const copy = [...arr];
  for (let i = 0; i < copy.length; i++) {
    const j = random.next() % copy.length;
    [copy[i], copy[j]] = [copy[j], copy[i]];
  }
  return copy;
};

type ItemPlacement = {[k: string]: string};

const fill = (world: World, random: Random, placement: ItemPlacement, pool: string[]) => {
  const shuffledPool = shuffle(random, pool);
  let i = 0;

  for (const location in world.checks) {
    if (placement[location]) {
      continue;
    }
    const item = shuffledPool[i++];
    placement[location] = item;
  }
  if (i !== shuffledPool.length) {
    throw new Error('Item Count Error');
  }
};

export const solve = (world: World, random: Random) => {
  const items: Items = {};
  const placement: ItemPlacement  = {};
  let reachable = undefined;
  const checksCount = Object.keys(world.checks).length;
  const itemPool = [...world.pool];

  for (;;) {
    reachable = pathfind(world, items, reachable);
    if (reachable.locations.size === checksCount) {
      break;
    }
  }

  /* At this point we have a beatable game */
  fill(world, random, placement, itemPool);

  return placement;
};
