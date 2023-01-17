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
}, {
  key: 'trickOoTLens',
  name: 'Fewer Lens Requirements (OoT)',
  category: 'trick',
  type: 'boolean',
  default: false
}, {
  key: 'trickFewerTunic',
  name: 'Fewer Tunic Requirements',
  category: 'trick',
  type: 'boolean',
  default: false
}, {
  key: 'trickHiddenGrottos',
  name: 'Hidden Grottos (OoT) without Stone of Agony',
  category: 'trick',
  type: 'boolean',
  default: false
}, {
  key: 'trickMido',
  name: 'Backflip Over Mido',
  category: 'trick',
  type: 'boolean',
  default: false
}, {
  key: 'trickManRoof',
  name: 'Man on Roof with Nothing',
  category: 'trick',
  type: 'boolean',
  default: false
}, {
  key: 'trickBlindWasteland',
  name: 'Blind Wasteland Crossing',
  category: 'trick',
  type: 'boolean',
  default: false
}, {
  key: 'trickNightSkulls',
  name: 'Nighttime Gold Skulltulas without Sun\'s Song',
  category: 'trick',
  type: 'boolean',
  default: true
}, {
  key: 'trickMMLens',
  name: 'Fewer Lens Requirements (MM)',
  category: 'trick',
  type: 'boolean',
  default: false
}, {
  key: 'trickGoronBomb',
  name: 'Bomb Jump Fences as Goron',
  category: 'trick',
  type: 'boolean',
  default: false
}, {
  key: 'tricknoSeahorse',
  name: 'Pinnacle Rock without Seahorse',
  category: 'trick',
  type: 'boolean',
  default: false
}, {
  key: 'trickZoraHallHuman',
  name: 'Swim to Zora Hall as Human',
  category: 'trick',
  type: 'boolean',
  default: false
}, {
  key: 'trickIcelessIkana',
  name: 'Climb Ikana Canyon without Ice Arrows',
  category: 'trick',
  type: 'boolean',
  default: false
}, {
  key: 'trickOneMaskStone',
  name: 'Climb Stone Tower with One Mask',
  category: 'trick',
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
}, {
  name: "Logic Tricks",
  key: "trick",
}];

type Tricks = {[k: string]: boolean};
export const TRICKS: Tricks = {
  TEST: false,
};

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
}).reduce((a, b) => ({...a, ...b}), {}), startingItems: {}, tricks: { ...TRICKS } } as Settings;

export const settings = (s: Partial<Settings>): Settings => ({...DEFAULT_SETTINGS, ...s});
