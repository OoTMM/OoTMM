import { describe, it, expect } from 'vitest';

import { exprOotTime, exprMmTime, OOT_TIME } from '../../src';
import { isDefaultRestrictions } from '../../src/expr/compile';
import { makeDeps, makeState } from './helpers';

describe('time-oot', () => {
  it('returns false when time does not match', () => {
    const state = makeState({ areaData: { ootTime: OOT_TIME.NIGHT, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 } });
    expect(exprOotTime('day').eval(state, makeDeps())).toEqual({ result: false });
  });

  it('returns true with a restriction when time matches', () => {
    const state = makeState({ areaData: { ootTime: OOT_TIME.DAY, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 } });
    const result = exprOotTime('day').eval(state, makeDeps());
    expect(result.result).toBe(true);
    expect((result as any).restrictions).toBeDefined();
    expect(isDefaultRestrictions((result as any).restrictions)).toBe(false);
  });
});

describe('time-mm', () => {
  it('returns false when time does not match', () => {
    const state = makeState({ areaData: { ootTime: 0, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 } });
    expect(exprMmTime('at', ['DAY1_AM_06_00']).eval(state, makeDeps())).toEqual({ result: false });
  });

  it('returns true with a restriction when time matches', () => {
    const state = makeState({ areaData: { ootTime: 0, mmTime: 1, mmTime2: 0, flagsOn: 0, flagsOff: 0 } });
    const result = exprMmTime('at', ['DAY1_AM_06_00']).eval(state, makeDeps());
    expect(result.result).toBe(true);
    expect((result as any).restrictions).toBeDefined();
  });
});
