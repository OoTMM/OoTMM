import type { ItemID } from './defs';
import type { CountMap } from '../util';

import { createMemo } from '../util';

export type PlayerItem = {
  item: ItemID;
  player: number | 'all';
  __brand: 'PlayerItem';
};

const playerItemMemo = createMemo<PlayerItem>();

export function makePlayerItem(item: ItemID, player: number | 'all'): PlayerItem {
  return playerItemMemo(`${item}@${player}`, () => ({ item, player } as PlayerItem));
}

export type ItemsCount = CountMap<ItemID>;
export type PlayerItems = CountMap<PlayerItem>;
