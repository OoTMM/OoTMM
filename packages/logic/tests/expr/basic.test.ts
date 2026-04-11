import { describe, it, expect } from 'vitest';
import { exprTrue, exprFalse, exprEvent, exprAge, AGE_CHILD, AGE_ADULT } from '../../src';
import { makeDeps, makeState } from './helpers';

describe('true / false', () => {
  it('exprTrue evaluates to true', () => {
    const deps = makeDeps();
    expect(exprTrue().eval(makeState(), deps)).toEqual({ result: true });
    expect(deps.items).toHaveLength(0);
    expect(deps.events).toHaveLength(0);
  });

  it('exprFalse evaluates to false', () => {
    expect(exprFalse().eval(makeState(), makeDeps())).toEqual({ result: false });
  });
});

describe('event', () => {
  it('returns true when event is present', () => {
    const state = makeState({ events: new Set(['MY_EVENT']) });
    const deps = makeDeps();
    expect(exprEvent('MY_EVENT').eval(state, deps)).toEqual({ result: true });
    expect(deps.events).toHaveLength(0);
  });

  it('returns false and tracks dependency when event is missing', () => {
    const deps = makeDeps();
    expect(exprEvent('MY_EVENT').eval(makeState(), deps)).toEqual({ result: false });
    expect(deps.events).toContain('MY_EVENT');
  });
});

/* ── Age ─────────────────────────────────────────────────────────────────── */

describe('age', () => {
  it('returns true for matching age', () => {
    expect(exprAge(AGE_CHILD).eval(makeState({ age: AGE_CHILD }), makeDeps())).toEqual({ result: true });
    expect(exprAge(AGE_ADULT).eval(makeState({ age: AGE_ADULT }), makeDeps())).toEqual({ result: true });
  });

  it('returns false for non-matching age', () => {
    expect(exprAge(AGE_ADULT).eval(makeState({ age: AGE_CHILD }), makeDeps())).toEqual({ result: false });
    expect(exprAge(AGE_CHILD).eval(makeState({ age: AGE_ADULT }), makeDeps())).toEqual({ result: false });
  });
});
