import type { Age, State } from './state';

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
];

const itemCount = (state: State, item: string): number => state.items[item] || 0;
const itemsCount = (state: State, items: string[]): number => items.reduce((acc, item) => acc + itemCount(state, item), 0);

export type Expr = (state: State) => boolean;

export const exprTrue = (): Expr => state => true;
export const exprFalse = (): Expr => state => false;
export const exprAnd = (exprs: Expr[]): Expr => state => exprs.every(expr => expr(state));
export const exprOr = (exprs: Expr[]): Expr => state => exprs.some(expr => expr(state));
export const exprAge = (age: Age): Expr => state => state.age === age;
export const exprHas = (item: string, count: number): Expr => state => itemCount(state, item) >= count;
export const exprEvent = (event: string): Expr => state => state.events.has(event);
export const exprMasks = (count: number): Expr => state => itemsCount(state, MASKS) >= count;
