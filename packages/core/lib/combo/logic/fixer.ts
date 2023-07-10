import { ItemHelpers } from '../items';
import { Monitor } from '../monitor';
import { Settings } from '../settings';
import { Location, MM_MERCHANTS, MM_SCRUBS, ONE_TIME_SHOP_CHECKS, OOT_ONE_TIME_SCRUBS, makeLocation } from './locations';
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

  private fixWorld(id: number) {
    const { settings } = this.state;
    const world = this.state.worlds[id];


    for (const locationId in world.checks) {
      const check = world.checks[locationId];
      const { type, item, game } = check;
      const location = makeLocation(locationId, id);

      if (ItemHelpers.isTingleMap(item) && settings.tingleShuffle === 'vanilla') {
        this.fixedLocations.add(location);
      }

      if (ItemHelpers.isOwlStatue(item) && settings.owlShuffle === 'none') {
        this.fixedLocations.add(location);
      }

      if (!settings.shuffleMerchantsMm && MM_MERCHANTS.includes(locationId)) {
        this.fixedLocations.add(location);
      }

      if (type === 'cow') {
        if (game === 'oot' && !this.state.settings.cowShuffleOot) {
          this.fixedLocations.add(location);
        }
        if (game === 'mm' && !this.state.settings.cowShuffleMm) {
          this.fixedLocations.add(location);
        }
      }

      if (type === 'shop') {
        if (game === 'oot' && this.state.settings.shopShuffleOot === 'none') {
          this.fixedLocations.add(location);
        }
        else if (game === 'mm' && this.state.settings.shopShuffleMm === 'none' && !ONE_TIME_SHOP_CHECKS.includes(locationId)) {
          this.fixedLocations.add(location);
        }
      }

      if (type === 'scrub') {
        if (game === 'oot' && !this.state.settings.scrubShuffleOot && !OOT_ONE_TIME_SCRUBS.includes(locationId)) {
          this.fixedLocations.add(location);
        }
      }

      if (MM_SCRUBS.includes(locationId) && !this.state.settings.scrubShuffleMm) {
        this.fixedLocations.add(location);
      }

      if (ItemHelpers.isGerudoCard(item) && !this.state.settings.shuffleGerudoCard) {
        this.fixedLocations.add(location);
      }

      if (ItemHelpers.isMasterSword(item) && !this.state.settings.shuffleMasterSword) {
        this.fixedLocations.add(location);
      }

      if (ItemHelpers.isGanonBossKey(item) && this.state.settings.ganonBossKey === 'vanilla') {
        this.fixedLocations.add(location);
      }

      if (ItemHelpers.isSmallKeyHideout(item) && this.state.settings.smallKeyShuffleHideout === 'vanilla') {
        this.fixedLocations.add(location);
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
