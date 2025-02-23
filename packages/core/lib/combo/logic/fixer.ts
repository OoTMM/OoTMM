import { ItemHelpers, Items } from '../items';
import { Monitor } from '../monitor';
import { Settings } from '../settings';
import { mustStartWithMasterSword } from '../settings/util';
import { Location, MM_MERCHANTS, MM_SCRUBS, ONE_TIME_SHOP_CHECKS, OOT_FROGS, OOT_MERCHANTS, OOT_ONE_TIME_SCRUBS, isLocationChestFairy, isLocationOtherFairy, makeLocation } from './locations';
import { World } from './world';

export class LogicPassFixer {
  private fixedLocations = new Set<Location>();

  constructor(
    private readonly state: {
      worlds: World[],
      monitor: Monitor,
      settings: Settings,
    }
  ) {
  }

  private isLocationFixed(worldId: number, locId: string) {
    const world = this.state.worlds[worldId];
    const loc = makeLocation(locId, worldId);
    const check = world.checks[locId];
    const { type, item, game } = check;
    const { settings } = this.state;

    if (ItemHelpers.isTingleMap(item) && settings.tingleShuffle === 'vanilla') {
      return true;
    }

    if (ItemHelpers.isOwlStatue(item) && settings.owlShuffle === 'none') {
      return true;
    }

    if (!settings.shuffleMerchantsMm && MM_MERCHANTS.includes(locId)) {
      return true;
    }

    if (!settings.shuffleMerchantsOot && OOT_MERCHANTS.includes(locId)) {
      return true;
    }

    if(!settings.shuffleFrogsRupeesOot && OOT_FROGS.includes(locId)) {
      return true;
    }

    if (type === 'cow') {
      if (game === 'oot' && !this.state.settings.cowShuffleOot) {
        return true;
      }
      if (game === 'mm' && !this.state.settings.cowShuffleMm) {
        return true;
      }
    }

    if (type === 'shop') {
      if (game === 'oot' && this.state.settings.shopShuffleOot === 'none') {
        return true;
      }
      else if (game === 'mm' && this.state.settings.shopShuffleMm === 'none' && !ONE_TIME_SHOP_CHECKS.includes(locId)) {
        return true;
      }
    }

    if (type === 'scrub') {
      if (game === 'oot' && !this.state.settings.scrubShuffleOot && !OOT_ONE_TIME_SCRUBS.includes(locId)) {
        return true;
      }
    }

    if (MM_SCRUBS.includes(locId) && !this.state.settings.scrubShuffleMm) {
      return true;
    }

    if (type === 'sr' && settings.silverRupeeShuffle === 'vanilla') {
      return true;
    }

    if (ItemHelpers.isGerudoCard(item) && !this.state.settings.shuffleGerudoCard) {
      return true;
    }

    if (ItemHelpers.isMasterSword(item) && !this.state.settings.shuffleMasterSword && !mustStartWithMasterSword(this.state.settings)) {
      return true;
    }

    if (ItemHelpers.isGanonBossKey(item) && this.state.settings.ganonBossKey === 'vanilla') {
      return true;
    }

    if (ItemHelpers.isSmallKeyHideout(item) && this.state.settings.smallKeyShuffleHideout === 'vanilla') {
      return true;
    }

    if (check.scene === 'OOT_TREASURE_SHOP' && item !== Items.OOT_HEART_PIECE && this.state.settings.smallKeyShuffleChestGame === 'vanilla') {
      return true;
    }

    if (ItemHelpers.isTownStrayFairy(item) && this.state.settings.townFairyShuffle === 'vanilla') {
      return true;
    }

    if (isLocationChestFairy(world, loc) && this.state.settings.strayFairyChestShuffle === 'vanilla') {
      return true;
    }

    if (isLocationOtherFairy(world, loc) && this.state.settings.strayFairyOtherShuffle === 'vanilla') {
      return true;
    }

    if (item === Items.OOT_OCARINA && !this.state.settings.shuffleOcarinasOot) {
      return true;
    }

    if (ItemHelpers.isFishingPondFish(item) && !this.state.settings.pondFishShuffle) {
      return true;
    }

    if(ItemHelpers.isSmallKeyRegularOot(item) && this.state.settings.smallKeyShuffleOot === 'vanilla') {
      return true;
    }

    if(ItemHelpers.isSmallKeyRegularMm(item) && this.state.settings.smallKeyShuffleMm === 'vanilla') {
      return true;
    }

    if(ItemHelpers.isRegularBossKeyOot(item) && this.state.settings.bossKeyShuffleOot === 'vanilla') {
      return true;
    }

    if(ItemHelpers.isRegularBossKeyMm(item) && this.state.settings.bossKeyShuffleMm === 'vanilla') {
      return true;
    }

    return false;
  }

  private fixWorld(id: number) {
    const world = this.state.worlds[id];

    for (const locationId in world.checks) {
      if (this.isLocationFixed(id, locationId)) {
        this.fixedLocations.add(makeLocation(locationId, id));
      }
    }
  }

  run() {
    this.state.monitor.log("Logic: Fixing");

    for (let i = 0; i < this.state.worlds.length; ++i) {
      this.fixWorld(i);
    }

    return { fixedLocations: this.fixedLocations };
  }
};
