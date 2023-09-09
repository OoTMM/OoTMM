import { Random, sample, shuffle } from '../random';
import { countMapArray, countMapCombine, countMapRemove } from '../util';
import { Pathfinder, PathfinderState } from './pathfind';
import { World } from './world';
import { LogicError, LogicSeedError } from './error';
import { Settings } from '../settings';
import { Monitor } from '../monitor';
import { Location, isLocationChestFairy, isLocationOtherFairy, isLocationRenewable, locationData, makeLocation } from './locations';
import { Item, ItemGroups, ItemHelpers, Items, ItemsCount, PlayerItem, PlayerItems, itemByID, makePlayerItem } from '../items';

const VALIDATION_CRITICAL_ITEMS = [
  Items.MM_SONG_TIME,
  Items.SHARED_SONG_TIME,
  Items.MM_OCARINA,
  Items.SHARED_OCARINA,
  Items.OOT_OCARINA,
  Items.OOT_SONG_TIME,
  Items.OOT_SWORD,
  Items.OOT_SWORD_MASTER,
  Items.MM_SONG_EPONA,
];

export type ItemPlacement = Map<Location, PlayerItem>;

const DUNGEON_ITEMS = {
  DT: [
    Items.OOT_MAP_DT,
    Items.OOT_COMPASS_DT
  ],
  DC: [
    Items.OOT_RUPEE_SILVER_DC,
    Items.OOT_MAP_DC,
    Items.OOT_COMPASS_DC
  ],
  JJ: [
    Items.OOT_MAP_JJ,
    Items.OOT_COMPASS_JJ
  ],
  Forest: [
    Items.OOT_SMALL_KEY_FOREST,
    Items.OOT_KEY_RING_FOREST,
    Items.OOT_BOSS_KEY_FOREST,
    Items.OOT_MAP_FOREST,
    Items.OOT_COMPASS_FOREST
  ],
  Fire: [
    Items.OOT_SMALL_KEY_FIRE,
    Items.OOT_KEY_RING_FIRE,
    Items.OOT_BOSS_KEY_FIRE,
    Items.OOT_MAP_FIRE,
    Items.OOT_COMPASS_FIRE
  ],
  Water: [
    Items.OOT_SMALL_KEY_WATER,
    Items.OOT_KEY_RING_WATER,
    Items.OOT_BOSS_KEY_WATER,
    Items.OOT_MAP_WATER,
    Items.OOT_COMPASS_WATER
  ],
  Shadow: [
    Items.OOT_SMALL_KEY_SHADOW,
    Items.OOT_KEY_RING_SHADOW,
    Items.OOT_RUPEE_SILVER_SHADOW_BLADES,
    Items.OOT_RUPEE_SILVER_SHADOW_PIT,
    Items.OOT_RUPEE_SILVER_SHADOW_SCYTHE,
    Items.OOT_RUPEE_SILVER_SHADOW_SPIKES,
    Items.OOT_BOSS_KEY_SHADOW,
    Items.OOT_MAP_SHADOW,
    Items.OOT_COMPASS_SHADOW
  ],
  Spirit: [
    Items.OOT_SMALL_KEY_SPIRIT,
    Items.OOT_KEY_RING_SPIRIT,
    Items.OOT_RUPEE_SILVER_SPIRIT_ADULT,
    Items.OOT_RUPEE_SILVER_SPIRIT_CHILD,
    Items.OOT_RUPEE_SILVER_SPIRIT_LOBBY,
    Items.OOT_RUPEE_SILVER_SPIRIT_SUN,
    Items.OOT_RUPEE_SILVER_SPIRIT_BOULDERS,
    Items.OOT_BOSS_KEY_SPIRIT,
    Items.OOT_MAP_SPIRIT,
    Items.OOT_COMPASS_SPIRIT
  ],
  BotW: [
    Items.OOT_SMALL_KEY_BOTW,
    Items.OOT_KEY_RING_BOTW,
    Items.OOT_RUPEE_SILVER_BOTW,
    Items.OOT_MAP_BOTW,
    Items.OOT_COMPASS_BOTW
  ],
  IC: [
    Items.OOT_RUPEE_SILVER_IC_BLOCK,
    Items.OOT_RUPEE_SILVER_IC_SCYTHE,
    Items.OOT_MAP_IC,
    Items.OOT_COMPASS_IC
  ],
  GF: [
    Items.OOT_SMALL_KEY_GF,
    Items.OOT_KEY_RING_GF
  ],
  GTG: [
    Items.OOT_SMALL_KEY_GTG,
    Items.OOT_KEY_RING_GTG,
    Items.OOT_RUPEE_SILVER_GTG_SLOPES,
    Items.OOT_RUPEE_SILVER_GTG_LAVA,
    Items.OOT_RUPEE_SILVER_GTG_WATER,
  ],
  Ganon: [
    Items.OOT_SMALL_KEY_GANON,
    Items.OOT_KEY_RING_GANON,
    Items.OOT_RUPEE_SILVER_GANON_LIGHT,
    Items.OOT_RUPEE_SILVER_GANON_FOREST,
    Items.OOT_RUPEE_SILVER_GANON_FIRE,
    Items.OOT_RUPEE_SILVER_GANON_WATER,
    Items.OOT_RUPEE_SILVER_GANON_SHADOW,
    Items.OOT_RUPEE_SILVER_GANON_SPIRIT,
    Items.OOT_BOSS_KEY_GANON
  ],
  WF: [
    Items.MM_SMALL_KEY_WF,
    Items.MM_KEY_RING_WF,
    Items.MM_BOSS_KEY_WF,
    Items.MM_STRAY_FAIRY_WF,
    Items.MM_MAP_WF,
    Items.MM_COMPASS_WF
  ],
  SH: [
    Items.MM_SMALL_KEY_SH,
    Items.MM_KEY_RING_SH,
    Items.MM_BOSS_KEY_SH,
    Items.MM_STRAY_FAIRY_SH,
    Items.MM_MAP_SH,
    Items.MM_COMPASS_SH
  ],
  GB: [
    Items.MM_SMALL_KEY_GB,
    Items.MM_KEY_RING_GB,
    Items.MM_BOSS_KEY_GB,
    Items.MM_STRAY_FAIRY_GB,
    Items.MM_MAP_GB,
    Items.MM_COMPASS_GB
  ],
  ST: [
    Items.MM_SMALL_KEY_ST,
    Items.MM_KEY_RING_ST,
    Items.MM_BOSS_KEY_ST,
    Items.MM_STRAY_FAIRY_ST,
    Items.MM_MAP_ST,
    Items.MM_COMPASS_ST
  ],
  SSH: [],
  OSH: [],
  PF: [],
  BtW: [],
  BtWE: [],
  ACoI: [],
  SS: [],
}

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

