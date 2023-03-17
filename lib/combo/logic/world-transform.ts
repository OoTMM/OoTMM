import { Monitor } from '../monitor';
import { Random, sample, shuffle } from '../random';
import { Settings } from '../settings';
import { Items, addItem, isGanonBossKey, isJunk, isMapCompass, itemsArray, removeItem } from './items';
import { World } from './world';

const EXTRA_ITEMS = [
  'OOT_MASK_SKULL',
  'OOT_MASK_SPOOKY',
  'OOT_MASK_GERUDO',
  'MM_MASK_DEKU',
  'MM_SWORD',
];

const EXTRA_MASKS_OOT = [
  'OOT_MASK_KEATON',
  'OOT_MASK_TRUTH',
  'OOT_MASK_BUNNY',
  'OOT_MASK_GORON',
  'OOT_MASK_ZORA',
];

export class LogicPassWorldTransform {
  private junkToggle = false;
  private itemsToReplace = new Map<string, string>();
  private itemsToJunk = new Set<string>();
  private extraItems: Items = {};

  constructor(
    private readonly state: {
      monitor: Monitor;
      world: World;
      settings: Settings;
      config: Set<string>;
      mq: Set<string>;
      random: Random;
    }
  ) {
    for (const item of EXTRA_ITEMS) {
      addItem(this.extraItems, item);
    }
  }

  private takeExtraItem() {
    const items = itemsArray(this.extraItems);
    if (items.length === 0) {
      return null;
    }
    const item = sample(this.state.random, items);
    removeItem(this.extraItems, item);
    return item;
  }

  private removeLocations(locs: string[]) {
    const { world } = this.state;
    for (const loc of locs) {
      delete world.checks[loc];
      delete world.regions[loc];
    }
    for (const areaName in world.areas) {
      const area = world.areas[areaName];
      const locations = area.locations;
      for (const loc of locs) {
        delete locations[loc];
      }
    }
  }

  private filterExtraItems() {
    for (const item in this.state.settings.startingItems) {
      const count = this.state.settings.startingItems[item];
      if (count > 0) {
        for (let i = 0; i < count; ++i) {
          removeItem(this.extraItems, item);
        }
      }
    }
  }

