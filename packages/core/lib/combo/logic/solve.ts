import { GAMES } from '../config';
import { Random, sample, shuffle } from '../random';
import { CountMap, countMapArray, countMapCombine, countMapRemove, gameId } from '../util';
import { Pathfinder, PathfinderState } from './pathfind';
import { World } from './world';
import { LogicError, LogicSeedError } from './error';
import { Settings } from '../settings';
import { Monitor } from '../monitor';
import { Location, isLocationRenewable, locationData, makeLocation } from './locations';
import { Item, ItemGroups, ItemHelpers, Items, ItemsCount, PlayerItem, PlayerItems, itemByID, makePlayerItem } from '../items';

export type ItemPlacement = Map<Location, PlayerItem>;

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
  required: PlayerItems,
  nice: PlayerItems,
  junk: PlayerItems,
};

const removeItemPools = (pools: ItemPools, item: PlayerItem) => {
  const keys = ['required', 'nice', 'junk'] as const;
  for (const key of keys) {
    const items = pools[key];
    if (items.has(item)) {
      countMapRemove(items, item);
      return;
    }
  }
};

export class LogicPassSolver {
  private locations!: Location[];
  private items!: ItemPlacement;
  private pathfinder!: Pathfinder;
  private pathfinderState!: PathfinderState;
  private pools!: ItemPools;
  private criticalRenewables!: Set<PlayerItem>;
  private junkDistribution!: PlayerItems;
  private placedCount!: number;

  constructor(
    private readonly state: {
      fixedLocations: Set<string>,
      world: World,
      settings: Settings,
      random: Random,
      monitor: Monitor,
      attempts: number,
      pool: ItemsCount;
      renewableJunks: ItemsCount;
    }
  ) {
    this.locations = this.makePlayerLocations(Object.keys(this.state.world.checks));
    this.pathfinder = new Pathfinder(this.state.world, this.state.settings);
    this.items = new Map;
    this.makeItemPools();
    this.pathfinderState = this.pathfinder.run(null);
    this.criticalRenewables = new Set<PlayerItem>();
  }

  run() {
    this.state.monitor.log(`Logic: Solver (attempt ${this.state.attempts})`);

    this.placedCount = 0;

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
      this.pathfinderState = this.pathfinder.run(this.pathfinderState, { inPlace: true, recursive: true, items: this.items });

      /* Stop cond */
      if (this.state.settings.logic === 'beatable') {
        if (this.pathfinderState.goal) {
          break;
        }
      }
      if (this.pathfinderState.locations.size === this.locations.length) {
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
      const plandoItemId = this.state.settings.plando.locations[loc];
      if (plandoItemId) {
        const plandoItem = itemByID(plandoItemId);
        for (let player = 0; player < this.state.settings.players; ++player) {
          const item = makePlayerItem(plandoItem, player);
          const l = makeLocation(loc, player);
          this.place(l, item);
          removeItemPools(this.pools, item);
        }
      }
    }
  }

  private fixItems() {
    for (const loc in this.state.world.checks) {
      if (this.state.fixedLocations.has(loc)) {
        for (let i = 0; i < this.state.settings.players; ++i) {
          const itemId = this.state.world.checks[loc].item;
          const item = makePlayerItem(itemId, i);
          const location = makeLocation(loc, i);
          this.place(location, item);
        }
      }
    }
  }

  private setPlayersItemPool(pool: PlayerItems, item: Item, amount: number) {
    for (let i = 0; i < this.state.settings.players; ++i) {
      const x = makePlayerItem(item, i);
      pool.set(x, amount);
    }
  }

  private removePlayersItemPools(pools: ItemPools, item: Item) {
    for (let i = 0; i < this.state.settings.players; ++i) {
      const x = makePlayerItem(item, i);
      removeItemPools(pools, x);
    }
  }

  private makeItemPools() {
    this.pools = { required: new Map, nice: new Map, junk: new Map };
    this.junkDistribution = new Map;

    /* Assign every item to its sub-pool */
    for (const [item, amount] of this.state.pool.entries()) {
      /*
       * Some items are both junk and important.
       * Right now it only concerns sticks.
       */
      const junk = ItemHelpers.isJunk(item);

      if (ItemHelpers.isDungeonReward(item) || ItemHelpers.isKey(item) || ItemHelpers.isStrayFairy(item) || ItemGroups.REQUIRED.has(item)) {
        if (junk && ItemHelpers.isItemConsumable(item)) {
          const renewableCount = this.state.renewableJunks.get(item) || 0;
          const junkCount = amount - renewableCount;

          if (renewableCount) {
            this.setPlayersItemPool(this.pools.required, item, renewableCount);
          }
          if (junkCount) {
            this.setPlayersItemPool(this.pools.junk, item, junkCount);
            this.setPlayersItemPool(this.junkDistribution, item, junkCount);
          }
        } else {
          this.setPlayersItemPool(this.pools.required, item, amount);
        }
      } else if (junk) {
        this.setPlayersItemPool(this.pools.junk, item, amount);
        this.setPlayersItemPool(this.junkDistribution, item, amount);
      } else {
        this.setPlayersItemPool(this.pools.nice, item, amount);
      }
    }

    /* Remove starting items */
    for (const itemId in this.state.settings.startingItems) {
      const item = itemByID(itemId);
      if (ItemHelpers.isItemUnlimitedStarting(item))
        continue;
      const count = this.state.settings.startingItems[item.id];
      for (let i = 0; i < count; ++i) {
        this.removePlayersItemPools(this.pools, item);
      }
    }
  }

