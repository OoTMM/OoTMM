import { GAMES } from '../config';
import { Random, sample, shuffle } from '../random';
import { gameId } from '../util';
import { pathfind, Reachable } from './pathfind';
import { Items } from './state';
import { World } from './world';
import { LogicSeedError } from './error';
import { Options } from '../options';
import { addItem, combinedItems, itemsArray, removeItem, ITEMS_REQUIRED, isDungeonReward, isGoldToken, isHouseToken, isKey, isStrayFairy, isSmallKey, isGanonBossKey, isRegularBossKey, isTownStrayFairy, isDungeonStrayFairy, isSong, isJunk, isMapCompass } from './items';

export const EXTRA_ITEMS = [
  'OOT_MASK_SKULL',
  'OOT_MASK_SPOOKY',
  'OOT_MASK_KEATON',
  'OOT_MASK_BUNNY',
  'OOT_MASK_TRUTH',
  'OOT_MASK_GORON',
  'OOT_MASK_ZORA',
  'OOT_MASK_GERUDO',
  'MM_MASK_DEKU',
  'MM_SWORD',
];

export type ItemPlacement = {[k: string]: string};

type ItemPools = {
  required: Items,
  nice: Items,
  junk: Items,
};

const poolsArray = (pools: ItemPools) => {
  return [
    ...itemsArray(pools.required),
    ...itemsArray(pools.nice),
    ...itemsArray(pools.junk),
  ];
};

const removeItemPools = (pools: ItemPools, item: string) => {
  const keys = ['required', 'nice', 'junk'] as const;
  for (const key of keys) {
    const items = pools[key];
    if (items[item]) {
      removeItem(items, item);
      return;
    }
  }
};

const maxRequired = (pools: ItemPools, item: string, count: number) => {
  if ((pools.required[item] || 0) <= count) {
    return;
  }
  const extra = pools.required[item] - count;
  pools.required[item] = count;
  pools.nice[item] = extra;
};

class Solver {
  private placement: ItemPlacement = {};
  private items: Items;
  private reachable: Reachable;
  private pools: ItemPools;
  private reachedLocations = new Set<string>();
  private fixedLocations = new Set<string>();
  private junkLocations = new Set<string>();

  constructor(
    private opts: Options,
    private world: World,
    private random: Random,
  ) {
    this.items = { ...opts.settings.startingItems };
    this.junkLocations = new Set<string>(opts.settings.junkLocations);
    this.pools = this.makeItemPools();
    if (this.opts.settings.noLogic) {
      const allLocations = new Set<string>(Object.keys(this.world.checks));
      this.reachable = { events: new Set<string>, areas: { child: new Set<string>(), adult: new Set<string>() }, locations: allLocations, gossip: new Set<string>() };
    } else {
      this.reachable = pathfind(this.world, {}, false);
    }
    this.propagate();
  }

  solve() {
    const checksCount = Object.keys(this.world.checks).length;

    /* Place junk into junkLocations */
    this.placeJunkLocations();

    /* Place items fixed to default */
    this.fixTokens();
    this.fixFairies();
    this.fixLocations();

    /* Place the required reward items */
    if (this.opts.settings.dungeonRewardShuffle === 'dungeonBlueWarps') {
      this.fixRewards();
    }

    /* Place songs */
    this.fixSongs();

    /* Handle dungeon items */
    for (const dungeon in this.world.dungeons) {
      this.fixDungeon(dungeon);
    }

    /* Place required itemss */
    for (;;) {
      if (this.reachable.locations.size === checksCount) {
        break;
      }

      /* We need to place a required item */
      this.randomAssumed(this.pools.required);
    }

    /* At this point we have a beatable game */
    this.fill();

    return this.placement;
  }

  private fixLocations() {
    if (!this.opts.settings.shuffleGerudoCard) {
      const location = 'OOT Gerudo Member Card';
      const item = this.world.checks[location].item;
      this.place(location, item);
      removeItemPools(this.pools, item);
    }

    if (!this.opts.settings.shuffleMasterSword) {
      const location = 'OOT Temple of Time Master Sword';
      const item = this.world.checks[location].item;
      this.place(location, item);
      removeItemPools(this.pools, item);
    }

    if (this.opts.settings.ganonBossKey === 'vanilla') {
      const location = 'OOT Ganon Castle Boss Key';
      const item = this.world.checks[location].item;
      this.place(location, item);
      removeItemPools(this.pools, item);
    }
  }

