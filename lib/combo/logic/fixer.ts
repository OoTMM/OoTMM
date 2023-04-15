import { Monitor } from '../monitor';
import { Settings } from '../settings';
import { ONE_TIME_SHOP_CHECKS, OOT_ONE_TIME_SCRUBS } from './locations';
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

    for (const loc in this.state.world.checks) {
      const check = this.state.world.checks[loc];
      const { type, item, game } = check;

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

      if (item === 'OOT_GERUDO_CARD' && !this.state.settings.shuffleGerudoCard) {
        this.fixedLocations.add(loc);
      }

      if (item === 'OOT_SWORD_MASTER' && !this.state.settings.shuffleMasterSword) {
        this.fixedLocations.add(loc);
      }

      if (item === 'OOT_BOSS_KEY_GANON' && this.state.settings.ganonBossKey === 'vanilla') {
        this.fixedLocations.add(loc);
      }
    }

    return { fixedLocations: this.fixedLocations };
  }
};
