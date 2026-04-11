import type { Settings, Random } from '@ootmm/core';
import { SETTINGS } from '@ootmm/core';

export const WORLD_FLAGS = [
  'ganonTrials',
  'smallKeyRingOot',
  'smallKeyRingMm',
  'silverRupeePouches',
  'openDungeonsMm',
  'openDungeonsOot',
  'mmPreActivatedOwls',
  'mqDungeons',
  'jpLayouts',
] as const;

export type WorldFlag = typeof WORLD_FLAGS[number];

class ResolvedWorldFlag {
  private values: Set<string>;
  constructor(
    public readonly setting: keyof Settings,
    public readonly value: 'all' | 'none' | 'specific'
  ) {
    this.values = new Set;
  }

  add(value: string) {
    this.values.add(value);
  }

  has(value: string) {
    if (this.value === 'all') {
      return true;
    }
    if (this.value === 'none') {
      return false;
    }
    return this.values.has(value);
  }
}

export type ResolvedWorldFlags = {[k in WorldFlag]: ResolvedWorldFlag};

function resolveWorldFlag<T extends WorldFlag>(settings: Settings, random: Random, flag: T): ResolvedWorldFlag {
  const v = settings[flag];
  let wf: ResolvedWorldFlag;
  if (v.type === 'random-mixed' || v.type === 'random') {
    const setting = SETTINGS.find(x => x.key === flag)!;
    const values = ((setting as any).values as any[]).map(x => x.value) as string[];
    let set: string[] = [];
    let unset: string[] = [];
    if (v.type === 'random-mixed') {
      set = v.set;
      unset = v.unset;
    }
    wf = new ResolvedWorldFlag(flag, 'specific');
    for (const v of values) {
      if (set.includes(v)) {
        wf.add(v);
      } else if (!unset.includes(v) && random.next() & 0x1000) {
        wf.add(v);
      }
    }
  } else {
    wf = new ResolvedWorldFlag(flag, v.type);
    if (v.type === 'specific') {
      for (const k of v.values) {
        wf.add(k);
      }
    }
  }

  return wf;
}

export function resolveWorldFlags(settings: Settings, random: Random): ResolvedWorldFlags {
  const result = {} as ResolvedWorldFlags;
  for (const flag of WORLD_FLAGS) {
    result[flag] = resolveWorldFlag(settings, random, flag);
  }
  return result;
}
