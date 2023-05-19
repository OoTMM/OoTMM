import { Confvar } from '../confvars';
import { Monitor } from '../monitor';
import { Settings } from '../settings';
import { exprTrue } from './expr';
import { ITEMS_BOSS_KEY_MM, ITEMS_BOSS_KEY_OOT, ITEMS_OWLS, ITEMS_SMALL_KEY_MM, ITEMS_SMALL_KEY_OOT } from './items';
import { Items, addItem, ITEMS_MAPS, ITEMS_COMPASSES, ITEMS_TINGLE_MAPS, ITEMS_SONGS, DUNGEON_REWARDS, isJunk, isItemConsumable } from './items';
import { LOCATIONS_ZELDA, isLocationRenewable } from './locations';
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

const ITEM_POOL_SCARCE = [
  'OOT_BOMB_BAG',
  'OOT_BOW',
  'OOT_MAGIC_UPGRADE',
  'OOT_OCARINA',
  'OOT_SLINGSHOT',
  'MM_MAGIC_UPGRADE',
  'MM_BOW',
  'MM_SWORD',
  'MM_BOMB_BAG',
  'MM_OCARINA',
  'SHARED_BOW',
  'SHARED_BOMB_BAG',
  'SHARED_MAGIC_UPGRADE',
];

const ITEM_POOL_SCARCE_NOLIMIT = [
  'OOT_STICK_UPGRADE',
  'OOT_NUT_UPGRADE',
  'OOT_SWORD',
  'OOT_SWORD_GORON',
];

const ITEM_POOL_PLENTIFUL = [
  'OOT_RUTO_LETTER',
  'OOT_WEIRD_EGG',
  'OOT_POCKET_EGG',
  'OOT_ARROW_FIRE',
  'OOT_ARROW_LIGHT',
  'OOT_ARROW_ICE',
  'OOT_BOMB_BAG',
  'OOT_BOOMERANG',
  'OOT_BOOTS_HOVER',
  'OOT_BOOTS_IRON',
  'OOT_BOW',
  'OOT_CHICKEN',
  'OOT_HAMMER',
  'OOT_HOOKSHOT',
  'OOT_LENS',
  'OOT_MAGIC_UPGRADE',
  'OOT_OCARINA',
  'OOT_SCALE',
  'OOT_SHIELD',
  'OOT_SHIELD_MIRROR',
  'OOT_SLINGSHOT',
  'OOT_SPELL_FIRE',
  'OOT_SPELL_WIND',
  'OOT_SPELL_LOVE',
  'OOT_STRENGTH',
  'OOT_SWORD',
  'OOT_SWORD_GORON',
  'OOT_SWORD_KOKIRI',
  'OOT_SWORD_KNIFE',
  'OOT_SWORD_BIGGORON',
  'OOT_TUNIC_GORON',
  'OOT_TUNIC_ZORA',
  'OOT_ZELDA_LETTER',
  'OOT_MAGIC_BEAN',
  'OOT_STONE_OF_AGONY',
  'OOT_WALLET',
  'OOT_POCKET_CUCCO',
  'OOT_COJIRO',
  'OOT_ODD_MUSHROOM',
  'OOT_ODD_POTION',
  'OOT_POACHER_SAW',
  'OOT_BROKEN_GORON_SWORD',
  'OOT_PRESCRIPTION',
  'OOT_EYEBALL_FROG',
  'OOT_EYE_DROPS',
  'OOT_CLAIM_CHECK',
  'OOT_MASK_TRUTH',
  'OOT_MASK_SKULL',
  'OOT_MASK_SPOOKY',
  'OOT_MASK_GERUDO',
  'OOT_MASK_ZORA',
  'OOT_MASK_GORON',
  'OOT_MASK_BUNNY',
  'OOT_MASK_KEATON',
  'MM_BOTTLED_GOLD_DUST',
  'MM_MASK_DEKU',
  'MM_MASK_GORON',
  'MM_MASK_ZORA',
  'MM_MASK_CAPTAIN',
  'MM_MASK_GIANT',
  'MM_MASK_ALL_NIGHT',
  'MM_MASK_BUNNY',
  'MM_MASK_KEATON',
  'MM_MASK_GARO',
  'MM_MASK_ROMANI',
  'MM_MASK_TROUPE_LEADER',
  'MM_MASK_POSTMAN',
  'MM_MASK_COUPLE',
  'MM_MASK_GREAT_FAIRY',
  'MM_MASK_GIBDO',
  'MM_MASK_DON_GERO',
  'MM_MASK_KAMARO',
  'MM_MASK_TRUTH',
  'MM_MASK_STONE',
  'MM_MASK_BREMEN',
  'MM_MASK_BLAST',
  'MM_MASK_SCENTS',
  'MM_MASK_KAFEI',
  'MM_MASK_FIERCE_DEITY',
  'MM_MAGIC_UPGRADE',
  'MM_BOMBER_NOTEBOOK',
  'MM_BOW',
  'MM_OCARINA',
  'MM_SWORD',
  'MM_SHIELD',
  'MM_SHIELD_MIRROR',
  'MM_BOMB_BAG',
  'MM_LENS',
  'MM_ARROW_FIRE',
  'MM_ARROW_ICE',
  'MM_ARROW_LIGHT',
  'MM_POWDER_KEG',
  'MM_HOOKSHOT',
  'MM_PICTOGRAPH_BOX',
  'MM_MAGIC_BEAN',
  'MM_MOON_TEAR',
  'MM_DEED_LAND',
  'MM_DEED_SWAMP',
  'MM_DEED_MOUNTAIN',
  'MM_DEED_OCEAN',
  'MM_ROOM_KEY',
  'MM_LETTER_TO_KAFEI',
  'MM_PENDANT_OF_MEMORIES',
  'MM_LETTER_TO_MAMA',
  'MM_WALLET',
  'MM_GREAT_FAIRY_SWORD',
  'MM_SPIN_UPGRADE',
  'SHARED_BOW',
  'SHARED_BOMB_BAG',
  'SHARED_MAGIC_UPGRADE',
  'SHARED_ARROW_FIRE',
  'SHARED_ARROW_ICE',
  'SHARED_ARROW_LIGHT',
  'SHARED_HOOKSHOT',
  'SHARED_LENS',
  'SHARED_OCARINA',
  'SHARED_MASK_GORON',
  'SHARED_MASK_ZORA',
  'SHARED_MASK_TRUTH',
  'SHARED_MASK_BUNNY',
  'SHARED_MASK_KEATON',
  'SHARED_WALLET',
];

