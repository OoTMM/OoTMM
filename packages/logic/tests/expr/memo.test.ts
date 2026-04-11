import { describe, it, expect } from 'vitest';
import { Items } from '@ootmm/core';

import { exprTrue, exprFalse, exprAnd, exprOr, exprHas } from '../../src';

describe('memoization', () => {
  it('returns the same object for identical expressions', () => {
    expect(exprTrue()).toBe(exprTrue());
    expect(exprFalse()).toBe(exprFalse());
  });

  it('returns the same object for identical item expressions', () => {
    expect(exprHas(Items.OOT_SWORD, 1)).toBe(exprHas(Items.OOT_SWORD, 1));
  });

  it('returns different objects for different item expressions', () => {
    expect(exprHas(Items.OOT_SWORD, 1)).not.toBe(exprHas(Items.OOT_SWORD, 2));
    expect(exprHas(Items.OOT_SWORD, 1)).not.toBe(exprHas(Items.OOT_BOW, 1));
  });

  it('deduplicates and/or children regardless of order', () => {
    const a = exprHas(Items.OOT_SWORD, 1);
    const b = exprHas(Items.OOT_BOW, 1);
    expect(exprAnd([a, b])).toBe(exprAnd([b, a]));
    expect(exprOr([a, b])).toBe(exprOr([b, a]));
  });

  it('gives each compiled expr an unique id', () => {
    const a = exprHas(Items.OOT_SWORD, 1);
    const b = exprHas(Items.OOT_BOW, 1);
    expect(a.id).not.toBe(b.id);
  });
});
