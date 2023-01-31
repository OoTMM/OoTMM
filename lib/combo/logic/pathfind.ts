import { World } from './world';
import { Age, AGES, Items } from './state';
import { Optional } from '../util';

/*
 * TODO: This whole module needs some serious refactoring.
 * Should probably be a class with some configurable options.
 * Should also probably have ItemPlacement access and perform item collection.
 */

export type Reachable = {
  areas: {
    child: Set<string>;
    adult: Set<string>;
  },
  locations: Set<string>;
  events: Set<string>;
  gossip: Set<string>;
};

const reachableDefault = (): Reachable => ({
  areas: {
    child: new Set(['OOT SPAWN']),
    adult: new Set(['OOT SPAWN']),
  },
  locations: new Set(),
  events: new Set(),
  gossip: new Set(),
});

const reachableDup = (reachable: Reachable): Reachable => ({
  areas: {
    child: new Set(reachable.areas.child),
    adult: new Set(reachable.areas.adult),
  },
  locations: new Set(reachable.locations),
  events: new Set(reachable.events),
  gossip: new Set(reachable.gossip),
});

export type EntranceOverrides = {[k: string]: {[k: string]: string | null}};
type PathfinderOptions = {
  entranceOverrides: EntranceOverrides;
  ignoreItems: boolean;
};

const pathfindAreas = (world: World, items: Items, age: Age, reachable: Reachable, opts: PathfinderOptions) => {
  const newAreas = new Set<string>();
  const oldAreas = reachable.areas[age];
  for (const area of oldAreas) {
    const worldArea = world.areas[area];
    if (!worldArea) {
      throw new Error(`Unknown area: ${area}`);
    }
    const exits = worldArea.exits;
    for (let exit in exits) {
      const expr = exits[exit];
      const overrides = opts.entranceOverrides[area] || {};
      const override = overrides[exit];
      if (override === null) {
        continue;
      }
      if (override !== undefined) {
        exit = override;
      }
      if (oldAreas.has(exit) || newAreas.has(exit)) {
        continue;
      }
      if (expr({ items, age, events: reachable.events, ignoreItems: opts.ignoreItems })) {
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

const pathfindEvents = (world: World, items: Items, age: Age, reachable: Reachable, opts: PathfinderOptions) => {
  let changed = false;
  for (const area of reachable.areas[age]) {
    const events = world.areas[area].events;
    for (const event in events) {
      if (reachable.events.has(event)) {
        continue;
      }
      const expr = events[event];
      if (expr({ items, age, events: reachable.events, ignoreItems: opts.ignoreItems })) {
        reachable.events.add(event);
        changed = true;
      }
    }
  }
  return changed;
};

const pathfindGossip = (world: World, items: Items, age: Age, reachable: Reachable, opts: PathfinderOptions) => {
  let changed = false;
  for (const area of reachable.areas[age]) {
    const gossips = world.areas[area].gossip;
    for (const gossip in gossips) {
      if (reachable.gossip.has(gossip)) {
        continue;
      }
      const expr = gossips[gossip];
      if (expr({ items, age, events: reachable.events, ignoreItems: opts.ignoreItems })) {
        reachable.gossip.add(gossip);
        changed = true;
      }
    }
  }
  return changed;
};

const pathfindLocations = (world: World, items: Items, age: Age, reachable: Reachable, opts: PathfinderOptions) => {
  const newLocations = new Set<string>();
  const oldLocations = reachable.locations;
  for (const area of reachable.areas[age]) {
    const locations = world.areas[area].locations;
    for (const location in locations) {
      if (oldLocations.has(location) || newLocations.has(location)) {
        continue;
      }
      const expr = locations[location];
      if (expr({ items, age, events: reachable.events, ignoreItems: opts.ignoreItems })) {
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

export const pathfind = (world: World, items: Items, gossip: boolean, reachable?: Reachable, opts?: Optional<PathfinderOptions>) => {
  const o: PathfinderOptions = { entranceOverrides: {}, ignoreItems: false, ...(opts || {}) };
  if (reachable === undefined) {
    reachable = reachableDefault();
  } else {
    reachable = reachableDup(reachable);
  }

  /* Reach all areas & events */
  for (;;) {
    let changed = false;
    for (const age of AGES) {
      changed ||= pathfindAreas(world, items, age, reachable, o);
      changed ||= pathfindEvents(world, items, age, reachable, o);
      if (gossip) {
        changed ||= pathfindGossip(world, items, age, reachable, o);
      }
    }
    if (!changed) {
      break;
    }
  }

  /* Reach all locations */
  for (;;) {
    let changed = false;
    for (const age of AGES) {
      changed ||= pathfindLocations(world, items, age, reachable, o);
    }
    if (!changed) {
      break;
    }
  }

  return reachable;
};
