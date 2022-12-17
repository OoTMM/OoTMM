import { ItemPlacement, ORDERED_DUNGEON_REWARDS } from './solve';
import { World } from './world';
import { findSpheres } from './playthrough';

export type Hints = {
  dungeonRewards: string[];
  lightArrow: string;
  oathToOrder: string;
};

const wayOfTheHero = (world: World, ItemPlacement: ItemPlacement, spheres: string[][]) => {
  const woth = new Set<string>();
  for (const sphere of spheres) {
    for (const loc of sphere) {
      if (findSpheres(world, ItemPlacement, undefined, new Set([loc])) === null) {
        woth.add(loc);
      }
    }
  }
  return woth;
};

const findItem = (items: ItemPlacement, item: string) => {
  for (const loc in items) {
    if (items[loc] === item) {
      return loc;
    }
  }
  return null;
};

export const hints = (world: World, items: ItemPlacement, spheres: string[][]): Hints => {
  const woth = wayOfTheHero(world, items, spheres);
  const dungeonRewards = ORDERED_DUNGEON_REWARDS.map(item => findItem(items, item)).map(loc => world.regions[loc!]);
  const lightArrow = world.regions[findItem(items, 'OOT_ARROW_LIGHT')!];
  const oathToOrder = world.regions[findItem(items, 'MM_SONG_ORDER')!];
  return { dungeonRewards, lightArrow, oathToOrder };
};
