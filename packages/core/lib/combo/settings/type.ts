import { SETTINGS } from './data';
import { DungeonSettings } from './dungeons';
import { SettingHint } from './hints';
import { SpecialConds } from './special-conds';
import { Trick } from './tricks';

type SettingDataEnumValue = {
  readonly value: string;
  readonly name: string;
  readonly description?: string;
};

type SettingDataCommon = {
  readonly key: string;
  readonly name: string;
  readonly category: string;
  readonly description?: string;
  readonly cond?: (settings: any) => boolean;
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

export type SettingsBase = UnionToIntersection<SettingShapes>;

type SettingsPlando = {
  locations: {[k: string]: string | null},
};
export type Settings = SettingsBase & {
  startingItems: {[k: string]: number};
  junkLocations: string[];
  tricks: Trick[];
  dungeon: DungeonSettings;
  specialConds: SpecialConds;
  plando: SettingsPlando;
  hints: SettingHint[];
};