type SolverState = {
  items: ItemPlacement;
  pools: ItemPools;
  criticalRenewables: Set<PlayerItem>;
  placedCount: number;
};

const cloneState = (state: SolverState) => {
  return {
    items: new Map(state.items),
    pools: {
      required: new Map(state.pools.required),
      nice: new Map(state.pools.nice),
      junk: new Map(state.pools.junk),
    },
    criticalRenewables: new Set(state.criticalRenewables),
    placedCount: state.placedCount,
  };
};

export class LogicPassSolver {
  private monitor: Monitor;
  private pathfinder!: Pathfinder;
  private junkDistribution!: PlayerItems;
  private locations!: Location[];
  private state: SolverState;
  private pathfinderState: PathfinderState;
  private modeValidate = false;
  private attempts = 0;
  private attemptsMax = 100;

  constructor(
    private readonly input: {
      fixedLocations: Set<Location>;
      worlds: World[];
      settings: Settings;
      random: Random;
      monitor: Monitor;
      pool: PlayerItems;
      renewableJunks: PlayerItems;
      startingItems: ItemsCount;
    }
  ) {
    this.monitor = this.input.monitor;
    this.locations = this.input.worlds.map((x, i) => [...x.locations].map(l => makeLocation(l, i))).flat();
    this.pathfinder = new Pathfinder(this.input.worlds, this.input.settings, this.input.startingItems);
    this.state = {
      items: new Map,
      pools: { required: new Map, nice: new Map, junk: new Map },
      criticalRenewables: new Set,
      placedCount: 0,
    }
    this.pathfinderState = this.pathfinder.run(null);
    this.makeItemPools();
  }

