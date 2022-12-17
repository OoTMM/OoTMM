import { addItem } from './items';
import { pathfind, Reachable } from './pathfind';
import { ItemPlacement, ITEMS_DUNGEON_REWARDS, ITEMS_REQUIRED } from './solve';
import { Items } from './state';
import { World } from './world';

const isItemImportant = (item: string) => (ITEMS_DUNGEON_REWARDS.has(item) || ITEMS_REQUIRED.has(item) || /_BOSS_KEY_/.test(item)) && !(/GS_TOKEN$/.test(item) || /MM_HEART_(PIECE|CONTAINER)$/.test(item));

export const playthrough = (world: World, placement: ItemPlacement) => {
  let reachable: Reachable | undefined;
  const spheres: string[][] = [];
  const items: Items = {};
  const reachedLocations = new Set<string>();
  const count = Object.keys(placement).length;

  for (;;) {
    const sphere = [];
    /* Get small keys */
    for (;;) {
      reachable = pathfind(world, items, reachable);
      let hasSmallKeys = false;
      for (const loc of reachable.locations) {
        if (reachedLocations.has(loc)) {
          continue;
        }
        const item = placement[loc];
        if (/_SMALL_KEY_/.test(item)) {
          reachedLocations.add(loc);
          addItem(items, item);
          hasSmallKeys = true;
        }
      }
      if (!hasSmallKeys) {
        break;
      }
    }
    for (const loc of reachable.locations) {
      if (reachedLocations.has(loc)) {
        continue;
      }
      const item = placement[loc];
      reachedLocations.add(loc);
      addItem(items, item);
      if (isItemImportant(item)) {
        sphere.push(loc);
      }
    }
    if (sphere.length !== 0) {
      spheres.push(sphere);
    }
    if (reachable.locations.size === count) {
      break;
    }
  }
  return spheres;
};
