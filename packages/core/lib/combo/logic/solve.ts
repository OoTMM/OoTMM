import { GAMES } from '../config';
import { Random, sample, shuffle } from '../random';
import { gameId } from '../util';
import { Pathfinder, PathfinderState } from './pathfind';
import { World } from './world';
import { LogicError, LogicSeedError } from './error';
import { Items, combinedItems, itemsArray, removeItem, ITEMS_REQUIRED, isDungeonReward, isGoldToken, isHouseToken, isKey, isStrayFairy, isGanonBossKey, isRegularBossKey, isTownStrayFairy, isDungeonStrayFairy, isSong, isJunk, isMapCompass, isSmallKeyRegular, isSmallKeyHideout, isItemUnlimitedStarting, isItemCriticalRenewable, isRupees, isItemConsumable, isItemMajor } from './items';
import { Settings } from '../settings';
import { Monitor } from '../monitor';
import { isLocationRenewable } from './locations';

export type ItemPlacement = {[k: string]: string};

const REWARDS_DUNGEONS = [
  'DT',
  'DC',
  'JJ',
  'Forest',
  'Fire',
  'Water',
  'Shadow',
  'Spirit',
  'WF',
  'SH',
  'GB',
  'ST',
  'BotW',
  'IC',
  'GTG',
  'SSH',
  'OSH',
  'PF',
  'BtW',
  'ACoI',
  'SS',
];

