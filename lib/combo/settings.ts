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
    { value: 'anywhere', name: 'Anywhere' },
    { value: 'starting', name: 'Starting Items' },
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
  key: 'smallKeyShuffleHideout',
  name: 'Hideout Small Key Shuffle',
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
  key: 'cowShuffleOot',
  name: 'Cow Shuffle (OoT)',
  category: 'main',
  type: 'boolean',
  default: false
}, {
  key: 'cowShuffleMm',
  name: 'Cow Shuffle (MM)',
  category: 'main',
  type: 'boolean',
  default: false
}, {
  key: 'shopShuffleOot',
  name: 'Shop Shuffle (OoT)',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'full', name: 'Full' },
  ],
  default: 'none'
}, {
  key: 'shuffleMasterSword',
  name: 'Master Sword Shuffle',
  category: 'main',
  type: 'boolean',
  default: true
}, {
  key: 'shuffleGerudoCard',
  name: 'Gerudo Card Shuffle',
  category: 'main',
  type: 'boolean',
  default: true
}, {
  key: 'eggShuffle',
  name: 'Weird / Pocket Egg Content Shuffle',
  category: 'main',
  type: 'boolean',
  default: false
}, {
  key: 'doorOfTime',
  name: 'Door of Time',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'closed', name: 'Closed' },
    { value: 'open', name: 'Open' },
  ],
  default: 'closed'
}, {
  key: 'crossWarpOot',
  name: 'Cross-Games OoT Warp Songs',
  category: 'main',
  type: 'boolean',
  default: false
}, {
  key: 'crossWarpMm',
  name: 'Cross-Games MM Song of Soaring',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'childOnly', name: 'Child Only' },
    { value: 'full', name: 'Child & Adult' },
  ],
  default: 'none'
}, {
  key: 'erBoss',
  name: 'Boss Entrance Shuffle',
  category: 'entrances',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'ownGame', name: 'Own Game' },
    { value: 'full', name: 'Full' },
  ],
  default: 'none'
}, {
  key: 'erDungeons',
  name: 'Dungeon Entrance Shuffle',
  category: 'entrances',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'ownGame', name: 'Own Game' },
    { value: 'full', name: 'Full' },
  ],
  default: 'none'
}, {
  key: 'erSpiderHouses',
  name: 'Shuffle Spider Houses with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erMinorDungeons',
  name: 'Shuffle OoT Minor Dungeons with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'progressiveShieldsOot',
  name: 'OoT Shields',
  category: 'items',
  type: 'enum',
  values: [
    { value: 'separate', name: 'Separate' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'separate'
}, {
  key: 'progressiveSwordsOot',
  name: 'OoT Swords',
  category: 'items',
  type: 'enum',
  values: [
    { value: 'separate', name: 'Separate' },
    { value: 'goron', name: 'Progressive Knife and Biggoron' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'goron'
}, {
  key: 'progressiveShieldsMm',
  name: 'MM Shields',
  category: 'items',
  type: 'enum',
  values: [
    { value: 'separate', name: 'Separate' },
    { value: 'start', name: 'Start with Hero Shield' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'start'
}, {
  key: 'progressiveGoronLullaby',
  name: 'MM Goron Lullaby',
  category: 'items',
  type: 'enum',
  values: [
    { value: 'single', name: 'Full Lullaby Only' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'progressive'
}, {
  key: 'sharedNutsSticks',
  name: 'Shared Nuts & Sticks',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'sharedBows',
  name: 'Shared Bows',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'sharedBombBags',
  name: 'Shared Bomb Bags',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMagic',
  name: 'Shared Magic',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMagicArrows',
  name: 'Shared Magic Arrows',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'sharedSongs',
  name: 'Shared Songs',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'sharedHookshot',
  name: 'Shared Hookshots',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'sharedLens',
  name: 'Shared Lens of Truth',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'sharedOcarina',
  name: 'Shared Ocarina of Time',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMasks',
  name: 'Shared Masks',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'sharedWallets',
  name: 'Shared Wallets',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'sharedHealth',
  name: 'Shared Health',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'csmc',
  name: 'Chest Size Matches Content',
  category: 'misc',
  type: 'boolean',
  default: false
}, {
  key: 'logic',
  name: 'Logic',
  category: 'advanced',
type: 'enum',
values: [
  { value: 'allLocations', name: 'All Locations' },
  { value: 'beatable', name: 'Beatable Only' },
  { value: 'none', name: 'No Logic' },
],
default: 'allLocations'
}, {
  key: 'generateSpoilerLog',
  name: 'Generate Spoiler Log',
  category: 'advanced',
  type: 'boolean',
  default: true
}] as const;

export const SETTINGS_CATEGORIES = [{
  name: "Main Settings",
  key: "main",
}, {
  name: "Items",
  key: "items",
}, {
  name: "Entrances",
  key: "entrances",
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
  OOT_DEKU_SKIP: "Deku Tree B1 Skip",
  OOT_DC_JUMP: "Dodongo's Cavern Upper Adult Jump",
  OOT_FOREST_HOOK: "Forest Temple Garden Vines with Hookshot",
  OOT_HAMMER_WALLS: "Hammer Through Walls",
  OOT_VOLCANO_HOVERS: "Volcano Item with Hover Boots",
  OOT_NIGHT_GS: "Nighttime Gold Skulltulas without Sun's Song",
  MM_LENS: "Fewer Lens Requirements (MM)",
  MM_PALACE_BEAN_SKIP: "Skip Planting Beans in Deku Palace",
  MM_DARMANI_WALL: "Climb Mountain Village Wall Blind",
  MM_NO_SEAHORSE: "Pinnacle Rock without Seahorse",
  MM_ZORA_HALL_HUMAN: "Swim to Zora Hall as Human",
  MM_ICELESS_IKANA: "Climb Ikana Canyon without Ice Arrows",
  MM_ONE_MASK_STONE_TOWER: "Climb Stone Tower with One Mask",
  MM_ISTT_EYEGORE: "Inverted Stone Tower Temple Eyegore Skips",
  MM_SCT_NOTHING: "South Clock Town Chest with Nothing",
  MM_GORON_BOMB_JUMP: "Bomb Jump Fences as Goron",
  MM_BOMBER_SKIP: "Guess Bomber Code",
  MM_CAPTAIN_SKIP: "Guess Oceanside Spider House Code",
};

export type Tricks = {[k in keyof typeof TRICKS]: boolean};
export type Trick = keyof Tricks;

const DEFAULT_ENABLED_TRICKS = new Set<Trick>(['OOT_NIGHT_GS']);

const DEFAULT_TRICKS = Object.keys(TRICKS).reduce((tricks, trick) => {
  tricks[trick as Trick] = DEFAULT_ENABLED_TRICKS.has(trick as Trick);
  return tricks;
}, {} as Tricks);

const DEFAULT_JUNK_LOCATIONS: string[] = [];

export const DUNGEONS = {
  DT: "Deku Tree",
  DC: "Dodongo's Cavern",
  JJ: "Jabu-Jabu",
  Forest: "Forest Temple",
  Fire: "Fire Temple",
  Water: "Water Temple",
  Spirit: "Spirit Temple",
  Shadow: "Shadow Temple",
  BotW: "Bottom of the Well",
  IC: "Ice Cavern",
  GTG: "Gerudo Training Grounds",
  Ganon: "Ganon's Castle",
} as const;

export type DungeonSettings = {[k in keyof typeof DUNGEONS]: 'vanilla' | 'mq' | 'random'};

const DEFAULT_DUNGEONS = Object.keys(DUNGEONS).reduce((dungeons, dungeon) => {
  dungeons[dungeon] = 'vanilla';
  return dungeons;
}, {} as any) as DungeonSettings;

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
  junkLocations: string[],
  tricks: Tricks,
  dungeon: DungeonSettings,
};

export const DEFAULT_SETTINGS: Settings = { ...SETTINGS.map(s => {
  return {[s.key]: s.default};
}).reduce((a, b) => ({...a, ...b}), {}), startingItems: {}, junkLocations: DEFAULT_JUNK_LOCATIONS, tricks: { ...DEFAULT_TRICKS }, dungeon: { ...DEFAULT_DUNGEONS } } as Settings;

export const settings = (s: Partial<Settings>): Settings => ({...DEFAULT_SETTINGS, ...s});
