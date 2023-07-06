import { Confvar } from '../confvars';
import { Item, ItemGroups, ItemHelpers, Items, ItemsCount } from '../items';
import { Monitor } from '../monitor';
import { Settings } from '../settings';
import { countMapAdd } from '../util';
import { exprTrue } from './expr';
import { LOCATIONS_ZELDA, isLocationRenewable, makeLocation } from './locations';
import { World } from './world';

const EXTRA_ITEMS = new Set([
  Items.OOT_MASK_SKULL,
  Items.OOT_MASK_SPOOKY,
  Items.OOT_MASK_GERUDO,
  Items.OOT_MASK_KEATON,
  Items.OOT_MASK_TRUTH,
  Items.OOT_MASK_BUNNY,
  Items.OOT_MASK_GORON,
  Items.OOT_MASK_ZORA,
  Items.MM_MASK_DEKU,
  Items.MM_SWORD,
]);

const ITEM_POOL_SCARCE = new Set([
  Items.OOT_BOMB_BAG,
  Items.OOT_BOW,
  Items.OOT_MAGIC_UPGRADE,
  Items.OOT_OCARINA,
  Items.OOT_SLINGSHOT,
  Items.MM_MAGIC_UPGRADE,
  Items.MM_BOW,
  Items.MM_SWORD,
  Items.MM_BOMB_BAG,
  Items.MM_OCARINA,
  Items.SHARED_BOW,
  Items.SHARED_BOMB_BAG,
  Items.SHARED_MAGIC_UPGRADE,
]);

const ITEM_POOL_SCARCE_NOLIMIT = new Set([
  Items.OOT_STICK_UPGRADE,
  Items.OOT_NUT_UPGRADE,
  Items.OOT_SWORD,
  Items.OOT_SWORD_GORON,
]);

