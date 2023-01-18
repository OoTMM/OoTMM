export const SETTINGS = [{
  key: 'songs',
  name: 'Song Shuffle',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'songLocations', name: 'Song Locations' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'songLocations'
}, {
  key: 'goldSkulltulaTokens',
  name: 'Gold Skulltula Tokens Shuffle',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'none', name: 'No Shuffle' },
    { value: 'dungeons', name: 'Dungeons Only' },
    { value: 'overworld', name: 'Overworld Only' },
    { value: 'all', name: 'All Tokens' },
  ],
  default: 'none'
}, {
  key: 'housesSkulltulaTokens',
  name: 'Houses Skulltula Tokens Shuffle',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'none', name: 'No Shuffle' },
    { value: 'cross', name: 'Gold Skulltulas Only' },
    { value: 'all', name: 'All Tokens' },
  ],
  default: 'none'
}, {
  key: 'mapCompassShuffle',
  name: 'Map / Compass Shuffle',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon' },
    { value: 'removed', name: 'Removed' },
  ],
  default: 'ownDungeon'
}, {
  key: 'smallKeyShuffle',
  name: 'Small Key Shuffle',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'bossKeyShuffle',
  name: 'Boss Key Shuffle',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'townFairyShuffle',
  name: 'Town Stray Fairy Shuffle',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'vanilla'
}, {
  key: 'strayFairyShuffle',
  name: 'Dungeon Stray Fairy Shuffle',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'mixed', name: 'Chests Own Dungeon' },
    { value: 'ownDungeon', name: 'Own Dungeon' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'mixed'
}, {
  key: 'ganonBossKey',
  name: 'Ganon Boss Key Shuffle',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'removed', name: 'Removed' },
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'ganon', name: 'Ganon\'s Castle' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'removed'
}, {
  key: 'dungeonRewardShuffle',
  name: 'Dungeon Reward Shuffle',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'dungeonBlueWarps', name: 'Dungeon Blue Warps' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'dungeonBlueWarps'
}, {
  key: 'shuffleGerudoCard',
  name: 'Gerudo Card Shuffle',
  category: 'main',
  type: 'boolean',
  default: true
}, {
  key: 'csmc',
  name: 'Chest Size Matches Content',
  category: 'misc',
  type: 'boolean',
  default: false
}, {
  key: 'noLogic',
  name: 'No Logic',
  category: 'advanced',
  type: 'boolean',
  default: false
}] as const;

export const SETTINGS_CATEGORIES = [{
  name: "Main Settings",
  key: "main",
}, {
  name: "Misc",
  key: "misc",
}, {
  name: "Advanced",
  key: "advanced",
}];

export const TRICKS = {
  OOT_LENS: "Fewer Lens Requirements (OoT)",
  OOT_TUNICS: "Fewer Tunic Requirements",
  OOT_HIDDEN_GROTTOS: "Hidden Grottos (OoT) without Stone of Agony",
  OOT_MIDO_SKIP: "Backflip Over Mido",
  OOT_MAN_ON_ROOF: "Man on Roof with Nothing",
  OOT_BLIND_WASTELAND: "Blind Wasteland Crossing",
  OOT_NIGHT_GS: "Nighttime Gold Skulltulas without Sun's Song",
  MM_LENS: "Fewer Lens Requirements (MM)",
  MM_GORON_BOMB_JUMP: "Bomb Jump Fences as Goron",
  MM_NO_SEAHORSE: "Pinnacle Rock without Seahorse",
  MM_ZORA_HALL_HUMAN: "Swim to Zora Hall as Human",
  MM_ICELESS_IKANA: "Climb Ikana Canyon without Ice Arrows",
  MM_ONE_MASK_STONE_TOWER: "Climb Stone Tower with One Mask",
};

export type Tricks = {[k in keyof typeof TRICKS]: boolean};
export type Trick = keyof Tricks;

const DEFAULT_ENABLED_TRICKS = new Set<Trick>(['OOT_NIGHT_GS']);

const DEFAULT_TRICKS = Object.keys(TRICKS).reduce((tricks, trick) => {
  tricks[trick as Trick] = DEFAULT_ENABLED_TRICKS.has(trick as Trick);
  return tricks;
}, {} as Tricks);

type SettingDataEnumValue = {
  readonly value: string;
  readonly name: string;
};

type SettingDataCommon = {
  readonly key: string;
  readonly name: string;
  readonly category: string;
};

type SettingDataEnum = SettingDataCommon & {
  readonly type: 'enum';
  readonly values: ReadonlyArray<SettingDataEnumValue>;
  readonly default: string;
};

type SettingDataBoolean = SettingDataCommon & {
  readonly type: 'boolean';
  readonly default: boolean;
};

type InputToShape<T> = T extends SettingDataEnum ? { [K in T['key']]: T['values'][number]['value'] }
  : T extends SettingDataBoolean ? { [K in T['key']]: boolean }
  : never;

type SettingDataEntry = typeof SETTINGS[number];

type SettingShapes = InputToShape<SettingDataEntry>;

type UnionToIntersection<U> =
  (U extends any ? (k: U)=>void : never) extends ((k: infer I)=>void) ? I : never

type SettingsBase = UnionToIntersection<SettingShapes>;

export type Settings = SettingsBase & {
  startingItems: {[k: string]: number},
  tricks: Tricks,
};

export const DEFAULT_SETTINGS: Settings = { ...SETTINGS.map(s => {
  return {[s.key]: s.default};
}).reduce((a, b) => ({...a, ...b}), {}), startingItems: {}, tricks: { ...DEFAULT_TRICKS } } as Settings;

export const settings = (s: Partial<Settings>): Settings => ({...DEFAULT_SETTINGS, ...s});
