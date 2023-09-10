import * as ItemGroups from './groups';
import { Item, Items } from './defs';

export const isCompass = (item: Item) => ItemGroups.COMPASSES.has(item);
export const isMap = (item: Item) => ItemGroups.MAPS.has(item);
export const isMapCompass = (item: Item) => isMap(item) || isCompass(item);
export const isSong = (item: Item) => ItemGroups.SONGS.has(item);
export const isSmallKey = (item: Item) => isSmallKeyRegular(item) || isSmallKeyHideout(item);
export const isSmallKeyHideout = (item: Item) => item === Items.OOT_SMALL_KEY_GF;
export const isSmallKeyRegularOot = (item: Item) => ItemGroups.SMALL_KEYS_OOT.has(item);
export const isSmallKeyRegularMm = (item: Item) => ItemGroups.SMALL_KEYS_MM.has(item);
export const isSmallKeyRegular = (item: Item) => isSmallKeyRegularOot(item) || isSmallKeyRegularMm(item);
export const isKeyRing = (item: Item) => isKeyRingRegular(item) || isKeyRingHideout(item);
export const isKeyRingHideout = (item: Item) => item === Items.OOT_KEY_RING_GF;
export const isKeyRingRegularOot = (item: Item) => ItemGroups.KEY_RINGS_OOT.has(item);
export const isKeyRingRegularMm = (item: Item) => ItemGroups.KEY_RINGS_MM.has(item);
export const isKeyRingRegular = (item: Item) => isKeyRingRegularOot(item) || isKeyRingRegularMm(item);
export const isGanonBossKey = (item: Item) => item === Items.OOT_BOSS_KEY_GANON;
export const isBossKey = (item: Item) => isRegularBossKey(item) || isGanonBossKey(item);
export const isRegularBossKeyOot = (item: Item) => ItemGroups.BOSS_KEYS_OOT.has(item);
export const isRegularBossKeyMm = (item: Item) => ItemGroups.BOSS_KEYS_MM.has(item);
export const isRegularBossKey = (item: Item) => isRegularBossKeyOot(item) || isRegularBossKeyMm(item);
export const isStrayFairy = (item: Item) => isTownStrayFairy(item) || isDungeonStrayFairy(item);
export const isTownStrayFairy = (item: Item) => item === Items.MM_STRAY_FAIRY_TOWN;
export const isDungeonStrayFairy = (item: Item) => ItemGroups.STRAY_FAIRIES_DUNGEON.has(item);
export const isKey = (item: Item) => isSmallKey(item) || isBossKey(item) || isKeyRing(item);
export const isDungeonItem = (item: Item) => isMapCompass(item) || isKey(item) || isStrayFairy(item) || isSilverRupee(item);
export const isDungeonReward = (item: Item) => ItemGroups.DUNGEON_REWARDS.has(item);
export const isItemMajor = (item: Item) => ItemGroups.REQUIRED.has(item);
export const isGoldToken = (item: Item) => item === Items.OOT_GS_TOKEN;
export const isHouseToken = (item: Item) => item === Items.MM_GS_TOKEN_SWAMP || item === Items.MM_GS_TOKEN_OCEAN;
export const isToken = (item: Item) => isGoldToken(item) || isHouseToken(item);
export const isJunk = (item: Item) => ItemGroups.JUNK.has(item);
export const isMasterSword = (item: Item) => item === Items.OOT_SWORD_MASTER;
export const isGerudoCard = (item: Item) => item === Items.OOT_GERUDO_CARD;
export const isItemImportant = (item: Item) => isItemMajor(item) || isItemConsumable(item) || isItemLicense(item);
export const isItemConsumable = (item: Item) => ItemGroups.CONSUMABLES.has(item);
export const isRupees = (item: Item) => ItemGroups.RUPEES.has(item);
export const isItemUnlimitedStarting = (item: Item) => isRupees(item) || isItemConsumable(item);
export const isItemCriticalRenewable = (item: Item) => ItemGroups.CRITICAL_RENEWABLES.has(item);
export const isTingleMap = (item: Item) => ItemGroups.TINGLE_MAPS.has(item);
export const isOwlStatue = (item: Item) => ItemGroups.OWLS.has(item);
export const isItemLicense = (item: Item) => ItemGroups.LICENSES.has(item);
export const isTriforcePiece = (item: Item) => ItemGroups.TRIFORCE.has(item);
export const isSilverRupee = (item: Item) => ItemGroups.RUPEES_SILVER.has(item);
export const isNamedTriforce = (item: Item) => ItemGroups.NAMED_TRIFORCE.has(item);
