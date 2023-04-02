import { cloneDeep } from 'lodash';
import { Settings } from '../settings';
import { Expr } from './expr';

import { addItem, combinedItems, isItemConsumable, Items } from './items';
import { ItemPlacement } from './solve';
import { World } from './world';
import { isLocationRenewable } from './locations';

export const AGES = ['child', 'adult'] as const;

export type Age = typeof AGES[number];

type PathfinderStateAge = {
  queueExits: {[k: string]: Set<string>};
}

export type PathfinderState = {
  items: Items;
  areas: {
    child: Set<string>;
    adult: Set<string>;
  },
  locations: Set<string>;
  newLocations: Set<string>;
  uncollectedLocations: Set<string>;
  events: Set<string>;
  gossip: Set<string>;
  goal: boolean;
  started: boolean;
  ageStates: {
    child: PathfinderStateAge;
    adult: PathfinderStateAge;
  }
}

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
    child: new Set(),
    adult: new Set(),
  },
  locations: new Set(),
  newLocations: new Set(),
  uncollectedLocations: new Set(),
  events: new Set(),
  gossip: new Set(),
  goal: false,
  started: false,
  ageStates: {
    child: {
      queueExits: {},
    },
    adult: {
      queueExits: {},
    },
  }
});

export type EntranceOverrides = {[k: string]: {[k: string]: string | null}};
type PathfinderOptions = {
  assumedItems?: Items;
  items?: ItemPlacement;
  entranceOverrides?: EntranceOverrides;
  ignoreItems?: boolean;
  recursive?: boolean;
  gossip?: boolean;
  stopAtGoal?: boolean;
  restrictedLocations?: Set<string>;
  forbiddenLocations?: Set<string>;
  extraStartAreas?: Set<string>;
};

export class Pathfinder {
  private opts!: PathfinderOptions;
  private state!: PathfinderState;
  private resolveOverride!: (area: string, exit: string) => string;

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

  private evalExpr(expr: Expr, age: Age) {
    return expr({ items: this.state.items, age, events: this.state.events, ignoreItems: this.opts.ignoreItems || false });
  }

  private exploreArea(area: string, age: Age) {
    const a = this.world.areas[area];
    const exits = Object.keys(a.exits).filter(x => !this.state.areas[age].has(x));
    if (exits.length > 0) {
      this.state.ageStates[age].queueExits[area] = new Set(exits);
    }
    this.state.areas[age].add(area);
  }

  private pathfindSingleArea(area: string, age: Age) {
    const remainingExits = this.state.ageStates[age].queueExits[area];
    const remainigExitsDup = new Set(remainingExits);
    const newExits = new Set<string>();
    if (!remainingExits) {
      return;
    }
    const a = this.world.areas[area];
    for (const originalExitArea of remainigExitsDup) {
      const expr = a.exits[originalExitArea];
      const exitArea = this.resolveOverride(area, originalExitArea);
      if (this.state.areas[age].has(exitArea)) {
        remainingExits.delete(originalExitArea);
        continue;
      }
      if (this.evalExpr(expr, age)) {
        remainingExits.delete(originalExitArea);
        newExits.add(exitArea);
      }
    }
    if (remainingExits.size === 0) {
      delete this.state.ageStates[age].queueExits[area];
    }
    for (const exitArea of newExits) {
      this.exploreArea(exitArea, age);
      this.pathfindSingleArea(exitArea, age);
    }
  }

  private resolveOverrideImpl(area: string, exitName: string) {
    const overrides = this.opts.entranceOverrides?.[area] || {};
    const override = overrides[exitName];
    if (override === null) {
      return "VOID";
    }
    if (override !== undefined) {
      return override;
    }
    return exitName;
  }

  private resolveOverrideIdentity(area: string, exitName: string) {
    return exitName;
  }

  private pathfindAreas(age: Age) {
    const areas = Object.keys(this.state.ageStates[age].queueExits);
    for (const area of areas) {
      this.pathfindSingleArea(area, age);
    }
  }

  private pathfindEvents(age: Age) {
    let changed = false;
    for (const area of this.state.areas[age]) {
      const events = this.world.areas[area].events;
      for (const event in events) {
        if (this.state.events.has(event)) {
          continue;
        }
        const expr = events[event];
        if (this.evalExpr(expr, age)) {
          this.state.events.add(event);
          changed = true;
        }
      }
    }
    return changed;
  }

