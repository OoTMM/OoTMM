import { Settings, Trick, TRICKS } from '../settings';
import { Items, ITEMS_MASKS_OOT, ITEMS_MASKS_REGULAR, ITEMS_MASKS_TRANSFORM, ITEMS_MEDALLIONS, ITEMS_REMAINS, ITEMS_STONES } from './items';
import { Age } from './pathfind';

type State = {
  items: Items;
  age: Age;
  events: Set<string>;
  ignoreItems: boolean;
};

const itemCount = (state: State, item: string): number => state.items[item] || 0;
const itemsCount = (state: State, items: string[]): number => items.reduce((acc, item) => acc + itemCount(state, item), 0);

function resolveSpecialCond(settings: Settings, state: State, special: string) {
  const { specialConds } = settings;
  if (!specialConds.hasOwnProperty(special)) {
    throw new Error(`Unknown special condition: ${special}`);
  }
  let items = new Set<string>();
  const cond = specialConds[special as keyof typeof specialConds];

  if (cond.stones) items = new Set([...items, ...ITEMS_STONES]);
  if (cond.medallions) items = new Set([...items, ...ITEMS_MEDALLIONS]);
  if (cond.remains) items = new Set([...items, ...ITEMS_REMAINS]);
  if (cond.skullsGold) items.add('OOT_GS_TOKEN');
  if (cond.skullsSwamp) items.add('MM_GS_TOKEN_SWAMP');
  if (cond.skullsOcean) items.add('MM_GS_TOKEN_OCEAN');
  if (cond.fairiesWF) items.add('MM_STRAY_FAIRY_WF');
  if (cond.fairiesSH) items.add('MM_STRAY_FAIRY_SH');
  if (cond.fairiesGB) items.add('MM_STRAY_FAIRY_GB');
  if (cond.fairiesST) items.add('MM_STRAY_FAIRY_ST');
  if (cond.fairyTown) items.add('MM_STRAY_FAIRY_TOWN');
  if (cond.masksRegular) items = new Set([...items, ...ITEMS_MASKS_REGULAR]);
  if (cond.masksTransform) items = new Set([...items, ...ITEMS_MASKS_TRANSFORM]);
  if (cond.masksOot) items = new Set([...items, ...ITEMS_MASKS_OOT]);

  return itemsCount(state, [...items]) >= cond.count;
}

export type Expr = (state: State) => boolean;

export const exprTrue = (): Expr => state => true;
export const exprFalse = (): Expr => state => false;
export const exprAnd = (exprs: Expr[]): Expr => state => exprs.every(expr => expr(state));
export const exprOr = (exprs: Expr[]): Expr => state => exprs.some(expr => expr(state));
export const exprNot = (expr: Expr): Expr => state => !expr(state);
export const exprCond = (cond: Expr, then: Expr, otherwise: Expr): Expr => state => cond(state) ? then(state) : otherwise(state);
export const exprAge = (age: Age): Expr => state => state.age === age;
export const exprHas = (item: string, itemShared: string, count: number): Expr => state => state.ignoreItems || ((itemCount(state, item) + itemCount(state, itemShared)) >= count);
export const exprEvent = (event: string): Expr => state => state.events.has(event);
export const exprMasks = (count: number): Expr => state => state.ignoreItems || (itemsCount(state, ITEMS_MASKS_REGULAR) >= count);
export const exprHealth = (count: number): Expr => state => state.ignoreItems || ((3 + itemCount(state, 'MM_HEART_CONTAINER') + itemCount(state, 'MM_HEART_PIECE') / 4) >= count);
export const exprSpecial = (settings: Settings, special: string): Expr => state => resolveSpecialCond(settings, state, special);

export const exprSetting = (settings: Settings, setting: string, value: any): Expr => {
  const v = (settings as any)[setting];
  if (v === undefined) {
    throw new Error(`Setting ${setting} not found`);
  }
  return v === value ? exprTrue() : exprFalse();
};

export const exprTrick = (settings: Settings, trick: string): Expr => {
  if (!TRICKS.hasOwnProperty(trick)) {
    throw new Error(`Trick ${trick} not found`);
  }
  return settings.tricks[trick as Trick] ? exprTrue() : exprFalse();
};
