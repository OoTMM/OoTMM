import { Entrance } from '@ootmm/data';

type BossMetadata = {
  readonly entrance: Entrance;
  readonly event: string;
  readonly dungeon: string;
  readonly eventClear?: string;
}

export const BOSS_METADATA: BossMetadata[] = [
  { entrance: 'OOT_BOSS_DEKU_TREE', event: 'OOT_BOSS_GOHMA', dungeon: 'DT' },
  { entrance: 'OOT_BOSS_DODONGO_CAVERN', event: 'OOT_BOSS_KING_DODONGO', dungeon: 'DC' },
  { entrance: 'OOT_BOSS_JABU_JABU', event: 'OOT_BOSS_BARINADE', dungeon: 'JJ' },
  { entrance: 'OOT_BOSS_TEMPLE_FOREST', event: 'OOT_BOSS_PHANTOM_GANON', dungeon: 'Forest' },
  { entrance: 'OOT_BOSS_TEMPLE_FIRE', event: 'OOT_BOSS_VOLVAGIA', dungeon: 'Fire' },
  { entrance: 'OOT_BOSS_TEMPLE_WATER', event: 'OOT_BOSS_MORPHA', dungeon: 'Water', eventClear: 'OOT_CLEAR_STATE_LAKE' },
  { entrance: 'OOT_BOSS_TEMPLE_SHADOW', event: 'OOT_BOSS_BONGO_BONGO', dungeon: 'Shadow' },
  { entrance: 'OOT_BOSS_TEMPLE_SPIRIT', event: 'OOT_BOSS_TWINROVA', dungeon: 'Spirit' },
  { entrance: 'MM_BOSS_TEMPLE_WOODFALL', event: 'MM_BOSS_ODOLWA', dungeon: 'WF', eventClear: 'MM_CLEAR_STATE_WOODFALL' },
  { entrance: 'MM_BOSS_TEMPLE_SNOWHEAD', event: 'MM_BOSS_GOHT', dungeon: 'SH', eventClear: 'MM_CLEAR_STATE_SNOWHEAD' },
  { entrance: 'MM_BOSS_TEMPLE_GREAT_BAY', event: 'MM_BOSS_GYORG', dungeon: 'GB', eventClear: 'MM_CLEAR_STATE_GREAT_BAY' },
  { entrance: 'MM_BOSS_TEMPLE_STONE_TOWER', event: 'MM_BOSS_TWINMOLD', dungeon: 'IST', eventClear: 'MM_CLEAR_STATE_IKANA' },
];

export const BOSS_METADATA_BY_ENTRANCE = new Map(BOSS_METADATA.map(boss => [boss.entrance, boss]));
export const BOSS_METADATA_BY_DUNGEON = new Map(BOSS_METADATA.map(boss => [boss.dungeon, boss]));
