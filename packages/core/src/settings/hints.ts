export const HINT_TYPES = {
  woth: 'Way of the Hero',
  always: 'Always',
  sometimes: 'Sometimes',
  playthrough: 'Playthrough',
  item: 'Specific Item',
  foolish: 'Foolish',
  junk: 'Junk',
} as const;

export type SettingHintType = keyof typeof HINT_TYPES;

export type SettingHint = {
  type: SettingHintType;
  item?: string;
  amount: number | 'max';
  extra: number;
};

export const SETTINGS_DEFAULT_HINTS: SettingHint[] = [
  { type: 'always', amount: 'max', extra: 1 },
  { type: 'sometimes', amount: 3, extra: 1 },
  { type: 'foolish', amount: 5, extra: 1 },
  { type: 'item', item: 'MM_SONG_SOARING', amount: 1, extra: 1 },
  { type: 'playthrough', amount: 4, extra: 1 },
  { type: 'woth', amount: 9, extra: 1 },
  { type: 'sometimes', amount: 'max', extra: 1 },
];