  private retry(cb: () => void) {
    const stateBackup = cloneState(this.state);

    for (;;) {
      try {
        cb();
        return;
      } catch (e) {
        this.monitor.debug((e as Error).stack!);
        if ((e instanceof LogicError) && this.attempts < this.attemptsMax) {
          this.attempts++;
          this.state = cloneState(stateBackup);
          this.monitor.log(`Logic: Solver (attempt ${this.attempts + 1})`);
          continue;
        } else {
          throw e;
        }
      }
    }
  }

  validate() {
    this.modeValidate = true;
    this.monitor = new Monitor({ onLog: () => {} });
    this.attemptsMax = 10;
    return this.run();
  }

  run() {
    this.monitor.log(`Logic: Solver`);

    /* Place plando items */
    this.placePlando();

    /* Place junk into junkLocations */
    this.placeJunkLocations();

    /* Fix vanilla items */
    this.fixItems();

    /* Place items fixed to default */
    this.fixTokens();

    /* Place dungeon rewards (when set to dungeon locs) */
    if (['dungeons', 'dungeonsLimited'].includes(this.input.settings.dungeonRewardShuffle)) {
      this.retry(() => {
        this.placeDungeonRewardsInDungeons();
      });
    }

    /* Place semi-shuffled items */
    this.retry(() => {
      this.placeSemiShuffled();
    });

    /* Handle dungeon items */
    const allDungeons = new Set(this.input.worlds.map(x => Object.keys(x.dungeons)).flat());
    for (const dungeon of allDungeons.values()) {
      this.retry(() => {
        this.fixDungeon(dungeon);
      });
    }

    /* Place required items */
    this.retry(() => {
      this.pathfinderState = this.pathfinder.run(null);

      for (;;) {
        /* Pathfind */
        this.pathfinderState = this.pathfinder.run(this.pathfinderState, { inPlace: true, recursive: true, items: this.state.items });

        /* Stop cond */
        if (this.input.settings.logic === 'beatable') {
          if (this.pathfinderState.goal) {
            break;
          }
        }
        if (this.pathfinderState.locations.size === this.locations.length) {
          break;
        }

        /* We need to place a required item */
        this.randomPlace(this.state.pools.required);
      }
    });

    /* At this point we have a beatable game */
    if (!this.modeValidate) {
      this.fillAll();
    }

    return { items: this.state.items };
  }

  private placePlando() {
    for (const loc in this.input.settings.plando.locations) {
      const plandoItemId = this.input.settings.plando.locations[loc];
      if (plandoItemId) {
        const plandoItem = itemByID(plandoItemId);
        for (let player = 0; player < this.input.settings.players; ++player) {
          const item = makePlayerItem(plandoItem, player);
          const l = makeLocation(loc, player);
          this.place(l, item);
          removeItemPools(this.state.pools, item);
        }
      }
    }
  }

  private fixItems() {
    for (const loc of this.input.fixedLocations.values()) {
      const locD = locationData(loc);
      const item = makePlayerItem(this.input.worlds[locD.world as number].checks[locD.id].item, locD.world as number);
      this.place(loc, item);
    }
  }

  private removePlayersItemPools(pools: ItemPools, item: Item) {
    for (let i = 0; i < this.input.settings.players; ++i) {
      const x = makePlayerItem(item, i);
      removeItemPools(pools, x);
    }
  }