  private pathfindGossip(age: Age) {
    let changed = false;
    for (const area of this.state.areas[age]) {
      const gossips = this.world.areas[area].gossip;
      for (const gossip in gossips) {
        if (this.state.gossip.has(gossip)) {
          continue;
        }
        const expr = gossips[gossip];
        if (this.evalExpr(expr, age)) {
          this.state.gossip.add(gossip);
          changed = true;
        }
      }
    }
    return changed;
  }

  private pathfindLocations(age: Age) {
    const newLocations = new Set<string>();
    const oldLocations = this.state.locations;
    for (const area of this.state.areas[age]) {
      const locations = this.world.areas[area].locations;
      for (const location in locations) {
        if (this.opts.restrictedLocations && !this.opts.restrictedLocations.has(location)) {
          continue;
        }
        if (this.opts.forbiddenLocations && this.opts.forbiddenLocations.has(location)) {
          continue;
        }
        if (oldLocations.has(location) || newLocations.has(location)) {
          continue;
        }
        const expr = locations[location];
        if (this.evalExpr(expr, age)) {
          newLocations.add(location);
        }
      }
    }
    if (newLocations.size > 0) {
      for (const loc of newLocations) {
        this.state.locations.add(loc);
        this.state.newLocations.add(loc);
      }
      return true;
    }
    return false;
  }

  private pathfind() {
    /* Bind resolver */
    if (this.opts.entranceOverrides) {
      this.resolveOverride = this.resolveOverrideImpl;
    } else {
      this.resolveOverride = this.resolveOverrideIdentity;
    }

    /* Handle initial state */
    if (!this.state.started) {
      this.state.started = true;
      this.exploreArea('OOT SPAWN', 'child');
      this.exploreArea('OOT SPAWN', 'adult');

      const extraStartAreas = Array.from(this.opts.extraStartAreas || []);
      for (const area of extraStartAreas) {
        this.exploreArea(area, 'child');
        this.exploreArea(area, 'adult');
      }
    }

    /* Handle no logic */
    if (this.settings.logic === 'none') {
      this.state.locations = new Set(Object.keys(this.world.checks));
      this.state.events = new Set(Object.values(this.world.areas).map(x => Object.keys(x.events || {})).flat());
      const allAreas = new Set(Object.keys(this.world.areas));
      this.state.areas = {
        child: allAreas,
        adult: allAreas,
      };
      if (this.opts.gossip) {
        this.state.gossip = new Set(Object.values(this.world.areas).map(x => Object.keys(x.gossip || {})).flat());
      }
      return;
    }

    /* Collect previous locations */
    for (const location of this.state.uncollectedLocations) {
      const item = this.opts.items?.[location];
      if (item) {
        if (!isItemConsumable(item) || isLocationRenewable(this.world, location)) {
          addItem(this.state.items, item);
        }
        this.state.uncollectedLocations.delete(location);
      }
    }

    /* Pathfind */
    for (;;) {
      const changed = this.pathfindStep();
      if (!changed || !this.opts.recursive) {
        break;
      }
    }
  }

  private pathfindStep() {
    this.state.newLocations = new Set();
    let anyChange = false;

    for (;;) {
      let eventChange = false;

      /* Propagate to areas */
      for (const age of AGES) {
        this.pathfindAreas(age);
      }

      /* Reach all areas & events */
      for (;;) {
        let changed = false;
        for (const age of AGES) {
          const newEvents = this.pathfindEvents(age);
          changed ||= newEvents;
          eventChange ||= newEvents;
          if (this.opts.gossip) {
            changed ||= this.pathfindGossip(age);
          }
        }
        this.state.goal = this.state.events.has('OOT_GANON') && this.state.events.has('MM_MAJORA');
        if (this.opts.stopAtGoal && this.state.goal) {
          return false;
        }
        anyChange = anyChange || changed;
        if (!changed) {
          break;
        }
      }

      if (!eventChange)
        break;
    }

    /* Reach all locations */
    for (;;) {
      let changed = false;
      for (const age of AGES) {
        changed ||= this.pathfindLocations(age);
      }
      anyChange = anyChange || changed;
      if (!changed) {
        break;
      }
    }

    /* Collect items */
    for (const location of this.state.newLocations) {
      const item = this.opts.items?.[location];
      if (item) {
        if (!isItemConsumable(item) || isLocationRenewable(this.world, location)) {
          addItem(this.state.items, item);
        }
      } else {
        this.state.uncollectedLocations.add(location);
      }
    }

    return anyChange;
  }
}
