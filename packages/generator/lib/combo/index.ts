export type { Settings, SettingsPatch, SettingHint, SettingHintType, TrickKey, ItemsCount } from '@ootmm/core';

export type { Presets } from './presets';
export type { Cosmetics } from './cosmetics';
export type { OptionRandomSettings } from './random-settings';
export type { GeneratorOutput, GeneratorParams } from './generator';
export type { Items } from './item-pool';
export type { OptionsInput, Options } from './options';

export { SETTINGS_DEFAULT_HINTS, HINT_TYPES, SETTINGS, TRICKS, SPECIAL_CONDS, SPECIAL_CONDS_FIELDS, makeSettings, mergeSettings, exportSettings, importSettings, DEFAULT_SETTINGS, SUBCATEGORIES } from '@ootmm/core';
export { locationList } from '@ootmm/logic';

export { PRESETS } from './presets';
export { COSMETICS, makeCosmetics } from './cosmetics';
export { COLORS } from './cosmetics/color';
export { makeRandomSettings } from './random-settings';
export { generate } from './generator';
export { itemPool } from './item-pool';
export { itemName } from './names';
