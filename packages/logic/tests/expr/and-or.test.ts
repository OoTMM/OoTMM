import { describe, it, expect } from 'vitest';
import { Items } from '@ootmm/core';

import { exprTrue, exprFalse, exprAnd, exprOr, exprHas } from '../../src';
import { makeDeps, makeState } from './helpers';

describe('and', () => {
  it('returns true when all children are true', () => {
    const a = exprHas(Items.OOT_SWORD, 1);
    const b = exprHas(Items.OOT_BOW, 1);
    const state = makeState({ items: new Map([[Items.OOT_SWORD, 1], [Items.OOT_BOW, 1]]) });
    const result = exprAnd([a, b]).eval(state, makeDeps());
    expect(result.result).toBe(true);
  });

  it('returns false when any child is false', () => {
    const a = exprHas(Items.OOT_SWORD, 1);
    const b = exprHas(Items.OOT_BOW, 1);
    const state = makeState({ items: new Map([[Items.OOT_SWORD, 1]]) });
    expect(exprAnd([a, b]).eval(state, makeDeps())).toEqual({ result: false });
  });

  it('collapses to its child when given a single expression', () => {
    const a = exprHas(Items.OOT_SWORD, 1);
    expect(exprAnd([a])).toBe(a);
  });

  it('collapses to true when given an empty list', () => {
    expect(exprAnd([])).toBe(exprTrue());
  });

  it('collapses to false when any child is exprFalse', () => {
    expect(exprAnd([exprHas(Items.OOT_SWORD, 1), exprFalse()])).toBe(exprFalse());
  });

  it('tracks deps from the failing child', () => {
    const a = exprHas(Items.OOT_SWORD, 1);
    const b = exprHas(Items.OOT_BOW, 1);
    const state = makeState({ items: new Map([[Items.OOT_SWORD, 1]]) });
    const deps = makeDeps();
    exprAnd([a, b]).eval(state, deps);
    expect(deps.items).toContain(Items.OOT_BOW);
    expect(deps.items).not.toContain(Items.OOT_SWORD);
  });
});

describe('or', () => {
  it('returns true when any child is true', () => {
    const a = exprHas(Items.OOT_SWORD, 1);
    const b = exprHas(Items.OOT_BOW, 1);
    const state = makeState({ items: new Map([[Items.OOT_SWORD, 1]]) });
    expect(exprOr([a, b]).eval(state, makeDeps()).result).toBe(true);
  });

  it('returns false when all children are false', () => {
    const a = exprHas(Items.OOT_SWORD, 1);
    const b = exprHas(Items.OOT_BOW, 1);
    const deps = makeDeps();
    expect(exprOr([a, b]).eval(makeState(), deps)).toEqual({ result: false });
    expect(deps.items).toContain(Items.OOT_SWORD);
    expect(deps.items).toContain(Items.OOT_BOW);
  });

  it('collapses to its child when given a single expression', () => {
    const a = exprHas(Items.OOT_SWORD, 1);
    expect(exprOr([a])).toBe(a);
  });

  it('collapses to false when given an empty list', () => {
    expect(exprOr([])).toBe(exprFalse());
  });

  it('collapses to true when any child is exprTrue', () => {
    expect(exprOr([exprHas(Items.OOT_SWORD, 1), exprTrue()])).toBe(exprTrue());
  });

  it('clears deps when a child returns true without restrictions', () => {
    const a = exprHas(Items.OOT_SWORD, 1);
    const b = exprHas(Items.OOT_BOW, 1);
    const state = makeState({ items: new Map([[Items.OOT_BOW, 1]]) });
    const deps = makeDeps();
    exprOr([a, b]).eval(state, deps);
    expect(deps.items).not.toContain(Items.OOT_SWORD);
  });
});
