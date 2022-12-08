import { Settings } from '../settings';
import { isSong } from './items';

export const CONSTRAINT_NONE = 'none';
export const CONSTRAINT_SONG = 'song';
export const CONSTRAINTS = [
  CONSTRAINT_NONE,
  CONSTRAINT_SONG
] as const;

export type Constraint = typeof CONSTRAINTS[number];

export const itemConstraint = (item: string, settings: Settings): Constraint => {
  if (isSong(item)) {
    if (settings.songs === 'songLocations') {
      return CONSTRAINT_SONG;
    }
  }
  return CONSTRAINT_NONE;
};