  private makeItemPools() {
    this.junkDistribution = new Map;

    /* Assign every item to its sub-pool */
    for (const [pi, amount] of this.input.pool.entries()) {
      /*
       * Some items are both junk and important.
       * Right now it only concerns sticks.
       */
      const junk = ItemHelpers.isJunk(pi.item);

      if (ItemHelpers.isItemMajor(pi.item)) {
        if (junk && ItemHelpers.isItemConsumable(pi.item)) {
          const renewableCount = this.input.renewableJunks.get(pi) || 0;
          const junkCount = amount - renewableCount;

          if (renewableCount) {
            this.state.pools.required.set(pi, renewableCount);
          }
          if (junkCount) {
            this.state.pools.junk.set(pi, junkCount);
            this.junkDistribution.set(pi, junkCount);
          }
        } else {
          this.state.pools.required.set(pi, amount);
        }
      } else if (junk) {
        this.state.pools.junk.set(pi, amount);
        this.junkDistribution.set(pi, amount);
      } else {
        this.state.pools.nice.set(pi, amount);
      }
    }

    /* Remove starting items */
    for (const [item, count] of this.input.startingItems.entries()) {
      if (ItemHelpers.isItemUnlimitedStarting(item))
        continue;
      for (let i = 0; i < count; ++i) {
        this.removePlayersItemPools(this.state.pools, item);
      }
    }
  }

  private goldTokenLocations() {
    const locations = new Set<Location>();
    const setting = this.input.settings.goldSkulltulaTokens;
    const shuffleInDungeons = ['dungeons', 'all'].includes(setting);
    const shuffleInOverworld = ['overworld', 'all'].includes(setting);
    for (let worldId = 0; worldId < this.input.worlds.length; ++worldId) {
      const world = this.input.worlds[worldId];
      const skullLocations = Object.keys(world.checks).filter(x => ItemHelpers.isGoldToken(world.checks[x].item));
      const dungeonLocations = Object.values(world.dungeons).reduce((acc, x) => new Set([...acc, ...x]));

      for (const location of skullLocations) {
        const isDungeon = dungeonLocations.has(location);
        if (!((isDungeon && shuffleInDungeons) || (!isDungeon && shuffleInOverworld))) {
          locations.add(makeLocation(location, worldId));
        }
      }
    }

    return locations;
  }

  private houseTokenLocations() {
    const locations = new Set<Location>();
    for (let worldId = 0; worldId < this.input.worlds.length; ++worldId) {
      const world = this.input.worlds[worldId];
      for (const location in world.checks) {
        const item = world.checks[location].item;
        if (ItemHelpers.isHouseToken(item)) {
          locations.add(makeLocation(location, worldId));
        }
      }
    }
    return locations;
  }

  private fixCrossTokens(gs: Set<Location>, house: Set<Location>) {
    if (this.input.settings.housesSkulltulaTokens !== 'cross') {
      return;
    }

    for (let player = 0; player < this.input.settings.players; ++player) {
      const locations = [...gs, ...house].filter(x => locationData(x).world === player);
      const world = this.input.worlds[player];
      const pool = shuffle(this.input.random, locations.map(loc => makePlayerItem(world.checks[locationData(loc).id].item, player)));
      for (const location of locations) {
        const item = pool.pop()!;
        this.place(location, item);
        removeItemPools(this.state.pools, item);
      }
    }
  }

  private fixTokens() {
    const gsLocations = this.goldTokenLocations();
    const houseLocations = this.houseTokenLocations();

    /* Fix the cross tokens */
    this.fixCrossTokens(gsLocations, houseLocations);

    /* Fix the non-shuffled GS */
    for (const location of gsLocations) {
      const locD = locationData(location);
      const world = this.input.worlds[locD.world as number];
      if (!this.state.items.has(location)) {
        const item = makePlayerItem(world.checks[locD.id].item, locD.world as number);
        this.place(location, item);
        removeItemPools(this.state.pools, item);
      }
    }

    /* Fix the non-shuffled house tokens */
    if (this.input.settings.housesSkulltulaTokens !== 'all') {
      for (const location of houseLocations) {
        const locD = locationData(location);
        const player = locD.world as number;
        const world = this.input.worlds[player];
        if (!this.state.items.has(location)) {
          const item = makePlayerItem(world.checks[locD.id].item, player);
          this.place(location, item);
          removeItemPools(this.state.pools, item);
        }
      }
    }
  }

