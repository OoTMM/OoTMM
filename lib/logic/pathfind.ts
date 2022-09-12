import { World } from './world';
import { Age, AGES, Items } from './state';

type Reachable = {
  regions: {
    child: Set<string>;
    adult: Set<string>;
  },
  locations: Set<string>;
};

const reachableDefault = (): Reachable => ({
  regions: {
    child: new Set(['SPAWN']),
    adult: new Set(['SPAWN']),
  },
  locations: new Set(),
});

const pathfindRegions = (world: World, items: Items, age: Age, reachable: Reachable) => {
  const newRegions = new Set<string>();
  const oldRegions = reachable.regions[age];
  for (const region of oldRegions) {
    const exits = world.regions[region].exits;
    for (const exit in exits) {
      if (oldRegions.has(exit) || newRegions.has(exit)) {
        continue;
      }
      const expr = exits[exit];
      if (expr({ items, age })) {
        newRegions.add(exit);
      }
    }
  }
  if (newRegions.size > 0) {
    newRegions.forEach(x => reachable.regions[age].add(x));
    return true;
  }
  return false;
};

const pathfindLocations = (world: World, items: Items, age: Age, reachable: Reachable) => {
  const newLocations = new Set<string>();
  const oldLocations = reachable.locations;
  for (const region of reachable.regions[age]) {
    const locations = world.regions[region].locations;
    for (const location in locations) {
      if (oldLocations.has(location) || newLocations.has(location)) {
        continue;
      }
      const expr = locations[location];
      if (expr({ items, age })) {
        newLocations.add(location);
      }
    }
  }
  if (newLocations.size > 0) {
    newLocations.forEach(x => reachable.locations.add(x));
    return true;
  }
  return false;
};

export const pathfind = (world: World, items: Items, reachable?: Reachable) => {
  if (reachable === undefined) {
    reachable = reachableDefault();
  }

  /* Reach all regions */
  for (;;) {
    let changed = false;
    for (const age of AGES) {
      changed ||= pathfindRegions(world, items, age, reachable);
    }
    if (!changed) {
      break;
    }
  }

  /* Reach all locations */
  for (;;) {
    let changed = false;
    for (const age of AGES) {
      changed ||= pathfindLocations(world, items, age, reachable);
    }
    if (!changed) {
      break;
    }
  }

  return reachable;
};
