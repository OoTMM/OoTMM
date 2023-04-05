import { cloneDeep } from 'lodash';
import { Settings } from '../settings';
import { AreaData, Expr, exprDependencies, ExprResult, isDefaultRestrictions, MM_TIME_SLICES } from './expr';

import { addItem, combinedItems, isItemConsumable, Items } from './items';
import { ItemPlacement } from './solve';
import { World } from './world';
import { isLocationRenewable } from './locations';

export const AGES = ['child', 'adult'] as const;

export type Age = typeof AGES[number];

type PathfinderDependencyList = {
  locations: Set<string>;
  events: Set<string>;
  gossips: Set<string>;
  exits: {
    child: Set<string>;
    adult: Set<string>;
  };
};

type PathfinderDependencySet = Map<string, Map<string, PathfinderDependencyList>>;

type PathfinderDependencies = {
  items: PathfinderDependencySet;
  events: PathfinderDependencySet;
};

type PathfinderQueue = {
  locations: Map<string, Set<string>>;
  events: Map<string, Set<string>>;
  gossips: Map<string, Set<string>>;
  exits: {
    child: Map<string, Set<string>>;
    adult: Map<string, Set<string>>;
  }
};

export type PathfinderState = {
  items: Items;
  areas: {
    child: Map<string, AreaData>;
    adult: Map<string, AreaData>;
  },
  locations: Set<string>;
  newLocations: Set<string>;
  uncollectedLocations: Set<string>;
  events: Set<string>;
  gossip: Set<string>;
  goal: boolean;
  started: boolean;
  queue: PathfinderQueue;
  dependencies: PathfinderDependencies;
}

const emptyDepList = (): PathfinderDependencyList => ({
  locations: new Set(),
  events: new Set(),
  gossips: new Set(),
  exits: {
    child: new Set(),
    adult: new Set(),
  },
});

function filterStartingItems(items: Items) {
  const newItems = {...items};
  for (const item of Object.keys(items)) {
    if (isItemConsumable(item)) {
      delete newItems[item];
    }
  }
  return newItems;
}

const defaultState = (settings: Settings): PathfinderState => ({
  items: filterStartingItems(settings.startingItems),
  areas: {
    child: new Map(),
    adult: new Map(),
  },
  locations: new Set(),
  newLocations: new Set(),
  uncollectedLocations: new Set(),
  events: new Set(),
  gossip: new Set(),
  goal: false,
  started: false,
  queue: {
    locations: new Map(),
    events: new Map(),
    gossips: new Map(),
    exits: {
      child: new Map(),
      adult: new Map(),
    },
  },
  dependencies: {
    items: new Map(),
    events: new Map(),
  },
});

const defaultAreaData = (): AreaData => ({
  oot: {
    day: false,
    night: false,
  },
  mmTime: 0,
});

const mergeAreaData = (a: AreaData, b: AreaData): AreaData => ({
  oot: {
    day: a.oot.day || b.oot.day,
    night: a.oot.night || b.oot.night,
  },
  mmTime: a.mmTime | b.mmTime,
});

const compareAreaData = (a: AreaData, b: AreaData): boolean => (
  a.oot.day === b.oot.day &&
  a.oot.night === b.oot.night &&
  a.mmTime === b.mmTime
);

const cloneAreaData = (a: AreaData): AreaData => ({
  oot: {
    day: a.oot.day,
    night: a.oot.night,
  },
  mmTime: a.mmTime,
});

export type EntranceOverrides = {[k: string]: {[k: string]: string | null}};
type PathfinderOptions = {
  assumedItems?: Items;
  items?: ItemPlacement;
  entranceOverrides?: EntranceOverrides;
  ignoreItems?: boolean;
  recursive?: boolean;
  stopAtGoal?: boolean;
  restrictedLocations?: Set<string>;
  forbiddenLocations?: Set<string>;
  extraStartAreas?: Set<string>;
};

export class Pathfinder {
  private opts!: PathfinderOptions;
  private state!: PathfinderState;

  constructor(
    private readonly world: World,
    private readonly settings: Settings,
  ) {
  }

  run(state: PathfinderState | null, opts?: PathfinderOptions) {
    this.opts = opts || {};
    this.state = state ? cloneDeep(state) : defaultState(this.settings);
    this.state.items = combinedItems(this.state.items, this.opts.assumedItems || {});
    this.pathfind();
    return this.state;
  }

  private queueLocation(loc: string, areaFrom: string) {
    const queue = this.state.queue.locations;
    if (!queue.has(loc)) {
      queue.set(loc, new Set([areaFrom]));
    } else {
      queue.get(loc)!.add(areaFrom);
    }
  }

