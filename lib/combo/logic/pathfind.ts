import { World } from './world';
import { Age, AGES, Items } from './state';

export type Reachable = {
  areas: {
    child: Set<string>;
    adult: Set<string>;
  },
  locations: Set<string>;
  events: Set<string>;
};

const reachableDefault = (): Reachable => ({
  areas: {
    child: new Set(['OOT SPAWN']),
    adult: new Set(['OOT SPAWN']),
  },
  locations: new Set(),
  events: new Set(),
});

const reachableDup = (reachable: Reachable): Reachable => ({
  areas: {
    child: new Set(reachable.areas.child),
    adult: new Set(reachable.areas.adult),
  },
  locations: new Set(reachable.locations),
  events: new Set(reachable.events),
});

const pathfindAreas = (world: World, items: Items, age: Age, reachable: Reachable) => {
  const newAreas = new Set<string>();
  const oldAreas = reachable.areas[age];
  for (const area of oldAreas) {
    const worldArea = world.areas[area];
    if (!worldArea) {
      throw new Error(`Unknown area: ${area}`);
    }
    const exits = worldArea.exits;
    for (const exit in exits) {
      if (oldAreas.has(exit) || newAreas.has(exit)) {
        continue;
      }
      const expr = exits[exit];
      if (expr({ items, age, events: reachable.events })) {
        newAreas.add(exit);
      }
    }
  }
  if (newAreas.size > 0) {
    newAreas.forEach(x => reachable.areas[age].add(x));
    return true;
  }
  return false;
};

const pathfindEvents = (world: World, items: Items, age: Age, reachable: Reachable) => {
  let changed = false;
  for (const area of reachable.areas[age]) {
    const events = world.areas[area].events;
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
  for (const area of reachable.areas[age]) {
    const locations = world.areas[area].locations;
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

  /* Reach all areas & events */
  for (;;) {
    let changed = false;
    for (const age of AGES) {
      changed ||= pathfindAreas(world, items, age, reachable);
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
