import { cloneDeep } from 'lodash';

import { Settings } from '../settings';
import { AreaData, Expr, ExprResult, MM_TIME_SLICES, RecursiveArray, isDefaultRestrictions } from './expr';
import { Location, locationData, makeLocation } from './locations';
import { World } from './world';
import { isLocationLicenseGranting, isLocationRenewable } from './locations';
import { ItemPlacement } from './solve';
import { countMapAdd } from '../util';
import { Item, Items, ItemsCount, PlayerItems } from '../items';
import { isTriforcePiece } from '../items/helpers';
import { exprPartialEvalAge } from './expr-partial-eval';
import { Age, AGE_ADULT, AGE_CHILD, AGES } from './constants';

const recursiveForEach = <T>(arr: any, cb: (x: T) => void) => {
  if (Array.isArray(arr)) {
    for (const x of arr) {
      recursiveForEach(x, cb);
    }
  } else {
    cb(arr);
  }
}

const EVENT_TIME_TRAVEL = 'OOT_TIME_TRAVEL_AT_WILL';

const MASKS_BITS_SET = [
  0x00000001, 0x00000003, 0x00000007, 0x0000000f,
  0x0000001f, 0x0000003f, 0x0000007f, 0x000000ff,
  0x000001ff, 0x000003ff, 0x000007ff, 0x00000fff,
  0x00001fff, 0x00003fff, 0x00007fff, 0x0000ffff,
  0x0001ffff, 0x0003ffff, 0x0007ffff, 0x000fffff,
  0x001fffff, 0x003fffff, 0x007fffff, 0x00ffffff,
  0x01ffffff, 0x03ffffff, 0x07ffffff, 0x0fffffff,
  0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff,
];

const MASK_MM_TIME = MM_TIME_SLICES.length > 32 ? 0xffffffff : MASKS_BITS_SET[MM_TIME_SLICES.length - 1];
const MASK_MM_TIME2 = MM_TIME_SLICES.length > 32 ? MASKS_BITS_SET[MM_TIME_SLICES.length - 33] : 0;

type PathfinderDependencyList = {
  locations: Set<string>;
  events: Set<string>;
  gossips: Set<string>;
  exits: Set<string>;
};

type PathfinderDependencySet<T> = Map<T, Map<string, PathfinderDependencyList>>;

type PathfinderDependencies = {
  items: PathfinderDependencySet<Item>;
  events: PathfinderDependencySet<string>;
};

type PathfinderAgeState = {
  areas: Map<string, AreaData>;
  dependencies: PathfinderDependencies;
};

type PathfinderWorldState = {
  ages: [PathfinderAgeState, PathfinderAgeState];
  uncollectedLocations: Set<string>;
  collectedLocations: Set<string>;
  items: ItemsCount;
  licenses: ItemsCount;
  renewables: ItemsCount;
  locations: Set<string>;
  forbiddenReachableLocations: Set<string>;
  events: Set<string>;
  restrictedLocations?: Set<string>;
  forbiddenLocations?: Set<string>;
}

export type PathfinderState = {
  stopped: boolean;
  goal: boolean;
  ganonMajora: boolean;
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
  exits: new Set(),
});

const defaultAgeState = (): PathfinderAgeState => ({
  areas: new Map(),
  dependencies: {
    items: new Map(),
    events: new Map(),
  },
});

const defaultWorldState = (startingItems: ItemsCount): PathfinderWorldState => ({
  ages: [defaultAgeState(), defaultAgeState()],
  uncollectedLocations: new Set(),
  collectedLocations: new Set(),
  items: new Map(startingItems),
  licenses: new Map(startingItems),
  renewables: new Map(),
  forbiddenReachableLocations: new Set(),
  events: new Set(),
  locations: new Set(),
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
    stopped: false,
    previousAssumedItems: new Map,
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
  flagsOn: 0,
  flagsOff: 0,
});

const mergeAreaData = (a: AreaData, b: AreaData): AreaData => ({
  oot: {
    day: a.oot.day || b.oot.day,
    night: a.oot.night || b.oot.night,
  },
  mmTime: (a.mmTime | b.mmTime) >>> 0,
  mmTime2: (a.mmTime2 | b.mmTime2) >>> 0,
  flagsOn: (a.flagsOn & b.flagsOn) >>> 0,
  flagsOff: (a.flagsOff & b.flagsOff) >>> 0,
});

