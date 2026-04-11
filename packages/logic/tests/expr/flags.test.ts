import { describe, it, expect } from 'vitest';

import { exprFlagOn, exprFlagOff, CONSTRAINT_FLAGS } from '../../src';
import { makeDeps, makeState } from './helpers';

describe('flag-on & flag-off', () => {
  const flag = CONSTRAINT_FLAGS[0];

  it('flag-on - true with restriction when flagsOff does not block it', () => {
    const state = makeState({ areaData: { ootTime: 0, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 } });
    const result = exprFlagOn(flag).eval(state, makeDeps());
    expect(result.result).toBe(true);
  });

  it('flag-on - false when flagsOff blocks it', () => {
    const state = makeState({ areaData: { ootTime: 0, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 1 } });
    expect(exprFlagOn(flag).eval(state, makeDeps())).toEqual({ result: false });
  });

  it('flag-off - true with restriction when flagsOn does not block it', () => {
    const state = makeState({ areaData: { ootTime: 0, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 } });
    const result = exprFlagOff(flag).eval(state, makeDeps());
    expect(result.result).toBe(true);
  });

  it('flag-off - false when flagsOn blocks it', () => {
    const state = makeState({ areaData: { ootTime: 0, mmTime: 0, mmTime2: 0, flagsOn: 1, flagsOff: 0 } });
    expect(exprFlagOff(flag).eval(state, makeDeps())).toEqual({ result: false });
  });
});
