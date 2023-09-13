import { Confvar } from '../confvars';
import { DATA_POOL } from '../data';
import { Item, ItemGroups, ItemHelpers, Items, PlayerItem, PlayerItems, makePlayerItem } from '../items';
import { Monitor } from '../monitor';
import { Settings } from '../settings';
import { countMapAdd, gameId } from '../util';
import { exprTrue } from './expr';
import { LOCATIONS_ZELDA, Location, isLocationOtherFairy, isLocationRenewable, locationData, makeLocation } from './locations';
import { ItemSharedDef, SharedItemGroups } from './shared';
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
  Items.OOT_SKELETON_KEY,
  Items.OOT_RUPEE_MAGICAL,
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
  Items.MM_SKELETON_KEY,
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
  Items.SHARED_SKELETON_KEY,
  ...ItemGroups.OOT_SOULS,
  ...ItemGroups.MM_SOULS,
  ...ItemGroups.SHARED_SOULS,
  Items.OOT_BUTTON_A,
  Items.OOT_BUTTON_C_RIGHT,
  Items.OOT_BUTTON_C_LEFT,
  Items.OOT_BUTTON_C_UP,
  Items.OOT_BUTTON_C_DOWN,
  Items.MM_BUTTON_A,
  Items.MM_BUTTON_C_RIGHT,
  Items.MM_BUTTON_C_LEFT,
  Items.MM_BUTTON_C_UP,
  Items.MM_BUTTON_C_DOWN,
  Items.SHARED_BUTTON_A,
  Items.SHARED_BUTTON_C_RIGHT,
  Items.SHARED_BUTTON_C_LEFT,
  Items.SHARED_BUTTON_C_UP,
  Items.SHARED_BUTTON_C_DOWN,
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

const KEY_RINGS_OOT = new Map([
  [Items.OOT_SMALL_KEY_FOREST, Items.OOT_KEY_RING_FOREST],
  [Items.OOT_SMALL_KEY_FIRE, Items.OOT_KEY_RING_FIRE],
  [Items.OOT_SMALL_KEY_WATER, Items.OOT_KEY_RING_WATER],
  [Items.OOT_SMALL_KEY_SHADOW, Items.OOT_KEY_RING_SHADOW],
  [Items.OOT_SMALL_KEY_SPIRIT, Items.OOT_KEY_RING_SPIRIT],
  [Items.OOT_SMALL_KEY_BOTW, Items.OOT_KEY_RING_BOTW],
  [Items.OOT_SMALL_KEY_GTG, Items.OOT_KEY_RING_GTG],
  [Items.OOT_SMALL_KEY_GANON, Items.OOT_KEY_RING_GANON],
]);

const KEY_RINGS_MM = new Map([
  [Items.MM_SMALL_KEY_WF, Items.MM_KEY_RING_WF],
  [Items.MM_SMALL_KEY_SH, Items.MM_KEY_RING_SH],
  [Items.MM_SMALL_KEY_GB, Items.MM_KEY_RING_GB],
  [Items.MM_SMALL_KEY_ST, Items.MM_KEY_RING_ST],
]);

