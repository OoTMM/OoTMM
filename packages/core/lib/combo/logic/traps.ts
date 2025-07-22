import { Settings } from '../settings';

export const TRAP_AMOUNTS: Record<Settings['trapsQuantity'], number> = {
  small: 10,
  medium: 30,
  large: 50,
  verylarge: 100,
  extreme: 200,
  insane: 500,
  obnoxious: 1000,
  absurd: 5000,
};
