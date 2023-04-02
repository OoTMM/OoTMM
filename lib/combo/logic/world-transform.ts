import { Monitor } from '../monitor';
import { Settings } from '../settings';
import { exprTrue } from './expr';
import { Items, addItem, ITEMS_MAPS, ITEMS_COMPASSES } from './items';
import { LOCATIONS_ZELDA } from './locations';
import { World } from './world';

const EXTRA_ITEMS = [
  'OOT_MASK_SKULL',
  'OOT_MASK_SPOOKY',
  'OOT_MASK_GERUDO',
  'OOT_MASK_KEATON',
  'OOT_MASK_TRUTH',
  'OOT_MASK_BUNNY',
  'OOT_MASK_GORON',
  'OOT_MASK_ZORA',
  'MM_MASK_DEKU',
  'MM_SWORD',
];

export class LogicPassWorldTransform {
  private pool: Items = {};
  private locsByItem = new Map<string, Set<string>>();

  constructor(
    private readonly state: {
      monitor: Monitor;
      world: World;
      settings: Settings;
      config: Set<string>;
      mq: Set<string>;
      fixedLocations: Set<string>;
    }
  ) {
  }

  private makePool() {
    const { world } = this.state;
    for (const loc in world.checks) {
      const check = world.checks[loc];
      addItem(this.pool, check.item);
      const set = this.locsByItem.get(check.item) || new Set();
      set.add(loc);
      this.locsByItem.set(check.item, set);
    }
  }

  /**
   * Replace an item in the pool with another item.
   */
  private replaceItem(from: string, to: string) {
    const prevCount = this.pool[from] || 0;
    delete this.pool[from];
    const newCount = (this.pool[to] || 0) + prevCount;
    if (newCount > 0) {
      this.pool[to] = newCount;
    } else {
      delete this.pool[to];
    }
    const oldSet = this.locsByItem.get(from) || new Set();
    const newSet = this.locsByItem.get(to) || new Set();
    for (const loc of oldSet) {
      const check = this.state.world.checks[loc];
      check.item = to;
      newSet.add(loc);
    }
    this.locsByItem.set(to, newSet);
    this.locsByItem.delete(from);
  }

  /**
   * Remove an item from the pool.
   * Optionally, limit the number of items removed.
   */
  private removeItem(item: string, amount?: number) {
    const count = this.pool[item] || 0;
    if (amount === undefined || amount >= count) {
      delete this.pool[item];
    } else {
      this.pool[item] = count - amount;
    }
  }

  private addItem(item: string, amount?: number) {
    const count = this.pool[item] || 0;
    if (amount === undefined) {
      amount = 1;
    }
    this.pool[item] = count + amount;
  }