  private makeItemPools() {
    const pools: ItemPools = { required: {}, nice: {}, junk: {} };

    for (const location in this.world.checks) {
      const check = this.world.checks[location];
      const { item } = check;

      if (this.placement[location]) {
        continue;
      }

      if (this.fixedLocations.has(location)) {
        this.place(location, item);
        continue;
      }

      if (isDungeonReward(item) || isKey(item) || isStrayFairy(item) || ITEMS_REQUIRED.has(item)) {
        addItem(pools.required, item);
      } else if (isJunk(item)) {
        addItem(pools.junk, item);
      } else {
        addItem(pools.nice, item);
      }
    }

    /* Add the extra items */
    for (const item of EXTRA_ITEMS) {
      this.insertItem(pools, item);
    }

    if (this.opts.settings.progressiveSwordsOot === 'progressive') {
      maxRequired(pools, 'OOT_SWORD', 2);
    }
    maxRequired(pools, 'OOT_WALLET', 1);
    maxRequired(pools, 'OOT_BOMB_BAG', 1);
    maxRequired(pools, 'OOT_BOW', 1);
    maxRequired(pools, 'OOT_SLINGSHOT', 1);
    maxRequired(pools, 'OOT_MAGIC_UPGRADE', 1);
    maxRequired(pools, 'OOT_OCARINA', 1);
    maxRequired(pools, 'OOT_BOMBCHU_10', 1);
    maxRequired(pools, 'OOT_GS_TOKEN', 50);

    maxRequired(pools, 'MM_BOMB_BAG', 1);
    maxRequired(pools, 'MM_BOW', 1);
    maxRequired(pools, 'MM_MAGIC_UPGRADE', 1);

    for (const item in this.opts.settings.startingItems) {
      const count = this.opts.settings.startingItems[item];
      for (let i = 0; i < count; ++i) {
        removeItemPools(pools, item);
        let junk = 'OOT_RUPEE_BLUE';
        if (item.startsWith('MM_')) {
          junk = 'MM_RUPEE_BLUE';
        }
        addItem(pools.junk, junk);
      }
    }

    return pools;
  };

  private insertItem(pools: ItemPools, item: string) {
    const junkItem = sample(this.random, itemsArray(pools.junk));
    removeItem(pools.junk, junkItem);
    addItem(pools.required, item);
  }

  private goldTokenLocations() {
    const locations = new Set<string>();
    const setting = this.opts.settings.goldSkulltulaTokens;
    const shuffleInDungeons = ['dungeons', 'all'].includes(setting);
    const shuffleInOverworld = ['overworld', 'all'].includes(setting);
    const skullLocations = Object.keys(this.world.checks).filter(x => isGoldToken(this.world.checks[x].item));
    const dungeonLocations = Object.values(this.world.dungeons).reduce((acc, x) => new Set([...acc, ...x]));

    for (const location of skullLocations) {
      const isDungeon = dungeonLocations.has(location);
      if (!((isDungeon && shuffleInDungeons) || (!isDungeon && shuffleInOverworld))) {
        locations.add(location);
      }
    }

    return locations;
  }

  private houseTokenLocations() {
    const locations = new Set<string>();
    for (const location in this.world.checks) {
      const { item } = this.world.checks[location];
      if (isHouseToken(item)) {
        locations.add(location);
      }
    }
    return locations;
  }

  private fixCrossTokens(gs: Set<string>, house: Set<string>) {
    if (this.opts.settings.housesSkulltulaTokens !== 'cross') {
      return;
    }

    const locations = new Set([...gs, ...house]);
    const pool = shuffle(this.random, Array.from(locations).map(loc => this.world.checks[loc].item));
    for (const location of locations) {
      const item = pool.pop();
      this.place(location, item!);
      removeItemPools(this.pools, item!);
    }
  }

