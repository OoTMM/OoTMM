import { Settings, Trick, TRICKS } from '../settings';
import { Items, ITEMS_MEDALLIONS, ITEMS_REMAINS, ITEMS_STONES } from './items';
import { Age } from './pathfind';

type State = {
  items: Items;
  age: Age;
  events: Set<string>;
  ignoreItems: boolean;
};


const MASKS = [
  'MM_MASK_CAPTAIN',
  'MM_MASK_GIANT',
  'MM_MASK_ALL_NIGHT',
  'MM_MASK_BUNNY',
  'MM_MASK_KEATON',
  'MM_MASK_GARO',
  'MM_MASK_ROMANI',
  'MM_MASK_TROUPE_LEADER',
  'MM_MASK_POSTMAN',
  'MM_MASK_COUPLE',
  'MM_MASK_GREAT_FAIRY',
  'MM_MASK_GIBDO',
  'MM_MASK_DON_GERO',
  'MM_MASK_KAMARO',
  'MM_MASK_TRUTH',
  'MM_MASK_STONE',
  'MM_MASK_BREMEN',
  'MM_MASK_BLAST',
  'MM_MASK_SCENTS',
  'MM_MASK_KAFEI',
  'SHARED_MASK_TRUTH',
  'SHARED_MASK_BUNNY',
  'SHARED_MASK_KEATON',
];

const itemCount = (state: State, item: string): number => state.items[item] || 0;
const itemsCount = (state: State, items: string[]): number => items.reduce((acc, item) => acc + itemCount(state, item), 0);

function resolveSpecialCond(settings: Settings, state: State, special: string) {
  const { specialConds } = settings;
  if (!specialConds.hasOwnProperty(special)) {
    throw new Error(`Unknown special condition: ${special}`);
  }
  let count = 0;
  const cond = specialConds[special as keyof typeof specialConds];

  if (cond.stones) count += itemsCount(state, ITEMS_STONES);
  if (cond.medallions) count += itemsCount(state, ITEMS_MEDALLIONS);
  if (cond.remains) count += itemsCount(state, ITEMS_REMAINS);
  if (cond.skullsGold) count += itemCount(state, 'OOT_GS_TOKEN');
  if (cond.skullsSwamp) count += itemCount(state, 'MM_GS_TOKEN_SWAMP');
  if (cond.skullsOcean) count += itemCount(state, 'MM_GS_TOKEN_OCEAN');
  if (cond.fairiesWF) count += itemCount(state, 'MM_STRAY_FAIRY_WF');
  if (cond.fairiesSH) count += itemCount(state, 'MM_STRAY_FAIRY_SH');
  if (cond.fairiesGB) count += itemCount(state, 'MM_STRAY_FAIRY_GB');
  if (cond.fairiesST) count += itemCount(state, 'MM_STRAY_FAIRY_ST');
  if (cond.fairyTown) count += itemCount(state, 'MM_STRAY_FAIRY_TOWN');

  return count >= cond.count;
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
export const exprMasks = (count: number): Expr => state => state.ignoreItems || (itemsCount(state, MASKS) >= count);
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