const ITEM_POOL_PLENTIFUL = new Set([
  Items.OOT_RUTO_LETTER,
  Items.OOT_WEIRD_EGG,
  Items.OOT_POCKET_EGG,
  Items.OOT_ARROW_FIRE,
  Items.OOT_ARROW_LIGHT,
  Items.OOT_ARROW_ICE,
  Items.OOT_BOMB_BAG,
  Items.OOT_BOOMERANG,
  Items.OOT_BOOTS_HOVER,
  Items.OOT_BOOTS_IRON,
  Items.OOT_BOW,
  Items.OOT_CHICKEN,
  Items.OOT_HAMMER,
  Items.OOT_HOOKSHOT,
  Items.OOT_LENS,
  Items.OOT_MAGIC_UPGRADE,
  Items.OOT_OCARINA,
  Items.OOT_SCALE,
  Items.OOT_SHIELD,
  Items.OOT_SHIELD_MIRROR,
  Items.OOT_SLINGSHOT,
  Items.OOT_SPELL_FIRE,
  Items.OOT_SPELL_WIND,
  Items.OOT_SPELL_LOVE,
  Items.OOT_STRENGTH,
  Items.OOT_SWORD,
  Items.OOT_SWORD_GORON,
  Items.OOT_SWORD_KOKIRI,
  Items.OOT_SWORD_KNIFE,
  Items.OOT_SWORD_BIGGORON,
  Items.OOT_TUNIC_GORON,
  Items.OOT_TUNIC_ZORA,
  Items.OOT_ZELDA_LETTER,
  Items.OOT_MAGIC_BEAN,
  Items.OOT_STONE_OF_AGONY,
  Items.OOT_WALLET,
  Items.OOT_POCKET_CUCCO,
  Items.OOT_COJIRO,
  Items.OOT_ODD_MUSHROOM,
  Items.OOT_ODD_POTION,
  Items.OOT_POACHER_SAW,
  Items.OOT_BROKEN_GORON_SWORD,
  Items.OOT_PRESCRIPTION,
  Items.OOT_EYEBALL_FROG,
  Items.OOT_EYE_DROPS,
  Items.OOT_CLAIM_CHECK,
  Items.OOT_MASK_TRUTH,
  Items.OOT_MASK_SKULL,
  Items.OOT_MASK_SPOOKY,
  Items.OOT_MASK_GERUDO,
  Items.OOT_MASK_ZORA,
  Items.OOT_MASK_GORON,
  Items.OOT_MASK_BUNNY,
  Items.OOT_MASK_KEATON,
  Items.MM_BOTTLED_GOLD_DUST,
  Items.MM_MASK_DEKU,
  Items.MM_MASK_GORON,
  Items.MM_MASK_ZORA,
  Items.MM_MASK_CAPTAIN,
  Items.MM_MASK_GIANT,
  Items.MM_MASK_ALL_NIGHT,
  Items.MM_MASK_BUNNY,
  Items.MM_MASK_KEATON,
  Items.MM_MASK_GARO,
  Items.MM_MASK_ROMANI,
  Items.MM_MASK_TROUPE_LEADER,
  Items.MM_MASK_POSTMAN,
  Items.MM_MASK_COUPLE,
  Items.MM_MASK_GREAT_FAIRY,
  Items.MM_MASK_GIBDO,
  Items.MM_MASK_DON_GERO,
  Items.MM_MASK_KAMARO,
  Items.MM_MASK_TRUTH,
  Items.MM_MASK_STONE,
  Items.MM_MASK_BREMEN,
  Items.MM_MASK_BLAST,
  Items.MM_MASK_SCENTS,
  Items.MM_MASK_KAFEI,
  Items.MM_MASK_FIERCE_DEITY,
  Items.MM_MAGIC_UPGRADE,
  Items.MM_BOMBER_NOTEBOOK,
  Items.MM_BOW,
  Items.MM_OCARINA,
  Items.MM_SWORD,
  Items.MM_SHIELD,
  Items.MM_SHIELD_MIRROR,
  Items.MM_BOMB_BAG,
  Items.MM_LENS,
  Items.MM_ARROW_FIRE,
  Items.MM_ARROW_ICE,
  Items.MM_ARROW_LIGHT,
  Items.MM_POWDER_KEG,
  Items.MM_HOOKSHOT,
  Items.MM_PICTOGRAPH_BOX,
  Items.MM_MAGIC_BEAN,
  Items.MM_MOON_TEAR,
  Items.MM_DEED_LAND,
  Items.MM_DEED_SWAMP,
  Items.MM_DEED_MOUNTAIN,
  Items.MM_DEED_OCEAN,
  Items.MM_ROOM_KEY,
  Items.MM_LETTER_TO_KAFEI,
  Items.MM_PENDANT_OF_MEMORIES,
  Items.MM_LETTER_TO_MAMA,
  Items.MM_WALLET,
  Items.MM_GREAT_FAIRY_SWORD,
  Items.MM_SPIN_UPGRADE,
  Items.SHARED_BOW,
  Items.SHARED_BOMB_BAG,
  Items.SHARED_MAGIC_UPGRADE,
  Items.SHARED_ARROW_FIRE,
  Items.SHARED_ARROW_ICE,
  Items.SHARED_ARROW_LIGHT,
  Items.SHARED_HOOKSHOT,
  Items.SHARED_LENS,
  Items.SHARED_OCARINA,
  Items.SHARED_MASK_GORON,
  Items.SHARED_MASK_ZORA,
  Items.SHARED_MASK_TRUTH,
  Items.SHARED_MASK_BUNNY,
  Items.SHARED_MASK_KEATON,
  Items.SHARED_WALLET,
]);

const ITEMS_HEART_PIECES_CONTAINERS_BY_GAME = {
  oot: {
    hp: Items.OOT_HEART_PIECE,
    hc: Items.OOT_HEART_CONTAINER,
  },
  mm: {
    hp: Items.MM_HEART_PIECE,
    hc: Items.MM_HEART_CONTAINER,
  },
  shared: {
    hp: Items.SHARED_HEART_PIECE,
    hc: Items.SHARED_HEART_CONTAINER,
  },
}

