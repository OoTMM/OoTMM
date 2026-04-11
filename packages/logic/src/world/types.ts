import type { Game, Item, Region } from '@ootmm/core';
import type { Entrance } from '@ootmm/data';
import type { ResolvedWorldFlags } from './flags';
import type { Expr, ExprParser } from '../expr';

export type ExprMap = {
  [k: string]: Expr;
}

export type WorldArea = {
  exits: ExprMap;
  events: ExprMap;
  locations: ExprMap;
  gossip: ExprMap;
  stay: Expr[] | null;
  game: Game;
  boss: boolean;
  ageChange: boolean;
  dungeon: string | null;
  time: 'still' | 'day' | 'night' | 'flow';
  region: string;
};

export type WorldCheckType = 'chest' | 'collectible' | 'gs' | 'sf' | 'cow' | 'shop' | 'scrub' | 'sr' | 'pot' | 'crate' | 'barrel' | 'grass' | 'tree' | 'bush' | 'rock' | 'soil' | 'wonder' | 'fish' | 'fairy' | 'snowball' | 'hive' | 'rupee' | 'heart' | 'fairy_spot' | 'butterfly' | 'redboulder' | 'icicle' | 'redice';

type WorldCheckNumeric = {
  type: WorldCheckType;
  id: number;
};

type WorldCheckSymbolic = {
  type: 'npc';
  id: string;
};

export type WorldCheck = {
  game: Game;
  scene: string;
  item: Item;
  hint: string;
} & (WorldCheckNumeric | WorldCheckSymbolic);

export type WorldGossip = {
  game: Game;
  type: 'gossip' | 'gossip-grotto' | 'gossip-moon';
};

export type ExprParsers = {
  oot: ExprParser;
  mm: ExprParser;
}

type DungeonEntranceReplace = {
  readonly type: 'replace';
  readonly entrance: Entrance;
};

type DungeonEntranceRegion = {
  readonly type: 'region';
  readonly region: Region;
};

type DungeonEntrance =
  | DungeonEntranceReplace
  | DungeonEntranceRegion
  ;

export type World = {
  areas: { [k: string]: WorldArea };
  checks: { [k: string]: WorldCheck };
  dungeons: { [k: string]: Set<string> };
  dungeonsBossAreas: { [k: string]: Set<string> };
  regions: { [k: string]: string };
  gossip: { [k: string]: WorldGossip };
  checkHints: { [k: string]: string[] };
  locations: Set<string>;
  songLocations: Set<string>;
  warpLocations: Set<string>;
  prices: number[];
  songEvents: number[];
  bossIds: number[];
  entranceOverrides: Map<string, string>;
  entranceOverridesRev: Map<string, readonly string[]>;
  preCompleted: Set<string>;
  resolvedFlags: ResolvedWorldFlags;
  exprParsers: ExprParsers;
  dungeonsEntrances: Map<string, DungeonEntrance>;
};
