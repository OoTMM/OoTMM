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

    for (const loc in this.state.world.checks) {
      const check = this.state.world.checks[loc];
      let item = check.item;

      if (itemsToReplace.has(item)) {
        item = itemsToReplace.get(item)!;
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
      if (['OOT_SHIELD_DEKU', 'OOT_SHIELD_HYLIAN', 'OOT_SHIELD_MIRROR'].includes(item) && this.state.config.has('OOT_PROGRESSIVE_SHIELDS')) {
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
        }
      } else if (item === 'MM_SONG_GORON_HALF' && !this.state.config.has('MM_PROGRESSIVE_LULLABY')) {
        item = 'MM_SONG_GORON';
      }

      check.item = item;
    }

    return {};
  }
}
