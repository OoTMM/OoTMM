import { Confvar } from '../confvars';
import { Items, PlayerItems, itemByID, makePlayerItem } from '../items';
import { Monitor } from '../monitor';
import { Random } from '../random';
import { Settings } from '../settings';
import { countMapAdd, countMapRemove } from '../util';

/* This pass pre-computes things from the settings */
export class LogicPassConfig {
  private startingItems: PlayerItems;

  constructor(
    private readonly state: {
      monitor: Monitor,
      settings: Settings,
      random: Random,
    }
  ) {
    /* Starting items */
    this.startingItems = new Map;

    for (const [itemId, count] of Object.entries(state.settings.startingItems)) {
      const item = itemByID(itemId);
      for (let playerId = 0; playerId < this.state.settings.players; ++playerId) {
        const pi = makePlayerItem(item, playerId);
        countMapAdd(this.startingItems, pi, count);
      }
    }
  }

  private startingFairies() {
    const { settings } = this.state;

    for (let playerId = 0; playerId < this.state.settings.players; ++playerId) {
      const piWf = makePlayerItem(Items.MM_STRAY_FAIRY_WF, playerId);
      const piSh = makePlayerItem(Items.MM_STRAY_FAIRY_SH, playerId);
      const piGb = makePlayerItem(Items.MM_STRAY_FAIRY_GB, playerId);
      const piSt = makePlayerItem(Items.MM_STRAY_FAIRY_ST, playerId);

      if (settings.strayFairyChestShuffle === 'starting') {
        countMapAdd(this.startingItems, piWf, 3);
        countMapAdd(this.startingItems, piSh, 7);
        countMapAdd(this.startingItems, piGb, 6);
        countMapAdd(this.startingItems, piSt, 15);
      }

      if (['starting', 'removed'].includes(settings.strayFairyOtherShuffle)) {
        countMapAdd(this.startingItems, piWf, 12);
        countMapAdd(this.startingItems, piSh, 8);
        countMapAdd(this.startingItems, piGb, 9);
      }
    }
  }

  run() {
    this.state.monitor.log('Logic: Config');
    const config = new Set<Confvar>;

    const { settings } = this.state;

    /* Handle starting age */
    if (settings.startingAge === 'random') {
      if (this.state.random.next() & 1) {
        settings.startingAge = 'adult';
      } else {
        settings.startingAge = 'child';
      }
    }

    for (let playerId = 0; playerId < this.state.settings.players; ++playerId) {
      const piSword = makePlayerItem(Items.OOT_SWORD, playerId);
      const piSwordMaster = makePlayerItem(Items.OOT_SWORD_MASTER, playerId);

      if (settings.startingAge === 'adult') {
        if (settings.progressiveSwordsOot === 'progressive') {
          if ((this.startingItems.get(piSword) || 0) > 1) {
            countMapRemove(this.startingItems, piSword, 1);
          }
        } else {
          if (this.startingItems.has(piSwordMaster)) {
            countMapRemove(this.startingItems, piSwordMaster, 1);
          }
        }
      }
    }

    /* Handle fairies */
    this.startingFairies();

    return { startingItems: this.startingItems };
  }
}
