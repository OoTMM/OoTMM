import { ItemPlacement, ORDERED_DUNGEON_REWARDS } from './solve';

const DUNGEON_LOCATIONS_TO_REGIONS: {[k: string]: string} = {
  "OOT Temple of Time Medallion": "SACRED_REALM",
  "OOT Deku Tree Boss": "DEKU_TREE",
  "OOT Dodongo Cavern Boss": "DODONGO_CAVERN",
  "OOT Jabu-Jabu Boss": "JABU_JABU",
  "OOT Forest Temple Boss": "TEMPLE_FOREST",
  "OOT Fire Temple Boss": "TEMPLE_FIRE",
  "OOT Water Temple Boss": "TEMPLE_WATER",
  "OOT Spirit Temple Boss": "TEMPLE_SPIRIT",
  "OOT Shadow Temple Boss": "TEMPLE_SHADOW",
  "MM Woodfall Temple Boss": "TEMPLE_WOODFALL",
  "MM Snowhead Temple Boss": "TEMPLE_SNOWHEAD",
  "MM Great Bay Temple Boss": "TEMPLE_GREAT_BAY",
  "MM Stone Tower Boss": "TEMPLE_STONE_TOWER",
};

const findItem = (items: ItemPlacement, item: string) => {
  for (const loc in items) {
    if (items[loc] === item) {
      return loc;
    }
  }
  return null;
};

export const dungeonRewardRegions = (items: ItemPlacement) => ORDERED_DUNGEON_REWARDS.map(item => findItem(items, item)).map(loc => DUNGEON_LOCATIONS_TO_REGIONS[loc!]);