  private fixTokens() {
    const gsLocations = this.goldTokenLocations();
    const houseLocations = this.houseTokenLocations();

    /* Fix the cross tokens */
    this.fixCrossTokens(gsLocations, houseLocations);

    /* Fix the non-shuffled GS */
    for (const location of gsLocations) {
      if (!this.placement[location]) {
        const item = this.world.checks[location].item;
        this.place(location, item);
        removeItemPools(this.pools, item);
      }
    }

    /* Fix the non-shuffled house tokens */
    if (this.opts.settings.housesSkulltulaTokens !== 'all') {
      for (const location of houseLocations) {
        if (!this.placement[location]) {
          const item = this.world.checks[location].item;
          this.place(location, item);
          removeItemPools(this.pools, item);
        }
      }
    }
  }

  private fixFairies() {
    for (const location in this.world.checks) {
      const check = this.world.checks[location];
      if (isTownStrayFairy(check.item) && this.opts.settings.townFairyShuffle === 'vanilla') {
        this.place(location, check.item);
        removeItemPools(this.pools, check.item);
      } else if (isDungeonStrayFairy(check.item)) {
        if (check.type === 'sf') {
          if (this.opts.settings.strayFairyShuffle !== 'anywhere' && this.opts.settings.strayFairyShuffle !== 'ownDungeon') {
            this.place(location, check.item);
            removeItemPools(this.pools, check.item);
          }
        } else {
          if (this.opts.settings.strayFairyShuffle === 'vanilla') {
            this.place(location, check.item);
            removeItemPools(this.pools, check.item);
          }
        }
      }
    }
  }

  private fixRewards() {
    let rewards: string[] = [];
    const locations: string[] = [];

    for (const location in this.world.checks) {
      const item = this.world.checks[location].item;
      if (isDungeonReward(item)) {
        rewards.push(item);
        locations.push(location);
      }
    }

    rewards = shuffle(this.random, rewards);
    for (let i = 0; i < rewards.length; i++) {
      this.place(locations[i], rewards[i]);
      removeItem(this.pools.required, rewards[i]);
    }
  }

  private fixDungeon(dungeon: string) {
    const pool = combinedItems(this.pools.required, this.pools.nice);

    for (const game of GAMES) {
      for (const baseItem of ['SMALL_KEY', 'BOSS_KEY', 'STRAY_FAIRY', 'MAP', 'COMPASS']) {
        const item = gameId(game, baseItem + '_' + dungeon.toUpperCase(), '_');

        if (isSmallKey(item) && this.opts.settings.smallKeyShuffle === 'anywhere') {
          continue;
        } else if (isGanonBossKey(item) && this.opts.settings.ganonBossKey === 'anywhere') {
          continue;
        } else if (isRegularBossKey(item) && this.opts.settings.bossKeyShuffle === 'anywhere') {
          continue;
        } else if (isDungeonStrayFairy(item) && this.opts.settings.strayFairyShuffle === 'anywhere') {
          continue;
        } else if (isMapCompass(item) && this.opts.settings.mapCompassShuffle === 'anywhere') {
          continue;
        }

        while (pool[item]) {
          this.randomAssumed(pool, { restrictedLocations: this.world.dungeons[dungeon], forcedItem: item });
          removeItemPools(this.pools, item);
        }
      }
    }
  }

  private fixSongs() {
    if (this.opts.settings.songs === 'anywhere') {
      return;
    }

    const pool = combinedItems(this.pools.required, this.pools.nice);
    const songs = shuffle(this.random, itemsArray(pool).filter(x => isSong(x)));
    const locations = new Set(Object.keys(this.world.checks).filter(x => isSong(this.world.checks[x].item)));

    if (songs.length > locations.size) {
      throw new Error(`Not enough song locations for ${songs.length} songs`);
    }

    if (songs.length < locations.size) {
      const count = locations.size - songs.length;
      const junk = shuffle(this.random, itemsArray(this.pools.junk));

      for (let i = 0; i < count; i++) {
        songs.push(junk.pop()!);
      }
    }

    for (let i = 0; i < songs.length; i++) {
      const song = songs[i];
      this.randomAssumed(pool, { restrictedLocations: locations, forcedItem: song });
      removeItemPools(this.pools, song);
    }
  }

