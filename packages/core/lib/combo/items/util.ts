import { CountMap, createMemo } from '../util';
import { Item } from './defs';

export type PlayerItem = {
  item: Item;
  player: number;
  __brand: 'PlayerItem';
};

const playerItemMemo = createMemo<PlayerItem>();

export function makePlayerItem(item: Item, player: number): PlayerItem {
  return playerItemMemo(`${item.id}@${player}`, () => ({ item, player } as PlayerItem));
}

export type ItemsCount = CountMap<Item>;
export type PlayerItems = CountMap<PlayerItem>;
