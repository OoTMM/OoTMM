import { Monitor } from '../monitor';
import { Random } from '../random';
import { DUNGEONS, Settings } from '../settings';
import { isEntranceShuffle } from './helpers';

/* This pass pre-computes things from the settings */
export class LogicPassConfig {
  constructor(
    private readonly state: {
      monitor: Monitor,
      settings: Settings,
      random: Random,
    }
  ) {
  }

  run() {
    this.state.monitor.log('Logic: Config');
    const config = new Set<string>;
    const mq = new Set<string>;

    const { settings } = this.state;

    /* MQ dungeons */
    let d: keyof typeof DUNGEONS;
    for (d in DUNGEONS) {
      if (this.state.settings.dungeon[d] === 'mq') {
        mq.add(d);
      } else if (this.state.settings.dungeon[d] === 'random') {
        if (this.state.random.next() & 0x10000) {
          mq.add(d);
        }
      }
    }

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

    if (this.state.settings.dekuTree === 'open') {
      config.add('OOT_OPEN_DEKU');
    }

    if (this.state.settings.erDungeons !== 'none') {
      config.add('ER_DUNGEONS');
    }

    if (isEntranceShuffle(this.state.settings)) {
      config.add('ER_ANY');
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

    if (this.state.settings.sharedMagicArrowFire) {
      config.add('SHARED_MAGIC_ARROW_FIRE');
    }

    if (this.state.settings.sharedMagicArrowIce) {
      config.add('SHARED_MAGIC_ARROW_ICE');
    }

    if (this.state.settings.sharedMagicArrowLight) {
      config.add('SHARED_MAGIC_ARROW_LIGHT');
    }

    if (this.state.settings.sharedSongEpona) {
      config.add('SHARED_SONG_EPONA');
    }

    if (this.state.settings.sharedSongStorms) {
      config.add('SHARED_SONG_STORMS');
    }

    if (this.state.settings.sharedSongTime) {
      config.add('SHARED_SONG_TIME');
    }

    if (this.state.settings.sharedSongSun) {
      config.add('SHARED_SONG_SUN');
    }

    if (this.state.settings.sharedNutsSticks) {
      config.add('SHARED_NUTS_STICKS');
    }

    if (this.state.settings.sharedHookshot) {
      config.add('SHARED_HOOKSHOT');
    }

    if (this.state.settings.sharedLens) {
      config.add('SHARED_LENS');
    }

    if (this.state.settings.sharedOcarina) {
      config.add('SHARED_OCARINA');
    }

    if (this.state.settings.sharedMasks) {
      config.add('SHARED_MASKS');
    }

    if (this.state.settings.sharedWallets) {
      config.add('SHARED_WALLETS');
    }

    if (this.state.settings.sharedHealth) {
      config.add('SHARED_HEALTH');
    }

    if (this.state.settings.crossWarpOot) {
      config.add('OOT_CROSS_WARP');
    }

    if (this.state.settings.crossWarpMm !== 'none') {
      config.add('MM_CROSS_WARP');
      if (this.state.settings.crossWarpMm === 'full') {
        config.add('MM_CROSS_WARP_ADULT');
      }
    }

    if (this.state.settings.fairyOcarinaMm) {
      config.add('MM_OCARINA_FAIRY');
    }

    if (this.state.settings.shortHookshotMm) {
      config.add('MM_HOOKSHOT_SHORT');
    }

    if (this.state.settings.sunSongMm) {
      config.add('MM_SONG_SUN');
    }

    if (this.state.settings.skipZelda) {
      config.add('OOT_SKIP_ZELDA');
    }

    if (this.state.settings.kakarikoGate === 'open') {
      config.add('OOT_OPEN_KAKARIKO_GATE');
    }

    if (this.state.settings.lacs === 'custom') {
      config.add('OOT_LACS_CUSTOM');
    }

    if (this.state.settings.ganonBossKey === 'custom') {
      config.add('OOT_GANON_BK_CUSTOM');
    }

    if (this.state.settings.zoraKing === 'open') {
      config.add('OOT_ZK_OPEN');
    } else if (this.state.settings.zoraKing === 'adult') {
      config.add('OOT_ZK_OPEN_ADULT');
    }

    if (settings.goal === 'both' || settings.goal === 'ganon') {
      config.add('GOAL_GANON');
    }

    if (settings.goal === 'both' || settings.goal === 'majora') {
      config.add('GOAL_MAJORA');
    }

    if (settings.majoraChild === 'custom') {
      config.add('MM_MAJORA_CHILD_CUSTOM');
    }

    if (settings.fillWallets) {
      config.add('FILL_WALLETS');
    }

    if (settings.childWallets) {
      config.add('CHILD_WALLET');
    }

    if (settings.wellAdult) {
      config.add('OOT_ADULT_WELL');
    }

    if (settings.colossalWallets) {
      config.add('COLOSSAL_WALLET');
    }
    if (settings.agelessBoots) {
      config.add('OOT_AGELESS_BOOTS')
    }

    if (settings.owlShuffle === 'anywhere') {
      config.add('MM_OWL_SHUFFLE');
    }

    return { mq, config };
  }
}