type ItemPools = {
  required: Items,
  nice: Items,
  junk: Items,
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
  private criticalRenewables!: Set<string>;
  private junkDistribution!: Items;

  constructor(
    private readonly state: {
      fixedLocations: Set<string>,
      world: World,
      settings: Settings,
      random: Random,
      monitor: Monitor,
      attempts: number,
      pool: Items;
      renewableJunks: Items;
    }
  ) {
    this.pathfinder = new Pathfinder(this.state.world, this.state.settings);
    this.items = {};
    this.makeItemPools();
    this.pathfinderState = this.pathfinder.run(null);
    this.criticalRenewables = new Set<string>();
  }

  run() {
    this.state.monitor.log(`Logic: Solver (attempt ${this.state.attempts})`);
    const checksCount = Object.keys(this.state.world.checks).length;

    /* Place plando items */
    this.placePlando();

    /* Place junk into junkLocations */
    this.placeJunkLocations();

    /* Fix vanilla items */
    this.fixItems();

    /* Place items fixed to default */
    this.fixTokens();
    this.fixFairies();

    /* Place dungeon rewards (when set to dungeon locs) */
    if (['dungeons', 'dungeonsLimited'].includes(this.state.settings.dungeonRewardShuffle)) {
      this.placeDungeonRewardsInDungeons();
    }

    /* Place semi-shuffled items */
    this.placeSemiShuffled();

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
    this.fillAll();

    return { items: this.items };
  }

  private placePlando() {
    for (const loc in this.state.settings.plando.locations) {
      const item = this.state.settings.plando.locations[loc];
      if (item) {
        this.place(loc, item);
        removeItemPools(this.pools, item);
      }
    }
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
    this.pools = { required: {}, nice: {}, junk: {} };
    this.junkDistribution = {};

    /* Assign every item to its sub-pool */
    for (const item in this.state.pool) {
      /*
       * Some items are both junk and important.
       * Right now it only concerns sticks.
       */
      const junk = isJunk(item);

      if (isDungeonReward(item) || isKey(item) || isStrayFairy(item) || ITEMS_REQUIRED.has(item)) {
        if (junk && isItemConsumable(item)) {
          const renewableCount = this.state.renewableJunks[item] || 0;
          const junkCount = this.state.pool[item] - renewableCount;

          if (renewableCount) {
            this.pools.required[item] = renewableCount;
          }
          if (junkCount) {
            this.pools.junk[item] = junkCount;
            this.junkDistribution[item] = junkCount;
          }
        } else {
          this.pools.required[item] = this.state.pool[item];
        }
      } else if (junk) {
        this.pools.junk[item] = this.state.pool[item];
        this.junkDistribution[item] = this.state.pool[item];
      } else {
        this.pools.nice[item] = this.state.pool[item];
      }
    }

    /* Remove starting items */
    for (const item in this.state.settings.startingItems) {
      if (isItemUnlimitedStarting(item))
        continue;
      const count = this.state.settings.startingItems[item];
      for (let i = 0; i < count; ++i) {
        removeItemPools(this.pools, item);
      }
    }
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

  private placeSemiShuffled() {
    const assumedPool = combinedItems(this.pools.required, this.pools.nice);
    const pool = itemsArray(assumedPool);
    let songLocations: string[] = [];
    let rewardsLocations: string[] = [];
    let items: string[] = [];

    if (this.state.settings.songs === 'songLocations') {
      const songs = pool.filter(x => isSong(x));
      songLocations = [ ...this.state.world.songLocations ];
      items = [ ...items, ...songs ];
    }

    if (this.state.settings.dungeonRewardShuffle === 'dungeonBlueWarps') {
      const rewards = pool.filter(x => isDungeonReward(x));
      rewardsLocations = [ ...this.state.world.warpLocations ];
      items = [ ...items, ...rewards ];
    }

    const locations = [...songLocations, ...rewardsLocations];

    if (items.length > locations.length) {
      throw new Error(`Not enough locations for songs/dungeon rewards`);
    }

    /* Place items */
    items = shuffle(this.state.random, items);
    for (;;) {
      /* Get an item */
      const item = items.pop();
      if (!item) {
        break;
      }

      /* Get available locations */
      let restrictedLocations: string[];
      if (isSong(item)) {
        restrictedLocations = songLocations;
      } else {
        restrictedLocations = rewardsLocations;
      }

      /* Place the item */
      this.randomAssumed(assumedPool, { restrictedLocations: new Set(restrictedLocations), forcedItem: item });
      removeItemPools(this.pools, item);
    }

    /* Fill extra locations with junk */
    this.fillJunk(locations);
  }

  private placeDungeonRewardsInDungeons() {
    const allDungeons = new Set([...REWARDS_DUNGEONS]);
    const rewards = shuffle(this.state.random, itemsArray(this.pools.required).filter(x => isDungeonReward(x)));

    for (const reward of rewards) {
      const pool = { ...this.pools.required };
      const dungeons = shuffle(this.state.random, [...allDungeons]);
      let error: LogicSeedError | null = null;

      for (const dungeon of dungeons) {
        /* We have a reward and a dungeon - try to place it */
        let locations = this.state.world.dungeons[dungeon];
        if (dungeon === 'ST') {
          locations = new Set([...locations, ...this.state.world.dungeons['IST']]);
        }
        error = null;
        try {
          this.randomAssumed(pool, { restrictedLocations: locations, forcedItem: reward });
        } catch (err) {
          if (err instanceof LogicSeedError) {
            error = err;
          } else {
            throw err;
          }
        }

        if (!error) {
          /* We placed the reward */
          removeItemPools(this.pools, reward);
          if (this.state.settings.dungeonRewardShuffle === 'dungeonsLimited') {
            allDungeons.delete(dungeon);
          }
          break;
        }
      }

      if (error) throw error;
    }
  }

  private placeJunkLocations() {
    const locs = [...this.state.settings.junkLocations];
    this.fillJunk(locs);
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

      /* If the item is a critical renewable and it's all locations, ensure it lands correctly */
      if (this.state.settings.logic === 'allLocations' && isItemCriticalRenewable(requiredItem) && !this.criticalRenewables.has(requiredItem)) {
        unplacedLocs = unplacedLocs.filter(x => isLocationRenewable(this.state.world, x));
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

  private fillAll() {
    /* Get all unplaced locs */
    let locs = Object.keys(this.state.world.checks).filter(loc => !this.items[loc]);

    /* Fill using every pool */
    this.fill(locs, this.pools.required, true);
    this.fill(locs, this.pools.nice, true);
    this.fillJunk(locs);
  }

  private fillJunk(locs: string[]) {
    /* Fill using the junk pool */
    this.fill(locs, this.pools.junk, false);

    /* Junk pool empty - fill with extra junk */
    locs = shuffle(this.state.random, locs.filter(loc => !this.items[loc]));
    const junkDistribution = itemsArray(this.junkDistribution);
    const junkDistributionRenewable = itemsArray(this.junkDistribution).filter(x => !isItemMajor(x));
    for (const loc of locs) {
      const junkPool = isLocationRenewable(this.state.world, loc) ? junkDistributionRenewable : junkDistribution;
      const item = sample(this.state.random, junkPool);
      this.place(loc, item);
    }
  }

  private fill(locs: string[], pool: Items, required: boolean) {
    const items = shuffle(this.state.random, itemsArray(pool));
    const locations = shuffle(this.state.random, locs.filter(loc => !this.items[loc]));

    for (const item of items) {
      if (locations.length === 0) {
        if (required) {
          throw new Error('Too many items');
        }
        break;
      }
      const loc = locations.pop()!;
      this.place(loc, item);
    }
  }

  private place(location: string, item: string) {
    if (this.state.world.checks[location] === undefined) {
      throw new Error('Invalid Location: ' + location);
    }
    if (this.items[location]) {
      throw new Error('Location already placed: ' + location);
    }
    this.items[location] = item;
    if (isLocationRenewable(this.state.world, location) && isItemCriticalRenewable(item)) {
      this.criticalRenewables.add(item);
    }
  }
}
