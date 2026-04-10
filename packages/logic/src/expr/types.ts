import type { Item, ItemsCount, Settings } from '@ootmm/core';
import type { Age } from '../age';

type ExprNodeTrue = { readonly type: 'true' };
type ExprNodeFalse = { readonly type: 'false' };
type ExprNodeOr = { readonly type: 'or'; readonly exprs: Expr[] };
type ExprNodeAnd = { readonly type: 'and'; readonly exprs: Expr[] };
type ExprNodeAge = { readonly type: 'age'; readonly age: Age };
type ExprNodeItem = { readonly type: 'item'; readonly item: Item; readonly count: number };
type ExprNodeRenewable = { readonly type: 'renewable'; readonly item: Item };
type ExprNodeLicense = { readonly type: 'license', readonly item: Item };
type ExprNodeEvent = { readonly type: 'event'; readonly event: string };
type ExprNodeMasks = { readonly type: 'masks'; readonly count: number };
type ExprNodeSpecial = { readonly type: 'special'; readonly specialId: string };
type ExprNodeTimeOot = { readonly type: 'time-oot'; readonly flag: number };
type ExprNodeTimeMm = { readonly type: 'time-mm'; readonly value: number; readonly value2: number };
type ExprNodePrice = { readonly type: 'price'; readonly slot: number; readonly max: number };
type ExprNodeSongEvent = { readonly type: 'song-event'; readonly songId: number; readonly cmp: number };
type ExprNodeFlagOn = { readonly type: 'flag-on'; readonly flag: number };
type ExprNodeFlagOff = { readonly type: 'flag-off'; readonly flag: number };

export type ExprNode =
  | ExprNodeTrue
  | ExprNodeFalse
  | ExprNodeOr
  | ExprNodeAnd
  | ExprNodeAge
  | ExprNodeItem
  | ExprNodeRenewable
  | ExprNodeLicense
  | ExprNodeEvent
  | ExprNodeMasks
  | ExprNodeSpecial
  | ExprNodeTimeOot
  | ExprNodeTimeMm
  | ExprNodePrice
  | ExprNodeSongEvent
  | ExprNodeFlagOn
  | ExprNodeFlagOff;

export type ExprFunc = (state: ExprState, deps: ExprDependencies) => ExprResult;
export type Expr = ExprNode & { readonly id: number; readonly eval: ExprFunc };

type ExprStateWorld = {
  songEvents: number[];
  prices: number[];
};

export type ExprState = {
  items: ItemsCount;
  renewables: ItemsCount;
  licenses: ItemsCount;
  age: Age;
  events: Set<string>;
  areaData: AreaData;
  settings: Settings;
  world: ExprStateWorld;
};

export type ExprDependencies = {
  items: Item[];
  events: string[];
}

export type ExprRestrictions = {
  ootTime: number;
  mmTime: number;
  mmTime2: number;
  flagsOn: number;
  flagsOff: number;
};

type ExprResultFalse = {
  result: false;
};

type ExprResultTrue = {
  result: true;
  restrictions?: ExprRestrictions;
};

export type ExprResult = ExprResultFalse | ExprResultTrue;

export type AreaData = {
  ootTime: number;
  mmTime: number;
  mmTime2: number;
  flagsOn: number;
  flagsOff: number;
};