  private queueEvent(event: string, areaFrom: string) {
    const queue = this.state.queue.events;
    if (!queue.has(event)) {
      queue.set(event, new Set([areaFrom]));
    } else {
      queue.get(event)!.add(areaFrom);
    }
  }

  private queueExit(age: Age, exit: string, area: string) {
    const queue = this.state.queue.exits[age];
    if (!queue.has(exit)) {
      queue.set(exit, new Set([area]));
    } else {
      queue.get(exit)!.add(area);
    }
  }

  private queueGossip(gossip: string, area: string) {
    const queue = this.state.queue.gossips;
    if (!queue.has(gossip)) {
      queue.set(gossip, new Set([area]));
    } else {
      queue.get(gossip)!.add(area);
    }
  }

  /**
   * Explore an area, adding all locations and events to the queue
   */
  private exploreArea(age: Age, area: string, sourceAreaData: AreaData, fromArea: string) {
    /* Compute the previous area data and compare it to the old one */
    const previousAreaData = this.state.areas[age].get(area);
    const newAreaData = previousAreaData ? mergeAreaData(previousAreaData, sourceAreaData) : sourceAreaData;
    const worldArea = this.world.areas[area];
    if (!worldArea) {
      throw new Error(`Area ${area} not found`);
    }
    if (worldArea.game === 'oot') {
      if (['day', 'flow'].includes(worldArea.time)) {
        newAreaData.oot.day = true;
      }
      if (['night', 'flow'].includes(worldArea.time)) {
        newAreaData.oot.night = true;
      }
    } else {
      /* MM: Expand the time range */
      if (newAreaData.mmTime === 0) {
        return;
      }

      /* If we come from OoT, we can song of time to get back to day 1 */
      const fa = this.world.areas[fromArea];
      if (fa.game === 'oot') {
        newAreaData.mmTime |= (1 << 0);
      }

      /* We can wait to reach later time slices */
      const earliest = Math.floor(Math.log2(newAreaData.mmTime));
      for (let i = earliest; i < MM_TIME_SLICES.length; ++i) {
        newAreaData.mmTime |= (1 << i);
      }
    }

    if (previousAreaData && compareAreaData(previousAreaData, newAreaData)) {
      return;
    }
    this.state.areas[age].set(area, newAreaData);
    const a = this.world.areas[area];
    let locs = Object.keys(a.locations).filter(x => !this.state.locations.has(x));
    if (this.opts.restrictedLocations) {
      locs = locs.filter(x => this.opts.restrictedLocations!.has(x));
    }
    if (this.opts.forbiddenLocations) {
      locs = locs.filter(x => !this.opts.forbiddenLocations!.has(x));
    }
    locs.forEach(x => this.queueLocation(x, area));
    Object.keys(a.events).filter(x => !this.state.events.has(x)).forEach(x => this.queueEvent(x, area));
    const exits = Object.keys(a.exits).filter(x => !this.state.areas[age].has(x));
    exits.forEach(x => this.queueExit(age, x, area));
    Object.keys(a.gossip).forEach(x => this.queueGossip(x, area));
  }

  private evalExpr(expr: Expr, age: Age, area: string) {
    const areaData = this.state.areas[age].get(area)!;
    const result = expr({ areaData, items: this.state.items, age, events: this.state.events, ignoreItems: this.opts.ignoreItems || false });
    if (result.result) {
      if (!result.restrictions || isDefaultRestrictions(result.restrictions)) {
        delete result.dependencies;
      }
    }
    return result;
  }

  private dependenciesLookup(set: PathfinderDependencySet, dependency: string, areaFrom: string) {
    let data1 = set.get(dependency);
    if (!data1) {
      data1 = new Map();
      set.set(dependency, data1);
    }

    let data2 = data1.get(areaFrom);
    if (!data2) {
      data2 = emptyDepList();
      data1.set(areaFrom, data2);
    }

    return data2;
  }

  private addExitsDependencies(set: PathfinderDependencySet, age: Age, exit: string, areaFrom: string, dependents: Set<string>) {
    for (const dep of dependents) {
      const data = this.dependenciesLookup(set, dep, areaFrom);
      data.exits[age].add(exit);
    }
  }

  private addEventsDependencies(set: PathfinderDependencySet, event: string, areaFrom: string, dependents: Set<string>) {
    for (const dep of dependents) {
      const data = this.dependenciesLookup(set, dep, areaFrom);
      data.events.add(event);
    }
  }

