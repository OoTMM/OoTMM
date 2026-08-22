export type CheckType = 'npc' | 'chest' | 'collectible' | 'gs' | 'sf' | 'cow' | 'shop' | 'scrub' | 'sr' | 'pot' | 'crate' | 'barrel' | 'grass' | 'tree' | 'bush' | 'rock' | 'soil' | 'wonder' | 'fish' | 'fairy' | 'snowball' | 'hive' | 'rupee' | 'heart' | 'fairy_spot' | 'butterfly' | 'boulder' | 'boulder-red' | 'boulder-silver' | 'icicle' | 'redice';
export type CheckOverrideType = 'npc' | 'chest' | 'collectible' | 'gs' | 'sf' | 'cow' | 'shop' | 'scrub' | 'sr' | 'xflag';

export type CheckData = {
  readonly ov: CheckOverrideType;
  readonly type: CheckType;
  readonly location: string;
  readonly hint: string;
  readonly key: number;
  readonly item: string;
};

import DATA_RAW from '../dist/data-checks.json';

const CHECKS_OOT = DATA_RAW.oot as CheckData[];
const CHECKS_MM = DATA_RAW.mm as CheckData[];

export const CHECKS = {
  oot: CHECKS_OOT,
  mm: CHECKS_MM,
};