  private placeJunkLocations() {
    const junkArray = shuffle(this.random, itemsArray(this.pools.junk));
    const junkLocations = [ ...this.junkLocations ];

    if (junkLocations.length > junkArray.length) {
      throw new Error(`Too many junk locations, max=${junkArray.length}`);
    }

    for (let i = 0; i < junkLocations.length; i++) {
      const junk = junkArray[i];
      this.place(junkLocations[i], junk);
      removeItemPools(this.pools, junk);
    }
  }

  private propagate() {
    for (;;) {
      this.reachable = pathfind(this.world, this.items, false, this.reachable);
      const changed = this.markAccessible();
      if (!changed) {
        break;
      }
    }
  }

  private randomAssumed(pool: Items, opts?: { restrictedLocations?: Set<string>, forcedItem?: string }) {
    const options = opts || {};

    /* Select a random item from the required pool */
    let requiredItem: string | null = null
    if (options.forcedItem) {
      requiredItem = options.forcedItem;
    } else {
      const items = itemsArray(pool);
      if (items.length === 0) {
        const unreachableLocs = Object.keys(this.world.checks).filter(x => !this.reachable.locations.has(x));
        throw new Error(`Unreachable locations: ${unreachableLocs.join(', ')}`);
      }
      requiredItem = sample(this.random, items);
    }

    /* Remove the selected item from the required pool */
    removeItem(pool, requiredItem);

    /* Get all assumed accessible items */
    const assumedAccessibleItems = combinedItems(this.items, pool);

    /* Get all assumed reachable locations */
    let reachable = this.reachable!;
    const reachableLocations = new Set(reachable.locations);
    for (;;) {
      let changed = false;
      reachable = pathfind(this.world, assumedAccessibleItems, false, reachable);
      for (const l of reachable.locations.values()) {
        if (!reachableLocations.has(l)) {
          changed = true;
          reachableLocations.add(l);
          if (this.placement[l]) {
            addItem(assumedAccessibleItems, this.placement[l]);
          }
        }
      }
      if (!changed) {
        break;
      }
    }
    const assumedReachable = reachable.locations;

    /* Get all assumed reachable locations that have not been placed */
    let unplacedLocs = Array.from(assumedReachable)
      .filter(location => !this.placement[location]);

    if (options.restrictedLocations) {
      unplacedLocs = unplacedLocs.filter(x => options.restrictedLocations!.has(x));
    }

    /* If there is nowhere to place an item, raise an error */
    if (unplacedLocs.length === 0) {
      throw new LogicSeedError(`No reachable locations for item ${requiredItem}`);
    }

    /* Select a random location from the assumed reachable locations */
    const location = sample(this.random, unplacedLocs);

    /* Place the selected item at the selected location */
    this.place(location, requiredItem);

    /* Propagate */
    this.propagate();
  }

  private fill() {
    const pool = poolsArray(this.pools);
    const shuffledPool = shuffle(this.random, pool);
    const locations = Object.keys(this.world.checks).filter(loc => !this.placement[loc]);

    for (const item of shuffledPool) {
      const loc = locations.pop()!;
      this.place(loc, item);
    }

    if (locations.length > 0) {
      throw new Error('Item Count Error');
    }
  }

  private markAccessible() {
    let changed = false;
    this.reachable!.locations.forEach(loc => {
      if (this.placement[loc] && !this.reachedLocations.has(loc)) {
        this.reachedLocations.add(loc);
        const item = this.placement[loc];
        addItem(this.items, item);
        changed = true;
      }
    });
    return changed;
  }

  private place(location: string, item: string) {
    if (this.world.checks[location] === undefined) {
      throw new Error('Invalid Location: ' + location);
    }
    if (!isJunk(item) && this.opts.settings.junkLocations.includes(location)) {
      throw new Error(`Unable to place ${item} at ${location}.`)
    }
    this.placement[location] = item;
  }
}

export const solve = (opts: Options, world: World, random: Random) => {
  const solver = new Solver(opts, world, random);
  return solver.solve();
}
