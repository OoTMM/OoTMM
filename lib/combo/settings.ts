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
}] as const;

export const SETTINGS_CATEGORIES = [{
  name: "Main Settings",
  key: "main",
}];

type SettingDataEnumValue = {
  readonly value: string;
  readonly name: string;
};

type SettingDataEnum = {
  readonly key: string;
  readonly name: string;
  readonly type: 'enum';
  readonly values: ReadonlyArray<SettingDataEnumValue>;
  readonly default: string;
};

type InputToShape<T> = T extends SettingDataEnum ? { [K in T['key']]: T['values'][number]['value'] } : never;

type SettingDataEntry = typeof SETTINGS[number];

type SettingShapes = InputToShape<SettingDataEntry>;

export type Settings = SettingShapes;

export const DEFAULT_SETTINGS: Settings = SETTINGS.map(s => {
  if (s.type === 'enum') {
    return {[s.key]: s.default};
  }
  return {};
}).reduce((a, b) => ({...a, ...b}), {}) as Settings;

export const settings = (s: Partial<Settings>): Settings => ({...DEFAULT_SETTINGS, ...s});
