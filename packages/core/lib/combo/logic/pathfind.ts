import { cloneDeep } from 'lodash';
import { Settings } from '../settings';
import { AreaData, ExprFunc, ExprResult, isDefaultRestrictions, MM_TIME_SLICES } from './expr';

import { Location, locationData, makeLocation, makePlayerLocations } from './locations';
import { World } from './world';
import { isLocationLicenseGranting, isLocationRenewable } from './locations';
import { ItemPlacement } from './solve';
import { CountMap, countMapAdd } from '../util';
import { Item, itemByID, Items, ItemsCount, PlayerItems } from '../items';
import { CompiledWorld, compileWorld } from './world-compiler';

export const AGES = ['child', 'adult'] as const;

export type Age = typeof AGES[number];

type PathfinderWorldState = {
  compiledWorld: CompiledWorld;
  atoms: Uint8Array;
  uncollectedLocations: Set<string>;
  items: ItemsCount;
  licenses: ItemsCount;
  renewables: ItemsCount;
  forbiddenReachableLocations: Set<string>;
  restrictedLocations?: Set<string>;
  forbiddenLocations?: Set<string>;
  events: Set<string>;
}

export type PathfinderState = {
  goal: boolean;
  ganonMajora: boolean;
  changed: boolean;
  ws: PathfinderWorldState[];
  previousAssumedItems: PlayerItems;

  /* Output */
  locations: Set<Location>;
  newLocations: Set<Location>;
  gossips: Set<string>[];
}

function makeWorldState(startingItems: ItemsCount, world: World): PathfinderWorldState {
  let compiled = world.compiled;
  if (!compiled) {
    compiled = compileWorld(world);
    world.compiled = compiled;
  }

  const atoms = new Uint8Array((compiled.atoms.length + 7) / 8);

  return {
    compiledWorld: compiled,
    atoms,
    uncollectedLocations: new Set(),
    items: new Map(startingItems),
    licenses: new Map(startingItems),
    renewables: new Map(),
    forbiddenReachableLocations: new Set(),
    events: new Set(),
  };
}

const defaultWorldStates = (startingItems: PlayerItems, worlds: World[]) => {
  const ws: PathfinderWorldState[] = [];

  for (let worldId = 0; worldId < worlds.length; ++worldId) {
    const world = worlds[worldId];
    const itemCount: ItemsCount = new Map;
    for (const [pi, count] of startingItems) {
      if (pi.player === worldId) {
        itemCount.set(pi.item, count);
      }
    }
    ws.push(makeWorldState(itemCount, world));
  }

  return ws;
}

const defaultState = (startingItems: PlayerItems, worlds: World[]): PathfinderState => {
  const gossips: Set<string>[] = [];

  for (let worldId = 0; worldId < worlds.length; ++worldId) {
    gossips.push(new Set());
  }

  return {
    previousAssumedItems: new Map,
    changed: false,
    goal: false,
    ganonMajora: false,
    ws: defaultWorldStates(startingItems, worlds),
    locations: new Set(),
    newLocations: new Set(),
    gossips,
  };
};

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
    this.state = state ? (this.opts.inPlace ? state : cloneDeep(state)) : defaultState(this.startingItems, this.worlds);

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

  private addLocationDelayed(worldId: number, loc: string) {
    if (this.opts.recursive) {
      this.addLocation(worldId, loc);
    } else {
      const globalLoc = makeLocation(loc, worldId);
      this.state.locations.add(globalLoc);
      this.state.newLocations.add(globalLoc);
    }
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
    } else {
      ws.uncollectedLocations.add(loc);
    }
  }

  private hasAtom(index: number, atoms: Uint8Array) {
    const bytePos = index >> 3;
    const bitPos = index & 7;
    const mask = 1 << bitPos;
    return !!(atoms[bytePos] & mask);
  }

  private pathfindStep() {
    let anyChange = false;

    /* Clear new locations */
    this.state.newLocations.clear();

    for (let worldId = 0; worldId < this.worlds.length; ++worldId) {
      const ws = this.state.ws[worldId];
      const compiled = ws.compiledWorld;
      const atomsState = ws.atoms;
      const atoms = compiled.atoms;

      if (this.opts.singleWorld !== undefined && this.opts.singleWorld !== worldId) {
        continue;
      }

      /* Pathfind using atoms as much as we can */;
      for (;;) {
        let changed = false;
        for (let i = 0; i < atoms.length; ++i) {
          const bytePos = i >> 3;
          const bitPos = i & 7;
          const mask = 1 << bitPos;

          if (atomsState[bytePos] & mask) {
            continue;
          }

          /* Check if atom is true */
          const atom = atoms[i];
          let result: boolean;
          switch (atom.type) {
          case 'true': result = true; break;
          case 'false': result = false; break;
          case 'item': result = (ws.items.get(atom.item) || 0) >= atom.count; break;
          case 'or': result = atom.children.some(x => this.hasAtom(x, atomsState)); break;
          case 'and': result = atom.children.every(x => this.hasAtom(x, atomsState)); break;
          }

          if (result) {
            atomsState[bytePos] |= mask;
            changed = true;
          }
        }

        if (!changed)
          break;
        else
          anyChange = true;
      }

      /* Pathfind locations */
      for (const [l, atomId] of compiled.locations.entries()) {
        const ll = makeLocation(l, worldId);
        if (this.state.locations.has(ll)) {
          continue;
        }
        const bitPos = atomId & 7;
        const bytePos = atomId >> 3;
        const mask = 1 << bitPos;
        if (atomsState[bytePos] & mask) {
          this.addLocationDelayed(worldId, l);
          anyChange = true;
        }
      }

      /* Pathfind events */
      for (const [e, atomId] of compiled.events.entries()) {
        if (ws.events.has(e)) {
          continue;
        }
        const bitPos = atomId & 7;
        const bytePos = atomId >> 3;
        const mask = 1 << bitPos;
        if (atomsState[bytePos] & mask) {
          ws.events.add(e);
          anyChange = true;
        }
      }
    }

    /* Add delayed locations */
    for (const globalLoc of this.state.newLocations) {
      const locD = locationData(globalLoc);
      this.addLocation(locD.world as number, locD.id);
    }

    return anyChange;
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
  }
}
