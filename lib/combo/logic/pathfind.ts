import { cloneDeep } from 'lodash';
import { Settings } from '../settings';
import { Expr } from './expr';

import { addItem, combinedItems, Items } from './items';
import { ItemPlacement } from './solve';
import { World } from './world';

export const AGES = ['child', 'adult'] as const;

export type Age = typeof AGES[number];

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
}

const defaultState = (settings: Settings): PathfinderState => ({
  items: { ...settings.startingItems },
  areas: {
    child: new Set(['OOT SPAWN']),
    adult: new Set(['OOT SPAWN']),
  },
  locations: new Set(),
  newLocations: new Set(),
  uncollectedLocations: new Set(),
  events: new Set(),
  gossip: new Set(),
});

export type EntranceOverrides = {[k: string]: {[k: string]: string | null}};
type PathfinderOptions = {
  assumedItems?: Items;
  items?: ItemPlacement;
  entranceOverrides?: EntranceOverrides;
  ignoreItems?: boolean;
  recursive?: boolean;
  gossip?: boolean;
  restrictedLocations?: Set<string>;
  forbiddenLocations?: Set<string>;
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

  private evalExpr(expr: Expr, age: Age) {
    return expr({ items: this.state.items, age, events: this.state.events, ignoreItems: this.opts.ignoreItems || false });
  }

  private pathfindAreas(age: Age) {
    const newAreas = new Set<string>();
    const oldAreas = this.state.areas[age];
    for (const area of oldAreas) {
      const worldArea = this.world.areas[area];
      if (!worldArea) {
        throw new Error(`Unknown area: ${area}`);
      }
      const exits = worldArea.exits;
      for (let exit in exits) {
        const expr = exits[exit];
        const overrides = this.opts.entranceOverrides?.[area] || {};
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
        if (this.evalExpr(expr, age)) {
          newAreas.add(exit);
        }
      }
    }
    if (newAreas.size > 0) {
      newAreas.forEach(x => this.state.areas[age].add(x));
      return true;
    }
    return false;
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
        if (oldLocations.has(location) || newLocations.has(location)) {
          continue;
        }
        if (this.opts.restrictedLocations && !this.opts.restrictedLocations.has(location)) {
          continue;
        }
        if (this.opts.forbiddenLocations && this.opts.forbiddenLocations.has(location)) {
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
    /* Collect previous locations */
    for (const location of this.state.uncollectedLocations) {
      const item = this.opts.items?.[location];
      if (item) {
        addItem(this.state.items, item);
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

    /* Reach all areas & events */
    for (;;) {
      let changed = false;
      for (const age of AGES) {
        changed ||= this.pathfindAreas(age);
        changed ||= this.pathfindEvents(age);
        if (this.opts.gossip) {
          changed ||= this.pathfindGossip(age);
        }
      }
      anyChange = anyChange || changed;
      if (!changed) {
        break;
      }
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

    /* Collect every item */
    for (const location of this.state.newLocations) {
      const item = this.opts.items?.[location];
      if (item) {
        addItem(this.state.items, item);
        anyChange = true;
      } else {
        this.state.uncollectedLocations.add(location);
      }
    }

    return anyChange;
  }
}
