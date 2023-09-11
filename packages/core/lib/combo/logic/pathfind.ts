import { cloneDeep } from 'lodash';
import { Settings } from '../settings';
import { AreaData, Expr, ExprResult, isDefaultRestrictions, MM_TIME_SLICES } from './expr';

import { Location, locationData, makeLocation, makePlayerLocations } from './locations';
import { World } from './world';
import { isLocationLicenseGranting, isLocationRenewable } from './locations';
import { ItemPlacement } from './solve';
import { CountMap, countMapAdd } from '../util';
import { Item, itemByID, Items, ItemsCount, PlayerItems } from '../items';

export const AGES = ['child', 'adult'] as const;

export type Age = typeof AGES[number];

const recursiveForEach = <T>(arr: any, cb: (x: T) => void) => {
  if (Array.isArray(arr)) {
    for (const x of arr) {
      recursiveForEach(x, cb);
    }
  } else {
    cb(arr);
  }
}

type PathfinderDependencyList = {
  locations: Set<string>;
  events: Set<string>;
  gossips: Set<string>;
  exits: {
    child: Set<string>;
    adult: Set<string>;
  };
};

type PathfinderDependencySet<T> = Map<T, Map<string, PathfinderDependencyList>>;

type PathfinderDependencies = {
  items: PathfinderDependencySet<Item>;
  events: PathfinderDependencySet<string>;
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

type PathfinderWorldState = {
  areas: {
    child: Map<string, AreaData>;
    adult: Map<string, AreaData>;
  },
  queue: PathfinderQueue;
  dependencies: PathfinderDependencies;
  uncollectedLocations: Set<string>;
  items: ItemsCount;
  licenses: ItemsCount;
  renewables: ItemsCount;
  forbiddenReachableLocations: Set<string>;
  events: Set<string>;
  restrictedLocations?: Set<string>;
  forbiddenLocations?: Set<string>;
}

export type PathfinderState = {
  goal: boolean;
  ganonMajora: boolean;
  changed: boolean;
  started: boolean;
  ws: PathfinderWorldState[];
  previousAssumedItems: PlayerItems;

  /* Output */
  locations: Set<Location>;
  newLocations: Set<Location>;
  gossips: Set<string>[];
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

const defaultWorldState = (startingItems: ItemsCount): PathfinderWorldState => ({
  areas: {
    child: new Map(),
    adult: new Map(),
  },
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
  uncollectedLocations: new Set(),
  items: new Map(startingItems),
  licenses: new Map(startingItems),
  renewables: new Map(),
  forbiddenReachableLocations: new Set(),
  events: new Set(),
});

const defaultWorldStates = (startingItems: PlayerItems, worldCount: number) => {
  const ws: PathfinderWorldState[] = [];

  for (let world = 0; world < worldCount; ++world) {
    const itemCount: ItemsCount = new Map;
    for (const [pi, count] of startingItems) {
      if (pi.player === world) {
        itemCount.set(pi.item, count);
      }
    }
    ws.push(defaultWorldState(itemCount));
  }

  return ws;
}

const defaultState = (startingItems: PlayerItems, worldCount: number): PathfinderState => {
  const gossips: Set<string>[] = [];

  for (let world = 0; world < worldCount; ++world) {
    gossips.push(new Set());
  }

  return {
    previousAssumedItems: new Map,
    changed: false,
    goal: false,
    ganonMajora: false,
    started: false,
    ws: defaultWorldStates(startingItems, worldCount),
    locations: new Set(),
    newLocations: new Set(),
    gossips,
  };
};

const defaultAreaData = (): AreaData => ({
  oot: {
    day: false,
    night: false,
  },
  mmTime: 0,
  mmTime2: 0,
});

const mergeAreaData = (a: AreaData, b: AreaData): AreaData => ({
  oot: {
    day: a.oot.day || b.oot.day,
    night: a.oot.night || b.oot.night,
  },
  mmTime: (a.mmTime | b.mmTime) >>> 0,
  mmTime2: (a.mmTime2 | b.mmTime2) >>> 0,
});

const coveringAreaData = (a: AreaData, b: AreaData): boolean => {
  if (!a.oot.day && b.oot.day) return false;
  if (!a.oot.night && b.oot.night) return false;
  if (((a.mmTime | b.mmTime) >>> 0) !== a.mmTime) return false;
  if (((a.mmTime2 | b.mmTime2) >>> 0) !== a.mmTime2) return false;
  return true;
}

const cloneAreaData = (a: AreaData): AreaData => ({
  oot: {
    day: a.oot.day,
    night: a.oot.night,
  },
  mmTime: a.mmTime,
  mmTime2: a.mmTime2,
});

export type EntranceOverrides = {[k: string]: {[k: string]: string | null}};
type PathfinderOptions = {
  assumedItems?: PlayerItems;
  items?: ItemPlacement;
  ignoreItems?: boolean;
  recursive?: boolean;
  stopAtGoal?: boolean;
  restrictedLocations?: Set<Location>;
  forbiddenLocations?: Set<Location>;
  includeForbiddenReachable?: boolean;
  gossips?: boolean;
  inPlace?: boolean;
  singleWorld?: number;
  ganonMajora?: boolean;
};

export class Pathfinder {
  private opts!: PathfinderOptions;
  private state!: PathfinderState;

  constructor(
    private readonly worlds: World[],
    private readonly settings: Settings,
    private readonly startingItems: PlayerItems,
  ) {
  }

  run(state: PathfinderState | null, opts?: PathfinderOptions) {
    this.opts = opts || {};
    this.state = state ? (this.opts.inPlace ? state : cloneDeep(state)) : defaultState(this.startingItems, this.worlds.length);

    /* Restricted locations */
    if (this.opts.restrictedLocations) {
      for (let world = 0; world < this.worlds.length; ++world) {
        this.state.ws[world].restrictedLocations = new Set();
      }

      for (const loc of this.opts.restrictedLocations) {
        const locD = locationData(loc);
        this.state.ws[locD.world as number].restrictedLocations!.add(locD.id);
      }
    } else {
      for (let world = 0; world < this.worlds.length; ++world) {
        this.state.ws[world].restrictedLocations = undefined;
      }
    }

    /* Forbidden locations */
    if (this.opts.forbiddenLocations) {
      for (let world = 0; world < this.worlds.length; ++world) {
        this.state.ws[world].forbiddenLocations = new Set();
      }

      for (const loc of this.opts.forbiddenLocations) {
        const locD = locationData(loc);
        this.state.ws[locD.world as number].forbiddenLocations!.add(locD.id);
      }
    } else {
      for (let world = 0; world < this.worlds.length; ++world) {
        this.state.ws[world].forbiddenLocations = undefined;
      }
    }

    this.pathfind();
    return this.state;
  }

  private queueLocation(world: number, loc: string, areaFrom: string) {
    const queue = this.state.ws[world].queue.locations;
    if (!queue.has(loc)) {
      queue.set(loc, new Set([areaFrom]));
    } else {
      queue.get(loc)!.add(areaFrom);
    }
  }

  private queueEvent(world: number, event: string, areaFrom: string) {
    const queue = this.state.ws[world].queue.events;
    if (!queue.has(event)) {
      queue.set(event, new Set([areaFrom]));
    } else {
      queue.get(event)!.add(areaFrom);
    }
  }

  private queueExit(world: number, age: Age, exit: string, area: string) {
    const queue = this.state.ws[world].queue.exits[age];
    if (!queue.has(exit)) {
      queue.set(exit, new Set([area]));
    } else {
      queue.get(exit)!.add(area);
    }
  }

  private queueGossip(world: number, gossip: string, area: string) {
    const queue = this.state.ws[world].queue.gossips;
    if (!queue.has(gossip)) {
      queue.set(gossip, new Set([area]));
    } else {
      queue.get(gossip)!.add(area);
    }
  }

  /**
   * Explore an area, adding all locations and events to the queue
   */
  private exploreArea(worldId: number, age: Age, area: string, sourceAreaData: AreaData, fromArea: string) {
    /* Compute the previous area data and compare it to the old one */
    const world = this.worlds[worldId];
    const ws = this.state.ws[worldId];
    const previousAreaData = ws.areas[age].get(area);
    const newAreaData = previousAreaData ? mergeAreaData(previousAreaData, sourceAreaData) : sourceAreaData;
    const worldArea = world.areas[area];
    if (worldArea === undefined) {
      throw new Error(`Unknown area ${area}`);
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
      if (newAreaData.mmTime === 0 && newAreaData.mmTime2 === 0) {
        return;
      }

      /* If we come from OoT, we can song of time to get back to day 1 */
      const fa = world.areas[fromArea];
      if (fa.game === 'oot') {
        newAreaData.mmTime = (newAreaData.mmTime | (1 << 0)) >>> 0;
      }

      if (worldArea.stay === null) {
        /* We can wait to reach later time slices */
        let earliest: number;
        if (newAreaData.mmTime) {
          earliest = Math.floor(Math.log2(newAreaData.mmTime));
        } else {
          earliest = Math.floor(Math.log2(newAreaData.mmTime2)) + 32;
        }
        for (let i = earliest; i < MM_TIME_SLICES.length; ++i) {
          if (i < 32) {
            newAreaData.mmTime = (newAreaData.mmTime | (1 << i)) >>> 0;
          } else {
            newAreaData.mmTime2 = (newAreaData.mmTime2 | (1 << (i - 32))) >>> 0;
          }
        }
      } else {
        /* We can wait but there are conditions */
        let waitMode = false;

        for (let i = 0; i < MM_TIME_SLICES.length; ++i) {
          const mask1 = (i < 32) ? (1 << i) >>> 0 : 0;
          const mask2 = (i < 32) ? 0 : (1 << ((i - 32) >>> 0)) >>> 0;
          if ((newAreaData.mmTime & mask1) || (newAreaData.mmTime2 & mask2)) {
            waitMode = true;
          } else if (waitMode) {
            const stayExpr = worldArea.stay![i];
            const result = this.evalExpr(worldId, stayExpr, age, area);
            if (result.result) {
              waitMode = true;
              newAreaData.mmTime = (newAreaData.mmTime | mask1) >>> 0;
              newAreaData.mmTime2 = (newAreaData.mmTime2 | mask2) >>> 0;
            } else {
              /* We can't wait! */
              waitMode = false;
              const d = this.getDeps([result]);
              this.addExitsDependencies(ws.dependencies.items, age, area, fromArea, d.items);
              this.addExitsDependencies(ws.dependencies.events, age, area, fromArea, d.events);
            }
          }
        }
      }
    }

    if (previousAreaData && coveringAreaData(previousAreaData, newAreaData)) {
      return;
    }
    ws.areas[age].set(area, newAreaData);
    const a = world.areas[area];
    let locs = Object.keys(a.locations).filter(x => !this.state.locations.has(makeLocation(x, worldId)));
    if (ws.restrictedLocations && !this.opts.includeForbiddenReachable) {
      locs = locs.filter(x => ws.restrictedLocations!.has(x));
    }
    if (ws.forbiddenLocations && !this.opts.includeForbiddenReachable) {
      locs = locs.filter(x => !ws.forbiddenLocations!.has(x));
    }
    locs.forEach(x => this.queueLocation(worldId, x, area));
    Object.keys(a.events).filter(x => !ws.events.has(x)).forEach(x => this.queueEvent(worldId, x, area));
    const exits = Object.keys(a.exits);
    exits.forEach(x => this.queueExit(worldId, age, x, area));

    if (this.opts.gossips) {
      Object.keys(a.gossip).forEach(x => this.queueGossip(worldId, x, area));
    }
  }

  private evalExpr(worldId: number, expr: Expr, age: Age, area: string) {
    const world = this.worlds[worldId];
    const ws = this.state.ws[worldId];
    const areaData = ws.areas[age].get(area)!;
    const result = expr({ world, areaData, items: ws.items, renewables: ws.renewables, licenses: ws.licenses, age, events: ws.events, ignoreItems: this.opts.ignoreItems || false });
    if (result.result) {
      if (!result.restrictions || isDefaultRestrictions(result.restrictions)) {
        result.depItems = [];
        result.depEvents = [];
      }
    }
    return result;
  }

  private dependenciesLookup<T>(set: PathfinderDependencySet<T>, dependency: T, areaFrom: string) {
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

  private addExitsDependencies<T>(set: PathfinderDependencySet<T>, age: Age, exit: string, areaFrom: string, dependents: Set<T>) {
    for (const dep of dependents) {
      const data = this.dependenciesLookup(set, dep, areaFrom);
      data.exits[age].add(exit);
    }
  }

  private addEventsDependencies<T>(set: PathfinderDependencySet<T>, event: string, areaFrom: string, dependents: Set<T>) {
    for (const dep of dependents) {
      const data = this.dependenciesLookup(set, dep, areaFrom);
      data.events.add(event);
    }
  }

  private addLocationDependencies<T>(set: PathfinderDependencySet<T>, location: string, areaFrom: string, dependents: Set<T>) {
    for (const dep of dependents) {
      const data = this.dependenciesLookup(set, dep, areaFrom);
      data.locations.add(location);
    }
  }

  private addGossipDependencies<T>(set: PathfinderDependencySet<T>, gossip: string, areaFrom: string, dependents: Set<T>) {
    for (const dep of dependents) {
      const data = this.dependenciesLookup(set, dep, areaFrom);
      data.gossips.add(gossip);
    }
  }

  private requeueItem(worldId: number, item: Item) {
    const ws = this.state.ws[worldId];
    const deps = ws.dependencies.items.get(item);
    if (deps) {
      ws.dependencies.items.delete(item);
      for (const [area, d] of deps) {
        d.exits.child.forEach(x => this.queueExit(worldId, 'child', x, area));
        d.exits.adult.forEach(x => this.queueExit(worldId, 'adult', x, area));
        d.events.forEach(x => this.queueEvent(worldId, x, area));
        d.locations.forEach(x => this.queueLocation(worldId, x, area));
        if (this.opts.gossips) {
          d.gossips.forEach(x => this.queueGossip(worldId, x, area));
        }
      }
    }
  }

  private addLocationDelayed(worldId: number, loc: string) {
    const globalLoc = makeLocation(loc, worldId);
    this.state.locations.add(globalLoc);
    this.state.newLocations.add(globalLoc);
  }

  private addLocation(worldId: number, loc: string) {
    const ws = this.state.ws[worldId];
    const world = this.worlds[worldId];
    const globalLoc = makeLocation(loc, worldId);
    this.state.locations.add(globalLoc);
    const playerItem = this.opts.items?.get(globalLoc);
    if (playerItem) {
      const otherWs = this.state.ws[playerItem.player];
      ws.uncollectedLocations.delete(loc);
      countMapAdd(otherWs.items, playerItem.item);
      if (isLocationRenewable(world, globalLoc)) {
        countMapAdd(otherWs.renewables, playerItem.item);
      }
      if (isLocationLicenseGranting(world, globalLoc))
        countMapAdd(otherWs.licenses, playerItem.item);
      this.requeueItem(playerItem.player, playerItem.item);
    } else {
      ws.uncollectedLocations.add(loc);
    }
  }

  private evalExits(worldId: number, age: Age) {
    /* Extract the queue */
    const ws = this.state.ws[worldId];
    const world = this.worlds[worldId];
    const queue = ws.queue.exits[age];
    ws.queue.exits[age] = new Map();

    /* Evaluate all the exits */
    for (const [exit, areas] of queue) {
      for (const area of areas) {
        const a = world.areas[area];
        const expr = a.exits[exit];
        const exprResult = this.evalExpr(worldId, expr, age, area);

        /* Track dependencies */
        const d = this.getDeps([exprResult]);
        this.addExitsDependencies(ws.dependencies.items, age, exit, area, d.items);
        this.addExitsDependencies(ws.dependencies.events, age, exit, area, d.events);

        if (exprResult.result) {
          const areaData = cloneAreaData(ws.areas[age].get(area)!);
          const r = exprResult.restrictions;
          if (r) {
            if (r.oot.day) areaData.oot.day = false;
            if (r.oot.night) areaData.oot.night = false;
            if (r.mmTime) {
              areaData.mmTime = (areaData.mmTime & ~(r.mmTime)) >>> 0;
            }
            if (r.mmTime2) {
              areaData.mmTime2 = (areaData.mmTime2 & ~(r.mmTime2)) >>> 0;
            }
          }
          this.exploreArea(worldId, age, exit, areaData, area);
        }
      }
    }
  }

  private getDeps(res: ExprResult[]) {
    const itemsArr = res.map(x => x.depItems);
    const eventsArr = res.map(x => x.depEvents);
    const items = new Set<Item>();
    const events = new Set<string>();

    recursiveForEach<Item>(itemsArr, x => items.add(x));
    recursiveForEach<string>(eventsArr, x => events.add(x));

    return { items, events };
  }

  private evalEvents(worldId: number) {
    /* Extract the queue */
    const ws = this.state.ws[worldId];
    const world = this.worlds[worldId];
    const queue = ws.queue.events;
    ws.queue.events = new Map();

    /* Evaluate all the events */
    for (const [event, areas] of queue) {
      for (const area of areas) {
        if (ws.events.has(event)) {
          continue;
        }

        /* Evaluate the event */
        const expr = world.areas[area].events[event];
        if (!expr) {
          throw new Error(`Event ${event} not found in area ${area}`);
        }
        const results: ExprResult[] = [];
        if (ws.areas.child.has(area)) {
          results.push(this.evalExpr(worldId, expr, 'child', area));
        }
        if (ws.areas.adult.has(area)) {
          results.push(this.evalExpr(worldId, expr, 'adult', area));
        }

        /* If any of the results are true, add the event to the state and queue up everything */
        /* Otherwise, track dependencies */
        if (results.some(x => x.result)) {
          ws.events.add(event);
          const deps = ws.dependencies.events.get(event);
          if (deps) {
            ws.dependencies.events.delete(event);
            for (const [area, d] of deps) {
              d.exits.child.forEach(x => this.queueExit(worldId, 'child', x, area));
              d.exits.adult.forEach(x => this.queueExit(worldId, 'adult', x, area));
              d.events.forEach(x => this.queueEvent(worldId, x, area));
              d.locations.forEach(x => this.queueLocation(worldId, x, area));
              if (this.opts.gossips) {
                d.gossips.forEach(x => this.queueGossip(worldId, x, area));
              }
            }
          }
        } else {
          /* Track dependencies */
          const d = this.getDeps(results);
          this.addEventsDependencies(ws.dependencies.items, event, area, d.items);
          this.addEventsDependencies(ws.dependencies.events, event, area, d.events);
        }
      }
    }
  }

  private evalLocations(worldId: number) {
    /* Extract the queue */
    const ws = this.state.ws[worldId];
    const world = this.worlds[worldId];
    const queue = ws.queue.locations;
    ws.queue.locations = new Map();

    /* Evaluate all the locations */
    for (const [location, areas] of queue) {
      let isAllowed = true;
      if (this.opts.includeForbiddenReachable) {
        if (ws.restrictedLocations && !ws.restrictedLocations.has(location)) {
          isAllowed = false;
        } else if (ws.forbiddenLocations && ws.forbiddenLocations.has(location)) {
          isAllowed = false;
        }
      }

      const globalLoc = makeLocation(location, worldId);

      for (const area of areas) {
        if (this.state.locations.has(globalLoc)) {
          continue;
        }

        /* Evaluate the location */
        const expr = world.areas[area].locations[location];
        const results: ExprResult[] = [];
        if (ws.areas.child.has(area)) {
          results.push(this.evalExpr(worldId, expr, 'child', area));
        }
        if (ws.areas.adult.has(area)) {
          results.push(this.evalExpr(worldId, expr, 'adult', area));
        }

        /* If any of the results are true, add the location to the state and queue up everything */
        /* Otherwise, track dependencies */
        if (results.some(x => x.result)) {
          if (isAllowed) {
            this.addLocationDelayed(worldId, location);
          } else {
            ws.forbiddenReachableLocations.add(location);
          }
        } else {
          /* Track dependencies */
          const d = this.getDeps(results);
          this.addLocationDependencies(ws.dependencies.items, location, area, d.items);
          this.addLocationDependencies(ws.dependencies.events, location, area, d.events);
        }
      }
    }
  }

  private evalGossips(worldId: number) {
    /* Extract the queue */
    const ws = this.state.ws[worldId];
    const world = this.worlds[worldId];
    const queue = ws.queue.gossips;
    ws.queue.gossips = new Map();

    /* Evaluate all the gossips */
    for (const [gossip, areas] of queue) {
      for (const area of areas) {
        if (this.state.gossips[worldId].has(gossip)) {
          continue;
        }

        /* Evaluate the location */
        const expr = world.areas[area].gossip[gossip];
        const results: ExprResult[] = [];
        if (ws.areas.child.has(area)) {
          results.push(this.evalExpr(worldId, expr, 'child', area));
        }
        if (ws.areas.adult.has(area)) {
          results.push(this.evalExpr(worldId, expr, 'adult', area));
        }

        /* If any of the results are true, add the location to the state and queue up everything */
        /* Otherwise, track dependencies */
        if (results.some(x => x.result)) {
          this.state.gossips[worldId].add(gossip);
        } else {
          /* Track dependencies */
          const d = this.getDeps(results);
          this.addGossipDependencies(ws.dependencies.items, gossip, area, d.items);
          this.addGossipDependencies(ws.dependencies.events, gossip, area, d.events);
        }
      }
    }
  }

  private pathfindStep() {
    /* Clear new locations */
    this.state.newLocations.clear();

    for (let worldId = 0; worldId < this.worlds.length; ++worldId) {
      if (this.opts.singleWorld !== undefined && this.opts.singleWorld !== worldId) {
        continue;
      }
      for (;;) {
        /* Expand as much as possible */
        const ws = this.state.ws[worldId];

        this.evalExits(worldId, 'child');
        this.evalExits(worldId, 'adult');
        this.evalEvents(worldId);

        const { queue } = ws;
        if (!queue.events.size && !queue.exits.child.size && !queue.exits.adult.size) {
          break;
        }
      }

      /* Get locations */
      this.evalLocations(worldId);
    }

    /* Add delayed locations */
    for (const globalLoc of this.state.newLocations) {
      const locD = locationData(globalLoc);
      this.addLocation(locD.world as number, locD.id);
    }

    /* Return true if there is more to do */
    for (let world = 0; world < this.worlds.length; ++world) {
      if (this.opts.singleWorld !== undefined && this.opts.singleWorld !== world) {
        continue;
      }
      const ws = this.state.ws[world];
      const { queue } = ws;
      if (queue.events.size || queue.exits.child.size || queue.exits.adult.size || queue.locations.size)
        return true;
    }

    return false;
  }

  private isGanonMajoraReached() {
    for (let worldId = 0; worldId < this.worlds.length; ++worldId) {
      if (this.opts.singleWorld !== undefined && this.opts.singleWorld !== worldId) {
        continue;
      }
      const ws = this.state.ws[worldId];
      if (!ws.events.has('OOT_GANON_PRE_BOSS')) return false;
      if (!ws.events.has('MM_MAJORA_PRE_BOSS')) return false;
    }

    return true;
  }

  private isGoalReached() {
    const { settings } = this;

    for (let worldId = 0; worldId < this.worlds.length; ++worldId) {
      if (this.opts.singleWorld !== undefined && this.opts.singleWorld !== worldId) {
        continue;
      }
      const ws = this.state.ws[worldId];
      const ganon = ws.events.has('OOT_GANON');
      const majora = ws.events.has('MM_MAJORA');
      let worldGoal = false;

      switch (settings.goal) {
      case 'any': worldGoal = ganon || majora; break;
      case 'ganon': worldGoal = ganon; break;
      case 'majora': worldGoal = majora; break;
      case 'both': worldGoal = ganon && majora; break;
      case 'triforce': worldGoal = (this.opts.ignoreItems || ((ws.items.get(Items.SHARED_TRIFORCE) || 0) >= settings.triforceGoal)); break;
      case 'triforce3': worldGoal = (this.opts.ignoreItems || (ws.items.has(Items.SHARED_TRIFORCE_POWER) && ws.items.has(Items.SHARED_TRIFORCE_COURAGE) && ws.items.has(Items.SHARED_TRIFORCE_WISDOM))); break;
      }

      if (!worldGoal) {
        return false;
      }
    }

    return true;
  }

  private pathfind() {
    /* Handle initial state */
    if (!this.state.started) {
      this.state.started = true;
      const initAreaData = defaultAreaData();
      initAreaData.mmTime = 1;

      for (let worldId = 0; worldId < this.worlds.length; ++worldId) {
        if (this.opts.singleWorld !== undefined && this.opts.singleWorld !== worldId) {
          continue;
        }
        this.exploreArea(worldId, 'child', 'OOT SPAWN', initAreaData, 'OOT SPAWN');
        this.exploreArea(worldId, 'adult', 'OOT SPAWN', initAreaData, 'OOT SPAWN');
      }
    }

    /* Assumed items */
    if (this.opts.assumedItems) {
      for (const [playerItem, amount] of this.opts.assumedItems.entries()) {
        const amountPrev = this.state.previousAssumedItems.get(playerItem) || 0;

        if (amount > amountPrev) {
          const ws = this.state.ws[playerItem.player];
          const delta = amount - amountPrev;

          countMapAdd(ws.items, playerItem.item, delta);
          countMapAdd(ws.renewables, playerItem.item, delta);
          countMapAdd(ws.licenses, playerItem.item, delta);
          this.requeueItem(playerItem.player, playerItem.item);
          this.state.previousAssumedItems.set(playerItem, amount);
        }
      }
    }

    /* Handle no logic */
    if (this.settings.logic === 'none') {
      this.state.goal = true;
      const locations: Location[][] = [];
      this.state.gossips = [];
      for (let worldId = 0; worldId < this.worlds.length; ++worldId) {
        const world = this.worlds[worldId];
        const worldGossips = new Set(Object.values(world.areas).map(x => Object.keys(x.gossip || {})).flat());
        this.state.gossips.push(worldGossips);
        const locs = Object.keys(world.checks).map(x => makeLocation(x, worldId));
        locations.push(locs);
      }
      this.state.locations = new Set(locations.flat());
      return;
    }

    for (let worldId = 0; worldId < this.worlds.length; ++worldId) {
      const ws = this.state.ws[worldId];

      /* Collect previous locations */
      for (const loc of ws.uncollectedLocations) {
        this.addLocation(worldId, loc);
      }

      /* Collect previously forbidden locations */
      for (const loc of ws.forbiddenReachableLocations) {
        let isAllowed = true;
        if (ws.restrictedLocations && !ws.restrictedLocations.has(loc)) {
          isAllowed = false;
        } else if (ws.forbiddenLocations && ws.forbiddenLocations.has(loc)) {
          isAllowed = false;
        }
        if (isAllowed) {
          this.addLocation(worldId, loc);
          ws.forbiddenReachableLocations.delete(loc);
        }
      }
    }

    /* Pathfind */
    for (;;) {
      this.state.changed = this.pathfindStep();
      if (this.opts.ganonMajora) {
        this.state.ganonMajora = this.isGanonMajoraReached();
      }
      if (this.isGoalReached()) {
        this.state.goal = true;
        if (this.opts.stopAtGoal) {
          break;
        }
      }
      if (!this.state.changed || !this.opts.recursive) {
        break;
      }
    }

    /* Check for gossips */
    if (this.opts.gossips) {
      for (let worldId = 0; worldId < this.worlds.length; ++worldId) {
        this.evalGossips(worldId);
      }
    }
  }
}
