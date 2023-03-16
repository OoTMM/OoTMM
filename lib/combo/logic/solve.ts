import { Game, GAMES } from '../config';
import { Random, sample, shuffle } from '../random';
import { gameId } from '../util';
import { Pathfinder, PathfinderState } from './pathfind';
import { World } from './world';
import { LogicError, LogicSeedError } from './error';
import { Items, addItem, combinedItems, itemsArray, removeItem, ITEMS_REQUIRED, isDungeonReward, isGoldToken, isHouseToken, isKey, isStrayFairy, isSmallKey, isGanonBossKey, isRegularBossKey, isTownStrayFairy, isDungeonStrayFairy, isSong, isJunk, isMapCompass, isSmallKeyRegular, isSmallKeyHideout, isItemUnlimitedStarting } from './items';
import { Settings } from '../settings';
import { Monitor } from '../monitor';

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

export class LogicPassSolver {
  private items!: ItemPlacement;
  private pathfinder!: Pathfinder;
  private pathfinderState!: PathfinderState;
  private pools!: ItemPools;
  private junkLocations!: Set<string>;

  constructor(
    private readonly state: {
      fixedLocations: Set<string>,
      world: World,
      settings: Settings,
      random: Random,
      monitor: Monitor,
      attempts: number,
    }
  ) {
    this.pathfinder = new Pathfinder(this.state.world, this.state.settings);
    this.items = {};
    this.junkLocations = new Set<string>(this.state.settings.junkLocations);
    this.pools = this.makeItemPools();
    this.pathfinderState = this.pathfinder.run(null);
  }

  run() {
    this.state.monitor.log(`Logic: Solver (attempt ${this.state.attempts})`);
    const checksCount = Object.keys(this.state.world.checks).length;

    /* Fix vanilla items */
    this.fixItems();

    /* Place junk into junkLocations */
    this.placeJunkLocations();

    /* Place items fixed to default */
    this.fixTokens();
    this.fixFairies();

    /* Place songs */
    this.fixSongs();

    /* Place the required reward items */
    if (this.state.settings.dungeonRewardShuffle === 'dungeonBlueWarps') {
      this.fixRewards();
    }

    /* Handle dungeon items */
    for (const dungeon in this.state.world.dungeons) {
      this.fixDungeon(dungeon);
    }

    /* Place required items */
    for (;;) {
      /* Pathfind */
      this.pathfinderState = this.pathfinder.run(this.pathfinderState, { recursive: true, items: this.items });

      /* Stop cond */
      if (this.state.settings.logic === 'beatable') {
        if (this.pathfinderState.goal) {
          break;
        }
      }
      if (this.pathfinderState.locations.size === checksCount) {
        break;
      }

      /* We need to place a required item */
      this.randomAssumed(this.pools.required);
    }

    /* At this point we have a beatable game */
    this.fill();

    return { items: this.items };
  }

  private fixItems() {
    for (const loc in this.state.world.checks) {
      if (this.state.fixedLocations.has(loc)) {
        const item = this.state.world.checks[loc].item;
        this.place(loc, item);
      }
    }
  }

