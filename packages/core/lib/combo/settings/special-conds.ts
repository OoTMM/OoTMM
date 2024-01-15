import type { Settings } from './type';

type SettingCond = (s: Settings) => boolean;

type SpecialCondField = {
  name: string;
  cond?: SettingCond;
};

export const SPECIAL_CONDS_FIELDS = {
  stones:           { name: "Spiritual Stones" },
  medallions:       { name: "Medallions" },
  remains:          { name: "Boss Remains" },
  skullsGold:       { name: "Gold Skulltulas Tokens" },
  skullsSwamp:      { name: "Swamp Skulltulas Tokens" },
  skullsOcean:      { name: "Ocean Skulltulas Tokens" },
  fairiesWF:        { name: "Stray Fairies (Woodfall)" },
  fairiesSH:        { name: "Stray Fairies (Snowhead)" },
  fairiesGB:        { name: "Stray Fairies (Great Bay)" },
  fairiesST:        { name: "Stray Fairies (Stone Tower)" },
  fairyTown:        { name: "Stray Fairy (Clock Town)" },
  masksRegular:     { name: "Regular Masks (MM)" },
  masksTransform:   { name: "Transformation Masks (MM)" },
  masksOot:         { name: "Masks (OoT)" },
  triforce:         { name: "Triforce Pieces", cond: (s: Settings) => s.goal === 'triforce' },
  coinsRed:         { name: "Coins (Red)", cond: (s: Settings) => s.coinsRed > 0 },
  coinsGreen:       { name: "Coins (Green)", cond: (s: Settings) => s.coinsGreen > 0 },
  coinsBlue:        { name: "Coins (Blue)", cond: (s: Settings) => s.coinsBlue > 0 },
  coinsYellow:      { name: "Coins (Yellow)", cond: (s: Settings) => s.coinsYellow > 0 },
};

export type SpecialCond = {[k in keyof typeof SPECIAL_CONDS_FIELDS]: boolean} & { count: number };

export const DEFAULT_SPECIAL_COND = Object.keys(SPECIAL_CONDS_FIELDS).reduce((conds, cond) => {
  conds[cond] = false;
  return conds;
}, { count: 0 } as any) as SpecialCond;

type SpecialCondDefiniton = {
  name: string;
  cond?: SettingCond;
};

export const SPECIAL_CONDS: {[k: string]: SpecialCondDefiniton} = {
  BRIDGE: { name: "Rainbow Bridge", cond: s => s.rainbowBridge === 'custom' },
  MOON: { name: "Moon Access" },
  LACS: { name: "Light Arrow Cutscene", cond: s => s.lacs === 'custom' },
  GANON_BK: { name: "Ganon Boss Key", cond: s => s.ganonBossKey === 'custom' },
  MAJORA: { name: "Majora Child Requirements", cond: s => s.majoraChild === 'custom' },
};

export const DEFAULT_SPECIAL_CONDS: SpecialConds = {
  BRIDGE: { ...DEFAULT_SPECIAL_COND },
  MOON: { ...DEFAULT_SPECIAL_COND, remains: true, count: 4 },
  LACS: { ...DEFAULT_SPECIAL_COND },
  GANON_BK: { ...DEFAULT_SPECIAL_COND },
  MAJORA: { ...DEFAULT_SPECIAL_COND },
};

export type SpecialConds = {[k in keyof typeof SPECIAL_CONDS]: SpecialCond };
