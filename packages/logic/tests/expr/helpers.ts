import type { Settings } from '@ootmm/core';
import type { ExprDependencies, ExprState } from '../../src';
import { AGE_CHILD } from '../../src';

export function makeState(overrides: Partial<ExprState> = {}): ExprState {
  return {
    items: new Map(),
    renewables: new Map(),
    licenses: new Map(),
    age: AGE_CHILD,
    events: new Set(),
    areaData: { ootTime: 0, mmTime: 0, mmTime2: 0, flagsOn: 0, flagsOff: 0 },
    settings: {} as Settings,
    world: { songEvents: [], prices: [] },
    ...overrides,
  };
}

export function makeDeps(): ExprDependencies {
  return { items: [], events: [] };
}