export class LogicPassWorldTransform {
  private pool: ItemsCount = new Map;
  private locsByItem = new Map<Item, Set<string>>();
  private fixedLocations: Set<string>;

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
    this.fixedLocations = new Set(state.fixedLocations);
  }

  private makePool() {
    const { world } = this.state;
    for (const loc in world.checks) {
      const check = world.checks[loc];
      const item = check.item;
      countMapAdd(this.pool, item);
      const set = this.locsByItem.get(check.item) || new Set();
      set.add(loc);
      this.locsByItem.set(check.item, set);
    }
  }

  /**
   * Replace an item in the pool with another item.
   */
  private replaceItem(from: Item, to: Item) {
    const prevCount = this.pool.get(from) || 0;
    this.pool.delete(from);
    const newCount = (this.pool.get(to) || 0) + prevCount;
    if (newCount > 0) {
      this.pool.set(to, newCount);
    } else {
      this.pool.delete(to);
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
  private removeItem(item: Item, amount?: number) {
    const count = this.pool.get(item) || 0;
    if (amount === undefined || amount >= count) {
      this.pool.delete(item);
    } else {
      this.pool.set(item, count - amount);
    }
  }

  private removeItems(items: Set<Item>, amount?: number) {
    for (const item of items) {
      this.removeItem(item, amount);
    }
  }

  private addItem(item: Item, amount?: number) {
    const count = this.pool.get(item) || 0;
    if (amount === undefined) {
      amount = 1;
    }
    this.pool.set(item, count + amount);
  }

  private scarcifyPool(delta: number) {
    const { settings } = this.state;
    const items = [...ITEM_POOL_SCARCE];
    const itemsNolimit = [...ITEM_POOL_SCARCE_NOLIMIT];

    /* Tunics - shopsanity */
    if (settings.shopShuffleOot === 'full') {
      items.push(Items.OOT_TUNIC_GORON);
      items.push(Items.OOT_TUNIC_ZORA);
    }

    /* MM shared + fairy oca */
    if (settings.sharedOcarina && settings.fairyOcarinaMm) {
      items.push(Items.SHARED_OCARINA);
    }

    for (const item of items) {
      const amount = this.pool.get(item);
      if (amount) {
        let newAmount = amount - delta;
        if (newAmount < 1)
          newAmount = 1;
        this.pool.set(item, newAmount);
      }
    }

    for (const item of itemsNolimit) {
      this.removeItem(item, delta);
    }

    /* Remove heart pieces */
    this.removeItem(Items.OOT_HEART_PIECE);
    this.removeItem(Items.MM_HEART_PIECE);
    this.removeItem(Items.SHARED_HEART_PIECE);

    /* Minimal - remove heart containers */
    if (delta >= 2) {
      this.removeItem(Items.OOT_HEART_CONTAINER);
      this.removeItem(Items.MM_HEART_CONTAINER);
      this.removeItem(Items.SHARED_HEART_CONTAINER);
    }
  }

  private mergeHearts() {
    for (const g of ['oot', 'mm', 'shared'] as const) {
      const { hp, hc } = ITEMS_HEART_PIECES_CONTAINERS_BY_GAME[g];

      const hpCount = this.pool.get(hp) || 0;
      if (hpCount) {
        this.removeItem(hp);
        this.addItem(hc, hpCount / 4);
      }
    }
  }

  private plentifulPool() {
    const { settings } = this.state;
    let items = [...ITEM_POOL_PLENTIFUL];

    if (settings.songs === 'anywhere') {
      items = [...items, ...ItemGroups.SONGS];
    }

    if (settings.dungeonRewardShuffle === 'anywhere' || settings.dungeonRewardShuffle === 'dungeonsLimited') {
      items = [...items, ...ItemGroups.DUNGEON_REWARDS];
    }

    if (settings.shuffleGerudoCard) {
      items.push(Items.OOT_GERUDO_CARD);
    }

    if (settings.shuffleMasterSword && settings.progressiveSwordsOot !== 'progressive') {
      items.push(Items.OOT_SWORD_MASTER);
    }

    if (settings.zoraKing === 'open') {
      items.push(Items.OOT_BOTTLE_EMPTY);
    }

    if (settings.smallKeyShuffleOot === 'anywhere') {
      items = [...items, ...ItemGroups.SMALL_KEYS_OOT];
    }

    if (settings.smallKeyShuffleMm === 'anywhere') {
      items = [...items, ...ItemGroups.SMALL_KEYS_MM];
    }

    if (settings.bossKeyShuffleOot === 'anywhere') {
      items = [...items, ...ItemGroups.BOSS_KEYS_OOT];
    }

    if (settings.bossKeyShuffleMm === 'anywhere') {
      items = [...items, ...ItemGroups.BOSS_KEYS_MM];
    }

    if (settings.ganonBossKey === 'anywhere') {
      items.push(Items.OOT_BOSS_KEY_GANON);
    }

    if (settings.smallKeyShuffleHideout === 'anywhere') {
      items = [...items, Items.OOT_SMALL_KEY_GF];
    }

    if (settings.mapCompassShuffle === 'anywhere') {
      items = [...items, ...ItemGroups.MAPS, ...ItemGroups.COMPASSES];
    }

    if (settings.tingleShuffle === 'anywhere') {
      items = [...items, ...ItemGroups.TINGLE_MAPS];
    }

    if (settings.owlShuffle === 'anywhere') {
      items = [...items, ...ItemGroups.OWLS];
    }

    /* Add extra items */
    for (const item of items) {
      const amount = this.pool.get(item);
      if (amount) {
        this.addItem(item);
      }
    }

    /* Merge pieces of hearts */
    this.mergeHearts();
  }

  /**
   * Setup the shared items.
   */
  private setupSharedItems() {
    const { config, settings } = this.state;
    if (config.has('SHARED_BOWS')) {
      /* Bows and quivers */
      this.replaceItem(Items.OOT_BOW, Items.SHARED_BOW);
      this.replaceItem(Items.MM_BOW, Items.SHARED_BOW);
      this.removeItem(Items.SHARED_BOW, 3);

      /* Arrows */
      this.replaceItem(Items.OOT_ARROWS_30, Items.SHARED_ARROWS_5);
      this.replaceItem(Items.OOT_ARROWS_10, Items.SHARED_ARROWS_10);
      this.replaceItem(Items.OOT_ARROWS_30, Items.SHARED_ARROWS_30);
      this.replaceItem(Items.MM_ARROWS_10, Items.SHARED_ARROWS_10);
      this.replaceItem(Items.MM_ARROWS_30, Items.SHARED_ARROWS_30);
      this.replaceItem(Items.MM_ARROWS_40, Items.SHARED_ARROWS_40);
    }

    if (config.has('SHARED_BOMB_BAGS')) {
      /* Bomb Bags */
      this.replaceItem(Items.OOT_BOMB_BAG, Items.SHARED_BOMB_BAG);
      this.replaceItem(Items.MM_BOMB_BAG, Items.SHARED_BOMB_BAG);
      this.removeItem(Items.SHARED_BOMB_BAG, 3);

      /* Bombs */
      this.replaceItem(Items.OOT_BOMB,      Items.SHARED_BOMB);
      this.replaceItem(Items.OOT_BOMBS_5,   Items.SHARED_BOMBS_5);
      this.replaceItem(Items.OOT_BOMBS_10,  Items.SHARED_BOMBS_10);
      this.replaceItem(Items.OOT_BOMBS_20,  Items.SHARED_BOMBS_20);
      this.replaceItem(Items.OOT_BOMBS_30,  Items.SHARED_BOMBS_30);
      this.replaceItem(Items.MM_BOMB,       Items.SHARED_BOMB);
      this.replaceItem(Items.MM_BOMBS_5,    Items.SHARED_BOMBS_5);
      this.replaceItem(Items.MM_BOMBS_10,   Items.SHARED_BOMBS_10);
      this.replaceItem(Items.MM_BOMBS_20,   Items.SHARED_BOMBS_20);
      this.replaceItem(Items.MM_BOMBS_30,   Items.SHARED_BOMBS_30);
    }

    if (config.has('SHARED_MAGIC')) {
      this.replaceItem(Items.OOT_MAGIC_UPGRADE, Items.SHARED_MAGIC_UPGRADE);
      this.replaceItem(Items.MM_MAGIC_UPGRADE,  Items.SHARED_MAGIC_UPGRADE);
      this.removeItem(Items.SHARED_MAGIC_UPGRADE, 2);
    }

    if (settings.sharedMagicArrowFire) {
      this.replaceItem(Items.OOT_ARROW_FIRE,  Items.SHARED_ARROW_FIRE);
      this.replaceItem(Items.MM_ARROW_FIRE,   Items.SHARED_ARROW_FIRE);
      this.removeItem(Items.SHARED_ARROW_FIRE, 1);
    }

    if (settings.sharedMagicArrowIce) {
      this.replaceItem(Items.OOT_ARROW_ICE,   Items.SHARED_ARROW_ICE);
      this.replaceItem(Items.MM_ARROW_ICE,    Items.SHARED_ARROW_ICE);
      this.removeItem(Items.SHARED_ARROW_ICE, 1);
    }

    if (settings.sharedMagicArrowLight) {
      this.replaceItem(Items.OOT_ARROW_LIGHT, Items.SHARED_ARROW_LIGHT);
      this.replaceItem(Items.MM_ARROW_LIGHT,  Items.SHARED_ARROW_LIGHT);
      this.removeItem(Items.SHARED_ARROW_LIGHT, 1);
    }

    if (settings.sharedSongEpona) {
      this.replaceItem(Items.OOT_SONG_EPONA,   Items.SHARED_SONG_EPONA);
      this.replaceItem(Items.MM_SONG_EPONA,    Items.SHARED_SONG_EPONA);
      this.removeItem(Items.SHARED_SONG_EPONA, 1);
    }

    if (settings.sharedSongStorms) {
      this.replaceItem(Items.OOT_SONG_STORMS,  Items.SHARED_SONG_STORMS);
      this.replaceItem(Items.MM_SONG_STORMS,   Items.SHARED_SONG_STORMS);
      this.removeItem(Items.SHARED_SONG_STORMS, 1);
    }

    if (settings.sharedSongTime) {
      this.replaceItem(Items.OOT_SONG_TIME,    Items.SHARED_SONG_TIME);
      this.replaceItem(Items.MM_SONG_TIME,     Items.SHARED_SONG_TIME);
      this.removeItem(Items.SHARED_SONG_TIME, 1);
    }

    if (settings.sharedSongSun && settings.sunSongMm) {
      this.replaceItem(Items.OOT_SONG_SUN, Items.SHARED_SONG_SUN);
    }

    if (config.has('SHARED_NUTS_STICKS')) {
      /* Nuts */
      this.replaceItem(Items.OOT_NUTS_5,      Items.SHARED_NUTS_5);
      this.replaceItem(Items.OOT_NUTS_5_ALT,  Items.SHARED_NUTS_5);
      this.replaceItem(Items.OOT_NUTS_10,     Items.SHARED_NUTS_10);
      this.replaceItem(Items.MM_NUT,          Items.SHARED_NUT);
      this.replaceItem(Items.MM_NUTS_5,       Items.SHARED_NUTS_5);
      this.replaceItem(Items.MM_NUTS_10,      Items.SHARED_NUTS_10);

      /* Sticks */
      this.replaceItem(Items.OOT_STICK,       Items.SHARED_STICK);
      this.replaceItem(Items.OOT_STICKS_5,    Items.SHARED_STICKS_5);
      this.replaceItem(Items.OOT_STICKS_10,   Items.SHARED_STICKS_10);
      this.replaceItem(Items.MM_STICK,        Items.SHARED_STICK);
    }

    if (config.has('SHARED_HOOKSHOT')) {
      this.replaceItem(Items.OOT_HOOKSHOT, Items.SHARED_HOOKSHOT);
      this.replaceItem(Items.MM_HOOKSHOT,  Items.SHARED_HOOKSHOT);
      this.removeItem(Items.SHARED_HOOKSHOT, 1);
    } else if (this.state.settings.shortHookshotMm) {
      this.addItem(Items.MM_HOOKSHOT);
    }

    if (config.has('SHARED_LENS')) {
      this.replaceItem(Items.OOT_LENS, Items.SHARED_LENS);
      this.replaceItem(Items.MM_LENS,  Items.SHARED_LENS);
      this.removeItem(Items.SHARED_LENS, 1);
    }

    if (config.has('SHARED_OCARINA')) {
      this.replaceItem(Items.OOT_OCARINA, Items.SHARED_OCARINA);
      this.replaceItem(Items.MM_OCARINA,  Items.SHARED_OCARINA);
      this.removeItem(Items.SHARED_OCARINA, 1);
    } else if (this.state.settings.fairyOcarinaMm) {
      this.addItem(Items.MM_OCARINA);
    }

    if (settings.sharedMaskGoron) {
      this.replaceItem(Items.OOT_MASK_GORON,  Items.SHARED_MASK_GORON);
      this.replaceItem(Items.MM_MASK_GORON,   Items.SHARED_MASK_GORON);
      this.removeItem(Items.SHARED_MASK_GORON, 1);
    }

    if (settings.sharedMaskZora) {
      this.replaceItem(Items.OOT_MASK_ZORA,   Items.SHARED_MASK_ZORA);
      this.replaceItem(Items.MM_MASK_ZORA,    Items.SHARED_MASK_ZORA);
      this.removeItem(Items.SHARED_MASK_ZORA, 1);
    }

    if (settings.sharedMaskTruth) {
      this.replaceItem(Items.OOT_MASK_TRUTH,  Items.SHARED_MASK_TRUTH);
      this.replaceItem(Items.MM_MASK_TRUTH,   Items.SHARED_MASK_TRUTH);
      this.removeItem(Items.SHARED_MASK_TRUTH, 1);
    }

    if (settings.sharedMaskBunny) {
      this.replaceItem(Items.OOT_MASK_BUNNY,  Items.SHARED_MASK_BUNNY);
      this.replaceItem(Items.MM_MASK_BUNNY,   Items.SHARED_MASK_BUNNY);
      this.removeItem(Items.SHARED_MASK_BUNNY, 1);
    }

    if (settings.sharedMaskKeaton) {
      this.replaceItem(Items.OOT_MASK_KEATON, Items.SHARED_MASK_KEATON);
      this.replaceItem(Items.MM_MASK_KEATON,  Items.SHARED_MASK_KEATON);
      this.removeItem(Items.SHARED_MASK_KEATON, 1);
    }

    if (config.has('SHARED_WALLETS')) {
      /* Wallets */
      this.replaceItem(Items.OOT_WALLET,  Items.SHARED_WALLET);
      this.replaceItem(Items.MM_WALLET,   Items.SHARED_WALLET);

      this.pool.set(Items.SHARED_WALLET, (this.pool.get(Items.SHARED_WALLET) || 0) / 2);

      /* Rupees */
      this.replaceItem(Items.OOT_RUPEE_GREEN,   Items.SHARED_RUPEE_GREEN);
      this.replaceItem(Items.OOT_RUPEE_BLUE,    Items.SHARED_RUPEE_BLUE);
      this.replaceItem(Items.OOT_RUPEE_RED,     Items.SHARED_RUPEE_RED);
      this.replaceItem(Items.OOT_RUPEE_PURPLE,  Items.SHARED_RUPEE_PURPLE);
      this.replaceItem(Items.OOT_RUPEE_HUGE,    Items.SHARED_RUPEE_GOLD);
      this.replaceItem(Items.MM_RUPEE_GREEN,    Items.SHARED_RUPEE_GREEN);
      this.replaceItem(Items.MM_RUPEE_BLUE,     Items.SHARED_RUPEE_BLUE);
      this.replaceItem(Items.MM_RUPEE_RED,      Items.SHARED_RUPEE_RED);
      this.replaceItem(Items.MM_RUPEE_PURPLE,   Items.SHARED_RUPEE_PURPLE);
      this.replaceItem(Items.MM_RUPEE_SILVER,   Items.SHARED_RUPEE_SILVER);
      this.replaceItem(Items.MM_RUPEE_GOLD,     Items.SHARED_RUPEE_GOLD);
    }

    if (config.has('SHARED_HEALTH')) {
      /* Pieces and containers */
      this.replaceItem(Items.OOT_HEART_CONTAINER, Items.SHARED_HEART_CONTAINER);
      this.replaceItem(Items.MM_HEART_CONTAINER,  Items.SHARED_HEART_CONTAINER);
      this.replaceItem(Items.MM_HEART_PIECE,      Items.SHARED_HEART_PIECE);
      this.replaceItem(Items.OOT_HEART_PIECE,     Items.SHARED_HEART_PIECE);

      this.pool.set(Items.SHARED_HEART_CONTAINER, 6);
      this.pool.set(Items.SHARED_HEART_PIECE, 44);

      /* Defense */
      this.replaceItem(Items.OOT_DEFENSE_UPGRADE, Items.SHARED_DEFENSE_UPGRADE);
      this.replaceItem(Items.MM_DEFENSE_UPGRADE,  Items.SHARED_DEFENSE_UPGRADE);
      this.removeItem(Items.SHARED_DEFENSE_UPGRADE, 1);

      /* Recovery */
      this.replaceItem(Items.OOT_RECOVERY_HEART, Items.SHARED_RECOVERY_HEART);
      this.replaceItem(Items.MM_RECOVERY_HEART,  Items.SHARED_RECOVERY_HEART);
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
      this.pool.set(Items.SHARED_TRIFORCE, settings.triforcePieces);
    }
  }

  private removeLocations(locs: string[]) {
    const { world } = this.state;
    for (const loc of locs) {
      delete world.checks[loc];
      delete world.regions[loc];
      world.locations.delete(loc);
      this.fixedLocations.delete(loc);
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
      this.addItem(Items.OOT_WALLET);
      this.addItem(Items.MM_WALLET);
    }

    if (this.state.settings.colossalWallets) {
      this.addItem(Items.OOT_WALLET);
      this.addItem(Items.MM_WALLET);
    }

    if (this.state.settings.bottomlessWallets) {
      this.addItem(Items.OOT_WALLET);
      this.addItem(Items.MM_WALLET);
    }

    /* Setup shared items */
    this.setupSharedItems();

    /* Handle progressive shields */
    if (this.state.settings.progressiveShieldsOot === 'progressive') {
      this.replaceItem(Items.OOT_SHIELD_MIRROR, Items.OOT_SHIELD);
      this.addItem(Items.OOT_SHIELD, 2);
    }

    if (this.state.config.has('MM_PROGRESSIVE_SHIELDS')) {
      this.replaceItem(Items.MM_SHIELD_MIRROR, Items.MM_SHIELD);
      this.addItem(Items.MM_SHIELD);
    }

    /* Handle non-MQ Fire */
    if (!this.state.config.has('SMALL_KEY_SHUFFLE') && !this.state.mq.has('Fire')) {
      this.removeItem(Items.OOT_SMALL_KEY_FIRE, 1);
    }

    /* Handle tingle maps */
    if (['starting', 'removed'].includes(this.state.settings.tingleShuffle)) {
      for (const item of ItemGroups.TINGLE_MAPS) {
        this.removeItem(item);
      }
    }

    /* Handle maps/compasses */
    if (['starting', 'removed'].includes(this.state.settings.mapCompassShuffle)) {
      for (const item of [...ItemGroups.MAPS, ...ItemGroups.COMPASSES]) {
        this.removeItem(item);
      }
    }

    /* Handle eggs */
    if (!this.state.settings.eggShuffle) {
      this.removeItem(Items.OOT_WEIRD_EGG);
      this.removeItem(Items.OOT_POCKET_EGG);
      this.removeLocations(['OOT Hatch Chicken', 'OOT Hatch Pocket Cucco']);
    }

    /* Handle Ganon BK */
    if (settings.ganonBossKey === 'removed' || settings.ganonBossKey === 'custom') {
      this.removeItem(Items.OOT_BOSS_KEY_GANON);
    }

    /* Handle other boss keys */
    if (settings.bossKeyShuffleOot === 'removed') {
      this.removeItems(ItemGroups.BOSS_KEYS_OOT);
    }

    if (settings.bossKeyShuffleMm === 'removed') {
      this.removeItems(ItemGroups.BOSS_KEYS_MM);
    }

    /* Handle small keys */
    if (settings.smallKeyShuffleOot === 'removed') {
      this.removeItems(ItemGroups.SMALL_KEYS_OOT);
    }

    if (settings.smallKeyShuffleMm === 'removed') {
      this.removeItems(ItemGroups.SMALL_KEYS_MM);
    }

    if (settings.zoraKing === 'open') {
      this.removeItem(Items.OOT_RUTO_LETTER);
      this.addItem(Items.OOT_BOTTLE_EMPTY);
    }

    /* Handle OoT swords */
    if (this.state.config.has('OOT_PROGRESSIVE_SWORDS')) {
      this.replaceItem(Items.OOT_SWORD_KOKIRI,    Items.OOT_SWORD);
      this.replaceItem(Items.OOT_SWORD_MASTER,    Items.OOT_SWORD);
      this.replaceItem(Items.OOT_SWORD_KNIFE,     Items.OOT_SWORD);
      this.replaceItem(Items.OOT_SWORD_BIGGORON,  Items.OOT_SWORD);
    } else if (this.state.config.has('OOT_PROGRESSIVE_SWORDS_GORON')) {
      this.replaceItem(Items.OOT_SWORD_KNIFE,     Items.OOT_SWORD_GORON);
      this.replaceItem(Items.OOT_SWORD_BIGGORON,  Items.OOT_SWORD_GORON);
    }

    /* Handle MM Lullaby */
    if (this.state.config.has('MM_PROGRESSIVE_LULLABY')) {
      this.replaceItem(Items.MM_SONG_GORON, Items.MM_SONG_GORON_HALF);
    } else {
      this.removeItem(Items.MM_SONG_GORON_HALF);
      this.state.world.songLocations.delete('MM Goron Baby');
    }

    /* Handle MM sun song */
    if (settings.sunSongMm && !settings.sharedSongSun) {
      this.addItem(Items.MM_SONG_SUN);
    }

    /* Handle Skip Zelda */
    if (this.state.settings.skipZelda) {
      this.removeItem(Items.OOT_CHICKEN);

      for (const loc of LOCATIONS_ZELDA) {
        this.state.world.areas['OOT SPAWN'].locations[loc] = exprTrue();
        this.state.world.regions[loc] = 'NONE';
      }
    }

    /* Handle open gate */
    if (this.state.settings.kakarikoGate === 'open') {
      this.removeItem(Items.OOT_ZELDA_LETTER);
    }

    /* Handle fixed locations */
    for (const loc of this.fixedLocations) {
      const check = this.state.world.checks[loc];
      const { item } = check;
      this.removeItem(item, 1);
    }

    /* Handle required junks */
    const renewableJunks: ItemsCount = new Map;
    for (const item of this.pool.keys()) {
      if (ItemHelpers.isJunk(item) && ItemHelpers.isItemConsumable(item)) {
        for (const loc of this.locsByItem.get(item) || []) {
          const l = makeLocation(loc);
          if (isLocationRenewable(this.state.world, l) && !this.fixedLocations.has(loc)) {
            countMapAdd(renewableJunks, item);
          }
        }
      }
    }

    return { pool: this.pool, renewableJunks, fixedLocations: this.fixedLocations };
  }
}
