import { Settings } from '../settings';

export const TRAP_AMOUNTS: Record<Settings['trapsQuantity'], number> = {
  small: 5,
  medium: 10,
  large: 20,
  verylarge: 50,
  extreme: 100,
  insane: 500,
  obnoxious: 1000,
  absurd: 5000,
};