  private fixDungeon(dungeon: string) {
    const { settings } = this.input;

    /* handle IST and ST */
    if (['IST', 'Tower'].includes(dungeon)) {
      return;
    }

    const pool = countMapCombine(this.state.pools.required, this.state.pools.nice);

    for (let player = 0; player < settings.players; ++player) {
      const world = this.input.worlds[player];
      let locationIds = world.dungeons[dungeon];
      if (dungeon === 'ST') {
        locationIds = new Set([...locationIds, ...world.dungeons['IST']]);
      }
      if (dungeon === 'Ganon') {
        locationIds = new Set([...locationIds, ...world.dungeons['Tower']]);
      }
      const items = DUNGEON_ITEMS[dungeon as keyof typeof DUNGEON_ITEMS];
      for (const item of items) {
        const playerItem = makePlayerItem(item, player);
        const locations = new Set([...locationIds].map(x => makeLocation(x, player)));

        if ((ItemHelpers.isSmallKeyHideout(item) || ItemHelpers.isKeyRingHideout(item)) && settings.smallKeyShuffleHideout === 'anywhere') {
          continue;
        } else if ((ItemHelpers.isSmallKeyRegularOot(item) || ItemHelpers.isKeyRingRegularOot(item)) && settings.smallKeyShuffleOot === 'anywhere') {
          continue;
        } else if ((ItemHelpers.isSmallKeyRegularMm(item) || ItemHelpers.isKeyRingRegularMm(item)) && settings.smallKeyShuffleMm === 'anywhere') {
          continue;
        } else if (ItemHelpers.isGanonBossKey(item) && settings.ganonBossKey === 'anywhere') {
          continue;
        } else if (ItemHelpers.isRegularBossKeyOot(item) && settings.bossKeyShuffleOot === 'anywhere') {
          continue;
        } else if (ItemHelpers.isRegularBossKeyMm(item) && settings.bossKeyShuffleMm === 'anywhere') {
          continue;
        } else if (ItemHelpers.isMapCompass(item) && settings.mapCompassShuffle === 'anywhere') {
          continue;
        } else if (ItemHelpers.isSilverRupee(item) && settings.silverRupeeShuffle === 'anywhere') {
          continue;
        }

        /* How much of each type of items do we need to place */
        let count = pool.get(playerItem) || 0;
        if (ItemHelpers.isDungeonStrayFairy(item)) {
          if (settings.strayFairyChestShuffle === 'anywhere') {
            for (const l of locations) {
              if (isLocationChestFairy(world, l)) {
                count--;
              }
            }
          }

          if (settings.strayFairyOtherShuffle === 'anywhere') {
            for (const l of locations) {
              if (isLocationOtherFairy(world, l)) {
                count--;
              }
            }
          }
        }
        const countRemaining = (pool.get(playerItem) || 0) - count;

        while (pool.get(playerItem)! > countRemaining) {
          this.randomAssumed(pool, { restrictedLocations: locations, forcedItem: playerItem });
          removeItemPools(this.state.pools, playerItem);
        }
      }
    }
  }

  private placeSemiShuffled() {
    const assumedPool = countMapCombine(this.state.pools.required, this.state.pools.nice);
    const pool = countMapArray(assumedPool);
    let songLocations: Location[] = [];
    let rewardsLocations: Location[] = [];
    let items: PlayerItem[] = [];

    if (this.input.settings.songs === 'songLocations') {
      const songs = pool.filter(x => ItemHelpers.isSong(x.item));
      songLocations = this.input.worlds.map((x, i) => [...x.songLocations].map(l => makeLocation(l, i))).flat();
      items = [ ...items, ...songs ];
    }

    if (this.input.settings.dungeonRewardShuffle === 'dungeonBlueWarps') {
      const rewards = pool.filter(x => ItemHelpers.isDungeonReward(x.item));
      rewardsLocations = this.input.worlds.map((x, i) => [...x.warpLocations].map(l => makeLocation(l, i))).flat();
      items = [ ...items, ...rewards ];
    }

    const locations = [...songLocations, ...rewardsLocations];

    if (items.length > locations.length) {
      throw new Error(`Not enough locations for songs/dungeon rewards`);
    }

    /* Place items */
    items = shuffle(this.input.random, items);
    for (;;) {
      /* Get an item */
      const item = items.pop();
      if (!item) {
        break;
      }

      /* Get the player */
      const player = item.player;

      /* Get available locations */
      let restrictedLocations: Location[];
      if (ItemHelpers.isSong(item.item)) {
        restrictedLocations = songLocations;
      } else {
        restrictedLocations = rewardsLocations;
      }

      const playerLocs = restrictedLocations.filter(x => locationData(x).world === player);

      /* Place the item */
      this.randomAssumed(assumedPool, { restrictedLocations: new Set(playerLocs), forcedItem: item });
      removeItemPools(this.state.pools, item);
    }

    /* Fill extra locations with junk */
    this.fillJunk(locations);
  }

