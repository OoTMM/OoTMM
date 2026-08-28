import type { Game, Settings, ItemID } from '@ootmm/core';

import { ItemHelpers, Item, gameId } from '@ootmm/core';
import { SharedItemGroups } from '@ootmm/logic';
import { ITEMS_SUBSTITUTIONS, SHARED_ITEMS } from './checks';
import { GI } from '../data';

export function gi(settings: Settings, game: Game, item: ItemID, generic: boolean) {
  let itemName: string = Item.name(item);
  if (generic) {
    if (ItemHelpers.isSmallKeyHideout(item) && settings.smallKeyShuffleHideout !== 'anywhere') {
      itemName = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isKeyRingHideout(item) && settings.smallKeyShuffleHideout !== 'anywhere') {
      itemName = gameId(game, 'KEY_RING', '_');
    } else if (ItemHelpers.isSmallKeyTCG(item) && settings.smallKeyShuffleChestGame !== 'anywhere') {
      itemName = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isKeyRingTCG(item) && settings.smallKeyShuffleChestGame !== 'anywhere') {
      itemName = gameId(game, 'KEY_RING', '_');
    } else if (ItemHelpers.isSmallKeyRegularOot(item) && settings.smallKeyShuffleOot === 'ownDungeon' && settings.erBoss === 'none') {
      itemName = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isKeyRingRegularOot(item) && settings.smallKeyShuffleOot === 'ownDungeon' && settings.erBoss === 'none') {
      itemName = gameId(game, 'KEY_RING', '_');
    } else if (ItemHelpers.isSmallKeyRegularMm(item) && settings.smallKeyShuffleMm === 'ownDungeon' && settings.erBoss === 'none') {
      itemName = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isKeyRingRegularMm(item) && settings.smallKeyShuffleMm === 'ownDungeon' && settings.erBoss === 'none') {
      itemName = gameId(game, 'KEY_RING', '_');
    } else if (ItemHelpers.isGanonBossKey(item) && settings.ganonBossKey !== 'anywhere') {
      itemName = gameId(game, 'BOSS_KEY', '_');
    } else if (ItemHelpers.isRegularBossKeyOot(item) && settings.bossKeyShuffleOot === 'ownDungeon' && settings.erBoss === 'none') {
      itemName = gameId(game, 'BOSS_KEY', '_');
    } else if (ItemHelpers.isRegularBossKeyMm(item) && settings.bossKeyShuffleMm === 'ownDungeon' && settings.erBoss === 'none') {
      itemName = gameId(game, 'BOSS_KEY', '_');
    } else if (ItemHelpers.isTownStrayFairy(item) && settings.townFairyShuffle === 'vanilla') {
      itemName = gameId(game, 'STRAY_FAIRY', '_');
    } else if (ItemHelpers.isDungeonStrayFairy(item) && settings.strayFairyChestShuffle !== 'anywhere' && settings.strayFairyOtherShuffle !== 'anywhere' && settings.erBoss === 'none') {
      itemName = gameId(game, 'STRAY_FAIRY', '_');
    } else if (ItemHelpers.isMap(item) && settings.mapCompassShuffle === 'ownDungeon' && settings.erBoss === 'none') {
      itemName = gameId(game, 'MAP', '_');
    } else if (ItemHelpers.isCompass(item) && settings.mapCompassShuffle === 'ownDungeon' && settings.erBoss === 'none') {
      itemName = gameId(game, 'COMPASS', '_');
    }
  }

  /* Resolve shared item */
  if (itemName === 'SHARED_OCARINA' && settings.fairyOcarinaMm && game === 'mm') {
    itemName = 'MM_OCARINA';
  } else {
    const sharedItems = SHARED_ITEMS[game];
    const sharedItem = sharedItems.get(itemName);
    if (sharedItem) {
      itemName = sharedItem;
    }
  }

  /* Resolve shared items - new system */
  for (const group of Object.values(SharedItemGroups)) {
    for (const def of group) {
      if (Item.name(def.shared) === itemName) {
        itemName = Item.name(def[game]);
        break;
      }
    }
  }

  /* Resolve substitutions */
  if (itemName === 'MM_OCARINA' && settings.fairyOcarinaMm) {
    itemName = 'MM_OCARINA_FAIRY';
  } else if (itemName === 'MM_HOOKSHOT' && settings.shortHookshotMm) {
    itemName = 'MM_HOOKSHOT_SHORT';
  } else if (itemName === 'OOT_SCALE' && settings.bronzeScale) {
    itemName = 'OOT_SCALE_BRONZE';
  } else if (itemName === 'MM_SCALE' && settings.bronzeScale) {
    itemName = 'MM_SCALE_BRONZE';
  } else if (itemName === 'OOT_WALLET' && settings.childWallets) {
    itemName = 'OOT_WALLET';
  } else if (itemName === 'MM_WALLET' && settings.childWallets) {
    itemName = 'MM_WALLET';
  } else if (itemName === 'OOT_STICK_UPGRADE' && !settings.sticksNutsUpgradesInitial) {
    itemName = 'OOT_STICK_UPGRADE2';
  } else if (itemName === 'OOT_NUT_UPGRADE' && !settings.sticksNutsUpgradesInitial) {
    itemName = 'OOT_NUT_UPGRADE2';
  } else if (itemName === 'MM_STICK_UPGRADE' && !settings.sticksNutsUpgradesInitial) {
    itemName = 'MM_STICK_UPGRADE2';
  } else if (itemName === 'MM_NUT_UPGRADE' && !settings.sticksNutsUpgradesInitial) {
    itemName = 'MM_NUT_UPGRADE2';
  } else {
    const subst = ITEMS_SUBSTITUTIONS[itemName];
    if (subst) {
      itemName = subst;
    }
  }

  if (!GI.hasOwnProperty(itemName)) {
    throw new Error(`Unknown item ${itemName}`);
  }
  let value = GI[itemName].index;

  return value;
}

export function playerId(player: number | 'all'): number {
  if (player === 'all')
    return 0xff;
  return player + 1;
}
