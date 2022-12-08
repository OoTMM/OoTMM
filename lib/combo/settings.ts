export const SETTINGS = [{
  name: 'Song Shuffle',
  key: 'songs',
  type: 'enum',
  values: {
    songLocations: 'Song Locations',
    anywhere: 'Anywhere',
  },
  default: 'songLocations'
}] as const;

type SettingDataEnum = {
  readonly name: string;
  readonly key: string;
  readonly type: 'enum';
  readonly values: {[k: string]: string};
  readonly default: string;
};

type InputToShape<T> = T extends SettingDataEnum ? { [K in T['key']]: keyof T['values'] } : never;

type SettingDataEntry = typeof SETTINGS[number];

type SettingShapes = InputToShape<SettingDataEntry>;

export type Settings = SettingShapes;

const DEFAULT_SETTINGS: Settings = SETTINGS.map(s => {
  if (s.type === 'enum') {
    return {[s.key]: s.default};
  }
  return {};
}).reduce((a, b) => ({...a, ...b}), {}) as Settings;

export const settings = (s: Partial<Settings>): Settings => ({...DEFAULT_SETTINGS, ...s});
