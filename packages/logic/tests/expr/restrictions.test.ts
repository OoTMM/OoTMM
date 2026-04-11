import { describe, it, expect } from 'vitest';
import { isDefaultRestrictions, exprRestrictionsAnd, exprRestrictionsOr } from '../../src/expr/compile';

describe('isDefaultRestrictions', () => {
  it('returns true for zeroed restrictions', () => {
    expect(isDefaultRestrictions({ ootTime: 0, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 })).toBe(true);
  });

  it('returns false when any field is non-zero', () => {
    expect(isDefaultRestrictions({ ootTime: 1, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 })).toBe(false);
    expect(isDefaultRestrictions({ ootTime: 0, mmTime: 1, mmTime2: 0, flagsOn: 0, flagsOff: 0 })).toBe(false);
  });
});

describe('exprRestrictionsAnd', () => {
  it('returns zeroed restrictions for empty input', () => {
    expect(isDefaultRestrictions(exprRestrictionsAnd([]))).toBe(true);
  });

  it('ORs restriction fields across true results (AND semantics = union of exclusions)', () => {
    const r = exprRestrictionsAnd([
      { result: true, restrictions: { ootTime: 0b01, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 } },
      { result: true, restrictions: { ootTime: 0b10, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 } },
    ]);
    expect(r.ootTime).toBe(0b11);
  });

  it('ignores false results', () => {
    const r = exprRestrictionsAnd([
      { result: false },
      { result: true, restrictions: { ootTime: 0b01, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 } },
    ]);
    expect(r.ootTime).toBe(0b01);
  });
});

describe('exprRestrictionsOr', () => {
  it('ANDs restriction fields across true results (OR semantics = intersection of exclusions)', () => {
    const r = exprRestrictionsOr([
      { result: true, restrictions: { ootTime: 0b11, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 } },
      { result: true, restrictions: { ootTime: 0b01, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 } },
    ]);
    expect(r.ootTime).toBe(0b01);
  });

  it('returns default restrictions when any true result has no restrictions', () => {
    const r = exprRestrictionsOr([
      { result: true, restrictions: { ootTime: 0b01, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 } },
      { result: true },
    ]);
    expect(isDefaultRestrictions(r)).toBe(true);
  });
});
