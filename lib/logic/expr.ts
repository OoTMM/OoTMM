import type { Age, State } from './state';

export type Expr = (state: State) => boolean;

export const exprTrue = (): Expr => state => true;
export const exprFalse = (): Expr => state => false;
export const exprAnd = (exprs: Expr[]): Expr => state => exprs.every(expr => expr(state));
export const exprOr = (exprs: Expr[]): Expr => state => exprs.some(expr => expr(state));
export const exprAge = (age: Age): Expr => state => state.age === age;
export const exprHas = (item: string, count: number): Expr => state => (state.items[item] || 0) >= count;
export const exprEvent = (event: string): Expr => state => state.events.has(event);
