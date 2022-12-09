import { World } from './world';
import { Age, AGES, Items } from './state';

export type Reachable = {
  regions: {
    child: Set<string>;
    adult: Set<string>;
  },
  locations: Set<string>;
  events: Set<string>;
};

const reachableDefault = (): Reachable => ({
  regions: {
    child: new Set(['OOT SPAWN']),
    adult: new Set(['OOT SPAWN']),
  },
  locations: new Set(),
  events: new Set(),
});

const reachableDup = (reachable: Reachable): Reachable => ({
  regions: {
    child: new Set(reachable.regions.child),
    adult: new Set(reachable.regions.adult),
  },
  locations: new Set(reachable.locations),
  events: new Set(reachable.events),
});

const pathfindRegions = (world: World, items: Items, age: Age, reachable: Reachable) => {
  const newRegions = new Set<string>();
  const oldRegions = reachable.regions[age];
  for (const region of oldRegions) {
    const worldRegion = world.regions[region];
    if (!worldRegion) {
      throw new Error(`Unknown region: ${region}`);
    }
    const exits = worldRegion.exits;
    for (const exit in exits) {
      if (oldRegions.has(exit) || newRegions.has(exit)) {
        continue;
      }
      const expr = exits[exit];
      if (expr({ items, age, events: reachable.events })) {
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

const pathfindEvents = (world: World, items: Items, age: Age, reachable: Reachable) => {
  let changed = false;
  for (const region of reachable.regions[age]) {
    const events = world.regions[region].events;
    for (const event in events) {
      if (reachable.events.has(event)) {
        continue;
      }
      const expr = events[event];
      if (expr({ items, age, events: reachable.events })) {
        reachable.events.add(event);
        changed = true;
      }
    }
  }
  return changed;
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
      if (expr({ items, age, events: reachable.events })) {
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
  } else {
    reachable = reachableDup(reachable);
  }

  /* Reach all regions & events */
  for (;;) {
    let changed = false;
    for (const age of AGES) {
      changed ||= pathfindRegions(world, items, age, reachable);
      changed ||= pathfindEvents(world, items, age, reachable);
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
