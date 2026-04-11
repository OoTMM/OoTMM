import type { Location } from '../types';

export type SphereEntryLocation = {
  type: 'location';
  location: Location;
};

export type SphereEntryEvent = {
  type: 'event';
  event: string;
  playerId: number;
};

export type SphereEntry = SphereEntryLocation | SphereEntryEvent;

export type Triforce3Type = 'Power' | 'Courage' | 'Wisdom';

export type AnalysisPathTypeWotH = { type: 'woth' };
export type AnalysisPathTypeTriforce = { type: 'triforce', triforce: Triforce3Type };
export type AnalysisPathTypeDungeon = { type: 'dungeon', dungeon: string };
export type AnalysisPathTypeBoss = { type: 'boss', boss: string };
export type AnalysisPathTypeEndBoss = { type: 'end-boss', boss: string };
export type AnalysisPathTypeEvent = { type: 'event', event: string };
export type AnalysisPathType =
  | AnalysisPathTypeWotH
  | AnalysisPathTypeTriforce
  | AnalysisPathTypeDungeon
  | AnalysisPathTypeBoss
  | AnalysisPathTypeEndBoss
  | AnalysisPathTypeEvent;

export type AnalysisPathBase = { player: number | 'all', locations: Set<Location> };
export type AnalysisPath = AnalysisPathBase & AnalysisPathType;

export type Analysis = {
  spheres: SphereEntry[][];
  required: Set<Location>;
  useless: Set<Location>;
  unreachable: Set<Location>;
  paths: AnalysisPath[];
};
