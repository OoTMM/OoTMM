import { Item, Items } from './defs';

export { Item, Items } from './defs';
export { PlayerItems, PlayerItem, ItemsCount, makePlayerItem } from './util';
export * as ItemGroups from './groups';
export * as ItemHelpers from './helpers';

export function itemByID(id: string): Item {
  /* HARDCODED */
  if (id === 'OOT_NOTHING') {
    id = 'OOT_RUPEE_GREEN';
  }
  if (Object.hasOwn(Items, id)) {
    return Items[id as keyof typeof Items];
  }
  throw new Error(`Unknown item ID: ${id}`);
}