  private makeItemPools() {
    const pools: ItemPools = { required: {}, nice: {}, junk: {} };

    for (const location in this.state.world.checks) {
      const check = this.state.world.checks[location];
      const { item, type } = check;

      if (this.items[location] || this.state.fixedLocations.has(location)) {
        continue;
      }

      if (isDungeonReward(item) || isKey(item) || isStrayFairy(item) || ITEMS_REQUIRED.has(item) || type == 'shop') {
        addItem(pools.required, item);
      } else if (isJunk(item)) {
        addItem(pools.junk, item);
      } else {
        addItem(pools.nice, item);
      }
    }

    for (const item in this.state.settings.startingItems) {
      if (isItemUnlimitedStarting(item))
        continue;
      const count = this.state.settings.startingItems[item];
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
  }

  private goldTokenLocations() {
    const locations = new Set<string>();
    const setting = this.state.settings.goldSkulltulaTokens;
    const shuffleInDungeons = ['dungeons', 'all'].includes(setting);
    const shuffleInOverworld = ['overworld', 'all'].includes(setting);
    const skullLocations = Object.keys(this.state.world.checks).filter(x => isGoldToken(this.state.world.checks[x].item));
    const dungeonLocations = Object.values(this.state.world.dungeons).reduce((acc, x) => new Set([...acc, ...x]));

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
    for (const location in this.state.world.checks) {
      const { item } = this.state.world.checks[location];
      if (isHouseToken(item)) {
        locations.add(location);
      }
    }
    return locations;
  }

  private fixCrossTokens(gs: Set<string>, house: Set<string>) {
    if (this.state.settings.housesSkulltulaTokens !== 'cross') {
      return;
    }

    const locations = new Set([...gs, ...house]);
    const pool = shuffle(this.state.random, Array.from(locations).map(loc => this.state.world.checks[loc].item));
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
      if (!this.items[location]) {
        const item = this.state.world.checks[location].item;
        this.place(location, item);
        removeItemPools(this.pools, item);
      }
    }

    /* Fix the non-shuffled house tokens */
    if (this.state.settings.housesSkulltulaTokens !== 'all') {
      for (const location of houseLocations) {
        if (!this.items[location]) {
          const item = this.state.world.checks[location].item;
          this.place(location, item);
          removeItemPools(this.pools, item);
        }
      }
    }
  }

