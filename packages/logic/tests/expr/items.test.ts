import { describe, it, expect } from 'vitest';
import { Items } from '@ootmm/core';

import { exprHas, exprLicense, exprRenewable } from '../../src';
import { makeDeps, makeState } from './helpers';

describe('item', () => {
  it('returns true when the item is present', () => {
    const state = makeState({ items: new Map([[Items.OOT_SWORD, 1]]) });
    const deps = makeDeps();
    expect(exprHas(Items.OOT_SWORD, 1).eval(state, deps)).toEqual({ result: true });
    expect(deps.items).toHaveLength(0);
  });

  it('returns false and tracks dependency when item is missing', () => {
    const deps = makeDeps();
    expect(exprHas(Items.OOT_SWORD, 1).eval(makeState(), deps)).toEqual({ result: false });
    expect(deps.items).toContain(Items.OOT_SWORD);
  });

  it('respects count threshold', () => {
    const state = makeState({ items: new Map([[Items.OOT_SWORD, 1]]) });
    expect(exprHas(Items.OOT_SWORD, 2).eval(state, makeDeps())).toEqual({ result: false });

    const state2 = makeState({ items: new Map([[Items.OOT_SWORD, 2]]) });
    expect(exprHas(Items.OOT_SWORD, 2).eval(state2, makeDeps())).toEqual({ result: true });
  });

  it('license checks the license map, not items', () => {
    const expr = exprLicense(Items.OOT_SWORD);
    const withItem = makeState({ items: new Map([[Items.OOT_SWORD, 1]]) });
    const withLicense = makeState({ licenses: new Map([[Items.OOT_SWORD, 1]]) });
    expect(expr.eval(withItem, makeDeps())).toEqual({ result: false });
    expect(expr.eval(withLicense, makeDeps())).toEqual({ result: true });
  });

  it('renewable checks the renewables map, not items', () => {
    const expr = exprRenewable(Items.OOT_SWORD);
    const withItem = makeState({ items: new Map([[Items.OOT_SWORD, 1]]) });
    const withRenewable = makeState({ renewables: new Map([[Items.OOT_SWORD, 1]]) });
    expect(expr.eval(withItem, makeDeps())).toEqual({ result: false });
    expect(expr.eval(withRenewable, makeDeps())).toEqual({ result: true });
  });
});
