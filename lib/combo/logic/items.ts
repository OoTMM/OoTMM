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

export const ITEMS_REQUIRED = new Set<string>([
  'OOT_GS_TOKEN',
  'OOT_ARROW_FIRE',
  'OOT_ARROW_LIGHT',
  'OOT_BOMB_BAG',
  'OOT_BOOMERANG',
  'OOT_BOOTS_HOVER',
  'OOT_BOOTS_IRON',
  'OOT_BOW',
  'OOT_CHICKEN',
  'OOT_EMPTY_BOTTLE',
  'OOT_GERUDO_CARD',
  'OOT_HAMMER',
  'OOT_HOOKSHOT',
  'OOT_LENS',
  'OOT_MAGIC_UPGRADE',
  'OOT_OCARINA',
  'OOT_RUTO_LETTER',
  'OOT_SCALE',
  'OOT_SHIELD',
  'OOT_SLINGSHOT',
  'OOT_SONG_EPONA',
  'OOT_SONG_SARIA',
  'OOT_SONG_STORMS',
  'OOT_SONG_SUN',
  'OOT_SONG_TIME',
  'OOT_SONG_ZELDA',
  'OOT_SONG_TP_FOREST',
  'OOT_SONG_TP_FIRE',
  'OOT_SONG_TP_SHADOW',
  'OOT_SONG_TP_SPIRIT',
  'OOT_SPELL_FIRE',
  'OOT_STRENGTH',
  'OOT_SWORD',
  'OOT_TUNIC_GORON',
  'OOT_TUNIC_ZORA',
  'OOT_ZELDA_LETTER',
  'OOT_MAGIC_BEAN',
  'OOT_MILK_BOTTLE',
  'OOT_STONE_OF_AGONY',
  'OOT_WALLET',
  'OOT_POCKET_CUCCO',
  'OOT_COJIRO',
  'OOT_ODD_MUSHROOM',
  'OOT_ODD_POTION',
  'OOT_POACHER_SAW',
  'OOT_BROKEN_GORON_SWORD',
  'OOT_PRESCRIPTION',
  'OOT_EYEBALL_FROG',
  'OOT_EYE_DROPS',
  'OOT_CLAIM_CHECK',
  'OOT_MASK_TRUTH',
  'OOT_MASK_SKULL',
  'MM_GS_TOKEN_SWAMP',
  'MM_GS_TOKEN_OCEAN',
  'MM_MASK_DEKU',
  'MM_MASK_GORON',
  'MM_MASK_ZORA',
  'MM_MASK_CAPTAIN',
  'MM_MASK_GIANT',
  'MM_MASK_ALL_NIGHT',
  'MM_MASK_BUNNY',
  'MM_MASK_KEATON',
  'MM_MASK_GARO',
  'MM_MASK_ROMANI',
  'MM_MASK_TROUPE_LEADER',
  'MM_MASK_POSTMAN',
  'MM_MASK_COUPLE',
  'MM_MASK_GREAT_FAIRY',
  'MM_MASK_GIBDO',
  'MM_MASK_DON_GERO',
  'MM_MASK_KAMARO',
  'MM_MASK_TRUTH',
  'MM_MASK_STONE',
  'MM_MASK_BREMEN',
  'MM_MASK_BLAST',
  'MM_MASK_SCENTS',
  'MM_MASK_KAFEI',
  'MM_MASK_FIERCE_DEITY',
  'MM_BOTTLED_POTION_RED',
  'MM_BOTTLED_GOLD_DUST',
  'MM_EMPTY_BOTTLE',
  'MM_BOTTLED_MILK',
  'MM_MAGIC_UPGRADE',
  'MM_SONG_TIME',
  'MM_SONG_AWAKENING',
  'MM_SONG_HEALING',
  'MM_SONG_EPONA',
  'MM_SONG_SOARING',
  'MM_SONG_GORON_HALF',
  'MM_SONG_ZORA',
  'MM_SONG_STORMS',
  'MM_SONG_EMPTINESS',
  'MM_SONG_ORDER',
  'MM_BOW',
  'MM_OCARINA',
  'MM_SWORD',
  'MM_SHIELD',
  'MM_BOMB_BAG',
  'MM_LENS',
  'MM_ARROW_FIRE',
  'MM_ARROW_ICE',
  'MM_ARROW_LIGHT',
  'MM_POWDER_KEG',
  'MM_HOOKSHOT',
  'MM_PICTOGRAPH_BOX',
  'MM_MAGIC_BEAN',
  'MM_MOON_TEAR',
  'MM_DEED_LAND',
  'MM_DEED_SWAMP',
  'MM_DEED_MOUNTAIN',
  'MM_DEED_OCEAN',
  'MM_ROOM_KEY',
  'MM_LETTER_TO_KAFEI',
  'MM_PENDANT_OF_MEMORIES',
  'MM_LETTER_TO_MAMA',
  'MM_WALLET',
  'MM_GREAT_FAIRY_SWORD',
  'MM_SPIN_UPGRADE',
]);

