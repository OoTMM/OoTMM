import { Confvar } from '../confvars';
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
    const config = new Set<Confvar>;
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

    const exprs: {[k in Confvar]: boolean} = {
      GANON_NO_BOSS_KEY: settings.ganonBossKey === 'removed',
      SMALL_KEY_SHUFFLE: settings.smallKeyShuffleOot === 'anywhere',
      CSMC: settings.csmc === 'always',
      CSMC_AGONY: settings.csmc === 'agony',
      OOT_PROGRESSIVE_SHIELDS: settings.progressiveShieldsOot === 'progressive',
      OOT_PROGRESSIVE_SWORDS: settings.progressiveSwordsOot === 'progressive',
      OOT_PROGRESSIVE_SWORDS_GORON: settings.progressiveSwordsOot === 'goron',
      MM_PROGRESSIVE_SHIELDS: settings.progressiveShieldsMm === 'progressive',
      MM_PROGRESSIVE_LULLABY: settings.progressiveGoronLullaby === 'progressive',
      DOOR_OF_TIME_OPEN: settings.doorOfTime === 'open',
      OOT_OPEN_DEKU: settings.dekuTree === 'open',
      ER_DUNGEONS: settings.erDungeons !== 'none',
      ER_BOSS: settings.erBoss !== 'none',
      ER_ANY: isEntranceShuffle(settings),
      SHARED_BOWS: settings.sharedBows,
      SHARED_BOMB_BAGS: settings.sharedBombBags,
      SHARED_MAGIC: settings.sharedMagic,
      SHARED_MAGIC_ARROW_FIRE: settings.sharedMagicArrowFire,
      SHARED_MAGIC_ARROW_ICE: settings.sharedMagicArrowIce,
      SHARED_MAGIC_ARROW_LIGHT: settings.sharedMagicArrowLight,
      SHARED_SONG_EPONA: settings.sharedSongEpona,
      SHARED_SONG_STORMS: settings.sharedSongStorms,
      SHARED_SONG_TIME: settings.sharedSongTime,
      SHARED_SONG_SUN: settings.sharedSongSun,
      SHARED_NUTS_STICKS: settings.sharedNutsSticks,
      SHARED_HOOKSHOT: settings.sharedHookshot,
      SHARED_LENS: settings.sharedLens,
      SHARED_OCARINA: settings.sharedOcarina,
      SHARED_MASK_GORON: settings.sharedMaskGoron,
      SHARED_MASK_ZORA: settings.sharedMaskZora,
      SHARED_MASK_BUNNY: settings.sharedMaskBunny,
      SHARED_MASK_TRUTH: settings.sharedMaskTruth,
      SHARED_MASK_KEATON: settings.sharedMaskKeaton,
      SHARED_WALLETS: settings.sharedWallets,
      SHARED_HEALTH: settings.sharedHealth,
      OOT_CROSS_WARP: settings.crossWarpOot,
      MM_CROSS_WARP: settings.crossWarpMm !== 'none',
      MM_CROSS_WARP_ADULT: settings.crossWarpMm === 'full',
      MM_OCARINA_FAIRY: settings.fairyOcarinaMm,
      MM_HOOKSHOT_SHORT: settings.shortHookshotMm,
      MM_SONG_SUN: settings.sunSongMm,
      OOT_SKIP_ZELDA: settings.skipZelda,
      OOT_OPEN_KAKARIKO_GATE: settings.kakarikoGate === 'open',
      OOT_LACS_CUSTOM: settings.lacs === 'custom',
      OOT_GANON_BK_CUSTOM: settings.ganonBossKey === 'custom',
      OOT_ZK_OPEN: settings.zoraKing === 'open',
      OOT_ZK_OPEN_ADULT: settings.zoraKing === 'adult',
      GOAL_GANON: settings.goal === 'ganon' || settings.goal === 'both',
      GOAL_MAJORA: settings.goal === 'majora' || settings.goal === 'both',
      GOAL_TRIFORCE: settings.goal === 'triforce',
      MM_MAJORA_CHILD_CUSTOM: settings.majoraChild === 'custom',
      FILL_WALLETS: settings.fillWallets,
      CHILD_WALLET: settings.childWallets,
      OOT_ADULT_WELL: settings.wellAdult,
      COLOSSAL_WALLET: settings.colossalWallets,
      BOTTOMLESS_WALLET: settings.bottomlessWallets,
      OOT_AGELESS_BOOTS: settings.agelessBoots,
      MM_OWL_SHUFFLE: settings.owlShuffle === 'anywhere',
      OOT_CARPENTERS_ONE: settings.gerudoFortress === 'single',
      OOT_CARPENTERS_NONE: settings.gerudoFortress === 'open',
      OOT_NO_BOSS_KEY: settings.bossKeyShuffleOot === 'removed',
      OOT_NO_SMALL_KEY: settings.smallKeyShuffleOot === 'removed',
      MM_NO_BOSS_KEY: settings.bossKeyShuffleMm === 'removed',
      MM_NO_SMALL_KEY: settings.smallKeyShuffleMm === 'removed',
      CSMC_HEARTS: settings.csmcHearts,
      OOT_BLUE_FIRE_ARROWS: settings.blueFireArrows,
    };

    for (const v in exprs) {
      if (exprs[v as Confvar]) {
        config.add(v as Confvar);
      }
    }

    return { mq, config };
  }
}
