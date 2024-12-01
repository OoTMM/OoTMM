export type DungeonMetadata = {
  readonly id: number;
  readonly key: string;
  readonly name: string;
};

const DUNGEONS_RAW = [
  { key: 'DT', name: 'Deku Tree' },
  { key: 'DC', name: 'Dodongo Cavern' },
  { key: 'JJ', name: 'Jabu-Jabu' },
  { key: 'Forest', name: 'Forest Temple' },
  { key: 'Fire', name: 'Fire Temple' },
  { key: 'Water', name: 'Water Temple' },
  { key: 'Shadow', name: 'Shadow Temple' },
  { key: 'Spirit', name: 'Spirit Temple' },
  { key: 'WF', name: 'Woodfall Temple' },
  { key: 'SH', name: 'Snowhead Temple' },
  { key: 'GB', name: 'Great Bay Temple' },
  { key: 'IST', name: 'Inverted Stone Tower Temple' },
  { key: 'ST', name: 'Stone Tower Temple' },
  { key: 'SSH', name: 'Swamp Spider House' },
  { key: 'OSH', name: 'Ocean Spider House' },
  { key: 'BotW', name: 'Bottom of the Well' },
  { key: 'IC', name: 'Ice Cavern' },
  { key: 'GTG', name: 'Gerudo Training Grounds' },
  { key: 'BtW', name: 'Beneath the Well' },
  { key: 'ACoI', name: 'Ancient Castle of Ikana' },
  { key: 'SS', name: 'Secret Shrine' },
  { key: 'BtWE', name: 'Beneath the Well End' },
  { key: 'PF', name: 'Pirate Fortress' },
  { key: 'Ganon', name: 'Ganon Castle' },
  { key: 'Tower', name: 'Ganon Tower' },
  { key: 'Moon', name: 'Clock Tower Roof' },
] as const;

export const DUNGEONS: DungeonMetadata[] = DUNGEONS_RAW.map((dungeon, id) => ({ ...dungeon, id }));
export const DUNGEONS_BY_KEY = new Map(DUNGEONS.map(dungeon => [dungeon.key, dungeon]));
