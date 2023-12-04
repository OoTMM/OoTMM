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
  if (Object.hasOwn(Items, id)) {
    return Items[id as keyof typeof Items];
  }
  throw new Error(`Unknown item ID: ${id}`);
}
