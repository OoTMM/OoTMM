import { ItemPlacement, ORDERED_DUNGEON_REWARDS } from './solve';
import { World } from './world';

export type Hints = {
  dungeonRewards: string[];
  lightArrow: string;
};

const findItem = (items: ItemPlacement, item: string) => {
  for (const loc in items) {
    if (items[loc] === item) {
      return loc;
    }
  }
  return null;
};

export const hints = (world: World, items: ItemPlacement): Hints => {
  const dungeonRewards = ORDERED_DUNGEON_REWARDS.map(item => findItem(items, item)).map(loc => world.regions[loc!]);
  const lightArrow = world.regions[findItem(items, 'OOT_ARROW_LIGHT')!];
  return { dungeonRewards, lightArrow };
};
