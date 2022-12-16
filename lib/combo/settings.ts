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
  key: 'ganonBossKey',
  name: 'Ganon Boss Key Shuffle',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'removed', name: 'Removed' },
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'ganon', name: 'Ganon\'s Castle' },
  ],
  default: 'removed'
}, {
  key: 'shuffleGerudoCard',
  name: 'Gerudo Card Shuffle',
  category: 'main',
  type: 'boolean',
  default: true
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
  name: "Advanced",
  key: "advanced",
}];

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

export type Settings = UnionToIntersection<SettingShapes>;

export const DEFAULT_SETTINGS: Settings = SETTINGS.map(s => {
  return {[s.key]: s.default};
}).reduce((a, b) => ({...a, ...b}), {}) as Settings;

export const settings = (s: Partial<Settings>): Settings => ({...DEFAULT_SETTINGS, ...s});