const coveringAreaData = (a: AreaData, b: AreaData): boolean => {
  if (!a.oot.day && b.oot.day) return false;
  if (!a.oot.night && b.oot.night) return false;
  if (((a.mmTime | b.mmTime) >>> 0) !== a.mmTime) return false;
  if (((a.mmTime2 | b.mmTime2) >>> 0) !== a.mmTime2) return false;
  if (((a.flagsOn & b.flagsOn) >>> 0) !== a.flagsOn) return false;
  if (((a.flagsOff & b.flagsOff) >>> 0) !== a.flagsOff) return false;
  return true;
}

const cloneAreaData = (a: AreaData): AreaData => ({
  oot: {
    day: a.oot.day,
    night: a.oot.night,
  },
  mmTime: a.mmTime,
  mmTime2: a.mmTime2,
  flagsOn: a.flagsOn,
  flagsOff: a.flagsOff,
});

export type EntranceOverrides = {[k: string]: {[k: string]: string | null}};
type PathfinderOptions = {
  assumedItems?: PlayerItems;
  items?: ItemPlacement;
  recursive?: boolean;
  stopAtGoal?: boolean;
  restrictedLocations?: Set<Location>;
  forbiddenLocations?: Set<Location>;
  includeForbiddenReachable?: boolean;
  gossips?: boolean;
  inPlace?: boolean;
  singleWorld?: number;
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
    } else {
      this.pathfind();
    }
    return this.state;
  }

  /**
   * Explore an area, adding all locations and events to the queue
   */
  private exploreArea(worldId: number, age: Age, area: string, sourceAreaData: AreaData, fromArea: string) {
    /* Compute the previous area data and compare it to the old one */
    const world = this.worlds[worldId];
    const ws = this.state.ws[worldId];
    const as = ws.ages[age];
    const previousAreaData = as.areas.get(area);
    const newAreaData = previousAreaData ? mergeAreaData(previousAreaData, sourceAreaData) : sourceAreaData;
    const worldArea = world.areas[area];

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

      if (worldArea.stay === null) {
        let mmTime;
        let mmTime2;
        if (newAreaData.mmTime) {
          mmTime = newAreaData.mmTime;
          mmTime = (mmTime | (mmTime << 1));
          mmTime = (mmTime | (mmTime << 2));
          mmTime = (mmTime | (mmTime << 4));
          mmTime = (mmTime | (mmTime << 8));
          mmTime = (mmTime | (mmTime << 16));
          mmTime2 = 0xffffffff;
        } else {
          mmTime2 = newAreaData.mmTime2;
          mmTime2 = (mmTime2 | (mmTime2 << 1));
          mmTime2 = (mmTime2 | (mmTime2 << 2));
          mmTime2 = (mmTime2 | (mmTime2 << 4));
          mmTime2 = (mmTime2 | (mmTime2 << 8));
          mmTime2 = (mmTime2 | (mmTime2 << 16));
          mmTime = 0;
        }

        mmTime = (mmTime & MASK_MM_TIME) >>> 0;
        mmTime2 = (mmTime2 & MASK_MM_TIME2) >>> 0;

        newAreaData.mmTime = mmTime;
        newAreaData.mmTime2 = mmTime2;
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
              this.trackDependencies('exits', as.dependencies, area, fromArea, result);
            }
          }
        }
      }
    }

    /* Age swap */
    if (ws.events.has(EVENT_TIME_TRAVEL) && worldArea.ageChange && area !== fromArea) {
      const otherAge = age === AGE_CHILD ? AGE_ADULT : AGE_CHILD;
      this.exploreArea(worldId, otherAge, area, cloneAreaData(newAreaData), area);
    }

    if (previousAreaData && coveringAreaData(previousAreaData, newAreaData)) {
      return;
    }
    as.areas.set(area, newAreaData);

    /* Eval locations */
    for (const l in worldArea.locations) {
      if (ws.restrictedLocations && !this.opts.includeForbiddenReachable && !ws.restrictedLocations.has(l)) {
        continue;
      }
      if (ws.forbiddenLocations && !this.opts.includeForbiddenReachable && ws.forbiddenLocations.has(l)) {
        continue;
      }
      this.evalLocation(worldId, age, area, l);
    }

    /* Eval events */
    for (const e in worldArea.events) {
      this.evalEvent(worldId, age, area, e);
    }

    /* Eval exits */
    for (const e in worldArea.exits) {
      this.evalExit(worldId, age, area, e);
    }

    /* Eval gossips */
    if (this.opts.gossips) {
      for (const g in worldArea.gossip) {
        this.evalGossip(worldId, age, area, g);
      }
    }
  }

  private evalExpr(worldId: number, expr: Expr, age: Age, area: string) {
    const world = this.worlds[worldId];
    const ws = this.state.ws[worldId];
    const as = ws.ages[age];
    const areaData = as.areas.get(area)!;
    const state = { settings: this.settings, world, areaData, items: ws.items, renewables: ws.renewables, licenses: ws.licenses, age, events: ws.events };

    expr = exprPartialEvalAge(expr, age);
    return expr.eval(state);
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

  private addDependencies<T>(type: 'exits' | 'locations' | 'events' | 'gossips', set: PathfinderDependencySet<T>, id: string, area: string, dependents: RecursiveArray<T>) {
    recursiveForEach<T>(dependents, dep => {
      const data = this.dependenciesLookup(set, dep, area);
      data[type].add(id);
    });
  }

  private resultNeedsTracking(result: ExprResult) {
    return (result.result === false || (result.restrictions && !isDefaultRestrictions(result.restrictions)));
  }

  private trackDependencies(type: 'exits' | 'locations' | 'events' | 'gossips', deps: PathfinderDependencies, id: string, area: string, result: ExprResult) {
    if (!this.resultNeedsTracking(result)) {
      return;
    }

    this.addDependencies(type, deps.items, id, area, result.depItems);
    this.addDependencies(type, deps.events, id, area, result.depEvents);
  }

  private requeueItem(worldId: number, item: Item) {
    const ws = this.state.ws[worldId];
    for (const age of AGES) {
      const as = ws.ages[age];
      const deps = as.dependencies.items.get(item);
      if (deps) {
        as.dependencies.items.delete(item);
        for (const [area, d] of deps) {
          d.exits.forEach(x => this.evalExit(worldId, age, area, x));
          d.events.forEach(x => this.evalEvent(worldId, age, area, x));
          d.locations.forEach(x => this.evalLocation(worldId, age, area, x));
          if (this.opts.gossips) {
            d.gossips.forEach(x => this.evalGossip(worldId, age, area, x));
          }
        }
      }
    }
  }

  private addLocationDelayed(worldId: number, loc: string) {
    if (this.opts.recursive) {
      this.addLocation(worldId, loc);
    } else {
      const ws = this.state.ws[worldId];
      const globalLoc = makeLocation(loc, worldId);
      this.state.locations.add(globalLoc);
      this.state.newLocations.add(globalLoc);
      ws.locations.add(loc);
      ws.uncollectedLocations.add(loc);
    }
  }

  private collectLocation(worldId: number, loc: string) {
    const ws = this.state.ws[worldId];
    if (ws.collectedLocations.has(loc)) {
      return;
    }
    const world = this.worlds[worldId];
    const globalLoc = makeLocation(loc, worldId);
    const playerItem = this.opts.items?.get(globalLoc);
    if (playerItem) {
      const otherWs = this.state.ws[playerItem.player];
      ws.uncollectedLocations.delete(loc);
      ws.collectedLocations.add(loc);
      countMapAdd(otherWs.items, playerItem.item);
      if (isLocationRenewable(world, globalLoc)) {
        countMapAdd(otherWs.renewables, playerItem.item);
      }
      if (isLocationLicenseGranting(world, globalLoc))
        countMapAdd(otherWs.licenses, playerItem.item);
      this.requeueItem(playerItem.player, playerItem.item);
      if (isTriforcePiece(playerItem.item)) {
        this.updateGoalFlags();
      }
    } else {
      ws.uncollectedLocations.add(loc);
    }
  }

  private addLocation(worldId: number, loc: string) {
    const ws = this.state.ws[worldId];
    const globalLoc = makeLocation(loc, worldId);
    this.state.locations.add(globalLoc);
    ws.locations.add(loc);
    this.collectLocation(worldId, loc);
  }

  private evalExit(worldId: number, age: Age, area: string, exit: string) {
    if (this.state.stopped) return;

    /* Extract the queue */
    const ws = this.state.ws[worldId];
    const as = ws.ages[age];
    const world = this.worlds[worldId];
    const a = world.areas[area];
    const expr = a.exits[exit];
    const exprResult = this.evalExpr(worldId, expr, age, area);

    /* Track dependencies */
    this.trackDependencies('exits', as.dependencies, exit, area, exprResult);

    if (exprResult.result) {
      const areaData = cloneAreaData(as.areas.get(area)!);
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
        areaData.flagsOn = (areaData.flagsOn | r.flagsOn) >>> 0;
        areaData.flagsOff = (areaData.flagsOff | r.flagsOff) >>> 0;
      }
      const previousAreaData = as.areas.get(exit);
      const exitArea = this.worlds[worldId].areas[exit];

      /* If we come from OoT, we can song of time to get back to day 1 */
      if (a.game === 'oot' && exitArea.game === 'mm') {
        areaData.mmTime = (areaData.mmTime | 1);
      }

      /* Extremely aggressive optimization, skip some areas early if we know there won't be time expansion */
      if (!(previousAreaData && coveringAreaData(previousAreaData, areaData) && exitArea.stay === null)) {
        this.exploreArea(worldId, age, exit, areaData, area);
      }
    }
  }

  private evalEvent(worldId: number, age: Age, area: string, event: string) {
    if (this.state.stopped) return;

    const ws = this.state.ws[worldId];
    if (ws.events.has(event)) {
      return;
    }

    const world = this.worlds[worldId];
    const as = ws.ages[age];
    const worldArea = world.areas[area];

    /* Evaluate the event */
    const expr = worldArea.events[event];
    if (!expr) {
      throw new Error(`Event ${event} not found in area ${area}`);
    }
    const result = this.evalExpr(worldId, expr, age, area);

    /* If the result is true, add the event to the state and queue up everything */
    /* Otherwise, track dependencies */
    if (result.result) {
      ws.events.add(event);
      if (event === 'OOT_GANON' || event === 'OOT_GANON_PRE_BOSS' || event === 'MM_MAJORA' || event === 'MM_MAJORA_PRE_BOSS') {
        this.updateGoalFlags();
      }

      for (const evAge of AGES) {
        const evAs = ws.ages[evAge];
        const deps = evAs.dependencies.events.get(event);
        if (deps) {
          evAs.dependencies.events.delete(event);
          for (const [area, d] of deps) {
            d.exits.forEach(x => this.evalExit(worldId, evAge, area, x));
            d.events.forEach(x => this.evalEvent(worldId, evAge, area, x));
            d.locations.forEach(x => this.evalLocation(worldId, evAge, area, x));
            if (this.opts.gossips) {
              d.gossips.forEach(x => this.evalGossip(worldId, evAge, area, x));
            }
          }
        }
      }

      /* If it's time travel at will, we need to re-explore everything */
      if (event === EVENT_TIME_TRAVEL) {
        for (const [area, areaData] of ws.ages[AGE_CHILD].areas) {
          const a = world.areas[area];
          if (a.ageChange)
            this.exploreArea(worldId, AGE_ADULT, area, cloneAreaData(areaData), area);
        }
        for (const [area, areaData] of ws.ages[AGE_ADULT].areas) {
          const a = world.areas[area];
          if (a.ageChange)
            this.exploreArea(worldId, AGE_CHILD, area, cloneAreaData(areaData), area);
        }
      }
    } else {
      /* Track dependencies */
      this.trackDependencies('events', as.dependencies, event, area, result);
    }
  }

  private evalLocation(worldId: number, age: Age, area: string, location: string) {
    if (this.state.stopped) return;

    const ws = this.state.ws[worldId];
    if (ws.locations.has(location)) {
      return;
    }
    const as = ws.ages[age];
    const worldArea = this.worlds[worldId].areas[area];

    let isAllowed = true;
    if (this.opts.includeForbiddenReachable) {
      if (ws.restrictedLocations && !ws.restrictedLocations.has(location)) {
        isAllowed = false;
      } else if (ws.forbiddenLocations && ws.forbiddenLocations.has(location)) {
        isAllowed = false;
      }
    }


    /* Evaluate the location */
    const expr = worldArea.locations[location];
    const result = this.evalExpr(worldId, expr, age, area);

    /* If the result is true, add the location to the state and queue up everything */
    /* Otherwise, track dependencies */
    if (result.result) {
      if (isAllowed) {
        this.addLocationDelayed(worldId, location);
      } else {
        ws.forbiddenReachableLocations.add(location);
      }
    } else {
      /* Track dependencies */
      this.trackDependencies('locations', as.dependencies, location, area, result);
    }
  }

  private evalGossip(worldId: number, age: Age, area: string, gossip: string) {
    if (this.state.gossips[worldId].has(gossip)) {
      return;
    }

    /* Extract the queue */
    const ws = this.state.ws[worldId];
    const as = ws.ages[age];
    const worldArea = this.worlds[worldId].areas[area];

    /* Evaluate the gossip */
    const expr = worldArea.gossip[gossip];
    const result = this.evalExpr(worldId, expr, age, area);

    /* If any of the results are true, add the gossip to the state and queue up everything */
    /* Otherwise, track dependencies */
    if (result.result) {
      this.state.gossips[worldId].add(gossip);
    } else {
      /* Track dependencies */
      this.trackDependencies('gossips', as.dependencies, gossip, area, result);
    }
  }

  private isGanonMajoraReached() {
    for (let worldId = 0; worldId < this.worlds.length; ++worldId) {
      if (this.opts.singleWorld !== undefined && this.opts.singleWorld !== worldId) {
        continue;
      }
      const ws = this.state.ws[worldId];
      if (this.settings.games !== 'mm' && !ws.events.has('OOT_GANON_PRE_BOSS')) return false;
      if (this.settings.games !== 'oot' && !ws.events.has('MM_MAJORA_PRE_BOSS')) return false;
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
      case 'triforce': worldGoal = (((ws.items.get(Items.SHARED_TRIFORCE) || 0) >= settings.triforceGoal)); break;
      case 'triforce3': worldGoal = ((ws.items.has(Items.SHARED_TRIFORCE_POWER) && ws.items.has(Items.SHARED_TRIFORCE_COURAGE) && ws.items.has(Items.SHARED_TRIFORCE_WISDOM))); break;
      }

      if (!worldGoal) {
        return false;
      }
    }

    return true;
  }

  private updateGoalFlags() {
    if (this.settings.goal === 'triforce3') {
      this.state.ganonMajora = this.isGanonMajoraReached();
    }
    if (this.isGoalReached()) {
      this.state.goal = true;
      if (this.opts.stopAtGoal) {
        this.state.stopped = true;
      }
    }
  }

  private pathfind() {
    /* Reset new locations */
    this.state.newLocations.clear();

    /* Collect previous stuff */
    const worldsUncollectedLocations = this.state.ws.map(ws => [...ws.uncollectedLocations]);
    const worldForbiddenReachableLocations = this.state.ws.map(ws => [...ws.forbiddenReachableLocations]);
    for (let i = 0; i < this.state.ws.length; ++i) {
      const uncollected = worldsUncollectedLocations[i];
      const frl = worldForbiddenReachableLocations[i];
      const ws = this.state.ws[i];

      for (const loc of uncollected) {
        this.collectLocation(i, loc);
      }

      for (const loc of frl) {
        let isAllowed = true;
        if (ws.restrictedLocations && !ws.restrictedLocations.has(loc)) {
          isAllowed = false;
        } else if (ws.forbiddenLocations && ws.forbiddenLocations.has(loc)) {
          isAllowed = false;
        }
        if (isAllowed) {
          this.addLocation(i, loc);
          ws.forbiddenReachableLocations.delete(loc);
        }
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

    /* Handle initial state */
    if (!this.state.started) {
      this.state.started = true;
      const initAreaData = defaultAreaData();
      initAreaData.mmTime = 1;

      this.updateGoalFlags();

      for (let worldId = 0; worldId < this.worlds.length; ++worldId) {
        if (this.opts.singleWorld !== undefined && this.opts.singleWorld !== worldId) {
          continue;
        }
        this.exploreArea(worldId, AGE_CHILD, 'OOT SPAWN', cloneAreaData(initAreaData), 'OOT SPAWN');
        this.exploreArea(worldId, AGE_ADULT, 'OOT SPAWN', cloneAreaData(initAreaData), 'OOT SPAWN');
      }
    }
  }
}