  private placeDungeonRewardsInDungeons() {
    const allDungeons: Set<string>[] = [];
    for (let i = 0; i < this.input.settings.players; ++i) {
      allDungeons.push(new Set([...REWARDS_DUNGEONS]));
    }

    const rewards = shuffle(this.input.random, countMapArray(this.state.pools.required)
      .filter(x => ItemHelpers.isDungeonReward(x.item)));

    for (const reward of rewards) {
      let candidates = allDungeons.flatMap((x, i) => [...x].map(y => ({ player: i, dungeon: y })));
      candidates = shuffle(this.input.random, candidates);
      const pool = new Map(this.state.pools.required);
      let error: LogicSeedError | null = null;

      for (const c of candidates) {
        const { player, dungeon } = c;
        const world = this.input.worlds[player];
        /* We have a reward and a dungeon - try to place it */
        let rawLocations = world.dungeons[dungeon];
        if (dungeon === 'ST') {
          rawLocations = new Set([...rawLocations, ...world.dungeons['IST']]);
        }

        /* Exclude song locs in non-songsanity */
        if (this.input.settings.songs === 'songLocations') {
          rawLocations = new Set([...rawLocations].filter(x => !world.songLocations.has(x)));
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
          removeItemPools(this.state.pools, reward);
          if (this.input.settings.dungeonRewardShuffle === 'dungeonsLimited') {
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
    for (let i = 0; i < this.input.settings.players; ++i) {
      result.push(...locs.map(x => makeLocation(x, i)));
    }
    return result;
  }

  private placeJunkLocations() {
    const { settings } = this.input;
    let locs = this.makePlayerLocations(this.input.settings.junkLocations);
    if (!settings.shuffleMasterSword && settings.startingAge === 'adult') {
      locs = [...locs, ...this.makePlayerLocations(['OOT Temple of Time Master Sword'])];
    }
    this.fillJunk(locs);
  }

  private randomPlace(pool: PlayerItems) {
    if (this.modeValidate) {
      this.forwardFill(pool);
    } else {
      this.randomAssumed(pool);
    }
  }

  private forwardFill(pool: PlayerItems) {
    /* Only used for validation, needs to be fast */
    const items = countMapArray(pool);
    let unplacedLocs = [...this.pathfinderState.locations].filter(x => !this.state.items.has(x));

    if (items.length === 0) {
      const unreachableLocs = this.locations.filter(x => !this.pathfinderState.locations.has(x));
      throw new LogicError(`Unreachable locations: ${unreachableLocs.join(', ')}`);
    }

    let item: PlayerItem | null = null;
    for (const candidate of VALIDATION_CRITICAL_ITEMS) {
      for (let playerId = 0; playerId < this.input.settings.players; ++playerId) {
        const pi = makePlayerItem(candidate, 0);
        if (pool.has(pi)) {
          item = pi;
          break;
        }
      }
    }

    if (!item) {
      item = sample(this.input.random, items);
    }

    if (this.input.settings.logic === 'allLocations' && ItemHelpers.isItemCriticalRenewable(item.item) && !this.state.criticalRenewables.has(item)) {
      unplacedLocs = unplacedLocs.filter(x => isLocationRenewable(this.input.worlds[locationData(x).world as number], x));
    }

    if (unplacedLocs.length === 0) {
      throw new LogicSeedError(`No locations left to place items`);
    }

    const location = sample(this.input.random, unplacedLocs);
    this.place(location, item);
    countMapRemove(pool, item);
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
      requiredItem = sample(this.input.random, items);
    }

    /* Remove the selected item from the required pool */
    countMapRemove(pool, requiredItem);

    let unplacedLocs: Location[] = [];
    if (this.input.settings.logic !== 'beatable') {
      /* Get all assumed reachable locations */
      //const prevNow = microtime.nowDouble();
      const result = this.pathfinder.run(null, { recursive: true, items: this.state.items, assumedItems: pool });
      //console.log("NEG: " + (microtime.nowDouble() - prevNow));

      /* Get all assumed reachable locations that have not been placed */
      unplacedLocs = Array.from(result.locations)
        .filter(location => !this.state.items.has(location));

        if (options.restrictedLocations) {
          unplacedLocs = unplacedLocs.filter(x => options.restrictedLocations!.has(x));
        }

        /* If the item is a critical renewable and it's all locations, ensure it lands correctly */
        if (this.input.settings.logic === 'allLocations' && ItemHelpers.isItemCriticalRenewable(requiredItem.item) && !this.state.criticalRenewables.has(requiredItem)) {
          unplacedLocs = unplacedLocs.filter(x => isLocationRenewable(this.input.worlds[locationData(x).world as number], x));
        }

        /* If there is nowhere to place an item, raise an error */
        if (unplacedLocs.length === 0) {
          throw new LogicSeedError(`No reachable locations for item ${requiredItem.item.id}@${requiredItem.player}`);
        }

      /* Select a random location from the assumed reachable locations */
      const location = sample(this.input.random, unplacedLocs);

      /* Place the selected item at the selected location */
      this.place(location, requiredItem);
    } else {
      /* Get all remainig locations */
      if (options.restrictedLocations) {
        unplacedLocs = Array.from(options.restrictedLocations);
      } else {
        unplacedLocs = this.locations;
      }
      unplacedLocs = shuffle(this.input.random, unplacedLocs.filter(x => !this.state.items.has(x)));

      while (unplacedLocs.length) {
        const loc = unplacedLocs.pop()!;
        const newPlacement = new Map(this.state.items);
        newPlacement.set(loc, requiredItem);
        const result = this.pathfinder.run(null, { recursive: true, stopAtGoal: true, items: newPlacement, assumedItems: pool });
        if (result.goal) {
          this.place(loc, requiredItem);
          return;
        }
      }

      throw new LogicSeedError(`No reachable locations for item ${requiredItem.item.id}`);
    }
  }

  private fillAll() {
    /* Get all unplaced locs */
    let locs = this.locations.filter(x => !this.state.items.has(x));

    /* Fill using every pool */
    this.fill(locs, this.state.pools.required, true);
    this.fill(locs, this.state.pools.nice, true);
    this.fillJunk(locs);
  }

  private fillJunk(locs: Location[]) {
    /* Fill using the junk pool */
    this.fill(locs, this.state.pools.junk, false);

    /* Junk pool empty - fill with extra junk */
    locs = shuffle(this.input.random, locs.filter(loc => !this.state.items.has(loc)));
    const junkDistribution = countMapArray(this.junkDistribution);
    const junkDistributionRenewable = countMapArray(this.junkDistribution).filter(x => !ItemHelpers.isItemMajor(x.item));
    for (const loc of locs) {
      const locD = locationData(loc);
      const junkPool = isLocationRenewable(this.input.worlds[locD.world as number], loc) ? junkDistributionRenewable : junkDistribution;
      const item = sample(this.input.random, junkPool);
      this.place(loc, item);
    }
  }

  private fill(locs: Location[], pool: PlayerItems, required: boolean) {
    const items = shuffle(this.input.random, countMapArray(pool));
    const locations = shuffle(this.input.random, locs.filter(loc => !this.state.items.has(loc)));

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
    const locD = locationData(location);
    const world = this.input.worlds[locD.world as number];
    if (world.checks[locationData(location).id] === undefined) {
      throw new Error('Invalid Location: ' + location);
    }
    if (this.state.items.has(location)) {
      throw new Error('Location already placed: ' + location);
    }
    this.state.items.set(location, item);
    if (isLocationRenewable(world, location) && ItemHelpers.isItemCriticalRenewable(item.item)) {
      this.state.criticalRenewables.add(item);
    }

    this.state.placedCount++;
    this.monitor.setProgress(this.state.placedCount, this.locations.length);
  }
}
