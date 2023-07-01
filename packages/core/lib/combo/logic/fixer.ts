import { ItemHelpers } from '../items';
import { Monitor } from '../monitor';
import { Settings } from '../settings';
import { MM_MERCHANTS, MM_SCRUBS, ONE_TIME_SHOP_CHECKS, OOT_ONE_TIME_SCRUBS } from './locations';
import { World } from './world';

export class LogicPassFixer {
  private fixedLocations = new Set<string>();

  constructor(
    private readonly state: {
      world: World,
      monitor: Monitor,
      settings: Settings,
    }
  ) {
  }

  run() {
    this.state.monitor.log("Logic: Fixing");

    const { settings } = this.state;

    for (const loc in this.state.world.checks) {
      const check = this.state.world.checks[loc];
      const { type, item, game } = check;

      if (ItemHelpers.isTingleMap(item) && settings.tingleShuffle === 'vanilla') {
        this.fixedLocations.add(loc);
      }

      if (ItemHelpers.isOwlStatue(item) && settings.owlShuffle === 'none') {
        this.fixedLocations.add(loc);
      }

      if (!settings.shuffleMerchantsMm && MM_MERCHANTS.includes(loc)) {
        this.fixedLocations.add(loc);
      }

      if (type === 'cow') {
        if (game === 'oot' && !this.state.settings.cowShuffleOot) {
          this.fixedLocations.add(loc);
        }
        if (game === 'mm' && !this.state.settings.cowShuffleMm) {
          this.fixedLocations.add(loc);
        }
      }

      if (type === 'shop') {
        if (game === 'oot' && this.state.settings.shopShuffleOot === 'none') {
          this.fixedLocations.add(loc);
        }
        else if (game === 'mm' && this.state.settings.shopShuffleMm === 'none' && !ONE_TIME_SHOP_CHECKS.includes(loc)) {
          this.fixedLocations.add(loc);
        }
      }

      if (type === 'scrub') {
        if (game === 'oot' && !this.state.settings.scrubShuffleOot && !OOT_ONE_TIME_SCRUBS.includes(loc)) {
          this.fixedLocations.add(loc);
        }
      }

      if (MM_SCRUBS.includes(loc) && !this.state.settings.scrubShuffleMm) {
        this.fixedLocations.add(loc);
      }

      if (ItemHelpers.isGerudoCard(item) && !this.state.settings.shuffleGerudoCard) {
        this.fixedLocations.add(loc);
      }

      if (ItemHelpers.isMasterSword(item) && !this.state.settings.shuffleMasterSword) {
        this.fixedLocations.add(loc);
      }

      if (ItemHelpers.isGanonBossKey(item) && this.state.settings.ganonBossKey === 'vanilla') {
        this.fixedLocations.add(loc);
      }

      if (ItemHelpers.isSmallKeyHideout(item) && this.state.settings.smallKeyShuffleHideout === 'vanilla') {
        this.fixedLocations.add(loc);
      }
    }

    return { fixedLocations: this.fixedLocations };
  }
};