  private setupSharedItems() {
    const { config } = this.state;
    if (config.has('SHARED_BOWS')) {
      /* Bows and quivers */
      this.itemsToReplace.set('OOT_BOW', 'SHARED_BOW');
      this.itemsToJunk.add('MM_BOW');

      /* Arrows */
      this.itemsToReplace.set('OOT_ARROWS_5', 'SHARED_ARROWS_5');
      this.itemsToReplace.set('OOT_ARROWS_10', 'SHARED_ARROWS_10');
      this.itemsToReplace.set('OOT_ARROWS_30', 'SHARED_ARROWS_30');
      this.itemsToReplace.set('MM_ARROWS_10', 'SHARED_ARROWS_10');
      this.itemsToReplace.set('MM_ARROWS_30', 'SHARED_ARROWS_30');
      this.itemsToReplace.set('MM_ARROWS_40', 'SHARED_ARROWS_40');
    }

    if (config.has('SHARED_BOMB_BAGS')) {
      /* Bomb Bags */
      this.itemsToReplace.set('OOT_BOMB_BAG', 'SHARED_BOMB_BAG');
      this.itemsToJunk.add('MM_BOMB_BAG');

      /* Bombs */
      this.itemsToReplace.set('OOT_BOMB',      'SHARED_BOMB');
      this.itemsToReplace.set('OOT_BOMBS_5',   'SHARED_BOMBS_5');
      this.itemsToReplace.set('OOT_BOMBS_10',  'SHARED_BOMBS_10');
      this.itemsToReplace.set('OOT_BOMBS_20',  'SHARED_BOMBS_20');
      this.itemsToReplace.set('OOT_BOMBS_30',  'SHARED_BOMBS_30');
      this.itemsToReplace.set('MM_BOMB',       'SHARED_BOMB');
      this.itemsToReplace.set('MM_BOMBS_5',    'SHARED_BOMBS_5');
      this.itemsToReplace.set('MM_BOMBS_10',   'SHARED_BOMBS_10');
      this.itemsToReplace.set('MM_BOMBS_20',   'SHARED_BOMBS_20');
      this.itemsToReplace.set('MM_BOMBS_30',   'SHARED_BOMBS_30');
    }

    if (config.has('SHARED_MAGIC')) {
      this.itemsToReplace.set('OOT_MAGIC_UPGRADE', 'SHARED_MAGIC_UPGRADE');
      this.itemsToJunk.add('MM_MAGIC_UPGRADE');
    }

    if (config.has('SHARED_MAGIC_ARROWS')) {
      this.itemsToReplace.set('OOT_ARROW_FIRE',  'SHARED_ARROW_FIRE');
      this.itemsToReplace.set('OOT_ARROW_ICE',   'SHARED_ARROW_ICE');
      this.itemsToReplace.set('OOT_ARROW_LIGHT', 'SHARED_ARROW_LIGHT');
      this.itemsToJunk.add('MM_ARROW_FIRE');
      this.itemsToJunk.add('MM_ARROW_ICE');
      this.itemsToJunk.add('MM_ARROW_LIGHT');
    }

    if (config.has('SHARED_SONGS')) {
      this.itemsToReplace.set('OOT_SONG_TIME',   'SHARED_SONG_TIME');
      this.itemsToReplace.set('OOT_SONG_EPONA',  'SHARED_SONG_EPONA');
      this.itemsToReplace.set('OOT_SONG_STORMS', 'SHARED_SONG_STORMS');
      this.itemsToJunk.add('MM_SONG_TIME');
      this.itemsToJunk.add('MM_SONG_EPONA');
      this.itemsToJunk.add('MM_SONG_STORMS');
    }

    if (config.has('SHARED_NUTS_STICKS')) {
      /* Nuts */
      this.itemsToReplace.set('OOT_NUTS_5',      'SHARED_NUTS_5');
      this.itemsToReplace.set('OOT_NUTS_5_ALT',  'SHARED_NUTS_5');
      this.itemsToReplace.set('OOT_NUTS_10',     'SHARED_NUTS_10');
      this.itemsToReplace.set('MM_NUT',          'SHARED_NUT');
      this.itemsToReplace.set('MM_NUTS_5',       'SHARED_NUTS_5');
      this.itemsToReplace.set('MM_NUTS_10',      'SHARED_NUTS_10');

      /* Sticks */
      this.itemsToReplace.set('OOT_STICK',       'SHARED_STICK');
      this.itemsToReplace.set('OOT_STICKS_5',    'SHARED_STICKS_5');
      this.itemsToReplace.set('OOT_STICKS_10',   'SHARED_STICKS_10');
      this.itemsToReplace.set('MM_STICK',        'SHARED_STICK');
    }

    if (config.has('SHARED_HOOKSHOT')) {
      this.itemsToReplace.set('OOT_HOOKSHOT', 'SHARED_HOOKSHOT');
      this.itemsToJunk.add('MM_HOOKSHOT');
    } else if (this.state.settings.shortHookshotMm) {
      addItem(this.extraItems, 'MM_HOOKSHOT');
    }

    if (config.has('SHARED_LENS')) {
      this.itemsToReplace.set('OOT_LENS', 'SHARED_LENS');
      this.itemsToJunk.add('MM_LENS');
    }

    if (config.has('SHARED_OCARINA')) {
      this.itemsToReplace.set('OOT_OCARINA', 'SHARED_OCARINA');
      this.itemsToJunk.add('MM_OCARINA');
    } else if (this.state.settings.fairyOcarinaMm) {
      addItem(this.extraItems, 'MM_OCARINA');
    }

    if (config.has('SHARED_MASKS')) {
      this.itemsToReplace.set('MM_MASK_ZORA', 'SHARED_MASK_ZORA');
      this.itemsToReplace.set('MM_MASK_GORON', 'SHARED_MASK_GORON');
      this.itemsToReplace.set('MM_MASK_TRUTH', 'SHARED_MASK_TRUTH');
      this.itemsToReplace.set('MM_MASK_BUNNY', 'SHARED_MASK_BUNNY');
      this.itemsToReplace.set('MM_MASK_KEATON', 'SHARED_MASK_KEATON');
      this.itemsToJunk.add('OOT_MASK_ZORA');
      this.itemsToJunk.add('OOT_MASK_GORON');
      this.itemsToJunk.add('OOT_MASK_TRUTH');
      this.itemsToJunk.add('OOT_MASK_BUNNY');
      this.itemsToJunk.add('OOT_MASK_KEATON');
    } else {
      for (const item of EXTRA_MASKS_OOT) {
        addItem(this.extraItems, item);
      }
    }

    if (config.has('SHARED_WALLETS')) {
      /* Wallets */
      this.itemsToReplace.set('OOT_WALLET', 'SHARED_WALLET');
      this.itemsToJunk.add('MM_WALLET');

      /* Rupees */
      this.itemsToReplace.set('OOT_RUPEE_GREEN',   'SHARED_RUPEE_GREEN');
      this.itemsToReplace.set('OOT_RUPEE_BLUE',    'SHARED_RUPEE_BLUE');
      this.itemsToReplace.set('OOT_RUPEE_RED',     'SHARED_RUPEE_RED');
      this.itemsToReplace.set('OOT_RUPEE_PURPLE',  'SHARED_RUPEE_PURPLE');
      this.itemsToReplace.set('OOT_RUPEE_HUGE',    'SHARED_RUPEE_GOLD');
      this.itemsToReplace.set('MM_RUPEE_GREEN',    'SHARED_RUPEE_GREEN');
      this.itemsToReplace.set('MM_RUPEE_BLUE',     'SHARED_RUPEE_BLUE');
      this.itemsToReplace.set('MM_RUPEE_RED',      'SHARED_RUPEE_RED');
      this.itemsToReplace.set('MM_RUPEE_PURPLE',   'SHARED_RUPEE_PURPLE');
      this.itemsToReplace.set('MM_RUPEE_SILVER',   'SHARED_RUPEE_SILVER');
      this.itemsToReplace.set('MM_RUPEE_GOLD',     'SHARED_RUPEE_GOLD');
    }

    if (config.has('SHARED_HEALTH')) {
      /* Pieces and containers */
      this.itemsToReplace.set('OOT_HEART_CONTAINER', 'SHARED_HEART_CONTAINER');
      this.itemsToReplace.set('MM_HEART_PIECE',      'SHARED_HEART_PIECE');
      this.itemsToJunk.add('MM_HEART_CONTAINER');
      this.itemsToJunk.add('OOT_HEART_PIECE');

      /* Defense */
      this.itemsToReplace.set('OOT_DEFENSE_UPGRADE', 'SHARED_DEFENSE_UPGRADE');
      this.itemsToJunk.add('MM_DEFENSE_UPGRADE');

      /* Recovery */
      this.itemsToReplace.set('OOT_RECOVERY_HEART', 'SHARED_RECOVERY_HEART');
      this.itemsToReplace.set('MM_RECOVERY_HEART',  'SHARED_RECOVERY_HEART');
    }
  }

