import { Game, Settings } from '@ootmm/core';
import { Item, ItemHelpers } from '../items';
import { gameId } from '../util';
import { ITEMS_SUBSTITUTIONS, SHARED_ITEMS } from './checks';
import { SharedItemGroups } from '../logic/shared';
import { GI } from '../data';

export function gi(settings: Settings, game: Game, item: Item, generic: boolean) {
  let itemId = item.id;
  if (generic) {
    if (ItemHelpers.isSmallKeyHideout(item) && settings.smallKeyShuffleHideout !== 'anywhere') {
      itemId = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isKeyRingHideout(item) && settings.smallKeyShuffleHideout !== 'anywhere') {
      itemId = gameId(game, 'KEY_RING', '_');
    } else if (ItemHelpers.isSmallKeyTCG(item) && settings.smallKeyShuffleChestGame !== 'anywhere') {
      itemId = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isKeyRingTCG(item) && settings.smallKeyShuffleChestGame !== 'anywhere') {
      itemId = gameId(game, 'KEY_RING', '_');
    } else if (ItemHelpers.isSmallKeyRegularOot(item) && settings.smallKeyShuffleOot === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isKeyRingRegularOot(item) && settings.smallKeyShuffleOot === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'KEY_RING', '_');
    } else if (ItemHelpers.isSmallKeyRegularMm(item) && settings.smallKeyShuffleMm === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isKeyRingRegularMm(item) && settings.smallKeyShuffleMm === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'KEY_RING', '_');
    } else if (ItemHelpers.isGanonBossKey(item) && settings.ganonBossKey !== 'anywhere') {
      itemId = gameId(game, 'BOSS_KEY', '_');
    } else if (ItemHelpers.isRegularBossKeyOot(item) && settings.bossKeyShuffleOot === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'BOSS_KEY', '_');
    } else if (ItemHelpers.isRegularBossKeyMm(item) && settings.bossKeyShuffleMm === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'BOSS_KEY', '_');
    } else if (ItemHelpers.isTownStrayFairy(item) && settings.townFairyShuffle === 'vanilla') {
      itemId = gameId(game, 'STRAY_FAIRY', '_');
    } else if (ItemHelpers.isDungeonStrayFairy(item) && settings.strayFairyChestShuffle !== 'anywhere' && settings.strayFairyOtherShuffle !== 'anywhere' && settings.erBoss === 'none') {
      itemId = gameId(game, 'STRAY_FAIRY', '_');
    } else if (ItemHelpers.isMap(item) && settings.mapCompassShuffle === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'MAP', '_');
    } else if (ItemHelpers.isCompass(item) && settings.mapCompassShuffle === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'COMPASS', '_');
    }
  }

  /* Resolve shared item */
  if (itemId === 'SHARED_OCARINA' && settings.fairyOcarinaMm && game === 'mm') {
    itemId = 'MM_OCARINA';
  } else {
    const sharedItems = SHARED_ITEMS[game];
    const sharedItem = sharedItems.get(itemId);
    if (sharedItem) {
      itemId = sharedItem;
    }
  }

  /* Resolve shared items - new system */
  for (const group of Object.values(SharedItemGroups)) {
    for (const def of group) {
      if (def.shared.id === itemId) {
        itemId = def[game].id;
        break;
      }
    }
  }

  /* Resolve substitutions */
  if (itemId === 'MM_OCARINA' && settings.fairyOcarinaMm) {
    itemId = 'MM_OCARINA_FAIRY';
  } else if (itemId === 'MM_HOOKSHOT' && settings.shortHookshotMm) {
    itemId = 'MM_HOOKSHOT_SHORT';
  } else {
    const subst = ITEMS_SUBSTITUTIONS[itemId];
    if (subst) {
      itemId = subst;
    }
  }

  if (!GI.hasOwnProperty(itemId)) {
    throw new Error(`Unknown item ${itemId}`);
  }
  let value = GI[itemId].index;

  return value;
}

export function playerId(player: number | 'all'): number {
  if (player === 'all')
    return 0xff;
  return player + 1;
}
