import { merge } from 'lodash';
import type { PartialDeep } from 'type-fest';

export const SETTINGS = [{
  key: 'logic',
  name: 'Logic',
  category: 'main',
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
  category: 'main',
  type: 'boolean',
  default: true
}, {
  key: 'songs',
  name: 'Song Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'songLocations', name: 'Song Locations' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'songLocations'
}, {
  key: 'goldSkulltulaTokens',
  name: 'Gold Skulltula Tokens Shuffle',
  category: 'main.shuffle',
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
  category: 'main.shuffle',
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
  category: 'main.shuffle',
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
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'smallKeyShuffleHideout',
  name: 'Hideout Small Key Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'bossKeyShuffle',
  name: 'Boss Key Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'townFairyShuffle',
  name: 'Town Stray Fairy Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'vanilla'
}, {
  key: 'strayFairyShuffle',
  name: 'Dungeon Stray Fairy Shuffle',
  category: 'main.shuffle',
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
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'removed', name: 'Removed' },
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'ganon', name: 'Ganon\'s Castle' },
    { value: 'anywhere', name: 'Anywhere' },
    { value: 'custom', name: 'Custom' },
  ],
  default: 'removed'
}, {
  key: 'dungeonRewardShuffle',
  name: 'Dungeon Reward Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'dungeonBlueWarps', name: 'Dungeon Blue Warps' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'dungeonBlueWarps'
}, {
  key: 'scrubShuffleOot',
  name: 'Scrub Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  default: false
}, {
  key: 'cowShuffleOot',
  name: 'Cow Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  default: false
}, {
  key: 'cowShuffleMm',
  name: 'Cow Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  default: false
}, {
  key: 'shopShuffleOot',
  name: 'Shop Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'full', name: 'Full' },
  ],
  default: 'none'
}, {
  key: 'shopShuffleMm',
  name: 'Shop Shuffle (MM)',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'full', name: 'Full' },
  ],
  default: 'none'
}, {
  key: 'shuffleMasterSword',
  name: 'Master Sword Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  default: true
}, {
  key: 'shuffleGerudoCard',
  name: 'Gerudo Card Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  default: true
}, {
  key: 'eggShuffle',
  name: 'Weird / Pocket Egg Content Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  default: false
}, {
  key: 'doorOfTime',
  name: 'Door of Time',
  category: 'main.events',
  type: 'enum',
  values: [
    { value: 'closed', name: 'Closed' },
    { value: 'open', name: 'Open' },
  ],
  default: 'closed'
}, {
  key: 'kakarikoGate',
  name: 'Kakariko Gate',
  category: 'main.events',
  type: 'enum',
  values: [
    { value: 'closed', name: 'Closed' },
    { value: 'open', name: 'Open' },
  ],
  default: 'closed'
}, {
  key: 'skipZelda',
  name: 'Skip Child Zelda',
  category: 'main.events',
  type: 'boolean',
  default: false,
}, {
  key: 'lacs',
  name: 'Light Arrow Cutscene',
  category: 'main.events',
  type: 'enum',
  values: [
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'custom', name: 'Custom' },
  ],
  default: 'vanilla'
}, {
  key: 'crossWarpOot',
  name: 'Cross-Games OoT Warp Songs',
  category: 'main.cross',
  type: 'boolean',
  default: false
}, {
  key: 'crossWarpMm',
  name: 'Cross-Games MM Song of Soaring',
  category: 'main.cross',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'childOnly', name: 'Child Only' },
    { value: 'full', name: 'Child & Adult' },
  ],
  default: 'none'
}, {
  key: 'sunSongMm',
  name: 'Sun\'s Song in MM',
  category: 'main.cross',
  type: 'boolean',
  default: false
}, {
  key: 'csmc',
  name: 'Chest Size Matches Content',
  category: 'main.misc',
  type: 'boolean',
  default: false
}, {
  key: 'progressiveShieldsOot',
  name: 'OoT Shields',
  category: 'items.progressive',
  type: 'enum',
  values: [
    { value: 'separate', name: 'Separate' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'separate'
}, {
  key: 'progressiveSwordsOot',
  name: 'OoT Swords',
  category: 'items.progressive',
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
  category: 'items.progressive',
  type: 'enum',
  values: [
    { value: 'separate', name: 'Separate' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'start'
}, {
  key: 'progressiveGoronLullaby',
  name: 'MM Goron Lullaby',
  category: 'items.progressive',
  type: 'enum',
  values: [
    { value: 'single', name: 'Full Lullaby Only' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'progressive'
}, {
  key: 'fairyOcarinaMm',
  name: 'Fairy Ocarina in MM',
  category: 'items.extensions',
  type: 'boolean',
  default: false
}, {
  key: 'shortHookshotMm',
  name: 'Short Hookshot in MM',
  category: 'items.extensions',
  type: 'boolean',
  default: false
}, {
  key: 'sharedNutsSticks',
  name: 'Shared Nuts & Sticks',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedBows',
  name: 'Shared Bows',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedBombBags',
  name: 'Shared Bomb Bags',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMagic',
  name: 'Shared Magic',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMagicArrowFire',
  name: 'Shared Fire Arrow',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMagicArrowIce',
  name: 'Shared Ice Arrow',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMagicArrowLight',
  name: 'Shared Light Arrow',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedSongEpona',
  name: 'Shared Epona\'s Song',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedSongStorms',
  name: 'Shared Song of Storms',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedSongTime',
  name: 'Shared Song of Time',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedSongSun',
  name: 'Shared Sun\'s Song',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedHookshot',
  name: 'Shared Hookshots',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedLens',
  name: 'Shared Lens of Truth',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedOcarina',
  name: 'Shared Ocarina of Time',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMasks',
  name: 'Shared Masks',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedWallets',
  name: 'Shared Wallets',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedHealth',
  name: 'Shared Health',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'probabilisticFoolish',
  name: 'Probabilistic Foolish Hints (If you don\'t know what this is, leave it ON)',
  category: 'hints',
  type: 'boolean',
  default: true
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
  key: 'erMinorDungeons',
  name: 'Shuffle OoT Minor Dungeons with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erSpiderHouses',
  name: 'Shuffle Spider Houses with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erPirateFortress',
  name: 'Shuffle Pirate Fortress with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erBeneathWell',
  name: 'Shuffle Beneath the Well with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erIkanaCastle',
  name: 'Shuffle Ikana Castle Interior with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erSecretShrine',
  name: 'Shuffle Secret Shrine with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erOverworld',
  name: 'Shuffle Major Regions',
  category: 'entrances',
  type: 'boolean',
  default: false
}] as const;

export type SettingCategory = {
  name: string;
  key: string;
  subcategories?: SettingCategory[];
};
export const SETTINGS_CATEGORIES: SettingCategory[] = [{
  name: "Main Settings",
  key: "main",
  subcategories: [{
    name: "Shuffle",
    key: "shuffle",
  }, {
    name: "Events",
    key: "events",
  }, {
    name: "Cross-Game",
    key: "cross",
  }, {
    name: "Misc.",
    key: "misc",
  }]
}, {
  name: "Items",
  key: "items",
  subcategories: [{
    name: "Progressive Items",
    key: "progressive",
  }, {
    name: "Shared Items",
    key: "shared",
  }, {
    name: "Item Extensions",
    key: "extensions",
  }],
}, {
  name: "Hints",
  key: "hints",
}, {
  name: "Entrances",
  key: "entrances",
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
  OOT_CHILD_DOMAIN: "Enter Child Zora's Domain with Cucco",
  OOT_ADULT_DOMAIN: "Enter Adult Zora's Domain using Hover Boots",
  OOT_WATER_LONGSHOT: "Drain Water Temple using Longshot",
  OOT_SAND_RIVER_NOTHING: "Cross the River of Sand with Nothing",
  OOT_SHADOW_FIRE_ARROW: "Enter Shadow Temple using Fire Arrows",
  OOT_KZ_SKIP: "Skip King Zora as Adult",
  OOT_LOST_WOODS_ADULT_GS: "Lost Woods Adult GS without Bean",
  OOT_WINDMILL_HP_NOTHING: "Windmill HP as Adult with Nothing",
  OOT_LAB_DIVE_NO_GOLD_SCALE: "Laboratory Dive without Gold Scale",
  OOT_LAB_WALL_GS: "Laboratory Wall GS with Jump Slash",
  OOT_PASS_COLLISION: "Pass through Visible One-Way Collisions",
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
  MM_ISTT_ENTRY_JUMP: "Inverted Stone Tower Temple Long Jump to Death Armos",
  MM_SHORT_HOOK_HARD: "Precise Short Hookshot Usage",
  MM_PFI_BOAT_HOOK: "Enter Pirate Fortress Interior using Hookshot from the Boats",
  MM_PALACE_GUARD_SKIP: "Backflip over Deku Palace Guards",
  MM_SHT_FIRELESS: "Complete Snowhead Temple without Fire Arrows",
  MM_KEG_EXPLOSIVES: "Use Powder Kegs as Explosives",
  MM_DOG_RACE_CHEST_NOTHING: "Doggy Racetrack Chest with Nothing",
  MM_MAJORA_LOGIC: "Fight Majora to Reset Time",
  MM_SOUTHERN_SWAMP_SCRUB_HP_GORON: "Southern Swamp Scrub HP as Goron",
  MM_ZORA_HALL_SCRUB_HP_NO_DEKU: "Zora Hall Scrub HP without Deku",
  MM_IKANA_ROOF_PARKOUR: "Jump from Ikana Castle's Roof Interior to Exterior",
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
};

export type SpecialCond = {[k in keyof typeof SPECIAL_CONDS_KEYS]: boolean} & { count: number };

const DEFAULT_SPECIAL_COND = Object.keys(SPECIAL_CONDS_KEYS).reduce((conds, cond) => {
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
};

const DEFAULT_SPECIAL_CONDS: SpecialConds = {
  BRIDGE: { ...DEFAULT_SPECIAL_COND, medallions: true, count: 6 },
  MOON: { ...DEFAULT_SPECIAL_COND, remains: true, count: 4 },
  LACS: { ...DEFAULT_SPECIAL_COND },
  GANON_BK: { ...DEFAULT_SPECIAL_COND },
};

export type SpecialConds = {[k in keyof typeof SPECIAL_CONDS]: SpecialCond };

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

type SettingsPlando = {
  locations: {[k: string]: string | null},
};
export type Settings = SettingsBase & {
  startingItems: {[k: string]: number};
  junkLocations: string[];
  tricks: Tricks;
  dungeon: DungeonSettings;
  specialConds: SpecialConds;
  plando: SettingsPlando;
};

export const DEFAULT_SETTINGS: Settings = { ...SETTINGS.map(s => {
  return {[s.key]: s.default};
}).reduce((a, b) => ({...a, ...b}), {}),
  startingItems: {},
  junkLocations: DEFAULT_JUNK_LOCATIONS,
  tricks: { ...DEFAULT_TRICKS },
  dungeon: { ...DEFAULT_DUNGEONS },
  specialConds: { ...DEFAULT_SPECIAL_CONDS },
  plando: { locations: {} },
} as Settings;

export function mergeSettings(base: Settings, arg: PartialDeep<Settings>): Settings {
  return merge({}, base, arg);
}

export function makeSettings(arg: PartialDeep<Settings>): Settings {
  return mergeSettings(DEFAULT_SETTINGS, arg);
}
