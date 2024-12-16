import { Entrance } from '@ootmm/data';

type BossMetadata = {
  readonly name: string;
  readonly entrance: Entrance;
  readonly event: string;
  readonly dungeon: string;
  readonly eventClear?: string;
}

export const BOSS_METADATA = [
  { name: 'Gohma', entrance: 'OOT_BOSS_DEKU_TREE', event: 'OOT_BOSS_GOHMA', dungeon: 'DT' },
  { name: 'King Dodongo', entrance: 'OOT_BOSS_DODONGO_CAVERN', event: 'OOT_BOSS_KING_DODONGO', dungeon: 'DC' },
  { name: 'Barinade', entrance: 'OOT_BOSS_JABU_JABU', event: 'OOT_BOSS_BARINADE', dungeon: 'JJ' },
  { name: 'Phantom Ganon', entrance: 'OOT_BOSS_TEMPLE_FOREST', event: 'OOT_BOSS_PHANTOM_GANON', dungeon: 'Forest' },
  { name: 'Volvagia', entrance: 'OOT_BOSS_TEMPLE_FIRE', event: 'OOT_BOSS_VOLVAGIA', dungeon: 'Fire' },
  { name: 'Morpha', entrance: 'OOT_BOSS_TEMPLE_WATER', event: 'OOT_BOSS_MORPHA', dungeon: 'Water', eventClear: 'OOT_CLEAR_STATE_LAKE' },
  { name: 'Bongo-Bongo', entrance: 'OOT_BOSS_TEMPLE_SHADOW', event: 'OOT_BOSS_BONGO_BONGO', dungeon: 'Shadow' },
  { name: 'Twinrova', entrance: 'OOT_BOSS_TEMPLE_SPIRIT', event: 'OOT_BOSS_TWINROVA', dungeon: 'Spirit' },
  { name: 'Odolwa', entrance: 'MM_BOSS_TEMPLE_WOODFALL', event: 'MM_BOSS_ODOLWA', dungeon: 'WF', eventClear: 'MM_CLEAR_STATE_WOODFALL' },
  { name: 'Goht', entrance: 'MM_BOSS_TEMPLE_SNOWHEAD', event: 'MM_BOSS_GOHT', dungeon: 'SH', eventClear: 'MM_CLEAR_STATE_SNOWHEAD' },
  { name: 'Gyorg', entrance: 'MM_BOSS_TEMPLE_GREAT_BAY', event: 'MM_BOSS_GYORG', dungeon: 'GB', eventClear: 'MM_CLEAR_STATE_GREAT_BAY' },
  { name: 'Twinmold', entrance: 'MM_BOSS_TEMPLE_STONE_TOWER', event: 'MM_BOSS_TWINMOLD', dungeon: 'IST', eventClear: 'MM_CLEAR_STATE_IKANA' },
] as const satisfies BossMetadata[];

export const BOSS_DUNGEONS = new Set(BOSS_METADATA.map(boss => boss.dungeon));
export const BOSS_METADATA_BY_ENTRANCE = new Map<Entrance, BossMetadata>(BOSS_METADATA.map(boss => [boss.entrance, boss]));
export const BOSS_METADATA_BY_DUNGEON = new Map<string, BossMetadata>(BOSS_METADATA.map(boss => [boss.dungeon, boss]));
export type BossDungeon = typeof BOSS_DUNGEONS extends Set<infer T> ? T : never;

type EndBossMetadata = {
  readonly name: string;
  readonly event: string;
};

export const END_BOSS_METADATA = [
  { name: 'Ganon', event: 'OOT_GANON' },
  { name: 'Majora', event: 'MM_MAJORA' },
] as const satisfies EndBossMetadata[];

export const END_BOSS_METADATA_BY_NAME = new Map<string, EndBossMetadata>(END_BOSS_METADATA.map(boss => [boss.name, boss]));
