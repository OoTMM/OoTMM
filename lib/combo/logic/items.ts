import { Items } from './state';

export const DUNGEON_REWARDS_ORDERED = [
  'OOT_STONE_EMERALD',
  'OOT_STONE_RUBY',
  'OOT_STONE_SAPPHIRE',
  'OOT_MEDALLION_LIGHT',
  'OOT_MEDALLION_FOREST',
  'OOT_MEDALLION_FIRE',
  'OOT_MEDALLION_WATER',
  'OOT_MEDALLION_SPIRIT',
  'OOT_MEDALLION_SHADOW',
  'MM_REMAINS_ODOLWA',
  'MM_REMAINS_GOHT',
  'MM_REMAINS_GYORG',
  'MM_REMAINS_TWINMOLD',
];
export const DUNGEON_REWARDS = new Set(DUNGEON_REWARDS_ORDERED);

export const isSong = (item: string) => !!item.match(/^(OOT|MM)_SONG_/);
export const isCompass = (item: string) => !!item.match(/^(OOT|MM)_COMPASS_/);
export const isMap = (item: string) => !!item.match(/^(OOT|MM)_MAP_/);
export const isSmallKey = (item: string) => !!item.match(/^(OOT|MM)_SMALL_KEY_/);
export const isBossKey = (item: string) => !!item.match(/^(OOT|MM)_BOSS_KEY_/);
export const isStrayFairy = (item: string) => !!item.match(/^(OOT|MM)_STRAY_FAIRY_/);
export const isMapCompass = (item: string) => isMap(item) || isCompass(item);
export const isKey = (item: string) => isSmallKey(item) || isBossKey(item);
export const isDungeonItem = (item: string) => isMapCompass(item) || isKey(item) || isStrayFairy(item);
export const isDungeonReward = (item: string) => DUNGEON_REWARDS.has(item);

export const itemsArray = (items: Items) => {
  const arr: string[] = [];
  for (const item in items) {
    for (let i = 0; i < items[item]; i++) {
      arr.push(item);
    }
  }
  return arr;
};

export const addItem = (items: Items, item: string) => {
  items[item] = (items[item] || 0) + 1;
};

export const removeItem = (items: Items, item: string) => {
  items[item] -= 1;
  if (items[item] === 0) {
    delete items[item];
  }
};

export const combinedItems = (items: Items, other: Items) => {
  const combined: Items = {};
  for (const item in items) {
    combined[item] = items[item];
  }
  for (const item in other) {
    combined[item] = (combined[item] || 0) + other[item];
  }
  return combined;
};