  private addLocationDependencies(set: PathfinderDependencySet, location: string, areaFrom: string, dependents: Set<string>) {
    for (const dep of dependents) {
      const data = this.dependenciesLookup(set, dep, areaFrom);
      data.locations.add(location);
    }
  }

  private addGossipDependencies(set: PathfinderDependencySet, gossip: string, areaFrom: string, dependents: Set<string>) {
    for (const dep of dependents) {
      const data = this.dependenciesLookup(set, dep, areaFrom);
      data.gossips.add(gossip);
    }
  }

  private requeueItem(item: string) {
    const deps = this.state.dependencies.items.get(item);
    if (deps) {
      this.state.dependencies.items.delete(item);
      for (const [area, d] of deps) {
        d.exits.child.forEach(x => this.queueExit('child', x, area));
        d.exits.adult.forEach(x => this.queueExit('adult', x, area));
        d.events.forEach(x => this.queueEvent(x, area));
        d.locations.forEach(x => this.queueLocation(x, area));
        d.gossips.forEach(x => this.queueGossip(x, area));
      }
    }
  }

  private addLocation(loc: string) {
    this.state.locations.add(loc);
    this.state.newLocations.add(loc);
    const item = this.opts.items?.[loc];
    if (item) {
      this.state.uncollectedLocations.delete(loc);
      if (!isItemConsumable(item) || isLocationRenewable(this.world, loc)) {
        addItem(this.state.items, item);
        this.requeueItem(item);
      }
    } else {
      this.state.uncollectedLocations.add(loc);
    }
  }

  private evalExits(age: Age) {
    /* Extract the queue */
    const queue = this.state.queue.exits[age];
    this.state.queue.exits[age] = new Map();

    /* Evaluate all the exits */
    for (const [exit, areas] of queue) {
      for (const area of areas) {
        const a = this.world.areas[area];
        const expr = a.exits[exit];
        const exprResult = this.evalExpr(expr, age, area);

        /* Track dependencies */
        if (exprResult.dependencies) {
          const d = exprResult.dependencies;
          if (d.items) this.addExitsDependencies(this.state.dependencies.items, age, exit, area, d.items);
          if (d.events) this.addExitsDependencies(this.state.dependencies.events, age, exit, area, d.events);
        }

        if (exprResult.result) {
          const areaData = cloneAreaData(this.state.areas[age].get(area)!);
          const r = exprResult.restrictions;
          if (r) {
            if (r.oot.day) areaData.oot.day = false;
            if (r.oot.night) areaData.oot.night = false;
            if (r.mmTime) {
              areaData.mmTime = areaData.mmTime & ~(r.mmTime);
            }
          }
          this.exploreArea(age, exit, areaData, area);
        }
      }
    }
  }

  private evalEvents() {
    /* Extract the queue */
    const queue = this.state.queue.events;
    this.state.queue.events = new Map();

    /* Evaluate all the events */
    for (const [event, areas] of queue) {
      for (const area of areas) {
        if (this.state.events.has(event)) {
          continue;
        }

        /* Evaluate the event */
        const expr = this.world.areas[area].events[event];
        if (!expr) {
          throw new Error(`Event ${event} not found in area ${area}`);
        }
        const results: ExprResult[] = [];
        if (this.state.areas.child.has(area)) {
          results.push(this.evalExpr(expr, 'child', area));
        }
        if (this.state.areas.adult.has(area)) {
          results.push(this.evalExpr(expr, 'adult', area));
        }

        /* If any of the results are true, add the event to the state and queue up everything */
        /* Otherwise, track dependencies */
        if (results.some(x => x.result)) {
          this.state.events.add(event);
          const deps = this.state.dependencies.events.get(event);
          if (deps) {
            this.state.dependencies.events.delete(event);
            for (const [area, d] of deps) {
              d.exits.child.forEach(x => this.queueExit('child', x, area));
              d.exits.adult.forEach(x => this.queueExit('adult', x, area));
              d.events.forEach(x => this.queueEvent(x, area));
              d.locations.forEach(x => this.queueLocation(x, area));
              d.gossips.forEach(x => this.queueGossip(x, area));
            }
          }
        } else {
          /* Track dependencies */
          const d = exprDependencies(results);
          if (d.items) this.addEventsDependencies(this.state.dependencies.items, event, area, d.items);
          if (d.events) this.addEventsDependencies(this.state.dependencies.events, event, area, d.events);
        }
      }
    }
  }

