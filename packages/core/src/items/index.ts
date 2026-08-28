import type { ItemID } from './defs';
import { Item, Items } from './defs';

export { Item, Items } from './defs';
export { makePlayerItem } from './util';
export * as ItemGroups from './groups';
export * as ItemHelpers from './helpers';

export type { ItemID, ItemName } from './defs';
export type { PlayerItems, PlayerItem, ItemsCount } from './util';

export function itemByID(id: string): ItemID {
  /* HARDCODED */
  if (['OOT_FLEXIBLE', 'OOT_RANDOM'].includes(id)) {
    id = 'OOT_RUPEE_GREEN';
  }
  if (['MM_???', 'MM_FLEXIBLE', 'MM_RANDOM'].includes(id)) {
    id = 'MM_RUPEE_GREEN';
  }
  if (['MM_ARROWS_20'].includes(id)) {
    id = 'MM_ARROWS_30';
  }
  if (['OOT_ICE_TRAP'].includes(id)) {
    id = 'OOT_RUPEE_BLUE';
  }
  if (Object.hasOwn(Items, id)) {
    return Items[id as keyof typeof Items];
  }
  throw new Error(`Unknown item ID: ${id}`);
}