const SILVER_POUCHES = new Map([
  [Items.OOT_RUPEE_SILVER_DC, Items.OOT_POUCH_SILVER_DC],
  [Items.OOT_RUPEE_SILVER_BOTW, Items.OOT_POUCH_SILVER_BOTW],
  [Items.OOT_RUPEE_SILVER_SPIRIT_CHILD, Items.OOT_POUCH_SILVER_SPIRIT_CHILD],
  [Items.OOT_RUPEE_SILVER_SPIRIT_SUN, Items.OOT_POUCH_SILVER_SPIRIT_SUN],
  [Items.OOT_RUPEE_SILVER_SPIRIT_BOULDERS, Items.OOT_POUCH_SILVER_SPIRIT_BOULDERS],
  [Items.OOT_RUPEE_SILVER_SPIRIT_LOBBY, Items.OOT_POUCH_SILVER_SPIRIT_LOBBY],
  [Items.OOT_RUPEE_SILVER_SPIRIT_ADULT, Items.OOT_POUCH_SILVER_SPIRIT_ADULT],
  [Items.OOT_RUPEE_SILVER_SHADOW_SCYTHE, Items.OOT_POUCH_SILVER_SHADOW_SCYTHE],
  [Items.OOT_RUPEE_SILVER_SHADOW_PIT, Items.OOT_POUCH_SILVER_SHADOW_PIT],
  [Items.OOT_RUPEE_SILVER_SHADOW_SPIKES, Items.OOT_POUCH_SILVER_SHADOW_SPIKES],
  [Items.OOT_RUPEE_SILVER_SHADOW_BLADES, Items.OOT_POUCH_SILVER_SHADOW_BLADES],
  [Items.OOT_RUPEE_SILVER_IC_SCYTHE, Items.OOT_POUCH_SILVER_IC_SCYTHE],
  [Items.OOT_RUPEE_SILVER_IC_BLOCK, Items.OOT_POUCH_SILVER_IC_BLOCK],
  [Items.OOT_RUPEE_SILVER_GTG_SLOPES, Items.OOT_POUCH_SILVER_GTG_SLOPES],
  [Items.OOT_RUPEE_SILVER_GTG_LAVA, Items.OOT_POUCH_SILVER_GTG_LAVA],
  [Items.OOT_RUPEE_SILVER_GTG_WATER, Items.OOT_POUCH_SILVER_GTG_WATER],
  [Items.OOT_RUPEE_SILVER_GANON_SPIRIT, Items.OOT_POUCH_SILVER_GANON_SPIRIT],
  [Items.OOT_RUPEE_SILVER_GANON_LIGHT, Items.OOT_POUCH_SILVER_GANON_LIGHT],
  [Items.OOT_RUPEE_SILVER_GANON_FIRE, Items.OOT_POUCH_SILVER_GANON_FIRE],
  [Items.OOT_RUPEE_SILVER_GANON_FOREST, Items.OOT_POUCH_SILVER_GANON_FOREST],
  [Items.OOT_RUPEE_SILVER_GANON_SHADOW, Items.OOT_POUCH_SILVER_GANON_SHADOW],
  [Items.OOT_RUPEE_SILVER_GANON_WATER, Items.OOT_POUCH_SILVER_GANON_WATER],
]);

export class LogicPassWorldTransform {
  private pool: PlayerItems = new Map;
  private locsByItem = new Map<PlayerItem, Set<Location>>();
  private fixedLocations: Set<Location>;

  constructor(
    private readonly state: {
      monitor: Monitor;
      worlds: World[];
      settings: Settings;
      config: Set<Confvar>;
      fixedLocations: Set<Location>;
    }
  ) {
    this.fixedLocations = new Set(state.fixedLocations);
  }

  private makePools() {
    const { worlds } = this.state;
    for (let i = 0; i < worlds.length; ++i) {
      const world = worlds[i];
      for (const locId in world.checks) {
        const check = world.checks[locId];
        const location = makeLocation(locId, i);
        const item = check.item;
        const playerItem = makePlayerItem(item, i);
        countMapAdd(this.pool, playerItem);
        const set = this.locsByItem.get(playerItem) || new Set();
        set.add(location);
        this.locsByItem.set(playerItem, set);
      }
    }
  }

