import { Random } from '../random';
import { pathfind, Reachable } from './pathfind';
import { Items } from './state';
import { World } from './world';

const ITEMS_DUNGEON = /^(MAP|COMPASS|SMALL_KEY|BOSS_KEY|STRAY_FAIRY)_[A-Z_]+$/;

const ITEMS_REQUIRED = new Set<string>([
  'SWORD',
  'CHICKEN',
  'OCARINA',
  'SLINGSHOT',
  'BOW',
]);
const ITEMS_NICE = new Set<string>([]);

const DUNGEON_ENTRANCES = {
  "DekuTree": "Deku tree Main",
};

const sample = <T>(random: Random, arr: T[]): T => {
  if (arr.length === 0) {
    throw new Error('Empty Array');
  }

  /* Biased towards low values, but that's fine */
  const index = random.next() % arr.length;
  return arr[index];
};

const shuffle = <T>(random: Random, arr: T[]): T[] => {
  const copy = [...arr];
  for (let i = 0; i < copy.length; i++) {
    const j = random.next() % copy.length;
    [copy[i], copy[j]] = [copy[j], copy[i]];
  }
  return copy;
};

type ItemPlacement = {[k: string]: string};

type ItemPools = {
  dungeon: Items,
  required: Items,
  nice: Items,
  junk: Items,
};

const itemsArray = (items: Items) => {
  const arr: string[] = [];
  for (const item in items) {
    for (let i = 0; i < items[item]; i++) {
      arr.push(item);
    }
  }
  return arr;
};

const poolsArray = (pools: ItemPools) => {
  return [
    ...itemsArray(pools.required),
    ...itemsArray(pools.nice),
    ...itemsArray(pools.junk),
  ];
};

const addItem = (items: Items, item: string) => {
  items[item] = (items[item] || 0) + 1;
};

const removeItem = (items: Items, item: string) => {
  items[item] -= 1;
  if (items[item] === 0) {
    delete items[item];
  }
};

const makeItemPools = (world: World) => {
  const pools: ItemPools = { dungeon: {}, required: {}, nice: {}, junk: {} };

  for (const item of world.pool) {
    if (ITEMS_DUNGEON.test(item)) {
      addItem(pools.dungeon, item);
    } else if (ITEMS_REQUIRED.has(item)) {
      addItem(pools.required, item);
    } else if (ITEMS_NICE.has(item)) {
      addItem(pools.nice, item);
    } else {
      addItem(pools.junk, item);
    }
  }
  return pools;
};

const combinedItems = (items: Items, other: Items) => {
  const combined: Items = {};
  for (const item in items) {
    combined[item] = items[item];
  }
  for (const item in other) {
    combined[item] = (combined[item] || 0) + other[item];
  }
  return combined;
};

class Solver {
  private placement: ItemPlacement = {};
  private items: Items = {};
  private reachable?: Reachable;
  private pools: ItemPools;
  private reachedLocations = new Set<string>();

  constructor(
    private world: World,
    private random: Random,
  ) {
    this.pools = makeItemPools(world);
  }

  solve() {
    const checksCount = Object.keys(this.world.checks).length;

    /* Handle dungeon items */
    for (const dungeon in this.world.dungeons) {
      this.fixDungeon(dungeon);
    }

    for (;;) {
      for (;;) {
        this.reachable = pathfind(this.world, this.items, this.reachable);
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

  private fixDungeon(dungeon: string) {
    /* TODO: Small keys */
    this.fixDungeonMapCompass(dungeon);
  }

  private fixDungeonMapCompass(dungeon: string) {
    const locations = shuffle(this.random, Array.from(this.world.dungeons[dungeon]));
    const items = Array.from(locations)
      .map(l => this.world.checks[l].item)
      .filter(x => /^(MAP|COMPASS)_/.test(x));
    for (let i = 0; i < items.length; i++) {
      this.placement[locations[i]] = items[i];
      removeItem(this.pools.dungeon, items[i]);
    }
  }

  private randomAssumed(pool: Items) {
    /* Select a random item from the required pool */
    const requiredItem = sample(this.random, itemsArray(pool));

    /* Remove the selected item from the required pool */
    removeItem(pool, requiredItem);

    /* Get all assumed accessible items */
    const assumedAccessibleItems = combinedItems(this.items, pool);

    /* Get all assumed reachable locations */
    const assumedReachable = pathfind(this.world, assumedAccessibleItems, this.reachable).locations;

    /* Get all assumed reachable locations that have not been placed */
    const unplacedLocs = Array.from(assumedReachable).filter(location => !this.placement[location]);

    /* Select a random location from the assumed reachable locations */
    const location = sample(this.random, unplacedLocs);

    /* Place the selected item at the selected location */
    this.placement[location] = requiredItem;
  }

  private fill() {
    const pool = poolsArray(this.pools);
    const shuffledPool = shuffle(this.random, pool);
    let i = 0;

    for (const location in this.world.checks) {
      if (this.placement[location]) {
        continue;
      }
      const item = shuffledPool[i++];
      this.placement[location] = item;
    }
    if (i !== shuffledPool.length) {
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
}

export const solve = (world: World, random: Random) => {
  const solver = new Solver(world, random);
  return solver.solve();
}
