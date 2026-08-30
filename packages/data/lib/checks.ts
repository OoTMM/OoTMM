import type { Game } from './game';

export type CheckType = 'npc' | 'chest' | 'collectible' | 'gs' | 'sf' | 'cow' | 'shop' | 'scrub' | 'sr' | 'pot' | 'crate' | 'barrel' | 'grass' | 'tree' | 'bush' | 'rock' | 'soil' | 'wonder' | 'fish' | 'fairy' | 'snowball' | 'hive' | 'rupee' | 'heart' | 'fairy_spot' | 'butterfly' | 'boulder' | 'boulder-red' | 'boulder-silver' | 'icicle' | 'redice' | 'gossip' | 'gossip-big';
export type CheckOverrideType = 'npc' | 'chest' | 'collectible' | 'gs' | 'sf' | 'cow' | 'shop' | 'scrub' | 'sr' | 'xflag';

export type CheckData = {
  readonly game: Game;
  readonly ov: CheckOverrideType;
  readonly type: CheckType;
  readonly location: string;
  readonly scene: string;
  readonly hint?: string;
  readonly key: number;
  readonly item: string;
};

import DATA_RAW from '../dist/data-checks.json';
export const CHECKS = DATA_RAW as CheckData[];
export const CHECKS_BY_LOCATION: { [k: string]: CheckData } = Object.fromEntries(CHECKS.map(c => [c.location, c]));
