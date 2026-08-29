import type { Item } from './defs';
import type { CountMap } from '@ootmm/core/src/util';

export type PlayerItem = {
  readonly item: Item;
  readonly player: number | 'all';
  readonly __brand: unique symbol;
};

const playerItemMemo = new Map<number, PlayerItem>();

export function makePlayerItem(item: Item, player: number | 'all'): PlayerItem {
  const keyPlayerId = player === 'all' ? 0 : player + 1;
  const key = item.index | (keyPlayerId << 16);
  let result = playerItemMemo.get(key);
  if (result) return result;
  result = Object.freeze({ item, player }) as PlayerItem;
  playerItemMemo.set(key, result);
  return result;
}

export type ItemsCount = CountMap<Item>;
export type PlayerItems = CountMap<PlayerItem>;
