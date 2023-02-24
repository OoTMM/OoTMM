import { Monitor } from '../monitor';
import { Settings } from '../settings';

/* This pass pre-computes things from the settings */
export class LogicPassConfig {
  constructor(
    private readonly state: {
      monitor: Monitor,
      settings: Settings,
    }
  ) {
  }

  run() {
    this.state.monitor.log('Logic: Config');
    const config = new Set<string>;

    if (this.state.settings.ganonBossKey === 'removed') {
      config.add('GANON_NO_BOSS_KEY');
    }

    if (this.state.settings.smallKeyShuffle === 'anywhere') {
      config.add('SMALL_KEY_SHUFFLE');
    }

    if (this.state.settings.csmc) {
      config.add('CSMC');
    }

    if (this.state.settings.progressiveShieldsOot === 'progressive') {
      config.add('OOT_PROGRESSIVE_SHIELDS');
    }

    if (this.state.settings.progressiveSwordsOot === 'progressive') {
      config.add('OOT_PROGRESSIVE_SWORDS');
    }

    if (this.state.settings.progressiveSwordsOot === 'goron') {
      config.add('OOT_PROGRESSIVE_SWORDS_GORON');
    }

    if (this.state.settings.progressiveShieldsMm === 'progressive') {
      config.add('MM_PROGRESSIVE_SHIELDS');
    }

    if (this.state.settings.progressiveGoronLullaby === 'progressive') {
      config.add('MM_PROGRESSIVE_LULLABY');
    }

    if (this.state.settings.doorOfTime === 'open') {
      config.add('DOOR_OF_TIME_OPEN');
    }

    if (this.state.settings.erDungeons !== 'none') {
      config.add('ER_DUNGEONS');
    }

    if (this.state.settings.sharedBows) {
      config.add('SHARED_BOWS');
    }

    if (this.state.settings.sharedBombBags) {
      config.add('SHARED_BOMB_BAGS');
    }

    if (this.state.settings.sharedMagic) {
      config.add('SHARED_MAGIC');
    }

    if (this.state.settings.sharedMagicArrows) {
      config.add('SHARED_MAGIC_ARROWS');
    }

    return { config };
  }
}