  private placeExtraItems() {
    const { checks } = this.state.world;
    const junkLocs = shuffle(this.state.random, Object.keys(checks).filter(x => isJunk(checks[x].item)));

    for (;;) {
      const extraItem = this.takeExtraItem();
      if (!extraItem) {
        break;
      }
      const loc = junkLocs.pop()!;
      checks[loc].item = extraItem;
    }
  }

  run() {
    this.state.monitor.log('Logic: World Transform');

    let shouldRemoveKeyFire = false;
    let mmExtraShield = false;
    let sharedHc = 0;
    let sharedHp = 0;

    if (this.state.settings.progressiveShieldsOot) {
      this.extraItems['OOT_SHIELD'] = 2;
    }

    /* Remove one key from fire in non-MQ, non keysanity */
    if (!this.state.config.has('SMALL_KEY_SHUFFLE') && !this.state.mq.has('Fire')) {
      shouldRemoveKeyFire = true;
    }

    if (!this.state.settings.eggShuffle) {
      this.removeLocations(['OOT Hatch Chicken', 'OOT Hatch Pocket Cucco']);
    }

    this.setupSharedItems();
    this.filterExtraItems();

    for (const loc in this.state.world.checks) {
      const check = this.state.world.checks[loc];
      let item = check.item;

      if (this.itemsToReplace.has(item)) {
        item = this.itemsToReplace.get(item)!;
        if (item === 'SHARED_HEART_CONTAINER') {
          sharedHc++;
          if (sharedHc === 6) {
            this.itemsToReplace.delete('OOT_HEART_CONTAINER');
            this.itemsToJunk.add('OOT_HEART_CONTAINER');
          }
        }

        if (item === 'SHARED_HEART_PIECE') {
          sharedHp++;
          if (sharedHp === 44) {
            this.itemsToReplace.delete('MM_HEART_PIECE');
            this.itemsToJunk.add('MM_HEART_PIECE');
          }
        }
      }

      if (this.itemsToJunk.has(item)) {
        const extraItem = this.takeExtraItem();
        if (extraItem) {
          item = extraItem;
        } else {
          item = this.junkToggle ? 'MM_RUPEE_BLUE' : 'OOT_RUPEE_BLUE';
          this.junkToggle = !this.junkToggle;
        }
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
      if (item === 'OOT_SMALL_KEY_FIRE' && shouldRemoveKeyFire) {
        shouldRemoveKeyFire = false;
        item = 'OOT_RUPEE_BLUE';
      }

      /* OoT shields */
      if (item === 'OOT_SHIELD_MIRROR' && this.state.config.has('OOT_PROGRESSIVE_SHIELDS')) {
        item = 'OOT_SHIELD';
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

    this.placeExtraItems();

    return {};
  }
}
