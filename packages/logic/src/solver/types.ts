import type { Monitor, PlayerItem, PlayerItems, Random, Settings } from '@ootmm/core';
import type { World } from '../world';
import type { ItemProperties } from '../item-properties';
import type { Location, ItemPlacement } from '../types';

export type SolverLogItem = { type: 'item', item: PlayerItem, location: Location };
export type SolverLogEntry = SolverLogItem;

export type LogicPassSolverState = {
  fixedLocations: Set<Location>;
  worlds: World[];
  settings: Settings;
  random: Random;
  monitor: Monitor;
  pool: PlayerItems;
  renewableJunks: PlayerItems;
  startingItems: PlayerItems;
  itemProperties: ItemProperties;
  plandoLocations: Map<Location, PlayerItem>;
};

export type ItemPools = {
  extra: PlayerItems,
  required: PlayerItems,
  nice: PlayerItems,
  junk: PlayerItems,
  nothing: PlayerItems,
};

export type SolverState = {
  startingItems: PlayerItems;
  items: ItemPlacement;
  pools: ItemPools;
  criticalRenewables: Set<PlayerItem>;
  placedCount: number;
  log: SolverLogEntry[];
};
