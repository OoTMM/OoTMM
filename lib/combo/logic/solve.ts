import { GAMES } from '../config';
import { Random, sample, shuffle } from '../random';
import { gameId } from '../util';
import { pathfind, Reachable } from './pathfind';
import { Items } from './state';
import { World } from './world';
import { LogicSeedError } from './error';
import { CONSTRAINTS, itemConstraint } from './constraints';
import { Options } from '../options';
import { addItem, combinedItems, itemsArray, removeItem, ITEMS_REQUIRED, isDungeonItem, isDungeonReward, isGoldToken, isHouseToken } from './items';

const ITEMS_JUNK = new Set<string>([
  'OOT_RUPEE_GREEN',
  'OOT_RUPEE_BLUE',
  'OOT_RUPEE_RED',
  'OOT_RECOVERY_HEART',
  'OOT_ARROWS_5',
  'OOT_ARROWS_10',
  'OOT_ARROWS_30',
  'MM_RUPEE_GREEN',
  'MM_RUPEE_BLUE',
  'MM_RUPEE_RED',
  'MM_RECOVERY_HEART',
]);

const EXTRA_ITEMS = [
  'OOT_MASK_SKULL',
  'OOT_MASK_SPOOKY',
  'OOT_MASK_KEATON',
  'OOT_MASK_BUNNY',
  'OOT_MASK_TRUTH',
  'OOT_MASK_GORON',
  'OOT_MASK_ZORA',
  'OOT_MASK_GERUDO',
  'MM_MASK_DEKU',
  'MM_SHIELD',
  'MM_SWORD',
];

export type ItemPlacement = {[k: string]: string};

