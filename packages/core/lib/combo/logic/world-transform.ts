import { POOL } from '@ootmm/data';

import { Item, ItemGroups, ItemHelpers, Items, PlayerItem, PlayerItems, itemByID, makePlayerItem } from '../items';
import { ItemID } from '../items/defs';
import { Monitor } from '../monitor';
import { sample, Random, randomInt } from '../random';
import { Settings } from '../settings';
import { countMapAdd, gameId } from '../util';
import { exprTrue } from './expr';
import { LOCATIONS_ZELDA, Location, isLocationOtherFairy, isLocationRenewable, locationData, makeLocation } from './locations';
import { ItemSharedDef, SharedItemGroups } from './shared';
import { World } from './world';
import { ItemProperties } from './item-properties';
import { CLOCKS } from '../items/groups';

const BROKEN_ACTORS_CHECKS = [
  'OOT Dodongo Cavern Grass East Corridor Side Room',
  'OOT Lake Hylia Pot 1',
  'OOT Lake Hylia Pot 2',
  'OOT Lake Hylia Grass Child 5',
  'OOT Hyrule Castle Pot 1',
  'OOT Hyrule Castle Pot 2',
  'OOT MQ Dodongo Cavern Grass Vanilla Bomb Bag Room',
  'OOT MQ Dodongo Cavern Grass Room Before Miniboss',
];

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
  ...CLOCKS,
  Items.MM_CLOCK,
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
  Items.OOT_MASK_BLAST,
  Items.SHARED_MASK_BLAST,
  Items.OOT_MASK_STONE,
  Items.SHARED_MASK_STONE,
  Items.OOT_SONG_EMPTINESS,
  Items.SHARED_SONG_EMPTINESS,
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
  Items.MM_SPELL_FIRE,
  Items.MM_SPELL_WIND,
  Items.MM_SPELL_LOVE,
  Items.MM_BOOTS_IRON,
  Items.MM_BOOTS_HOVER,
  Items.MM_TUNIC_GORON,
  Items.MM_TUNIC_ZORA,
  Items.MM_SCALE,
  Items.MM_STRENGTH,
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
  ...ItemGroups.OOT_SOULS_ENEMY,
  ...ItemGroups.MM_SOULS_ENEMY,
  ...ItemGroups.OOT_SOULS_BOSS,
  ...ItemGroups.MM_SOULS_BOSS,
  ...ItemGroups.OOT_SOULS_NPC,
  ...ItemGroups.SHARED_SOULS_ENEMY,
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
  Items.SHARED_SPELL_FIRE,
  Items.SHARED_SPELL_WIND,
  Items.SHARED_SPELL_LOVE,
  Items.SHARED_BOOTS_IRON,
  Items.SHARED_BOOTS_HOVER,
  Items.SHARED_TUNIC_GORON,
  Items.SHARED_TUNIC_ZORA,
  Items.SHARED_SCALE,
  Items.SHARED_STRENGTH,
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
  private pool: PlayerItems = new Map;
  private locsByItem = new Map<PlayerItem, Set<Location>>();
  private fixedLocations: Set<Location>;

  constructor(
    private readonly state: {
      random: Random;
      monitor: Monitor;
      worlds: World[];
      settings: Settings;
      fixedLocations: Set<Location>;
      itemProperties: ItemProperties;
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

  private addItems(items: Iterable<Item>, amount?: number) {
    for (const item of items) {
      this.addItem(item, amount);
    }
  }

  private scarcifyPool(delta: number) {
    const { settings } = this.state;
    const items = [...ITEM_POOL_SCARCE];
    const itemsNolimit = [...ITEM_POOL_SCARCE_NOLIMIT];

    /* Ocarinas and age change */
    if (settings.ageChange !== 'oot') {
      items.push(Items.OOT_OCARINA);
    }

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

    if (settings.smallKeyShuffleChestGame === 'anywhere') {
      items = [...items, Items.OOT_SMALL_KEY_TCG, Items.OOT_KEY_RING_TCG];
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

    if (settings.pondFishShuffle) {
      items.push(Items.OOT_FISHING_POND_CHILD_FISH);
      items.push(Items.OOT_FISHING_POND_ADULT_FISH);
      items.push(Items.OOT_FISHING_POND_CHILD_LOACH);
      items.push(Items.OOT_FISHING_POND_ADULT_LOACH);
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

  private shareItems(defs: ItemSharedDef[], policy: 'max' | 'sum') {
    for (let playerId = 0; playerId < this.state.worlds.length; ++playerId) {
      for (const def of defs) {
        const piShared = makePlayerItem(def.shared, playerId);
        const piOot = makePlayerItem(def.oot, playerId);
        const piMm = makePlayerItem(def.mm, playerId);
        const amountOot = this.pool.get(piOot) || 0;
        const amountMm = this.pool.get(piMm) || 0;
        let newAmount: number;
        switch (policy) {
        case 'max': newAmount = Math.max(amountOot, amountMm); break;
        case 'sum': newAmount = amountOot + amountMm; break;
        }
        this.removePlayerItem(piOot);
        this.removePlayerItem(piMm);
        this.addPlayerItem(piShared, newAmount);
      }
    }
  }

  private setupExtraTraps() {
    let extraTraps: Item[] = [];

    if (this.state.settings.trapRupoor) {
      extraTraps.push(Items.OOT_TRAP_RUPOOR);
      extraTraps.push(Items.MM_TRAP_RUPOOR);
    }

    if (extraTraps.length === 0)
      return;

    const junkCount = Array.from(this.pool.entries())
      .filter(([pi, _]) => this.state.itemProperties.junk.has(pi.item))
      .reduce((acc, [_, count]) => acc + count, 0);
    const trapCount = (junkCount * 20) / (this.state.worlds.length * extraTraps.length * 100);
    this.addItems(extraTraps, trapCount);
  }

  /**
   * Setup the shared items.
   */
  private setupSharedItems() {
    const { settings } = this.state;

    if (settings.sharedBows) {
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

    if (settings.sharedBombBags) {
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

    if (settings.sharedBombchuBags) {
      this.replaceItem(Items.OOT_BOMBCHU_5,   Items.SHARED_BOMBCHU_5);
      this.replaceItem(Items.OOT_BOMBCHU_10,  Items.SHARED_BOMBCHU_10);
      this.replaceItem(Items.OOT_BOMBCHU_20,  Items.SHARED_BOMBCHU_20);
      this.replaceItem(Items.MM_BOMBCHU,      Items.SHARED_BOMBCHU);
      this.replaceItem(Items.MM_BOMBCHU_5,    Items.SHARED_BOMBCHU_5);
      this.replaceItem(Items.MM_BOMBCHU_10,   Items.SHARED_BOMBCHU_10);
      this.replaceItem(Items.MM_BOMBCHU_20,   Items.SHARED_BOMBCHU_20);
    }

    if (settings.sharedMagic) {
      this.replaceItem(Items.OOT_MAGIC_UPGRADE, Items.SHARED_MAGIC_UPGRADE);
      this.replaceItem(Items.MM_MAGIC_UPGRADE,  Items.SHARED_MAGIC_UPGRADE);
      this.removeItem(Items.SHARED_MAGIC_UPGRADE, 2);
      this.replaceItem(Items.OOT_MAGIC_JAR_SMALL, Items.SHARED_MAGIC_JAR_SMALL);
      this.replaceItem(Items.OOT_MAGIC_JAR_LARGE, Items.SHARED_MAGIC_JAR_LARGE);
      this.replaceItem(Items.MM_MAGIC_JAR_SMALL, Items.SHARED_MAGIC_JAR_SMALL);
      this.replaceItem(Items.MM_MAGIC_JAR_LARGE, Items.SHARED_MAGIC_JAR_LARGE);
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

    if (settings.sharedNutsSticks) {
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

    if (settings.sharedHookshot) {
      this.replaceItem(Items.OOT_HOOKSHOT, Items.SHARED_HOOKSHOT);
      this.replaceItem(Items.MM_HOOKSHOT,  Items.SHARED_HOOKSHOT);
      this.removeItem(Items.SHARED_HOOKSHOT, 1);
    } else if (this.state.settings.shortHookshotMm) {
      this.addItem(Items.MM_HOOKSHOT);
    }

    if (settings.sharedSpellFire) {
      this.replaceItem(Items.OOT_SPELL_FIRE, Items.SHARED_SPELL_FIRE);
    } else if (settings.spellFireMm) {
      this.addItem(Items.MM_SPELL_FIRE);
    }

    if (settings.sharedSpellWind) {
      this.replaceItem(Items.OOT_SPELL_WIND, Items.SHARED_SPELL_WIND);
    } else if (settings.spellWindMm) {
      this.addItem(Items.MM_SPELL_WIND);
    }

    if (settings.sharedSpellLove) {
      this.replaceItem(Items.OOT_SPELL_LOVE, Items.SHARED_SPELL_LOVE);
    } else if (settings.spellLoveMm) {
      this.addItem(Items.MM_SPELL_LOVE);
    }

    if (settings.sharedBootsIron) {
      this.replaceItem(Items.OOT_BOOTS_IRON, Items.SHARED_BOOTS_IRON);
    } else if (settings.bootsIronMm) {
      this.addItem(Items.MM_BOOTS_IRON);
    }

    if (settings.sharedBootsHover) {
      this.replaceItem(Items.OOT_BOOTS_HOVER, Items.SHARED_BOOTS_HOVER);
    } else if (settings.bootsHoverMm) {
      this.addItem(Items.MM_BOOTS_HOVER);
    }

    if (settings.sharedTunicGoron) {
      this.replaceItem(Items.OOT_TUNIC_GORON, Items.SHARED_TUNIC_GORON);
    } else if (settings.tunicGoronMm) {
      this.addItem(Items.MM_TUNIC_GORON);
    }

    if (settings.sharedTunicZora) {
      this.replaceItem(Items.OOT_TUNIC_ZORA, Items.SHARED_TUNIC_ZORA);
    } else if (settings.tunicZoraMm) {
      this.addItem(Items.MM_TUNIC_ZORA);
    }

    if (settings.sharedScales) {
      this.replaceItem(Items.OOT_SCALE, Items.SHARED_SCALE);
    } else if (settings.scalesMm) {
      this.addItem(Items.MM_SCALE, 2);
    }

    if (settings.sharedStrength) {
      this.replaceItem(Items.OOT_STRENGTH, Items.SHARED_STRENGTH);
    } else if (settings.strengthMm) {
      this.addItem(Items.MM_STRENGTH, 3);
    }

    if (settings.sharedMaskBlast) {
      this.replaceItem(Items.MM_MASK_BLAST, Items.SHARED_MASK_BLAST);
    } else if (settings.blastMaskOot) {
      this.addItem(Items.OOT_MASK_BLAST);
    }

    if (settings.sharedMaskStone) {
      this.replaceItem(Items.MM_MASK_STONE, Items.SHARED_MASK_STONE);
    } else if (settings.stoneMaskOot) {
      this.addItem(Items.OOT_MASK_STONE);
    }

    if (settings.sharedSongElegy) {
      this.replaceItem(Items.MM_SONG_EMPTINESS, Items.SHARED_SONG_EMPTINESS);
    } else if (settings.elegyOot) {
      this.addItem(Items.OOT_SONG_EMPTINESS);
    }

    if (settings.sharedLens) {
      this.replaceItem(Items.OOT_LENS, Items.SHARED_LENS);
      this.replaceItem(Items.MM_LENS,  Items.SHARED_LENS);
      this.removeItem(Items.SHARED_LENS, 1);
    }

    if (settings.sharedOcarina) {
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

    if (settings.sharedWallets) {
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
      this.replaceItem(Items.OOT_TRAP_RUPOOR,   Items.SHARED_TRAP_RUPOOR);
      this.replaceItem(Items.MM_RUPEE_GREEN,    Items.SHARED_RUPEE_GREEN);
      this.replaceItem(Items.MM_RUPEE_BLUE,     Items.SHARED_RUPEE_BLUE);
      this.replaceItem(Items.MM_RUPEE_RED,      Items.SHARED_RUPEE_RED);
      this.replaceItem(Items.MM_RUPEE_PURPLE,   Items.SHARED_RUPEE_PURPLE);
      this.replaceItem(Items.MM_RUPEE_SILVER,   Items.SHARED_RUPEE_SILVER);
      this.replaceItem(Items.MM_RUPEE_GOLD,     Items.SHARED_RUPEE_GOLD);
      this.replaceItem(Items.MM_TRAP_RUPOOR,    Items.SHARED_TRAP_RUPOOR);
    }

    if (settings.sharedHealth) {
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

    if (settings.sharedHealth && settings.sharedMagic) {
      this.replaceItem(Items.OOT_FAIRY_BIG, Items.SHARED_FAIRY_BIG);
      this.replaceItem(Items.MM_FAIRY_BIG, Items.SHARED_FAIRY_BIG);
    }

    if (settings.sharedSoulsEnemy) {
      this.shareItems(SharedItemGroups.SOULS_ENEMY, 'max');
    }

    if (settings.sharedSkeletonKey) {
      this.shareItems(SharedItemGroups.SKELETON_KEY, 'max');
    }

    if (settings.sharedOcarinaButtons) {
      this.shareItems(SharedItemGroups.OCARINA_BUTTONS, 'max');
    }

    if (settings.sharedShields) {
      this.shareItems(SharedItemGroups.SHIELDS, 'max');
      this.replaceItem(Items.OOT_SHIELD_HYLIAN, Items.SHARED_SHIELD_HYLIAN);
      this.replaceItem(Items.MM_SHIELD_HERO,    Items.SHARED_SHIELD_HYLIAN);
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

  private randomizeGroup(group: Set<Item>) {
    for (let i = 0; i < this.state.worlds.length; ++i) {
      let count = 0;
      for (const item of group) {
        const pi = makePlayerItem(item, i);
        const amount = this.pool.get(pi) || 0;
        count += amount;

        this.removePlayerItem(pi);
      }

      for (let j = 0; j < count; ++j) {
        const item = sample(this.state.random, Array.from(group));
        const pi = makePlayerItem(item, i);
        this.addPlayerItem(pi);
      }
    }
  }

  private keyRing(worldId: number, key: Item, keyRing: Item) {
    const playerKey = makePlayerItem(key, worldId);
    const playerKeyRing = makePlayerItem(keyRing, worldId);
    const count = this.pool.get(playerKey) || 0;
    if (count) {
      this.removePlayerItem(playerKey);
      this.addPlayerItem(playerKeyRing);
    }
  }

  run() {
    const { settings } = this.state;
    this.state.monitor.log('Logic: World Transform');

    /* Broken actors */
    if (!settings.restoreBrokenActors) {
      this.removeLocations(BROKEN_ACTORS_CHECKS);
    }

    /* Pond */
    if (!settings.pondFishShuffle) {
      const locs = POOL.oot.filter((x: any) => x.type === 'fish').map((x: any) => gameId('oot', x.location, ' ')) as string[];
      this.removeLocations(locs);
    }

    /* Potsanity */
    if (!settings.shufflePotsOot) {
      const pots = POOL.oot.filter((x: any) => x.type === 'pot').map((x: any) => gameId('oot', x.location, ' ')) as string[];
      this.removeLocations(pots);
    } else {
      if (settings.goal === 'triforce' || settings.goal === 'triforce3') {
        const potsGanonTower = POOL.oot.filter((x: any) => x.type === 'pot' && x.scene === 'GANON_TOWER').map((x: any) => gameId('oot', x.location, ' ')) as string[];
        this.removeLocations(potsGanonTower);
      }
    }

    if (!settings.shufflePotsMm) {
      const pots = POOL.mm.filter((x: any) => x.type === 'pot').map((x: any) => gameId('mm', x.location, ' ')) as string[];
      this.removeLocations(pots);
    } else {
      if (settings.goal === 'triforce' || settings.goal === 'triforce3') {
        const potsMajora = POOL.mm.filter((x: any) => x.type === 'pot' && x.scene === 'LAIR_MAJORA').map((x: any) => gameId('mm', x.location, ' ')) as string[];
        this.removeLocations(potsMajora);
      }
    }

    /* Grasssanity */
    if (!settings.shuffleGrassOot) {
      const grass = POOL.oot.filter((x: any) => x.type === 'grass').map((x: any) => gameId('oot', x.location, ' ')) as string[];
      this.removeLocations(grass);
    }

    if (!settings.shuffleGrassMm) {
      const grass = POOL.mm.filter((x: any) => x.type === 'grass').map((x: any) => gameId('mm', x.location, ' ')) as string[];
      this.removeLocations(grass);
    }

    if (!settings.shuffleFreeRupeesOot) {
      const locs = POOL.oot.filter((x: any) => x.type === 'rupee').map((x: any) => gameId('oot', x.location, ' ')) as string[];
      this.removeLocations(locs);
    }

    if (!settings.shuffleFreeRupeesMm) {
      const locs = POOL.mm.filter((x: any) => x.type === 'rupee').map((x: any) => gameId('mm', x.location, ' ')) as string[];
      this.removeLocations(locs);
    }

    if (!settings.shuffleFreeHeartsOot) {
      const locs = POOL.oot.filter((x: any) => x.type === 'heart').map((x: any) => gameId('oot', x.location, ' ')) as string[];
      this.removeLocations(locs);
    }

    if (!settings.shuffleFreeHeartsMm) {
      const locs = POOL.mm.filter((x: any) => x.type === 'heart').map((x: any) => gameId('mm', x.location, ' ')) as string[];
      this.removeLocations(locs);
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

    /* Handle Diving Game Rupees */
    if (!settings.divingGameRupeeShuffle) {
      this.removeLocations([
        'OOT Zora Domain Diving Game Green Rupee',
        'OOT Zora Domain Diving Game Blue Rupee',
        'OOT Zora Domain Diving Game Red Rupee',
        'OOT Zora Domain Diving Game Purple Rupee',
        'OOT Zora Domain Diving Game Huge Rupee',
      ]);
    }

    /* Handle Fairy Fountains */
    if (!settings.fairyFountainFairyShuffleOot) {
      const fairies = POOL.oot.filter((x: any) => x.type === 'fairy').map((x: any) => gameId('oot', x.location, ' ')) as string[];
      this.removeLocations(fairies);
    }

    /* Handle Fairy Fountains */
    if (!settings.fairyFountainFairyShuffleMm) {
      const fairies = POOL.mm.filter((x: any) => x.type === 'fairy').map((x: any) => gameId('mm', x.location, ' ')) as string[];
      this.removeLocations(fairies);
    }

    /* Handle Fairy Spots */
    if (!settings.fairySpotShuffleOot) {
      const fairies = POOL.oot.filter((x: any) => x.type === 'fairy_spot').map((x: any) => gameId('oot', x.location, ' ')) as string[];
      this.removeLocations(fairies);
    }

    /* Make the basic item pool */
    this.makePools();

    /* Add extra items */
    for (const item of EXTRA_ITEMS) {
      this.addItem(item);
    }

    /* Add souls */
    if (settings.soulsEnemyOot) this.addItems(ItemGroups.OOT_SOULS_ENEMY);
    if (settings.soulsEnemyMm) this.addItems(ItemGroups.MM_SOULS_ENEMY);
    if (settings.soulsBossOot) this.addItems(ItemGroups.OOT_SOULS_BOSS);
    if (settings.soulsBossMm) this.addItems(ItemGroups.MM_SOULS_BOSS);
    if (settings.soulsNpcOot) this.addItems(ItemGroups.OOT_SOULS_NPC);

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

    /* Add clocks */
    if (this.state.settings.clocks) {
      if (this.state.settings.progressiveClocks === 'separate') {
        this.addItems(ItemGroups.CLOCKS);
      } else {
        this.addItem(Items.MM_CLOCK, 5);
      }
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

    /* Handle progressive shields */
    if (this.state.settings.progressiveShieldsOot === 'progressive') {
      this.replaceItem(Items.OOT_SHIELD_MIRROR, Items.OOT_SHIELD);
      this.addItem(Items.OOT_SHIELD, 2);
    }

    if (settings.progressiveShieldsMm === 'progressive') {
      this.replaceItem(Items.MM_SHIELD_MIRROR, Items.MM_SHIELD);
      this.addItem(Items.MM_SHIELD);
    }

    /* Setup extra traps */
    this.setupExtraTraps();

    /* Setup shared items */
    this.setupSharedItems();

    /* Handle non-MQ Fire */
    if (settings.smallKeyShuffleOot !== 'anywhere') {
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

    /* Handle alt items */
    for (let worldId = 0; worldId < this.state.worlds.length; ++worldId) {
      const world = this.state.worlds[worldId];
      const rf = world.resolvedFlags;

      const ootKeys = rf.smallKeyRingOot;
      if (ootKeys.has('Forest')) this.keyRing(worldId, Items.OOT_SMALL_KEY_FOREST, Items.OOT_KEY_RING_FOREST);
      if (ootKeys.has('Fire'))   this.keyRing(worldId, Items.OOT_SMALL_KEY_FIRE,   Items.OOT_KEY_RING_FIRE);
      if (ootKeys.has('Water'))  this.keyRing(worldId, Items.OOT_SMALL_KEY_WATER,  Items.OOT_KEY_RING_WATER);
      if (ootKeys.has('Spirit')) this.keyRing(worldId, Items.OOT_SMALL_KEY_SPIRIT, Items.OOT_KEY_RING_SPIRIT);
      if (ootKeys.has('Shadow')) this.keyRing(worldId, Items.OOT_SMALL_KEY_SHADOW, Items.OOT_KEY_RING_SHADOW);
      if (ootKeys.has('BotW'))   this.keyRing(worldId, Items.OOT_SMALL_KEY_BOTW,   Items.OOT_KEY_RING_BOTW);
      if (ootKeys.has('GTG'))    this.keyRing(worldId, Items.OOT_SMALL_KEY_GTG,    Items.OOT_KEY_RING_GTG);
      if (ootKeys.has('Ganon'))  this.keyRing(worldId, Items.OOT_SMALL_KEY_GANON,  Items.OOT_KEY_RING_GANON);

      if (ootKeys.has('GF') && settings.smallKeyShuffleHideout !== 'vanilla')
        this.keyRing(worldId, Items.OOT_SMALL_KEY_GF,     Items.OOT_KEY_RING_GF);

      if (ootKeys.has('TCG') && settings.smallKeyShuffleChestGame !== 'vanilla')
        this.keyRing(worldId, Items.OOT_SMALL_KEY_TCG,    Items.OOT_KEY_RING_TCG);

      const mm = rf.smallKeyRingMm;
      if (mm.has('WF')) this.keyRing(worldId, Items.MM_SMALL_KEY_WF, Items.MM_KEY_RING_WF);
      if (mm.has('SH')) this.keyRing(worldId, Items.MM_SMALL_KEY_SH, Items.MM_KEY_RING_SH);
      if (mm.has('GB')) this.keyRing(worldId, Items.MM_SMALL_KEY_GB, Items.MM_KEY_RING_GB);
      if (mm.has('ST')) this.keyRing(worldId, Items.MM_SMALL_KEY_ST, Items.MM_KEY_RING_ST);

      const srp = rf.silverRupeePouches;
      if (srp.has('DC')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_DC, Items.OOT_POUCH_SILVER_DC);
      if (srp.has('BotW')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_BOTW, Items.OOT_POUCH_SILVER_BOTW);
      if (srp.has('Spirit_Child')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_SPIRIT_CHILD, Items.OOT_POUCH_SILVER_SPIRIT_CHILD);
      if (srp.has('Spirit_Sun')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_SPIRIT_SUN, Items.OOT_POUCH_SILVER_SPIRIT_SUN);
      if (srp.has('Spirit_Boulders')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_SPIRIT_BOULDERS, Items.OOT_POUCH_SILVER_SPIRIT_BOULDERS);
      if (srp.has('Spirit_Lobby')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_SPIRIT_LOBBY, Items.OOT_POUCH_SILVER_SPIRIT_LOBBY);
      if (srp.has('Spirit_Adult')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_SPIRIT_ADULT, Items.OOT_POUCH_SILVER_SPIRIT_ADULT);
      if (srp.has('Shadow_Scythe')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_SHADOW_SCYTHE, Items.OOT_POUCH_SILVER_SHADOW_SCYTHE);
      if (srp.has('Shadow_Pit')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_SHADOW_PIT, Items.OOT_POUCH_SILVER_SHADOW_PIT);
      if (srp.has('Shadow_Spikes')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_SHADOW_SPIKES, Items.OOT_POUCH_SILVER_SHADOW_SPIKES);
      if (srp.has('Shadow_Blades')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_SHADOW_BLADES, Items.OOT_POUCH_SILVER_SHADOW_BLADES);
      if (srp.has('IC_Scythe')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_IC_SCYTHE, Items.OOT_POUCH_SILVER_IC_SCYTHE);
      if (srp.has('IC_Block')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_IC_BLOCK, Items.OOT_POUCH_SILVER_IC_BLOCK);
      if (srp.has('GTG_Slopes')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_GTG_SLOPES, Items.OOT_POUCH_SILVER_GTG_SLOPES);
      if (srp.has('GTG_Lava')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_GTG_LAVA, Items.OOT_POUCH_SILVER_GTG_LAVA);
      if (srp.has('GTG_Water')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_GTG_WATER, Items.OOT_POUCH_SILVER_GTG_WATER);
      if (srp.has('Ganon_Light')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_GANON_LIGHT, Items.OOT_POUCH_SILVER_GANON_LIGHT);
      if (srp.has('Ganon_Forest')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_GANON_FOREST, Items.OOT_POUCH_SILVER_GANON_FOREST);
      if (srp.has('Ganon_Fire')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_GANON_FIRE, Items.OOT_POUCH_SILVER_GANON_FIRE);
      if (srp.has('Ganon_Water')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_GANON_WATER, Items.OOT_POUCH_SILVER_GANON_WATER);
      if (srp.has('Ganon_Shadow')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_GANON_SHADOW, Items.OOT_POUCH_SILVER_GANON_SHADOW);
      if (srp.has('Ganon_Spirit')) this.keyRing(worldId, Items.OOT_RUPEE_SILVER_GANON_SPIRIT, Items.OOT_POUCH_SILVER_GANON_SPIRIT);
    }

    if (settings.zoraKing === 'open') {
      this.removeItem(Items.OOT_RUTO_LETTER);
      this.addItem(Items.OOT_BOTTLE_EMPTY);
    }

    /* Handle OoT swords */
    if (settings.startingAge === 'adult') {
      this.removeItem(Items.OOT_SWORD_MASTER);
    }
    if (settings.progressiveSwordsOot === 'progressive') {
      this.replaceItem(Items.OOT_SWORD_KOKIRI,    Items.OOT_SWORD);
      this.replaceItem(Items.OOT_SWORD_MASTER,    Items.OOT_SWORD);
      this.replaceItem(Items.OOT_SWORD_KNIFE,     Items.OOT_SWORD);
      this.replaceItem(Items.OOT_SWORD_BIGGORON,  Items.OOT_SWORD);
    } else if (settings.progressiveSwordsOot === 'goron') {
      this.replaceItem(Items.OOT_SWORD_KNIFE,     Items.OOT_SWORD_GORON);
      this.replaceItem(Items.OOT_SWORD_BIGGORON,  Items.OOT_SWORD_GORON);
    }

    /* Handle MM Swords */
    if (settings.progressiveGFS === 'progressive') {
      this.replaceItem(Items.MM_GREAT_FAIRY_SWORD, Items.MM_SWORD);
    }

    /* Handle MM Lullaby */
    if (settings.progressiveGoronLullaby === 'progressive') {
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

    /* Alter the item pools */
    switch (settings.itemPool) {
    case 'scarce':
      this.scarcifyPool(1);
      break;
    case 'minimal':
    case 'barren':
      this.scarcifyPool(2);
      break;
    case 'plentiful':
      this.plentifulPool();
      break;
    }

    /* handle random bottle contents */
    if (settings.bottleContentShuffle) {
      this.randomizeGroup(ItemGroups.BOTTLES_OOT);
      this.randomizeGroup(ItemGroups.BOTTLES_MM);
    }

    if (settings.pondFishShuffle) {
      const baseFishWeights = [38, 36, 41, 35, 39, 44, 40, 34, 54, 47, 42, 33, 57, 63, 71];
      const baseChildLoachWeights = [45, 43];
      const baseAdultLoachWeights = [45];
      for (let i = 0; i < this.state.worlds.length; ++i) {
        const addFish = (type: 'OOT_FISHING_POND_CHILD_FISH' | 'OOT_FISHING_POND_ADULT_FISH' | 'OOT_FISHING_POND_CHILD_LOACH' | 'OOT_FISHING_POND_ADULT_LOACH') => {
          const item = Items[type];
          const pi = makePlayerItem(item, i);
          const amount = this.pool.get(pi) || 0;

          this.removePlayerItem(pi);

          let baseWeights: number[];
          let rewardFish: number[];
          let isAdult = false;
          let isLoach = false;
          switch (type) {
            case 'OOT_FISHING_POND_CHILD_FISH':
              baseWeights = baseFishWeights;
              rewardFish = baseWeights.filter(x => x >= 59); // 58 * 0.73 = 42 would be converted to 6 pounds which converts to 39 weight
              break;
            case 'OOT_FISHING_POND_ADULT_FISH':
              baseWeights = baseFishWeights;
              rewardFish = baseWeights.filter(x => x >= 46); // 45 would be converted to 7 pounds which converts to 42 weight
              isAdult = true;
              break;
            case 'OOT_FISHING_POND_CHILD_LOACH':
              baseWeights = baseChildLoachWeights;
              rewardFish = baseWeights;
              isLoach = true;
              break;
            case 'OOT_FISHING_POND_ADULT_LOACH':
              baseWeights = baseAdultLoachWeights;
              rewardFish = baseWeights;
              isAdult = true;
              isLoach = true;
              break;
          }

          const numFishToAdd = amount - baseWeights.length;

          if (numFishToAdd > 0) {
            // increase amount of fish
            const fishToAdd: number[] = []
            for (let fishIndex = 0; fishIndex < numFishToAdd; fishIndex++) {
              fishToAdd.push(sample(this.state.random, rewardFish));
            }
            baseWeights = baseWeights.concat(fishToAdd);
          } else if (numFishToAdd < 0) {
            // reduce amount of fish
            baseWeights = baseWeights.slice(-numFishToAdd);
          }

          for (const baseWeight of baseWeights) {
            let weight = baseWeight;
            weight += randomInt(this.state.random, 5);
            if (weight >= 65 && randomInt(this.state.random, 20) === 0) {
              weight += randomInt(this.state.random, 8);
            }

            if (!isAdult) {
              weight *= 0.73;
            }

            if (isLoach) {
              weight *= 2;
            }

            const lbs = Math.floor((weight * weight * 0.0036) + 0.5);

            const itemKey = `${type}_${lbs}LBS` as ItemID; // maybe needs validation?
            const item = itemByID(itemKey);
            const pi = makePlayerItem(item, i);
            this.addPlayerItem(pi);
          }
        }

        addFish('OOT_FISHING_POND_CHILD_FISH');
        addFish('OOT_FISHING_POND_ADULT_FISH');
        addFish('OOT_FISHING_POND_CHILD_LOACH');
        addFish('OOT_FISHING_POND_ADULT_LOACH');
      }
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
      if (this.state.itemProperties.junk.has(pi.item) && ItemHelpers.isItemConsumable(pi.item)) {
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
