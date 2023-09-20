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
  atomsQueue: number[];
  atomsInQueue: Set<number>;
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
    atomsQueue: [1],
    atomsInQueue: new Set([1]),
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
      if (isLocationLicenseGranting(world, globalLoc)) {
        countMapAdd(otherWs.licenses, playerItem.item);
      }
      this.queueItem(playerItem.player, playerItem.item);
    } else {
      ws.uncollectedLocations.add(loc);
    }
  }

  private hasAtom(ws: PathfinderWorldState, index: number) {
    const bytePos = index >> 3;
    const bitPos = index & 7;
    const mask = 1 << bitPos;
    return !!(ws.atoms[bytePos] & mask);
  }

  private queueItem(worldId: number, item: Item) {
    const ws = this.state.ws[worldId];
    const compiled = ws.compiledWorld;
    const atomsToCheck = compiled.itemsToAtoms.get(item) || [];
    for (const atomId of atomsToCheck) {
      if (ws.atomsInQueue.has(atomId) || this.hasAtom(ws, atomId)) {
        continue;
      }
      ws.atomsQueue.push(atomId);
      ws.atomsInQueue.add(atomId);
    }
  }

  private pathfindStep() {
    let changed = false;
    this.state.newLocations.clear();

    for (let worldId = 0; worldId < this.worlds.length; ++worldId) {
      if (this.pathfindWorld(worldId)) {
        changed = true;
      }
    }

    /* Add delayed locations */
    for (const globalLoc of this.state.newLocations) {
      const locD = locationData(globalLoc);
      this.addLocation(locD.world as number, locD.id);
      changed = true;
    }

    return changed;
  }

  private pathfindWorld(worldId: number) {
    let changed = false;
    const ws = this.state.ws[worldId];
    const compiled = ws.compiledWorld;

    if (this.opts.singleWorld !== undefined && this.opts.singleWorld !== worldId) {
      return;
    }

    /* Pathfind using atom dependencies */
    const atoms = compiled.atoms;
    const atomsState = ws.atoms;
    const atomsQueue = ws.atomsQueue;
    const atomsInQueue = ws.atomsInQueue;
    while (atomsQueue.length) {
      const atomId = atomsQueue.pop()!;

      /* Check if the atom was already processed */
      if (this.hasAtom(ws, atomId)) {
        atomsInQueue.delete(atomId);
        continue;
      }

      /* Evaluate the atom */
      const atom = atoms[atomId];
      let result: boolean;
      switch (atom.type) {
      case 'true': result = true; break;
      case 'false': result = false; break;
      case 'item': result = (ws.items.get(atom.item) || 0) >= atom.count; break;
      case 'or': result = atom.children.some(x => this.hasAtom(ws, x)); break;
      case 'and': result = atom.children.every(x => this.hasAtom(ws, x)); break;
      }

      /* If the atom is false, let's just stop there */
      if (!result) {
        atomsInQueue.delete(atomId);
        continue;
      }

      /* Mark the atom as true */
      atomsState[atomId >> 3] |= 1 << (atomId & 7);
      atomsInQueue.delete(atomId);
      changed = true;

      /* Push atom dependencies */
      const atomDeps = compiled.atomsToAtoms.get(atomId) || [];
      for (const dep of atomDeps) {
        if (atomsInQueue.has(dep) || this.hasAtom(ws, dep)) {
          continue;
        }
        atomsQueue.push(dep);
        atomsInQueue.add(dep);
      }

      /* Push new locations */
      const newLocs = compiled.atomsToLocations.get(atomId) || [];
      for (const loc of newLocs) {
        this.addLocationDelayed(worldId, loc);
      }

      /* Push new events */
      const newEvents = compiled.atomsToEvents.get(atomId) || [];
      for (const e of newEvents) {
        if (ws.events.has(e)) {
          continue;
        }
        ws.events.add(e);
      }
    }

    return changed;
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
    /* Started */
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

          this.queueItem(playerItem.player, playerItem.item);
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
