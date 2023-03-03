import { Monitor } from '../monitor';
import { Settings } from '../settings';
import { isGanonBossKey, isJunk, isMapCompass } from './items';
import { World } from './world';

export class LogicPassWorldTransform {
  private junkToggle = false;

  constructor(
    private readonly state: {
      monitor: Monitor;
      world: World;
      settings: Settings;
      config: Set<string>;
    }
  ){
  }

  run() {
    this.state.monitor.log('Logic: World Transform');

    const { config } = this.state;

    let fireTempleKeyRemoved = false;
    let mmExtraShield = false;
    let ootShields = 3;
    let sharedHc = 0;
    let sharedHp = 0;

    const itemsToReplace = new Map<string, string>();
    const itemsToJunk = new Set<string>();

    if (config.has('SHARED_BOWS')) {
      /* Bows and quivers */
      itemsToReplace.set('OOT_BOW', 'SHARED_BOW');
      itemsToJunk.add('MM_BOW');

      /* Arrows */
      itemsToReplace.set('OOT_ARROWS_5', 'SHARED_ARROWS_5');
      itemsToReplace.set('OOT_ARROWS_10', 'SHARED_ARROWS_10');
      itemsToReplace.set('OOT_ARROWS_30', 'SHARED_ARROWS_30');
      itemsToReplace.set('MM_ARROWS_10', 'SHARED_ARROWS_10');
      itemsToReplace.set('MM_ARROWS_30', 'SHARED_ARROWS_30');
      itemsToReplace.set('MM_ARROWS_40', 'SHARED_ARROWS_40');
    }

    if (config.has('SHARED_BOMB_BAGS')) {
      /* Bomb Bags */
      itemsToReplace.set('OOT_BOMB_BAG', 'SHARED_BOMB_BAG');
      itemsToJunk.add('MM_BOMB_BAG');

      /* Bombs */
      itemsToReplace.set('OOT_BOMB',      'SHARED_BOMB');
      itemsToReplace.set('OOT_BOMBS_5',   'SHARED_BOMBS_5');
      itemsToReplace.set('OOT_BOMBS_10',  'SHARED_BOMBS_10');
      itemsToReplace.set('OOT_BOMBS_20',  'SHARED_BOMBS_20');
      itemsToReplace.set('OOT_BOMBS_30',  'SHARED_BOMBS_30');
      itemsToReplace.set('MM_BOMB',       'SHARED_BOMB');
      itemsToReplace.set('MM_BOMBS_5',    'SHARED_BOMBS_5');
      itemsToReplace.set('MM_BOMBS_10',   'SHARED_BOMBS_10');
      itemsToReplace.set('MM_BOMBS_20',   'SHARED_BOMBS_20');
      itemsToReplace.set('MM_BOMBS_30',   'SHARED_BOMBS_30');
    }

    if (config.has('SHARED_MAGIC')) {
      itemsToReplace.set('OOT_MAGIC_UPGRADE', 'SHARED_MAGIC_UPGRADE');
      itemsToJunk.add('MM_MAGIC_UPGRADE');
    }

    if (config.has('SHARED_MAGIC_ARROWS')) {
      itemsToReplace.set('OOT_ARROW_FIRE',  'SHARED_ARROW_FIRE');
      itemsToReplace.set('OOT_ARROW_ICE',   'SHARED_ARROW_ICE');
      itemsToReplace.set('OOT_ARROW_LIGHT', 'SHARED_ARROW_LIGHT');
      itemsToJunk.add('MM_ARROW_FIRE');
      itemsToJunk.add('MM_ARROW_ICE');
      itemsToJunk.add('MM_ARROW_LIGHT');
    }

    if (config.has('SHARED_SONGS')) {
      itemsToReplace.set('OOT_SONG_TIME',   'SHARED_SONG_TIME');
      itemsToReplace.set('OOT_SONG_EPONA',  'SHARED_SONG_EPONA');
      itemsToReplace.set('OOT_SONG_STORMS', 'SHARED_SONG_STORMS');
      itemsToJunk.add('MM_SONG_TIME');
      itemsToJunk.add('MM_SONG_EPONA');
      itemsToJunk.add('MM_SONG_STORMS');
    }

    if (config.has('SHARED_NUTS_STICKS')) {
      /* Nuts */
      itemsToReplace.set('OOT_NUTS_5',      'SHARED_NUTS_5');
      itemsToReplace.set('OOT_NUTS_5_ALT',  'SHARED_NUTS_5');
      itemsToReplace.set('OOT_NUTS_10',     'SHARED_NUTS_10');
      itemsToReplace.set('MM_NUT',          'SHARED_NUT');
      itemsToReplace.set('MM_NUTS_5',       'SHARED_NUTS_5');
      itemsToReplace.set('MM_NUTS_10',      'SHARED_NUTS_10');

      /* Sticks */
      itemsToReplace.set('OOT_STICK',       'SHARED_STICK');
      itemsToReplace.set('OOT_STICKS_5',    'SHARED_STICKS_5');
      itemsToReplace.set('OOT_STICKS_10',   'SHARED_STICKS_10');
      itemsToReplace.set('MM_STICK',        'SHARED_STICK');
    }

    if (config.has('SHARED_HOOKSHOT')) {
      itemsToReplace.set('OOT_HOOKSHOT', 'SHARED_HOOKSHOT');
      itemsToJunk.add('MM_HOOKSHOT');
    }

    if (config.has('SHARED_LENS')) {
      itemsToReplace.set('OOT_LENS', 'SHARED_LENS');
      itemsToJunk.add('MM_LENS');
    }

    if (config.has('SHARED_OCARINA')) {
      itemsToReplace.set('OOT_OCARINA', 'SHARED_OCARINA');
      itemsToJunk.add('MM_OCARINA');
    }

    if (config.has('SHARED_MASKS')) {
      itemsToReplace.set('MM_MASK_ZORA', 'SHARED_MASK_ZORA');
      itemsToReplace.set('MM_MASK_GORON', 'SHARED_MASK_GORON');
      itemsToReplace.set('MM_MASK_TRUTH', 'SHARED_MASK_TRUTH');
      itemsToReplace.set('MM_MASK_BUNNY', 'SHARED_MASK_BUNNY');
      itemsToReplace.set('MM_MASK_KEATON', 'SHARED_MASK_KEATON');
      itemsToJunk.add('OOT_MASK_ZORA');
      itemsToJunk.add('OOT_MASK_GORON');
      itemsToJunk.add('OOT_MASK_TRUTH');
      itemsToJunk.add('OOT_MASK_BUNNY');
      itemsToJunk.add('OOT_MASK_KEATON');
    }

    if (config.has('SHARED_WALLETS')) {
      /* Wallets */
      itemsToReplace.set('OOT_WALLET', 'SHARED_WALLET');
      itemsToJunk.add('MM_WALLET');

      /* Rupees */
      itemsToReplace.set('OOT_RUPEE_GREEN',   'SHARED_RUPEE_GREEN');
      itemsToReplace.set('OOT_RUPEE_BLUE',    'SHARED_RUPEE_BLUE');
      itemsToReplace.set('OOT_RUPEE_RED',     'SHARED_RUPEE_RED');
      itemsToReplace.set('OOT_RUPEE_PURPLE',  'SHARED_RUPEE_PURPLE');
      itemsToReplace.set('OOT_RUPEE_HUGE',    'SHARED_RUPEE_GOLD');
      itemsToReplace.set('MM_RUPEE_GREEN',    'SHARED_RUPEE_GREEN');
      itemsToReplace.set('MM_RUPEE_BLUE',     'SHARED_RUPEE_BLUE');
      itemsToReplace.set('MM_RUPEE_RED',      'SHARED_RUPEE_RED');
      itemsToReplace.set('MM_RUPEE_PURPLE',   'SHARED_RUPEE_PURPLE');
      itemsToReplace.set('MM_RUPEE_SILVER',   'SHARED_RUPEE_SILVER');
      itemsToReplace.set('MM_RUPEE_GOLD',     'SHARED_RUPEE_GOLD');
    }

    if (config.has('SHARED_HEALTH')) {
      /* Pieces and containers */
      itemsToReplace.set('OOT_HEART_CONTAINER', 'SHARED_HEART_CONTAINER');
      itemsToReplace.set('MM_HEART_PIECE',      'SHARED_HEART_PIECE');
      itemsToJunk.add('MM_HEART_CONTAINER');
      itemsToJunk.add('OOT_HEART_PIECE');

      /* Defense */
      itemsToReplace.set('OOT_DEFENSE_UPGRADE', 'SHARED_DEFENSE_UPGRADE');
      itemsToJunk.add('MM_DEFENSE_UPGRADE');

      /* Recovery */
      itemsToReplace.set('OOT_RECOVERY_HEART', 'SHARED_RECOVERY_HEART');
      itemsToReplace.set('MM_RECOVERY_HEART',  'SHARED_RECOVERY_HEART');
    }

    for (const loc in this.state.world.checks) {
      const check = this.state.world.checks[loc];
      let item = check.item;

      if (itemsToReplace.has(item)) {
        item = itemsToReplace.get(item)!;
        if (item === 'SHARED_HEART_CONTAINER') {
          sharedHc++;
          if (sharedHc === 6) {
            itemsToReplace.delete('OOT_HEART_CONTAINER');
            itemsToJunk.add('OOT_HEART_CONTAINER');
          }
        }

        if (item === 'SHARED_HEART_PIECE') {
          sharedHp++;
          if (sharedHp === 44) {
            itemsToReplace.delete('MM_HEART_PIECE');
            itemsToJunk.add('MM_HEART_PIECE');
          }
        }
      }

      if (itemsToJunk.has(item)) {
        item = this.junkToggle ? 'MM_RUPEE_BLUE' : 'OOT_RUPEE_BLUE';
        this.junkToggle = !this.junkToggle;
      }

      /* Maps/Compass */
      if (isMapCompass(item) && ['starting', 'removed'].includes(this.state.settings.mapCompassShuffle)) {
        if (check.game === 'oot') {
          item = 'OOT_RUPEE_BLUE';
        } else {
          item = 'MM_RUPEE_BLUE';
        }
      }

      /* Ganon BK */
      if (isGanonBossKey(item) && this.state.config.has('GANON_NO_BOSS_KEY')) {
        item = 'OOT_RUPEE_BLUE';
      }

      /* Fire temple key */
      if (item === 'OOT_SMALL_KEY_FIRE' && !this.state.config.has('SMALL_KEY_SHUFFLE') && !fireTempleKeyRemoved) {
        fireTempleKeyRemoved = true;
        item = 'OOT_RUPEE_BLUE';
      }

      /* OoT shields */
      if (['OOT_SHIELD_DEKU', 'OOT_SHIELD_HYLIAN', 'OOT_SHIELD_MIRROR'].includes(item) && check.type !== 'shop' && this.state.config.has('OOT_PROGRESSIVE_SHIELDS')) {
        if (ootShields > 0) {
          ootShields -= 1;
          item = 'OOT_SHIELD';
        } else {
          item = 'OOT_RUPEE_BLUE';
        }
      }

      /* MM shields */
      if (item === 'MM_SHIELD_MIRROR' && this.state.config.has('MM_PROGRESSIVE_SHIELDS')) {
        item = 'MM_SHIELD';
      }

      if (isJunk(item) && this.state.config.has('MM_PROGRESSIVE_SHIELDS') && !mmExtraShield) {
        mmExtraShield = true;
        item = 'MM_SHIELD';
      }

      /* OoT swords */
      if (['OOT_SWORD_KOKIRI', 'OOT_SWORD_MASTER', 'OOT_SWORD_KNIFE', 'OOT_SWORD_BIGGORON'].includes(item) && this.state.config.has('OOT_PROGRESSIVE_SWORDS')) {
        item = 'OOT_SWORD';
      }

      /* OoT swords (Goron) */
      if (['OOT_SWORD_KNIFE', 'OOT_SWORD_BIGGORON'].includes(item) && this.state.config.has('OOT_PROGRESSIVE_SWORDS_GORON')) {
        item = 'OOT_SWORD_GORON';
      }

      /* MM lullaby */
      if (item === 'MM_SONG_GORON') {
        if (this.state.config.has('MM_PROGRESSIVE_LULLABY')) {
          item = 'MM_SONG_GORON_HALF';
        } else {
          item = 'MM_RUPEE_BLUE';
          this.state.world.songLocations.delete(loc);
        }
      } else if (item === 'MM_SONG_GORON_HALF' && !this.state.config.has('MM_PROGRESSIVE_LULLABY')) {
        item = 'MM_SONG_GORON';
      }

      check.item = item;
    }

    return {};
  }
}
