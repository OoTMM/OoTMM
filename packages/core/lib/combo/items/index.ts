import { Item, Items } from './defs';

export { Item, Items } from './defs';
export { PlayerItems, PlayerItem, ItemsCount, makePlayerItem } from './util';
export * as ItemGroups from './groups';
export * as ItemHelpers from './helpers';

export function itemByID(id: string): Item {
  /* HARDCODED */
  if (['OOT_NOTHING', 'OOT_FLEXIBLE', 'OOT_MAGIC_JAR_SMALL', 'OOT_MAGIC_JAR_LARGE', 'OOT_RANDOM'].includes(id)) {
    id = 'OOT_RUPEE_GREEN';
  }
  if (['MM_NOTHING', 'MM_MAGIC_JAR_SMALL', 'MM_MAGIC_JAR_LARGE', 'MM_???', 'MM_RANDOM'].includes(id)) {
    id = 'MM_RUPEE_GREEN';
  }
  if (['MM_ARROWS_20'].includes(id)) {
    id = 'MM_ARROWS_30';
  }
  if (id === 'OOT_FISHING_POND_CHILD_FISH') {
    id = 'OOT_FISHING_POND_CHILD_FISH_2LBS'
  } else if (id === 'OOT_FISHING_POND_ADULT_FISH') {
    id = 'OOT_FISHING_POND_CHILD_FISH_4LBS'
  } else if (id === 'OOT_FISHING_POND_CHILD_LOACH') {
    id = 'OOT_FISHING_POND_CHILD_LOACH_14LBS'
  } else if (id === 'OOT_FISHING_POND_ADULT_LOACH') {
    id = 'OOT_FISHING_POND_ADULT_LOACH_29LBS'
  }
  if (Object.hasOwn(Items, id)) {
    return Items[id as keyof typeof Items];
  }
  throw new Error(`Unknown item ID: ${id}`);
}