type ItemPools = {
  dungeon: Items,
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
  const keys = ['dungeon', 'required', 'nice', 'junk'] as const;
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
  private items: Items = {};
  private reachable: Reachable;
  private pools: ItemPools;
  private reachedLocations = new Set<string>();
  private fixedLocations = new Set<string>();

  constructor(
    private opts: Options,
    private world: World,
    private random: Random,
  ) {
    this.fixGoldTokens();
    this.fixHouseTokens();
    this.fixFairies();
    this.fixLocations();
    this.pools = this.makeItemPools();
    if (this.opts.settings.noLogic) {
      const allLocations = new Set<string>(Object.keys(this.world.checks));
      this.reachable = { events: new Set<string>, areas: { child: new Set<string>(), adult: new Set<string>() }, locations: allLocations, gossip: new Set<string>() };
    } else {
      this.reachable = pathfind(this.world, {}, false);
    }
  }

  solve() {
    /* Fix the item pool */
    for (const item of EXTRA_ITEMS) {
      this.insertItem(item);
    }

    const checksCount = Object.keys(this.world.checks).length;

    /* Place the required reward items */
    this.fixRewards();

    /* Handle dungeon items */
    for (const dungeon in this.world.dungeons) {
      this.fixDungeon(dungeon);
    }

    for (;;) {
      for (;;) {
        this.reachable = pathfind(this.world, this.items, false, this.reachable);
        const changed = this.markAccessible();
        if (!changed) {
          break;
        }
      }

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
      this.fixedLocations.add('OOT Gerudo Member Card');
    }

    if (this.opts.settings.ganonBossKey === 'vanilla') {
      this.fixedLocations.add('OOT Ganon Castle Boss Key');
    }
  }

  private makeItemPools() {
    const pools: ItemPools = { dungeon: {}, required: {}, nice: {}, junk: {} };

    for (const location in this.world.checks) {
      const check = this.world.checks[location];
      const { item } = check;

      if (this.fixedLocations.has(location)) {
        this.place(location, item);
        continue;
      }

      if (isDungeonItem(item) || isDungeonReward(item)) {
        addItem(pools.dungeon, item);
      } else if (ITEMS_REQUIRED.has(item)) {
        addItem(pools.required, item);
      } else if (ITEMS_JUNK.has(item)) {
        addItem(pools.junk, item);
      } else {
        addItem(pools.nice, item);
      }
    }

    maxRequired(pools, 'OOT_SWORD', 2);
    maxRequired(pools, 'OOT_WALLET', 1);
    maxRequired(pools, 'OOT_BOMB_BAG', 1);
    maxRequired(pools, 'OOT_BOW', 1);
    maxRequired(pools, 'OOT_SLINGSHOT', 1);
    maxRequired(pools, 'OOT_MAGIC_UPGRADE', 1);
    maxRequired(pools, 'OOT_OCARINA', 1);
    maxRequired(pools, 'OOT_BOMBCHUS_10', 1);
    maxRequired(pools, 'OOT_GS_TOKEN', 50);

    maxRequired(pools, 'MM_BOMB_BAG', 1);
    maxRequired(pools, 'MM_BOW', 1);
    maxRequired(pools, 'MM_MAGIC_UPGRADE', 1);

    return pools;
  };

  private insertItem(item: string) {
    const junkItem = sample(this.random, itemsArray(this.pools.junk));
    removeItem(this.pools.junk, junkItem);
    addItem(this.pools.required, item);
  }

  private constraint(item: string) {
    return itemConstraint(item, this.opts.settings);
  }

  private fixGoldTokens() {
    const setting = this.opts.settings.goldSkulltulaTokens;
    const shuffleInDungeons = ['dungeons', 'all'].includes(setting);
    const shuffleInOverworld = ['overworld', 'all'].includes(setting);
    const skullLocations = Object.keys(this.world.checks).filter(x => isGoldToken(this.world.checks[x].item));
    const dungeonLocations = Object.values(this.world.dungeons).reduce((acc, x) => new Set([...acc, ...x]));

    for (const location of skullLocations) {
      const isDungeon = dungeonLocations.has(location);
      if (!((isDungeon && shuffleInDungeons) || (!isDungeon && shuffleInOverworld))) {
        this.fixedLocations.add(location);
      }
    }
  }

  private fixHouseTokens() {
    for (const location in this.world.checks) {
      const item = this.world.checks[location].item;
      if (isHouseToken(item)) {
        this.fixedLocations.add(location);
      }
    }
  }

  private fixFairies() {
    for (const location in this.world.checks) {
      const check = this.world.checks[location];
      if (check.type === 'sf') {
        this.fixedLocations.add(location);
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
      removeItem(this.pools.dungeon, rewards[i]);
    }
  }

  private fixDungeon(dungeon: string) {
    const pool = combinedItems(this.pools.dungeon, this.pools.required);

    for (const game of GAMES) {
      for (const baseItem of ['SMALL_KEY', 'BOSS_KEY', 'STRAY_FAIRY', 'MAP', 'COMPASS']) {
        const item = gameId(game, baseItem + '_' + dungeon.toUpperCase(), '_');
        while (pool[item]) {
          this.randomAssumed(pool, { restrictedLocations: this.world.dungeons[dungeon], forcedItem: item });
        }
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

    /* Get the constraint associated with the item */
    const constraint = this.constraint(requiredItem);

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
      .filter(location => this.world.checks[location].constraint === constraint)
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
  }

  private fill() {
    const pool = poolsArray(this.pools);
    const shuffledPool = shuffle(this.random, pool);
    const locations = Object.keys(this.world.checks).filter(loc => !this.placement[loc]);
    const locationsByConstraint = Object.fromEntries(CONSTRAINTS.map(c => [c, new Array<string>()]));

    /* Filter the locations */
    for (const loc of locations) {
      const constraint = this.world.checks[loc].constraint;
      locationsByConstraint[constraint].push(loc);
    }

    for (const item of shuffledPool) {
      const constraint = this.constraint(item);
      const loc = locationsByConstraint[constraint].pop()!;
      this.place(loc, item);
    }

    if (!Object.values(locationsByConstraint).every(x => x.length === 0)) {
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
    this.placement[location] = item;
  }
}

export const solve = (opts: Options, world: World, random: Random) => {
  const solver = new Solver(opts, world, random);
  return solver.solve();
}