  private goldTokenLocations() {
    const locations = new Set<string>();
    const setting = this.state.settings.goldSkulltulaTokens;
    const shuffleInDungeons = ['dungeons', 'all'].includes(setting);
    const shuffleInOverworld = ['overworld', 'all'].includes(setting);
    const skullLocations = Object.keys(this.state.world.checks).filter(x => ItemHelpers.isGoldToken(this.state.world.checks[x].item));
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
      const item = this.state.world.checks[location].item;
      if (ItemHelpers.isHouseToken(item)) {
        locations.add(location);
      }
    }
    return locations;
  }

  private fixCrossTokens(gs: Set<string>, house: Set<string>) {
    if (this.state.settings.housesSkulltulaTokens !== 'cross') {
      return;
    }

    for (let player = 0; player < this.state.settings.players; ++player) {
      const locations = new Set([...gs, ...house].map(x => makeLocation(x, player)));
      const pool = shuffle(this.state.random, Array.from(locations).map(loc => this.state.world.checks[locationData(loc).id].item, player));
      for (const location of locations) {
        const itemId = pool.pop();
        const item = makePlayerItem(itemId!, player);
        this.place(location, item!);
        removeItemPools(this.pools, item!);
      }
    }
  }

  private fixTokens() {
    const rawGsLocations = this.goldTokenLocations();
    const rawHouseLocations = this.houseTokenLocations();

    /* Fix the cross tokens */
    this.fixCrossTokens(rawGsLocations, rawHouseLocations);

    for (let player = 0; player < this.state.settings.players; ++player) {
      /* Fix the non-shuffled GS */
      for (const rawLocation of rawGsLocations) {
        const location = makeLocation(rawLocation, player);
        if (!this.items.has(location)) {
          const item = makePlayerItem(this.state.world.checks[rawLocation].item, player);
          this.place(location, item);
          removeItemPools(this.pools, item);
        }
      }

      /* Fix the non-shuffled house tokens */
      if (this.state.settings.housesSkulltulaTokens !== 'all') {
        for (const rawLocation of rawHouseLocations) {
          const location = makeLocation(rawLocation, player);
          if (!this.items.has(location)) {
            const item = makePlayerItem(this.state.world.checks[rawLocation].item, player);
            this.place(location, item);
            removeItemPools(this.pools, item);
          }
        }
      }
    }
  }

  private fixFairies() {
    for (let player = 0; player < this.state.settings.players; ++player) {
      for (const locationId in this.state.world.checks) {
        const location = makeLocation(locationId, player);
        const check = this.state.world.checks[locationId];
        const item = check.item;
        const checkItem = makePlayerItem(item, player);
        if (ItemHelpers.isTownStrayFairy(item) && this.state.settings.townFairyShuffle === 'vanilla') {
          this.place(location, checkItem);
          removeItemPools(this.pools, checkItem);
        } else if (ItemHelpers.isDungeonStrayFairy(item)) {
          if (check.type === 'sf') {
            if (this.state.settings.strayFairyShuffle !== 'anywhere' && this.state.settings.strayFairyShuffle !== 'ownDungeon') {
              this.place(location, checkItem);
              removeItemPools(this.pools, checkItem);
            }
          } else {
            if (this.state.settings.strayFairyShuffle === 'vanilla') {
              this.place(location, checkItem);
              removeItemPools(this.pools, checkItem);
            }
          }
        }
      }
    }
  }

  private fixDungeon(dungeon: string) {
    /* handle IST and ST */
    if (['IST', 'Tower'].includes(dungeon)) {
      return;
    }

    let locationIds = this.state.world.dungeons[dungeon];
    if (dungeon === 'ST') {
      locationIds = new Set([...locationIds, ...this.state.world.dungeons['IST']]);
    }
    if (dungeon === 'Ganon') {
      locationIds = new Set([...locationIds, ...this.state.world.dungeons['Tower']]);
    }

    const pool = countMapCombine(this.pools.required, this.pools.nice);

    for (let player = 0; player < this.state.settings.players; ++player) {
      for (const game of GAMES) {
        for (const baseItem of ['SMALL_KEY', 'BOSS_KEY', 'STRAY_FAIRY', 'MAP', 'COMPASS']) {
          const itemId = gameId(game, baseItem + '_' + dungeon.toUpperCase(), '_');
          if (!Object.hasOwn(Items, itemId))
            continue;
          const item = itemByID(itemId);
          const playerItem = makePlayerItem(item, player);
          const locations = new Set([...locationIds].map(x => makeLocation(x, player)));

          if (ItemHelpers.isSmallKeyHideout(item) && this.state.settings.smallKeyShuffleHideout === 'anywhere') {
            continue;
          } else if (ItemHelpers.isSmallKeyRegularOot(item) && this.state.settings.smallKeyShuffleOot === 'anywhere') {
            continue;
          } else if (ItemHelpers.isSmallKeyRegularMm(item) && this.state.settings.smallKeyShuffleMm === 'anywhere') {
            continue;
          } else if (ItemHelpers.isGanonBossKey(item) && this.state.settings.ganonBossKey === 'anywhere') {
            continue;
          } else if (ItemHelpers.isRegularBossKeyOot(item) && this.state.settings.bossKeyShuffleOot === 'anywhere') {
            continue;
          } else if (ItemHelpers.isRegularBossKeyMm(item) && this.state.settings.bossKeyShuffleMm === 'anywhere') {
            continue;
          } else if (ItemHelpers.isDungeonStrayFairy(item) && this.state.settings.strayFairyShuffle === 'anywhere') {
            continue;
          } else if (ItemHelpers.isMapCompass(item) && this.state.settings.mapCompassShuffle === 'anywhere') {
            continue;
          }

          while (pool.has(playerItem)) {
            this.randomAssumed(pool, { restrictedLocations: locations, forcedItem: playerItem });
            removeItemPools(this.pools, playerItem);
          }
        }
      }
    }
  }

  private placeSemiShuffled() {
    const assumedPool = countMapCombine(this.pools.required, this.pools.nice);
    const pool = countMapArray(assumedPool);
    let songLocations: string[] = [];
    let rewardsLocations: string[] = [];
    let items: PlayerItem[] = [];

    if (this.state.settings.songs === 'songLocations') {
      const songs = pool.filter(x => ItemHelpers.isSong(x.item));
      songLocations = [ ...this.state.world.songLocations ];
      items = [ ...items, ...songs ];
    }

    if (this.state.settings.dungeonRewardShuffle === 'dungeonBlueWarps') {
      const rewards = pool.filter(x => ItemHelpers.isDungeonReward(x.item));
      rewardsLocations = [ ...this.state.world.warpLocations ];
      items = [ ...items, ...rewards ];
    }

    const locations = this.makePlayerLocations([...songLocations, ...rewardsLocations]);

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

      /* Get the player */
      const player = item.player;

      /* Get available locations */
      let restrictedLocations: string[];
      if (ItemHelpers.isSong(item.item)) {
        restrictedLocations = songLocations;
      } else {
        restrictedLocations = rewardsLocations;
      }

      const playerLocs = restrictedLocations.map(x => makeLocation(x, player));

      /* Place the item */
      this.randomAssumed(assumedPool, { restrictedLocations: new Set(playerLocs), forcedItem: item });
      removeItemPools(this.pools, item);
    }

    /* Fill extra locations with junk */
    this.fillJunk(locations);
  }

  private placeDungeonRewardsInDungeons() {
    const allDungeons: Set<string>[] = [];
    for (let i = 0; i < this.state.settings.players; ++i) {
      allDungeons.push(new Set([...REWARDS_DUNGEONS]));
    }

    const rewards = shuffle(this.state.random, countMapArray(this.pools.required)
      .filter(x => ItemHelpers.isDungeonReward(x.item)));

    for (const reward of rewards) {
      let candidates = allDungeons.flatMap((x, i) => [...x].map(y => ({ player: i, dungeon: y })));
      candidates = shuffle(this.state.random, candidates);
      const pool = { ...this.pools.required };
      let error: LogicSeedError | null = null;

      for (const c of candidates) {
        const { player, dungeon } = c;
        /* We have a reward and a dungeon - try to place it */
        let rawLocations = this.state.world.dungeons[dungeon];
        if (dungeon === 'ST') {
          rawLocations = new Set([...rawLocations, ...this.state.world.dungeons['IST']]);
        }
        const locations = new Set([...rawLocations].map(x => makeLocation(x, player)));
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
            allDungeons[player].delete(dungeon);
          }
          break;
        }
      }

      if (error) throw error;
    }
  }

  private makePlayerLocations(locs: string[]) {
    const result: Location[] = [];
    for (let i = 0; i < this.state.settings.players; ++i) {
      result.push(...locs.map(x => makeLocation(x, i)));
    }
    return result;
  }

  private placeJunkLocations() {
    const locs = this.makePlayerLocations(this.state.settings.junkLocations);
    this.fillJunk(locs);
  }

  private randomAssumed(pool: PlayerItems, opts?: { restrictedLocations?: Set<Location>, forcedItem?: PlayerItem }) {
    const options = opts || {};

    /* Select a random item from the required pool */
    let requiredItem: PlayerItem | null = null
    if (options.forcedItem) {
      requiredItem = options.forcedItem;
    } else {
      const items = countMapArray(pool);
      if (items.length === 0) {
        const unreachableLocs = this.locations.filter(x => !this.pathfinderState.locations.has(x));
        throw new LogicError(`Unreachable locations: ${unreachableLocs.join(', ')}`);
      }
      requiredItem = sample(this.state.random, items);
    }

    /* Remove the selected item from the required pool */
    countMapRemove(pool, requiredItem);

    let unplacedLocs: Location[] = [];
    if (this.state.settings.logic !== 'beatable') {
      /* Get all assumed reachable locations */
      //const prevNow = microtime.nowDouble();
      const result = this.pathfinder.run(null, { recursive: true, items: this.items, assumedItems: pool });
      //console.log("NEG: " + (microtime.nowDouble() - prevNow));

      /* Get all assumed reachable locations that have not been placed */
      unplacedLocs = Array.from(result.locations)
        .filter(location => !this.items.has(location));

        if (options.restrictedLocations) {
          unplacedLocs = unplacedLocs.filter(x => options.restrictedLocations!.has(x));
        }

        /* If the item is a critical renewable and it's all locations, ensure it lands correctly */
        if (this.state.settings.logic === 'allLocations' && ItemHelpers.isItemCriticalRenewable(requiredItem.item) && !this.criticalRenewables.has(requiredItem)) {
          unplacedLocs = unplacedLocs.filter(x => isLocationRenewable(this.state.world, x));
        }

        /* If there is nowhere to place an item, raise an error */
        if (unplacedLocs.length === 0) {
          throw new LogicSeedError(`No reachable locations for item ${requiredItem.item.id}@${requiredItem.player}`);
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
        unplacedLocs = this.locations;
      }
      unplacedLocs = shuffle(this.state.random, unplacedLocs.filter(x => !this.items.has(x)));

      while (unplacedLocs.length) {
        const loc = unplacedLocs.pop()!;
        const newPlacement = new Map(this.items);
        newPlacement.set(loc, requiredItem);
        const result = this.pathfinder.run(null, { recursive: true, stopAtGoal: true, items: newPlacement, assumedItems: pool });
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
    let locs = this.locations.filter(x => !this.items.has(x));

    /* Fill using every pool */
    this.fill(locs, this.pools.required, true);
    this.fill(locs, this.pools.nice, true);
    this.fillJunk(locs);
  }

  private fillJunk(locs: Location[]) {
    /* Fill using the junk pool */
    this.fill(locs, this.pools.junk, false);

    /* Junk pool empty - fill with extra junk */
    locs = shuffle(this.state.random, locs.filter(loc => !this.items.has(loc)));
    const junkDistribution = countMapArray(this.junkDistribution);
    const junkDistributionRenewable = countMapArray(this.junkDistribution).filter(x => !ItemHelpers.isItemMajor(x.item));
    for (const loc of locs) {
      const junkPool = isLocationRenewable(this.state.world, loc) ? junkDistributionRenewable : junkDistribution;
      const item = sample(this.state.random, junkPool);
      this.place(loc, item);
    }
  }

  private fill(locs: Location[], pool: PlayerItems, required: boolean) {
    const items = shuffle(this.state.random, countMapArray(pool));
    const locations = shuffle(this.state.random, locs.filter(loc => !this.items.has(loc)));

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

  private place(location: Location, item: PlayerItem) {
    if (this.state.world.checks[locationData(location).id] === undefined) {
      throw new Error('Invalid Location: ' + location);
    }
    if (this.items.has(location)) {
      throw new Error('Location already placed: ' + location);
    }
    this.items.set(location, item);
    if (isLocationRenewable(this.state.world, location) && ItemHelpers.isItemCriticalRenewable(item.item)) {
      this.criticalRenewables.add(item);
    }

    this.placedCount++;
    this.state.monitor.setProgress(this.placedCount, this.locations.length);
  }
}
