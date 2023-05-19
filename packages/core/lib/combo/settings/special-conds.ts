import type { Settings } from './type';

export const SPECIAL_CONDS_KEYS = {
  stones: "Spiritual Stones",
  medallions: "Medallions",
  remains: "Boss Remains",
  skullsGold: "Gold Skulltulas Tokens",
  skullsSwamp: "Swamp Skulltulas Tokens",
  skullsOcean: "Ocean Skulltulas Tokens",
  fairiesWF: "Stray Fairies (Woodfall)",
  fairiesSH: "Stray Fairies (Snowhead)",
  fairiesGB: "Stray Fairies (Great Bay)",
  fairiesST: "Stray Fairies (Stone Tower)",
  fairyTown: "Stray Fairy (Clock Town)",
  masksRegular: "Regular Masks (MM)",
  masksTransform: "Transformation Masks (MM)",
  masksOot: "Masks (OoT)",
  triforce: "Triforce Pieces",
};

export type SpecialCond = {[k in keyof typeof SPECIAL_CONDS_KEYS]: boolean} & { count: number };

export const DEFAULT_SPECIAL_COND = Object.keys(SPECIAL_CONDS_KEYS).reduce((conds, cond) => {
  conds[cond] = false;
  return conds;
}, { count: 0 } as any) as SpecialCond;

type SettingCond = (s: Settings) => boolean;

type SpecialCondDefiniton = {
  name: string;
  cond?: SettingCond;
};

export const SPECIAL_CONDS: {[k: string]: SpecialCondDefiniton} = {
  BRIDGE: { name: "Rainbow Bridge" },
  MOON: { name: "Moon Access" },
  LACS: { name: "Light Arrow Cutscene", cond: s => s.lacs === 'custom' },
  GANON_BK: { name: "Ganon Boss Key", cond: s => s.ganonBossKey === 'custom' },
  MAJORA: { name: "Majora Child Requirements", cond: s => s.majoraChild === 'custom' },
};

export const DEFAULT_SPECIAL_CONDS: SpecialConds = {
  BRIDGE: { ...DEFAULT_SPECIAL_COND, medallions: true, count: 6 },
  MOON: { ...DEFAULT_SPECIAL_COND, remains: true, count: 4 },
  LACS: { ...DEFAULT_SPECIAL_COND },
  GANON_BK: { ...DEFAULT_SPECIAL_COND },
  MAJORA: { ...DEFAULT_SPECIAL_COND },
};

export type SpecialConds = {[k in keyof typeof SPECIAL_CONDS]: SpecialCond };