export const ITEMS_SOMETIMES_REQUIRED = new Set<string>([
  'OOT_GS_TOKEN',
  'OOT_CHICKEN',
  'OOT_SONG_SUN',
  'OOT_WALLET',
  'OOT_POCKET_CUCCO',
  'OOT_COJIRO',
  'OOT_ODD_MUSHROOM',
  'OOT_ODD_POTION',
  'OOT_POACHER_SAW',
  'OOT_BROKEN_GORON_SWORD',
  'OOT_PRESCRIPTION',
  'OOT_EYEBALL_FROG',
  'OOT_EYE_DROPS',
  'OOT_CLAIM_CHECK',
  'OOT_MASK_TRUTH',
  'OOT_MASK_SKULL',
  'MM_MASK_CAPTAIN',
  'MM_MASK_ALL_NIGHT',
  'MM_MASK_BUNNY',
  'MM_MASK_KEATON',
  'MM_MASK_ROMANI',
  'MM_MASK_TROUPE_LEADER',
  'MM_MASK_POSTMAN',
  'MM_MASK_COUPLE',
  'MM_MASK_GREAT_FAIRY',
  'MM_MASK_DON_GERO',
  'MM_MASK_KAMARO',
  'MM_MASK_TRUTH',
  'MM_MASK_STONE',
  'MM_MASK_BREMEN',
  'MM_MASK_KAFEI',
  'MM_SONG_HEALING',
  'MM_POWDER_KEG',
  'MM_DEED_SWAMP',
  'MM_DEED_MOUNTAIN',
  'MM_DEED_OCEAN',
  'MM_ROOM_KEY',
  'MM_LETTER_TO_KAFEI',
  'MM_PENDANT_OF_MEMORIES',
  'MM_LETTER_TO_MAMA',
]);

const ITEMS_JUNK = new Set<string>([
  'OOT_RUPEE_GREEN',
  'OOT_RUPEE_BLUE',
  'OOT_RUPEE_RED',
  'OOT_RECOVERY_HEART',
  'OOT_ARROWS_5',
  'OOT_ARROWS_10',
  'OOT_ARROWS_30',
  'MM_RUPEE_GREEN',
  'MM_RUPEE_BLUE',
  'MM_RUPEE_RED',
  'MM_RECOVERY_HEART',
  'MM_ARROWS_10',
  'MM_ARROWS_30',
  'MM_ARROWS_40',
  'MM_BOMB',
  'MM_BOMBS_5',
  'MM_BOMBS_10',
  'MM_BOMBS_20',
  'MM_BOMBS_30',
]);

export const isSong = (item: string) => !!item.match(/^(OOT|MM)_SONG_/);
export const isCompass = (item: string) => !!item.match(/^(OOT|MM)_COMPASS_/);
export const isMap = (item: string) => !!item.match(/^(OOT|MM)_MAP_/);
export const isSmallKey = (item: string) => !!item.match(/^(OOT|MM)_SMALL_KEY_/);
export const isGanonBossKey = (item: string) => item === 'OOT_BOSS_KEY_GANON';
export const isBossKey = (item: string) => !!item.match(/^(OOT|MM)_BOSS_KEY_/);
export const isRegularBossKey = (item: string) => isBossKey(item) && !isGanonBossKey(item);
export const isStrayFairy = (item: string) => !!item.match(/^(OOT|MM)_STRAY_FAIRY_/);
export const isTownStrayFairy = (item: string) => item === 'MM_STRAY_FAIRY_TOWN';
export const isDungeonStrayFairy = (item: string) => isStrayFairy(item) && !isTownStrayFairy(item);
export const isMapCompass = (item: string) => isMap(item) || isCompass(item);
export const isKey = (item: string) => isSmallKey(item) || isBossKey(item);
export const isDungeonItem = (item: string) => isMapCompass(item) || isKey(item) || isStrayFairy(item);
export const isDungeonReward = (item: string) => DUNGEON_REWARDS.has(item);
export const isItemMajor = (item: string) => ITEMS_REQUIRED.has(item);
export const isGoldToken = (item: string) => !!item.match(/^OOT_GS_TOKEN/);
export const isHouseToken = (item: string) => !!item.match(/^MM_GS_TOKEN/);
export const isToken = (item: string) => isGoldToken(item) || isHouseToken(item);
export const isJunk = (item: string) => ITEMS_JUNK.has(item);

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
