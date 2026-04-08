import type { Item } from './defs';
import type { CountMap } from '../util';

import { createMemo } from '../util';

export type PlayerItem = {
  item: Item;
  player: number | 'all';
  __brand: 'PlayerItem';
};

const playerItemMemo = createMemo<PlayerItem>();

export function makePlayerItem(item: Item, player: number | 'all'): PlayerItem {
  return playerItemMemo(`${item.id}@${player}`, () => ({ item, player } as PlayerItem));
}

export type ItemsCount = CountMap<Item>;
export type PlayerItems = CountMap<PlayerItem>;