export class LogicPassWorldTransform {
  private pool: Items = {};
  private locsByItem = new Map<string, Set<string>>();

  constructor(
    private readonly state: {
      monitor: Monitor;
      world: World;
      settings: Settings;
      config: Set<Confvar>;
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

  private removeItems(items: Set<string>, amount?: number) {
    for (const item of items) {
      this.removeItem(item, amount);
    }
  }

  private addItem(item: string, amount?: number) {
    const count = this.pool[item] || 0;
    if (amount === undefined) {
      amount = 1;
    }
    this.pool[item] = count + amount;
  }

  private scarcifyPool(delta: number) {
    const { settings } = this.state;
    const items = [...ITEM_POOL_SCARCE];
    const itemsNolimit = [...ITEM_POOL_SCARCE_NOLIMIT];

    /* Tunics - shopsanity */
    if (settings.shopShuffleOot === 'full') {
      items.push('OOT_TUNIC_GORON');
      items.push('OOT_TUNIC_ZORA');
    }

    /* MM shared + fairy oca */
    if (settings.sharedOcarina && settings.fairyOcarinaMm) {
      items.push('SHARED_OCARINA');
    }

    for (const item of items) {
      const amount = this.pool[item];
      if (amount) {
        let newAmount = amount - delta;
        if (newAmount < 1)
          newAmount = 1;
        this.pool[item] = newAmount;
      }
    }

    for (const item of itemsNolimit) {
      this.removeItem(item, delta);
    }

    /* Remove heart pieces */
    this.removeItem('OOT_HEART_PIECE');
    this.removeItem('MM_HEART_PIECE');
    this.removeItem('SHARED_HEART_PIECE');

    /* Minimal - remove heart containers */
    if (delta >= 2) {
      this.removeItem('OOT_HEART_CONTAINER');
      this.removeItem('MM_HEART_CONTAINER');
      this.removeItem('SHARED_HEART_CONTAINER');
    }
  }

  private mergeHearts(prefix: string) {
    const hp = prefix + '_HEART_PIECE';
    const hc = prefix + '_HEART_CONTAINER';

    const hpCount = this.pool[hp] || 0;
    if (hpCount) {
      this.removeItem(hp);
      this.addItem(hc, hpCount / 4);
    }
  }

  private plentifulPool() {
    const { settings } = this.state;
    let items = [...ITEM_POOL_PLENTIFUL];

    if (settings.songs === 'anywhere') {
      items = [...items, ...ITEMS_SONGS];
    }

    if (settings.dungeonRewardShuffle === 'anywhere' || settings.dungeonRewardShuffle === 'dungeonsLimited') {
      items = [...items, ...DUNGEON_REWARDS];
    }

    if (settings.shuffleGerudoCard) {
      items.push('OOT_GERUDO_CARD');
    }

    if (settings.shuffleMasterSword && settings.progressiveSwordsOot !== 'progressive') {
      items.push('OOT_SWORD_MASTER');
    }

    if (settings.zoraKing === 'open') {
      items.push('OOT_BOTTLE_EMPTY');
    }

    if (settings.smallKeyShuffleOot === 'anywhere') {
      items = [...items, ...ITEMS_SMALL_KEY_OOT];
    }

    if (settings.smallKeyShuffleMm === 'anywhere') {
      items = [...items, ...ITEMS_SMALL_KEY_MM];
    }

    if (settings.bossKeyShuffleOot === 'anywhere') {
      items = [...items, ...ITEMS_BOSS_KEY_OOT];
    }

    if (settings.bossKeyShuffleMm === 'anywhere') {
      items = [...items, ...ITEMS_BOSS_KEY_MM];
    }

    if (settings.ganonBossKey === 'anywhere') {
      items.push('OOT_BOSS_KEY_GANON');
    }

    if (settings.smallKeyShuffleHideout === 'anywhere') {
      items = [...items, 'OOT_SMALL_KEY_GF'];
    }

    if (settings.mapCompassShuffle === 'anywhere') {
      items = [...items, ...ITEMS_MAPS, ...ITEMS_COMPASSES];
    }

    if (settings.tingleShuffle === 'anywhere') {
      items = [...items, ...ITEMS_TINGLE_MAPS];
    }

    if (settings.owlShuffle === 'anywhere') {
      items = [...items, ...ITEMS_OWLS];
    }

    /* Add extra items */
    for (const item of items) {
      const amount = this.pool[item];
      if (amount) {
        this.addItem(item);
      }
    }

    /* Merge pieces of hearts */
    this.mergeHearts('OOT');
    this.mergeHearts('MM');
    this.mergeHearts('SHARED');
  }

  /**
   * Setup the shared items.
   */
  private setupSharedItems() {
    const { config, settings } = this.state;
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

    if (settings.sharedMagicArrowFire) {
      this.replaceItem('OOT_ARROW_FIRE',  'SHARED_ARROW_FIRE');
      this.replaceItem('MM_ARROW_FIRE',   'SHARED_ARROW_FIRE');
      this.removeItem('SHARED_ARROW_FIRE', 1);
    }

    if (settings.sharedMagicArrowIce) {
      this.replaceItem('OOT_ARROW_ICE',   'SHARED_ARROW_ICE');
      this.replaceItem('MM_ARROW_ICE',    'SHARED_ARROW_ICE');
      this.removeItem('SHARED_ARROW_ICE', 1);
    }

    if (settings.sharedMagicArrowLight) {
      this.replaceItem('OOT_ARROW_LIGHT', 'SHARED_ARROW_LIGHT');
      this.replaceItem('MM_ARROW_LIGHT',  'SHARED_ARROW_LIGHT');
      this.removeItem('SHARED_ARROW_LIGHT', 1);
    }

    if (settings.sharedSongEpona) {
      this.replaceItem('OOT_SONG_EPONA',   'SHARED_SONG_EPONA');
      this.replaceItem('MM_SONG_EPONA',    'SHARED_SONG_EPONA');
      this.removeItem('SHARED_SONG_EPONA', 1);
    }

    if (settings.sharedSongStorms) {
      this.replaceItem('OOT_SONG_STORMS',  'SHARED_SONG_STORMS');
      this.replaceItem('MM_SONG_STORMS',   'SHARED_SONG_STORMS');
      this.removeItem('SHARED_SONG_STORMS', 1);
    }

    if (settings.sharedSongTime) {
      this.replaceItem('OOT_SONG_TIME',    'SHARED_SONG_TIME');
      this.replaceItem('MM_SONG_TIME',     'SHARED_SONG_TIME');
      this.removeItem('SHARED_SONG_TIME', 1);
    }

    if (settings.sharedSongSun && settings.sunSongMm) {
      this.replaceItem('OOT_SONG_SUN', 'SHARED_SONG_SUN');
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

      this.pool['SHARED_WALLET'] = this.pool['SHARED_WALLET'] / 2;

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

    switch (settings.itemPool) {
    case 'scarce':
      this.scarcifyPool(1);
      break;
    case 'minimal':
      this.scarcifyPool(2);
      break;
    case 'plentiful':
      this.plentifulPool();
      break;
    }

    /* Triforce hunt */
    if (settings.goal === 'triforce') {
      this.pool['SHARED_TRIFORCE'] = 30;
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
    const { settings } = this.state;
    this.state.monitor.log('Logic: World Transform');

    /* Carpenters */
    if (['open', 'single'].includes(settings.gerudoFortress)) {
      this.removeLocations(['OOT Gerudo Fortress Jail 2', 'OOT Gerudo Fortress Jail 3', 'OOT Gerudo Fortress Jail 4']);

      if (settings.gerudoFortress === 'open') {
        this.removeLocations(['OOT Gerudo Fortress Jail 1']);
        const loc = 'OOT Gerudo Member Card';
        this.state.world.areas['OOT SPAWN'].locations[loc] = exprTrue();
        this.state.world.regions[loc] = 'NONE';
        this.state.world.dungeons['GF'].delete(loc);
      }
    }

    /* Make the basic item pool */
    this.makePool();

    /* Add extra items */
    for (const item of EXTRA_ITEMS) {
      this.addItem(item);
    }

    /* Handle extra wallets */
    if (this.state.settings.childWallets) {
      this.addItem('OOT_WALLET');
      this.addItem('MM_WALLET');
    }

    if (this.state.settings.colossalWallets) {
      this.addItem('OOT_WALLET');
      this.addItem('MM_WALLET');
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

    /* Handle tingle maps */
    if (['starting', 'removed'].includes(this.state.settings.tingleShuffle)) {
      for (const item of ITEMS_TINGLE_MAPS) {
        this.removeItem(item);
      }
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
    if (settings.ganonBossKey === 'removed' || settings.ganonBossKey === 'custom') {
      this.removeItem('OOT_BOSS_KEY_GANON');
    }

    /* Handle other boss keys */
    if (settings.bossKeyShuffleOot === 'removed') {
      this.removeItems(ITEMS_BOSS_KEY_OOT);
    }

    if (settings.bossKeyShuffleMm === 'removed') {
      this.removeItems(ITEMS_BOSS_KEY_MM);
    }

    /* Handle small keys */
    if (settings.smallKeyShuffleOot === 'removed') {
      this.removeItems(ITEMS_SMALL_KEY_OOT);
    }

    if (settings.smallKeyShuffleMm === 'removed') {
      this.removeItems(ITEMS_SMALL_KEY_MM);
    }

    if (settings.zoraKing === 'open') {
      this.removeItem('OOT_RUTO_LETTER');
      this.addItem('OOT_BOTTLE_EMPTY');
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
    if (settings.sunSongMm && !settings.sharedSongSun) {
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

    /* Handle required junks */
    const renewableJunks: Items = {};
    for (const item of Object.keys(this.pool)) {
      if (isJunk(item) && isItemConsumable(item)) {
        for (const loc of this.locsByItem.get(item) || []) {
          if (isLocationRenewable(this.state.world, loc) && !this.state.fixedLocations.has(loc)) {
            addItem(renewableJunks, item);
          }
        }
      }
    }

    return { pool: this.pool, renewableJunks };
  }
}