  /**
   * Setup the shared items.
   */
  private setupSharedItems() {
    const { config } = this.state;
    if (config.has('SHARED_BOWS')) {
      /* Bows and quivers */
      this.replaceItem('OOT_BOW', 'SHARED_BOW');
      this.replaceItem('MM_BOW', 'SHARED_BOW');
      this.removeItem('SHARED_BOW', 3);

      /* Arrows */
      this.replaceItem('OOT_ARROWS_5', 'SHARED_ARROWS_5');
      this.replaceItem('OOT_ARROWS_10', 'SHARED_ARROWS_10');
      this.replaceItem('OOT_ARROWS_30', 'SHARED_ARROWS_30');
      this.replaceItem('MM_ARROWS_10', 'SHARED_ARROWS_10');
      this.replaceItem('MM_ARROWS_30', 'SHARED_ARROWS_30');
      this.replaceItem('MM_ARROWS_40', 'SHARED_ARROWS_40');
    }

    if (config.has('SHARED_BOMB_BAGS')) {
      /* Bomb Bags */
      this.replaceItem('OOT_BOMB_BAG', 'SHARED_BOMB_BAG');
      this.replaceItem('MM_BOMB_BAG', 'SHARED_BOMB_BAG');
      this.removeItem('SHARED_BOMB_BAG', 3);

      /* Bombs */
      this.replaceItem('OOT_BOMB',      'SHARED_BOMB');
      this.replaceItem('OOT_BOMBS_5',   'SHARED_BOMBS_5');
      this.replaceItem('OOT_BOMBS_10',  'SHARED_BOMBS_10');
      this.replaceItem('OOT_BOMBS_20',  'SHARED_BOMBS_20');
      this.replaceItem('OOT_BOMBS_30',  'SHARED_BOMBS_30');
      this.replaceItem('MM_BOMB',       'SHARED_BOMB');
      this.replaceItem('MM_BOMBS_5',    'SHARED_BOMBS_5');
      this.replaceItem('MM_BOMBS_10',   'SHARED_BOMBS_10');
      this.replaceItem('MM_BOMBS_20',   'SHARED_BOMBS_20');
      this.replaceItem('MM_BOMBS_30',   'SHARED_BOMBS_30');
    }

    if (config.has('SHARED_MAGIC')) {
      this.replaceItem('OOT_MAGIC_UPGRADE', 'SHARED_MAGIC_UPGRADE');
      this.replaceItem('MM_MAGIC_UPGRADE',  'SHARED_MAGIC_UPGRADE');
      this.removeItem('SHARED_MAGIC_UPGRADE', 2);
    }

    if (config.has('SHARED_MAGIC_ARROWS')) {
      this.replaceItem('OOT_ARROW_FIRE',  'SHARED_ARROW_FIRE');
      this.replaceItem('OOT_ARROW_ICE',   'SHARED_ARROW_ICE');
      this.replaceItem('OOT_ARROW_LIGHT', 'SHARED_ARROW_LIGHT');
      this.replaceItem('MM_ARROW_FIRE',   'SHARED_ARROW_FIRE');
      this.replaceItem('MM_ARROW_ICE',    'SHARED_ARROW_ICE');
      this.replaceItem('MM_ARROW_LIGHT',  'SHARED_ARROW_LIGHT');

      this.removeItem('SHARED_ARROW_FIRE', 1);
      this.removeItem('SHARED_ARROW_ICE', 1);
      this.removeItem('SHARED_ARROW_LIGHT', 1);
    }

    if (config.has('SHARED_SONGS')) {
      this.replaceItem('OOT_SONG_TIME',    'SHARED_SONG_TIME');
      this.replaceItem('OOT_SONG_EPONA',   'SHARED_SONG_EPONA');
      this.replaceItem('OOT_SONG_STORMS',  'SHARED_SONG_STORMS');
      this.replaceItem('MM_SONG_TIME',     'SHARED_SONG_TIME');
      this.replaceItem('MM_SONG_EPONA',    'SHARED_SONG_EPONA');
      this.replaceItem('MM_SONG_STORMS',   'SHARED_SONG_STORMS');

      if (config.has('MM_SONG_SUN')) {
        this.replaceItem('OOT_SONG_SUN', 'SHARED_SONG_SUN');
      }

      this.removeItem('SHARED_SONG_TIME', 1);
      this.removeItem('SHARED_SONG_EPONA', 1);
      this.removeItem('SHARED_SONG_STORMS', 1);
    }

    if (config.has('SHARED_NUTS_STICKS')) {
      /* Nuts */
      this.replaceItem('OOT_NUTS_5',      'SHARED_NUTS_5');
      this.replaceItem('OOT_NUTS_5_ALT',  'SHARED_NUTS_5');
      this.replaceItem('OOT_NUTS_10',     'SHARED_NUTS_10');
      this.replaceItem('MM_NUT',          'SHARED_NUT');
      this.replaceItem('MM_NUTS_5',       'SHARED_NUTS_5');
      this.replaceItem('MM_NUTS_10',      'SHARED_NUTS_10');

      /* Sticks */
      this.replaceItem('OOT_STICK',       'SHARED_STICK');
      this.replaceItem('OOT_STICKS_5',    'SHARED_STICKS_5');
      this.replaceItem('OOT_STICKS_10',   'SHARED_STICKS_10');
      this.replaceItem('MM_STICK',        'SHARED_STICK');
    }

    if (config.has('SHARED_HOOKSHOT')) {
      this.replaceItem('OOT_HOOKSHOT', 'SHARED_HOOKSHOT');
      this.replaceItem('MM_HOOKSHOT',  'SHARED_HOOKSHOT');
      this.removeItem('SHARED_HOOKSHOT', 1);
    } else if (this.state.settings.shortHookshotMm) {
      this.addItem('MM_HOOKSHOT');
    }

    if (config.has('SHARED_LENS')) {
      this.replaceItem('OOT_LENS', 'SHARED_LENS');
      this.replaceItem('MM_LENS',  'SHARED_LENS');
      this.removeItem('SHARED_LENS', 1);
    }

    if (config.has('SHARED_OCARINA')) {
      this.replaceItem('OOT_OCARINA', 'SHARED_OCARINA');
      this.replaceItem('MM_OCARINA',  'SHARED_OCARINA');
      this.removeItem('SHARED_OCARINA', 1);
    } else if (this.state.settings.fairyOcarinaMm) {
      this.addItem('MM_OCARINA');
    }

    if (config.has('SHARED_MASKS')) {
      this.replaceItem('OOT_MASK_ZORA',   'SHARED_MASK_ZORA');
      this.replaceItem('OOT_MASK_GORON',  'SHARED_MASK_GORON');
      this.replaceItem('OOT_MASK_TRUTH',  'SHARED_MASK_TRUTH');
      this.replaceItem('OOT_MASK_BUNNY',  'SHARED_MASK_BUNNY');
      this.replaceItem('OOT_MASK_KEATON', 'SHARED_MASK_KEATON');
      this.replaceItem('MM_MASK_ZORA',    'SHARED_MASK_ZORA');
      this.replaceItem('MM_MASK_GORON',   'SHARED_MASK_GORON');
      this.replaceItem('MM_MASK_TRUTH',   'SHARED_MASK_TRUTH');
      this.replaceItem('MM_MASK_BUNNY',   'SHARED_MASK_BUNNY');
      this.replaceItem('MM_MASK_KEATON',  'SHARED_MASK_KEATON');

      this.removeItem('SHARED_MASK_ZORA', 1);
      this.removeItem('SHARED_MASK_GORON', 1);
      this.removeItem('SHARED_MASK_TRUTH', 1);
      this.removeItem('SHARED_MASK_BUNNY', 1);
      this.removeItem('SHARED_MASK_KEATON', 1);
    }

    if (config.has('SHARED_WALLETS')) {
      /* Wallets */
      this.replaceItem('OOT_WALLET',  'SHARED_WALLET');
      this.replaceItem('MM_WALLET',   'SHARED_WALLET');
      this.removeItem('SHARED_WALLET', 2);

      /* Rupees */
      this.replaceItem('OOT_RUPEE_GREEN',   'SHARED_RUPEE_GREEN');
      this.replaceItem('OOT_RUPEE_BLUE',    'SHARED_RUPEE_BLUE');
      this.replaceItem('OOT_RUPEE_RED',     'SHARED_RUPEE_RED');
      this.replaceItem('OOT_RUPEE_PURPLE',  'SHARED_RUPEE_PURPLE');
      this.replaceItem('OOT_RUPEE_HUGE',    'SHARED_RUPEE_GOLD');
      this.replaceItem('MM_RUPEE_GREEN',    'SHARED_RUPEE_GREEN');
      this.replaceItem('MM_RUPEE_BLUE',     'SHARED_RUPEE_BLUE');
      this.replaceItem('MM_RUPEE_RED',      'SHARED_RUPEE_RED');
      this.replaceItem('MM_RUPEE_PURPLE',   'SHARED_RUPEE_PURPLE');
      this.replaceItem('MM_RUPEE_SILVER',   'SHARED_RUPEE_SILVER');
      this.replaceItem('MM_RUPEE_GOLD',     'SHARED_RUPEE_GOLD');
    }

    if (config.has('SHARED_HEALTH')) {
      /* Pieces and containers */
      this.replaceItem('OOT_HEART_CONTAINER', 'SHARED_HEART_CONTAINER');
      this.replaceItem('MM_HEART_CONTAINER',  'SHARED_HEART_CONTAINER');
      this.replaceItem('MM_HEART_PIECE',      'SHARED_HEART_PIECE');
      this.replaceItem('OOT_HEART_PIECE',     'SHARED_HEART_PIECE');

      this.pool['SHARED_HEART_CONTAINER'] = 6;
      this.pool['SHARED_HEART_PIECE'] = 44;

      /* Defense */
      this.replaceItem('OOT_DEFENSE_UPGRADE', 'SHARED_DEFENSE_UPGRADE');
      this.replaceItem('MM_DEFENSE_UPGRADE',  'SHARED_DEFENSE_UPGRADE');
      this.removeItem('SHARED_DEFENSE_UPGRADE', 1);

      /* Recovery */
      this.replaceItem('OOT_RECOVERY_HEART', 'SHARED_RECOVERY_HEART');
      this.replaceItem('MM_RECOVERY_HEART',  'SHARED_RECOVERY_HEART');
    }
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

  run() {
    this.state.monitor.log('Logic: World Transform');

    /* Make the basic item pool */
    this.makePool();

    /* Add extra items */
    for (const item of EXTRA_ITEMS) {
      this.addItem(item);
    }

    /* Setup shared items */
    this.setupSharedItems();

    /* Handle progressive shields */
    if (this.state.settings.progressiveShieldsOot === 'progressive') {
      this.replaceItem('OOT_SHIELD_MIRROR', 'OOT_SHIELD');
      this.addItem('OOT_SHIELD', 2);
    }

    if (this.state.config.has('MM_PROGRESSIVE_SHIELDS')) {
      this.replaceItem('MM_SHIELD_MIRROR', 'MM_SHIELD');
      this.addItem('MM_SHIELD');
    }

    /* Handle non-MQ Fire */
    if (!this.state.config.has('SMALL_KEY_SHUFFLE') && !this.state.mq.has('Fire')) {
      this.removeItem('OOT_SMALL_KEY_FIRE', 1);
    }

    /* Handle maps/compasses */
    if (['starting', 'removed'].includes(this.state.settings.mapCompassShuffle)) {
      for (const item of [...ITEMS_MAPS, ...ITEMS_COMPASSES]) {
        this.removeItem(item);
      }
    }

    /* Handle eggs */
    if (!this.state.settings.eggShuffle) {
      this.removeItem('OOT_WEIRD_EGG');
      this.removeItem('OOT_POCKET_EGG');
      this.removeLocations(['OOT Hatch Chicken', 'OOT Hatch Pocket Cucco']);
    }

    /* Handle Ganon BK */
    if (this.state.config.has('GANON_NO_BOSS_KEY')) {
      this.removeItem('OOT_BOSS_KEY_GANON');
    }

    /* Handle OoT swords */
    if (this.state.config.has('OOT_PROGRESSIVE_SWORDS')) {
      this.replaceItem('OOT_SWORD_KOKIRI',    'OOT_SWORD');
      this.replaceItem('OOT_SWORD_MASTER',    'OOT_SWORD');
      this.replaceItem('OOT_SWORD_KNIFE',     'OOT_SWORD');
      this.replaceItem('OOT_SWORD_BIGGORON',  'OOT_SWORD');
    } else if (this.state.config.has('OOT_PROGRESSIVE_SWORDS_GORON')) {
      this.replaceItem('OOT_SWORD_KNIFE',     'OOT_SWORD_GORON');
      this.replaceItem('OOT_SWORD_BIGGORON',  'OOT_SWORD_GORON');
    }

    /* Handle MM Lullaby */
    if (this.state.config.has('MM_PROGRESSIVE_LULLABY')) {
      this.replaceItem('MM_SONG_GORON', 'MM_SONG_GORON_HALF');
    } else {
      this.removeItem('MM_SONG_GORON_HALF');
      this.state.world.songLocations.delete('MM Goron Baby');
    }

    /* Handle MM sun song */
    if (this.state.config.has('MM_SONG_SUN') && !this.state.config.has('SHARED_SONGS')) {
      this.addItem('MM_SONG_SUN');
    }

    /* Handle Skip Zelda */
    if (this.state.settings.skipZelda) {
      this.removeItem('OOT_CHICKEN');

      for (const loc of LOCATIONS_ZELDA) {
        this.state.world.areas['OOT SPAWN'].locations[loc] = exprTrue();
        this.state.world.regions[loc] = 'NONE';
      }
    }

    /* Handle open gate */
    if (this.state.settings.kakarikoGate === 'open') {
      this.removeItem('OOT_ZELDA_LETTER');
    }

    /* Handle fixed locations */
    for (const loc of this.state.fixedLocations) {
      const check = this.state.world.checks[loc];
      const { item } = check;
      this.removeItem(item, 1);
    }

    return { pool: this.pool };
  }
}
