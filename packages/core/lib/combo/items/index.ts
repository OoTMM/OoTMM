import { Item, Items } from './defs';

export { Items } from './defs';
export { makePlayerItem } from './util';
export * as ItemGroups from './groups';
export * as ItemHelpers from './helpers';

export type { Item } from './defs';
export type { PlayerItems, PlayerItem, ItemsCount } from './util';

export function itemByID(id: string): Item {
  /* HARDCODED */
  if (['OOT_FLEXIBLE', 'OOT_RANDOM'].includes(id)) {
    id = 'OOT_RUPEE_GREEN';
  }
  if (['MM_???', 'MM_RANDOM'].includes(id)) {
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