  private evalLocations() {
    /* Extract the queue */
    const queue = this.state.queue.locations;
    this.state.queue.locations = new Map();

    /* Evaluate all the locations */
    for (const [location, areas] of queue) {
      for (const area of areas) {
        if (this.state.locations.has(location)) {
          continue;
        }

        /* Evaluate the location */
        const expr = this.world.areas[area].locations[location];
        const results: ExprResult[] = [];
        if (this.state.areas.child.has(area)) {
          results.push(this.evalExpr(expr, 'child', area));
        }
        if (this.state.areas.adult.has(area)) {
          results.push(this.evalExpr(expr, 'adult', area));
        }

        /* If any of the results are true, add the location to the state and queue up everything */
        /* Otherwise, track dependencies */
        if (results.some(x => x.result)) {
          this.addLocation(location);
        } else {
          /* Track dependencies */
          const d = exprDependencies(results);
          if (d.items) this.addLocationDependencies(this.state.dependencies.items, location, area, d.items);
          if (d.events) this.addLocationDependencies(this.state.dependencies.events, location, area, d.events);
        }
      }
    }
  }

  private evalGossips() {
    /* Extract the queue */
    const queue = this.state.queue.gossips;
    this.state.queue.gossips = new Map();

    /* Evaluate all the gossips */
    for (const [gossip, areas] of queue) {
      for (const area of areas) {
        if (this.state.gossip.has(gossip)) {
          continue;
        }

        /* Evaluate the location */
        const expr = this.world.areas[area].gossip[gossip];
        const results: ExprResult[] = [];
        if (this.state.areas.child.has(area)) {
          results.push(this.evalExpr(expr, 'child', area));
        }
        if (this.state.areas.adult.has(area)) {
          results.push(this.evalExpr(expr, 'adult', area));
        }

        /* If any of the results are true, add the location to the state and queue up everything */
        /* Otherwise, track dependencies */
        if (results.some(x => x.result)) {
          this.state.gossip.add(gossip);
        } else {
          /* Track dependencies */
          const d = exprDependencies(results);
          if (d.items) this.addGossipDependencies(this.state.dependencies.items, gossip, area, d.items);
          if (d.events) this.addGossipDependencies(this.state.dependencies.events, gossip, area, d.events);
        }
      }
    }
  }

  private pathfindStep() {
    for (;;) {
      /* Expand as much as possible */
      this.evalExits('child');
      this.evalExits('adult');
      this.evalEvents();

      const { queue } = this.state;
      if (!queue.events.size && !queue.exits.child.size && !queue.exits.adult.size) {
        break;
      }
    }

    /* Get locations */
    this.evalLocations();

    /* Return true if there is more to do */
    const { queue } = this.state;
    return (queue.events.size || queue.exits.child.size || queue.exits.adult.size || queue.locations.size);
  }

  private pathfind() {
    /* Clear new locations */
    this.state.newLocations = new Set();

    /* Handle initial state */
    if (!this.state.started) {
      this.state.started = true;
      const initAreaData = defaultAreaData();
      initAreaData.mmTime = 1;
      this.exploreArea('child', 'OOT SPAWN', initAreaData, 'OOT SPAWN');
      this.exploreArea('adult', 'OOT SPAWN', initAreaData, 'OOT SPAWN');
    }

    /* Requeue assumed items */
    if (this.opts.assumedItems) {
      for (const item in this.opts.assumedItems) {
        this.requeueItem(item);
      }
    }

    /* Handle no logic */
    if (this.settings.logic === 'none') {
      this.state.locations = new Set(Object.keys(this.world.checks));
      this.state.events = new Set(Object.values(this.world.areas).map(x => Object.keys(x.events || {})).flat());
      const allAreas = new Map<string, AreaData>;
      for (const area of Object.keys(this.world.areas)) {
        allAreas.set(area, { oot: { day: true, night: true }, mmTime: 0xffffffff });
      }
      this.state.areas = {
        child: allAreas,
        adult: allAreas,
      };
      this.state.gossip = new Set(Object.values(this.world.areas).map(x => Object.keys(x.gossip || {})).flat());
      this.state.goal = true;
      return;
    }

    /* Collect previous locations */
    const uncollected = [...this.state.uncollectedLocations];
    for (const location of uncollected) {
      this.addLocation(location);
    }

    /* Pathfind */
    for (;;) {
      const changed = this.pathfindStep();
      if (this.state.events.has('OOT_GANON') && this.state.events.has('MM_MAJORA')) {
        this.state.goal = true;
        if (this.opts.stopAtGoal) {
          break;
        }
      }
      if (!changed || !this.opts.recursive) {
        break;
      }
    }

    /* Check for gossips */
    this.evalGossips();
  }
}