  /**
   * Replace an item in the pool with another item.
   */
  private replacePlayerItem(from: PlayerItem, to: PlayerItem) {
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
      const locData = locationData(loc);
      const check = this.state.worlds[locData.world!].checks[locData.id];
      check.item = to.item;
      newSet.add(loc);
    }
    this.locsByItem.set(to, newSet);
    this.locsByItem.delete(from);
  }

  private replaceItem(from: Item, to: Item) {
    for (let i = 0; i < this.state.worlds.length; ++i) {
      this.replacePlayerItem(makePlayerItem(from, i), makePlayerItem(to, i));
    }
  }

  /**
   * Remove an item from the pool.
   * Optionally, limit the number of items removed.
   */
  private removePlayerItem(item: PlayerItem, amount?: number) {
    const count = this.pool.get(item) || 0;
    if (amount === undefined || amount >= count) {
      this.pool.delete(item);
    } else {
      this.pool.set(item, count - amount);
    }
  }

  private removeItem(item: Item, amount?: number) {
    for (let i = 0; i < this.state.worlds.length; ++i) {
      this.removePlayerItem(makePlayerItem(item, i), amount);
    }
  }

  private removeItems(items: Set<Item>, amount?: number) {
    for (const item of items) {
      this.removeItem(item, amount);
    }
  }

  private addPlayerItem(item: PlayerItem, amount?: number) {
    const count = this.pool.get(item) || 0;
    if (amount === undefined) {
      amount = 1;
    }
    this.pool.set(item, count + amount);
  }

  private addItem(item: Item, amount?: number) {
    for (let i = 0; i < this.state.worlds.length; ++i) {
      this.addPlayerItem(makePlayerItem(item, i), amount);
    }
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

    for (let i = 0; i < this.state.worlds.length; ++i) {
      for (const item of items) {
        const playerItem = makePlayerItem(item, i);
        const amount = this.pool.get(playerItem);
        if (amount) {
          let newAmount = amount - delta;
          if (newAmount < 1)
            newAmount = 1;
          this.pool.set(playerItem, newAmount);
        }
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
    for (let i = 0; i < this.state.worlds.length; ++i) {
      for (const g of ['oot', 'mm', 'shared'] as const) {
        const { hp, hc } = ITEMS_HEART_PIECES_CONTAINERS_BY_GAME[g];
        const playerHp = makePlayerItem(hp, i);
        const playerHc = makePlayerItem(hc, i);
        const hpCount = this.pool.get(playerHp) || 0;
        if (hpCount) {
          this.removePlayerItem(playerHp);
          this.addPlayerItem(playerHc, hpCount / 4);
        }
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
      items = [...items, ...ItemGroups.SMALL_KEYS_OOT, ...ItemGroups.KEY_RINGS_OOT];
    }

    if (settings.smallKeyShuffleMm === 'anywhere') {
      items = [...items, ...ItemGroups.SMALL_KEYS_MM, ...ItemGroups.KEY_RINGS_MM];
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
      items = [...items, Items.OOT_SMALL_KEY_GF, Items.OOT_KEY_RING_GF];
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

    if (settings.silverRupeeShuffle === 'anywhere') {
      items = [...items, ...ItemGroups.RUPEES_SILVER];
    }

    /* Add extra items */
    for (let i = 0; i < this.state.worlds.length; ++i) {
      for (const item of items) {
        const playerItem = makePlayerItem(item, i);
        const amount = this.pool.get(playerItem);
        if (amount) {
          this.addPlayerItem(playerItem);
        }
      }
    }

    /* Merge pieces of hearts */
    this.mergeHearts();
  }

  private shareItems(defs: ItemSharedDef[]) {
    for (let playerId = 0; playerId < this.state.worlds.length; ++playerId) {
      for (const def of defs) {
        const piShared = makePlayerItem(def.shared, playerId);
        const piOot = makePlayerItem(def.oot, playerId);
        const piMm = makePlayerItem(def.mm, playerId);
        const amountOot = this.pool.get(piOot) || 0;
        const amountMm = this.pool.get(piMm) || 0;
        const newAmount = (amountOot + amountMm) / 2;
        this.removePlayerItem(piOot);
        this.removePlayerItem(piMm);
        this.addPlayerItem(piShared, newAmount);
      }
    }
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

      for (let i = 0; i < this.state.worlds.length; ++i) {
        const playerItem = makePlayerItem(Items.SHARED_WALLET, i);
        this.pool.set(playerItem, (this.pool.get(playerItem) || 0) / 2);
      }

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

      for (let i = 0; i < this.state.worlds.length; ++i) {
        this.pool.set(makePlayerItem(Items.SHARED_HEART_CONTAINER, i), 6);
        this.pool.set(makePlayerItem(Items.SHARED_HEART_PIECE, i), 44);
      }

      /* Defense */
      this.replaceItem(Items.OOT_DEFENSE_UPGRADE, Items.SHARED_DEFENSE_UPGRADE);
      this.replaceItem(Items.MM_DEFENSE_UPGRADE,  Items.SHARED_DEFENSE_UPGRADE);
      this.removeItem(Items.SHARED_DEFENSE_UPGRADE, 1);

      /* Recovery */
      this.replaceItem(Items.OOT_RECOVERY_HEART, Items.SHARED_RECOVERY_HEART);
      this.replaceItem(Items.MM_RECOVERY_HEART,  Items.SHARED_RECOVERY_HEART);
    }

    if (settings.sharedSouls) {
      this.shareItems(SharedItemGroups.SOULS);
    }

    if (settings.sharedSkeletonKey) {
      this.shareItems(SharedItemGroups.SKELETON_KEY);
    }

    if (settings.sharedOcarinaButtons) {
      this.shareItems(SharedItemGroups.OCARINA_BUTTONS);
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
      for (let i = 0; i < this.state.worlds.length; ++i) {
        this.pool.set(makePlayerItem(Items.SHARED_TRIFORCE, i), settings.triforcePieces);
      }
    }

    /* Triforce quest */
    if (settings.goal === 'triforce3') {
      for (let i = 0; i < this.state.worlds.length; ++i) {
        this.pool.set(makePlayerItem(Items.SHARED_TRIFORCE_POWER, i), 1);
        this.pool.set(makePlayerItem(Items.SHARED_TRIFORCE_COURAGE, i), 1);
        this.pool.set(makePlayerItem(Items.SHARED_TRIFORCE_WISDOM, i), 1);
      }
    }

    /* Coins */
    for (let i = 0; i < this.state.worlds.length; ++i) {
      if (settings.coinsRed)
        this.pool.set(makePlayerItem(Items.OOT_COIN_RED, i), settings.coinsRed);
      if (settings.coinsGreen)
        this.pool.set(makePlayerItem(Items.OOT_COIN_GREEN, i), settings.coinsGreen);
      if (settings.coinsBlue)
        this.pool.set(makePlayerItem(Items.OOT_COIN_BLUE, i), settings.coinsBlue);
      if (settings.coinsYellow)
        this.pool.set(makePlayerItem(Items.OOT_COIN_YELLOW, i), settings.coinsYellow);
    }
  }

  private removeLocations(locs: string[]) {
    const { worlds } = this.state;
    for (let i = 0; i < worlds.length; ++i) {
      const world = worlds[i];
      for (const loc of locs) {
        delete world.checks[loc];
        delete world.regions[loc];
        world.locations.delete(loc);
        this.fixedLocations.delete(makeLocation(loc, i));
      }
      for (const areaName in world.areas) {
        const area = world.areas[areaName];
        const locations = area.locations;
        for (const loc of locs) {
          delete locations[loc];
        }
      }
      for (const dungeonId of Object.keys(world.dungeons)) {
        const dungeon = world.dungeons[dungeonId];
        for (const l of locs) {
          dungeon.delete(l);
        }
      }
    }
  }

  private makeLocationStarting(loc: string | string[]) {
    if (typeof loc === 'string') {
      loc = [loc];
    }

    for (const locId of loc) {
      for (const world of this.state.worlds) {
        world.areas['OOT SPAWN'].locations[locId] = exprTrue();
        world.regions[locId] = 'NONE';
      }
    }
  }

  run() {
    const { settings } = this.state;
    this.state.monitor.log('Logic: World Transform');

    /* Potsanity */
    if (!settings.shufflePotsOot) {
      const pots = DATA_POOL.oot.filter((x: any) => x.type === 'pot').map((x: any) => gameId('oot', x.location, ' ')) as string[];
      this.removeLocations(pots);
    } else {
      if (settings.goal === 'triforce' || settings.goal === 'triforce3') {
        const potsGanonTower = DATA_POOL.oot.filter((x: any) => x.type === 'pot' && x.scene === 'GANON_TOWER').map((x: any) => gameId('oot', x.location, ' ')) as string[];
        this.removeLocations(potsGanonTower);
      }
    }

    if (!settings.shufflePotsMm) {
      const pots = DATA_POOL.mm.filter((x: any) => x.type === 'pot').map((x: any) => gameId('mm', x.location, ' ')) as string[];
      this.removeLocations(pots);
    } else {
      if (settings.goal === 'triforce' || settings.goal === 'triforce3') {
        const potsMajora = DATA_POOL.mm.filter((x: any) => x.type === 'pot' && x.scene === 'LAIR_MAJORA').map((x: any) => gameId('mm', x.location, ' ')) as string[];
        this.removeLocations(potsMajora);
      }
    }

    /* Grasssanity */
    if (!settings.shuffleGrassOot) {
      const pots = DATA_POOL.oot.filter((x: any) => x.type === 'grass').map((x: any) => gameId('oot', x.location, ' ')) as string[];
      this.removeLocations(pots);
    }

    /* Carpenters */
    if (['open', 'single'].includes(settings.gerudoFortress)) {
      this.removeLocations(['OOT Gerudo Fortress Jail 2', 'OOT Gerudo Fortress Jail 3', 'OOT Gerudo Fortress Jail 4']);

      if (settings.gerudoFortress === 'open') {
        this.removeLocations(['OOT Gerudo Fortress Jail 1']);
        const loc = 'OOT Gerudo Member Card';
        for (let i = 0; i < this.state.worlds.length; ++i) {
          const world = this.state.worlds[i];
          world.areas['OOT SPAWN'].locations[loc] = exprTrue();
          world.regions[loc] = 'NONE';
          world.dungeons['GF'].delete(loc);
        }
      }
    }

    /* Make the basic item pool */
    this.makePools();

    /* Add extra items */
    for (const item of EXTRA_ITEMS) {
      this.addItem(item);
    }

    /* Add souls */
    if (settings.enemySoulsOot) {
      for (const item of ItemGroups.OOT_SOULS) {
        this.addItem(item);
      }
    }

    if (settings.enemySoulsMm) {
      for (const item of ItemGroups.MM_SOULS) {
        this.addItem(item);
      }
    }

    /* Add skeleton keys */
    if (settings.skeletonKeyOot) {
      this.addItem(Items.OOT_SKELETON_KEY);
    }

    if (settings.skeletonKeyMm) {
      this.addItem(Items.MM_SKELETON_KEY);
    }

    /* Add magical rupee */
    if (settings.magicalRupee) {
      this.addItem(Items.OOT_RUPEE_MAGICAL);
    }

    /* Add ocarina buttons */
    if (settings.ocarinaButtonsShuffleOot) {
      this.addItem(Items.OOT_BUTTON_A);
      this.addItem(Items.OOT_BUTTON_C_RIGHT);
      this.addItem(Items.OOT_BUTTON_C_LEFT);
      this.addItem(Items.OOT_BUTTON_C_UP);
      this.addItem(Items.OOT_BUTTON_C_DOWN);
    }

    if (settings.ocarinaButtonsShuffleMm) {
      this.addItem(Items.MM_BUTTON_A);
      this.addItem(Items.MM_BUTTON_C_RIGHT);
      this.addItem(Items.MM_BUTTON_C_LEFT);
      this.addItem(Items.MM_BUTTON_C_UP);
      this.addItem(Items.MM_BUTTON_C_DOWN);
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
    if (!this.state.config.has('SMALL_KEY_SHUFFLE')) {
      for (let i = 0; i < this.state.worlds.length; ++i) {
        const world = this.state.worlds[i];
        if (!world.mq.has('Fire')) {
          this.removePlayerItem(makePlayerItem(Items.OOT_SMALL_KEY_FIRE, i), 1);
        }
      }
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

    /* Handle key rings */
    if (settings.smallKeyRingOot === 'keyRings') {
      for (let worldId = 0; worldId < this.state.worlds.length; ++worldId) {
        for (const [key, ring] of KEY_RINGS_OOT.entries()) {
          const piKey = makePlayerItem(key, worldId);
          const piRing = makePlayerItem(ring, worldId);
          if (this.pool.has(piKey)) {
            this.removePlayerItem(piKey);
            this.addPlayerItem(piRing);
          }
        }

        /* Hideout keys need special handling */
        if (settings.smallKeyShuffleHideout !== 'vanilla') {
          this.removePlayerItem(makePlayerItem(Items.OOT_SMALL_KEY_GF, worldId));
          this.addPlayerItem(makePlayerItem(Items.OOT_KEY_RING_GF, worldId));
        }
      }
    }

    if (settings.smallKeyRingMm === 'keyRings') {
      for (let worldId = 0; worldId < this.state.worlds.length; ++worldId) {
        for (const [key, ring] of KEY_RINGS_MM.entries()) {
          const piKey = makePlayerItem(key, worldId);
          const piRing = makePlayerItem(ring, worldId);
          if (this.pool.has(piKey)) {
            this.removePlayerItem(piKey);
            this.addPlayerItem(piRing);
          }
        }
      }
    }

    /* Handle silver pouches */
    if (settings.silverRupeePouches) {
      for (let worldId = 0; worldId < this.state.worlds.length; ++worldId) {
        for (const [rupee, pouch] of SILVER_POUCHES.entries()) {
          const piRupee = makePlayerItem(rupee, worldId);
          const piPouch = makePlayerItem(pouch, worldId);
          if (this.pool.has(piRupee)) {
            this.removePlayerItem(piRupee);
            this.addPlayerItem(piPouch);
          }
        }
      }
    }

    if (settings.zoraKing === 'open') {
      this.removeItem(Items.OOT_RUTO_LETTER);
      this.addItem(Items.OOT_BOTTLE_EMPTY);
    }

    /* Handle OoT swords */
    if (settings.startingAge === 'adult') {
      this.removeItem(Items.OOT_SWORD_MASTER);
    }
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
      for (let i = 0; i < this.state.worlds.length; ++i) {
        this.state.worlds[i].songLocations.delete('MM Goron Baby');
      }
    }

    /* Handle MM sun song */
    if (settings.sunSongMm && !settings.sharedSongSun) {
      this.addItem(Items.MM_SONG_SUN);
    }

    /* Handle MM other fairies */
    if (this.state.settings.strayFairyOtherShuffle === 'removed') {
      for (let i = 0; i < this.state.worlds.length; ++i) {
        const world = this.state.worlds[i];
        for (const locId of world.locations) {
          const loc = makeLocation(locId, i);
          if (isLocationOtherFairy(world, loc)) {
            this.removeLocations([locId]);
          }
        }
      }
    }

    /* Handle Skip Zelda */
    if (this.state.settings.skipZelda) {
      this.removeItem(Items.OOT_CHICKEN);
      this.makeLocationStarting(LOCATIONS_ZELDA);
    }

    /* Handle open gate */
    if (this.state.settings.kakarikoGate === 'open') {
      this.removeItem(Items.OOT_ZELDA_LETTER);
    }

    /* Handle fixed locations */
    for (const loc of this.fixedLocations) {
      const worldId = locationData(loc).world as number;
      const world = this.state.worlds[worldId];
      const check = world.checks[locationData(loc).id];
      const { item } = check;
      const pi = makePlayerItem(item, worldId);
      this.removePlayerItem(pi, 1);
    }

    /* Handle required junks */
    const renewableJunks: PlayerItems = new Map;
    for (const pi of this.pool.keys()) {
      if (ItemHelpers.isJunk(pi.item) && ItemHelpers.isItemConsumable(pi.item)) {
        for (const loc of this.locsByItem.get(pi) || []) {
          const world = this.state.worlds[locationData(loc).world as number];
          if (isLocationRenewable(world, loc) && !this.fixedLocations.has(loc)) {
            countMapAdd(renewableJunks, pi);
          }
        }
      }
    }

    return { pool: this.pool, renewableJunks, fixedLocations: this.fixedLocations };
  }
}