  private fixFairies() {
    for (const location in this.state.world.checks) {
      const check = this.state.world.checks[location];
      if (isTownStrayFairy(check.item) && this.state.settings.townFairyShuffle === 'vanilla') {
        this.place(location, check.item);
        removeItemPools(this.pools, check.item);
      } else if (isDungeonStrayFairy(check.item)) {
        if (check.type === 'sf') {
          if (this.state.settings.strayFairyShuffle !== 'anywhere' && this.state.settings.strayFairyShuffle !== 'ownDungeon') {
            this.place(location, check.item);
            removeItemPools(this.pools, check.item);
          }
        } else {
          if (this.state.settings.strayFairyShuffle === 'vanilla') {
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

    for (const location in this.state.world.checks) {
      const item = this.state.world.checks[location].item;
      if (isDungeonReward(item)) {
        rewards.push(item);
        locations.push(location);
      }
    }

    rewards = shuffle(this.state.random, rewards);
    for (let i = 0; i < rewards.length; i++) {
      this.place(locations[i], rewards[i]);
      removeItem(this.pools.required, rewards[i]);
    }
  }

  private fixDungeon(dungeon: string) {
    /* handle IST and ST */
    if (dungeon === 'IST') {
      return;
    }

    let locations = this.state.world.dungeons[dungeon];
    if (dungeon === 'ST') {
      locations = new Set([...locations, ...this.state.world.dungeons['IST']]);
    }

    const pool = combinedItems(this.pools.required, this.pools.nice);

    for (const game of GAMES) {
      for (const baseItem of ['SMALL_KEY', 'BOSS_KEY', 'STRAY_FAIRY', 'MAP', 'COMPASS']) {
        const item = gameId(game, baseItem + '_' + dungeon.toUpperCase(), '_');

        if (isSmallKeyHideout(item) && this.state.settings.smallKeyShuffleHideout === 'anywhere') {
          continue;
        } else if (isSmallKeyRegular(item) && this.state.settings.smallKeyShuffle === 'anywhere') {
          continue;
        } else if (isGanonBossKey(item) && this.state.settings.ganonBossKey === 'anywhere') {
          continue;
        } else if (isRegularBossKey(item) && this.state.settings.bossKeyShuffle === 'anywhere') {
          continue;
        } else if (isDungeonStrayFairy(item) && this.state.settings.strayFairyShuffle === 'anywhere') {
          continue;
        } else if (isMapCompass(item) && this.state.settings.mapCompassShuffle === 'anywhere') {
          continue;
        }

        while (pool[item]) {
          this.randomAssumed(pool, { restrictedLocations: locations, forcedItem: item });
          removeItemPools(this.pools, item);
        }
      }
    }
  }

  private fixSongs() {
    if (this.state.settings.songs === 'anywhere') {
      return;
    }

    const pool = combinedItems(this.pools.required, this.pools.nice);
    const songs = shuffle(this.state.random, itemsArray(pool).filter(x => isSong(x)));
    const locations = new Set([...this.state.world.songLocations].filter(x => !this.items[x]));

    if (songs.length > locations.size) {
      throw new Error(`Not enough song locations for ${songs.length} songs`);
    }

    if (songs.length < locations.size) {
      const count = locations.size - songs.length;
      const junk = shuffle(this.state.random, itemsArray(this.pools.junk));

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
    const junkArray = shuffle(this.state.random, itemsArray(this.pools.junk));
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

  private randomAssumed(pool: Items, opts?: { restrictedLocations?: Set<string>, forcedItem?: string }) {
    const options = opts || {};

    /* Select a random item from the required pool */
    let requiredItem: string | null = null
    if (options.forcedItem) {
      requiredItem = options.forcedItem;
    } else {
      const items = itemsArray(pool);
      if (items.length === 0) {
        const unreachableLocs = Object.keys(this.state.world.checks).filter(x => !this.pathfinderState.locations.has(x));
        throw new LogicError(`Unreachable locations: ${unreachableLocs.join(', ')}`);
      }
      requiredItem = sample(this.state.random, items);
    }

    /* Remove the selected item from the required pool */
    removeItem(pool, requiredItem);

    let unplacedLocs: string[] = [];
    if (this.state.settings.logic !== 'beatable') {
      /* Get all assumed reachable locations */
      const result = this.pathfinder.run(this.pathfinderState, { recursive: true, items: this.items, assumedItems: pool });

      /* Get all assumed reachable locations that have not been placed */
      unplacedLocs = Array.from(result.locations)
        .filter(location => !this.items[location]);

      if (options.restrictedLocations) {
        unplacedLocs = unplacedLocs.filter(x => options.restrictedLocations!.has(x));
      }

      /* If there is nowhere to place an item, raise an error */
      if (unplacedLocs.length === 0) {
        throw new LogicSeedError(`No reachable locations for item ${requiredItem}`);
      }

      /* Select a random location from the assumed reachable locations */
      const location = sample(this.state.random, unplacedLocs);

      /* Place the selected item at the selected location */
      this.place(location, requiredItem);
    } else {
      /* Get all remainig locations */
      if (options.restrictedLocations) {
        unplacedLocs = Array.from(options.restrictedLocations);
      } else {
        unplacedLocs = Object.keys(this.state.world.checks);
      }
      unplacedLocs = shuffle(this.state.random, unplacedLocs.filter(x => !this.items[x]));

      while (unplacedLocs.length) {
        const loc = unplacedLocs.pop()!;
        const result = this.pathfinder.run(this.pathfinderState, { recursive: true, stopAtGoal: true, items: { ...this.items, [loc]: requiredItem }, assumedItems: pool });
        if (result.goal) {
          this.place(loc, requiredItem);
          return;
        }
      }

      throw new LogicSeedError(`No reachable locations for item ${requiredItem}`);
    }
  }

  private fill() {
    const pool = poolsArray(this.pools);
    const shuffledPool = shuffle(this.state.random, pool);
    const locations = Object.keys(this.state.world.checks).filter(loc => !this.items[loc]);

    for (const item of shuffledPool) {
      const loc = locations.pop()!;
      this.place(loc, item);
    }

    if (locations.length > 0) {
      throw new Error('Item Count Error');
    }
  }

  private place(location: string, item: string) {
    if (this.state.world.checks[location] === undefined) {
      throw new Error('Invalid Location: ' + location);
    }
    if (!isJunk(item) && this.state.settings.junkLocations.includes(location)) {
      throw new Error(`Unable to place ${item} at ${location}.`)
    }
    this.items[location] = item;
  }
}
