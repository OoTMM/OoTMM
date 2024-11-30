import { Random } from '../random';
import { Settings } from '../settings';
import { cloneWorld, World } from './world';
import { Pathfinder, PathfinderState } from './pathfind';
import { Monitor } from '../monitor';
import { Location } from './locations';
import { ItemPlacement } from './solve';
import { Item, Items, PlayerItems } from '../items';
import { ItemProperties } from './item-properties';
import { BOSS_DUNGEONS, BOSS_METADATA_BY_DUNGEON, END_BOSS_METADATA_BY_NAME } from './boss';
import { ENTRANCES } from '@ootmm/data';
import { Analysis } from './analysis';

type Triforce3Type = 'Power' | 'Courage' | 'Wisdom';
const TRIFORCE3_ITEMS: { [key in Triforce3Type]: Item } = {
  Power: Items.SHARED_TRIFORCE_POWER,
  Courage: Items.SHARED_TRIFORCE_COURAGE,
  Wisdom: Items.SHARED_TRIFORCE_WISDOM,
};

export type AnalysisPathTypeWotH = { type: 'woth' };
export type AnalysisPathTypeTriforce = { type: 'triforce', triforce: Triforce3Type };
export type AnalysisPathTypeBoss = { type: 'boss', boss: string };
export type AnalysisPathTypeEndBoss = { type: 'end-boss', boss: string };
export type AnalysisPathType =
  | AnalysisPathTypeWotH
  | AnalysisPathTypeTriforce
  | AnalysisPathTypeBoss
  | AnalysisPathTypeEndBoss;

export type AnalysisPathBase = { locations: Set<Location> };
export type AnalysisPath = AnalysisPathBase & AnalysisPathType;
export type AnalysisPathState = {
  key: string;
  name: string;
  locks: string[];
  path: AnalysisPath;
  pathfinderState: PathfinderState;
  pred: (x: PathfinderState) => boolean;
};

export class LogicPassAnalysisPaths {
  private pathfinder: Pathfinder;
  private states: { [key: string]: AnalysisPathState } = {};
  private paths: AnalysisPath[];

  constructor(
    private readonly state: {
      analysis: Analysis;
      settings: Settings;
      random: Random;
      worlds: World[];
      items: ItemPlacement;
      monitor: Monitor;
      startingItems: PlayerItems;
      itemProperties: ItemProperties;
    },
  ){
    this.pathfinder = new Pathfinder(this.state.worlds, this.state.settings, this.state.startingItems);
    this.paths = [];
  }

  private registerState(state: AnalysisPathState) {
    this.states[state.key] = state;
  }

  private makePathLocations(name: string, pred: (x: PathfinderState) => boolean) {
    const path = new Set<Location>();
    this.state.monitor.log(`Analysis - ${name}`);
    let count = 0;
    const locations = new Set(this.state.analysis.required);
    for (const loc of locations) {
      this.state.monitor.setProgress(count++, locations.size);
      const pathfinderState = this.pathfinder.run(null, { items: this.state.items, forbiddenLocations: new Set([loc]), recursive: true, stopAtGoal: true });
      if (!pred(pathfinderState)) {
        path.add(loc);
      }
    }
    return path;
  }

  private addPath(path: AnalysisPath) {
    if (path.locations.size) {
      this.paths.push(path);
    }
  }

  private bossPathfindState(dungeon: string, worldId: number) {
    const meta = BOSS_METADATA_BY_DUNGEON.get(dungeon)!;

    /* Create a distinct world */
    const worlds = [...this.state.worlds];
    const newWorld = cloneWorld(worlds[worldId]);
    worlds[worldId] = newWorld;
    const area = newWorld.areas[ENTRANCES[meta.entrance].to];
    if (!area)
      return null;

    /* Remove the boss */
    area.exits = {};
    const pathfinder = new Pathfinder(worlds, this.state.settings, this.state.startingItems);
    const pathfinderState = pathfinder.run(null, { items: this.state.items, recursive: true, stopAtGoal: true });

    return pathfinderState;
  }

  private endBossPathfindState(boss: string, worldId: number) {
    const meta = END_BOSS_METADATA_BY_NAME.get(boss)!;

    /* Create a distinct world */
    const worlds = [...this.state.worlds];
    const newWorld = cloneWorld(worlds[worldId]);
    worlds[worldId] = newWorld;
    const area = newWorld.areas[meta.area];
    if (!area)
      return null;

    /* Remove the boss */
    area.events = {};
    const pathfinder = new Pathfinder(worlds, this.state.settings, this.state.startingItems);
    const pathfinderState = pathfinder.run(null, { items: this.state.items, recursive: true, stopAtGoal: true });

    return pathfinderState;
  }

  private registerStateBoss(dungeon: string) {
    const meta = BOSS_METADATA_BY_DUNGEON.get(dungeon)!;

    for (let i = 0; i < this.state.worlds.length; i++) {
      const pathfinderState = this.bossPathfindState(dungeon, i);
      if (!pathfinderState || pathfinderState.goal) {
        continue;
      }

      /* The boss is required for this player */
      const pred = (x: PathfinderState) => x.ws[i].events.has(meta.event);
      this.registerState({
        key: `boss.${dungeon}.${i}`,
        locks: [],
        path: { type: 'boss', boss: dungeon, locations: new Set },
        name: `Path to Boss`,
        pathfinderState,
        pred,
      });
    }
  }

  private registerStateEndBoss(boss: string) {
    const meta = END_BOSS_METADATA_BY_NAME.get(boss)!;

    for (let i = 0; i < this.state.worlds.length; i++) {
      const pathfinderState = this.endBossPathfindState(boss, i);
      if (!pathfinderState || pathfinderState.goal) {
        continue;
      }

      /* The boss is required for this player */
      const pred = (x: PathfinderState) => x.ws[i].events.has(meta.event);
      this.registerState({
        key: `end-boss.${boss}.${i}`,
        locks: [],
        path: { type: 'end-boss', boss, locations: new Set },
        name: `Path to End Boss`,
        pathfinderState,
        pred,
      });
    }
  }

  /*
  private makePathTriforce3(state: AnalysisPathState, states: AnalysisPathState[], triforce: Triforce3Type) {
    const triforceItem = TRIFORCE3_ITEMS[triforce];
    const locs = Array.from(this.state.items.entries()).filter(([_, item]) => item.item === triforceItem).map(([loc, _]) => loc);
    const path = this.makePath(`Path of ${triforce}`, x => locs.every(l => x.locations.has(l)));
    this.addPath({ type: 'triforce', triforce, locations: path });
  }
  */

  /*
  private makePathsOld() {
    this.addPath({ type: 'woth', locations: this.requiredLocs });

    if (this.state.settings.goal === 'triforce3') {
      const locsPower = Array.from(this.state.items.entries()).filter(([_, item]) => item.item === Items.SHARED_TRIFORCE_POWER).map(([loc, _]) => loc);
      const locsCourage = Array.from(this.state.items.entries()).filter(([_, item]) => item.item === Items.SHARED_TRIFORCE_COURAGE).map(([loc, _]) => loc);
      const locsWisdom = Array.from(this.state.items.entries()).filter(([_, item]) => item.item === Items.SHARED_TRIFORCE_WISDOM).map(([loc, _]) => loc);

      const pathPower = this.makePath('Path of Power', x => locsPower.every(l => x.locations.has(l)));
      const pathCourage = this.makePath('Path of Courage', x => locsCourage.every(l => x.locations.has(l)));
      const pathWisdom = this.makePath('Path of Wisdom', x => locsWisdom.every(l => x.locations.has(l)));

      this.addPath({ type: 'triforce', triforce: 'Power', locations: pathPower });
      this.addPath({ type: 'triforce', triforce: 'Courage', locations: pathCourage });
      this.addPath({ type: 'triforce', triforce: 'Wisdom', locations: pathWisdom });
    }

    if (this.state.settings.hintPathBoss) {
      for (const dungeon of BOSS_DUNGEONS) {
        const meta = BOSS_METADATA_BY_DUNGEON.get(dungeon)!;
        this.registerState({
          key: `boss.${dungeon}`,
          locks: [],
          path: { type: 'boss', boss: dungeon, locations: new Set() },
          run: (state, states) => this.makePathBoss(state, states, dungeon),
        });
      }
    }

    if (this.state.settings.hintPathEndBoss) {
      for (const boss of END_BOSS_METADATA_BY_NAME.keys()) {
        //this.makePathEndBoss(boss);
        this.registerState({ key: `end-boss.${boss}`, locks: [], path: { type: 'end-boss', boss, locations: new Set() }, run: (state, states) => this.makePathEndBoss(state, states, boss) });
      }
    }
  }*/

  private makePath(state: AnalysisPathState) {
    /* Build the actual path */
    state.path.locations = this.makePathLocations(state.name, state.pred);

    /* Check for locks */
    for (const stateName in this.states) {
      const otherState = this.states[stateName];
      if (otherState === state)
        continue;
      if (!otherState.pred(state.pathfinderState)) {
        /* We have a lock */
        state.locks.push(otherState.key);
      }
    }

    /* Add the actual path */
    this.addPath(state.path);
  }

  private makePaths() {
    for (const state of Object.values(this.states)) {
      this.makePath(state);
    }
  }

  private registerStates() {
    if (this.state.settings.hintPathBoss) {
      for (const dungeon of BOSS_DUNGEONS) {
        this.registerStateBoss(dungeon);
      }
    }

    if (this.state.settings.hintPathEndBoss) {
      for (const boss of END_BOSS_METADATA_BY_NAME.keys()) {
        this.registerStateEndBoss(boss);
      }
    }
  }

  private cleanPaths() {
    const rawLocations = new Map<string, Set<Location>>();

    /* Collect all locations */
    for (const state of Object.values(this.states)) {
      rawLocations.set(state.key, new Set(state.path.locations));
    }

    for (const state of Object.values(this.states)) {
      const rawStateLocs = rawLocations.get(state.key)!;
      for (const lock of state.locks) {
        const otherState = this.states[lock];

        /* The other state is locked, so it shouldn't have any location that this state has */
        for (const loc of rawStateLocs) {
          otherState.path.locations.delete(loc);
        }
      }
    }
  }

  run() {
    if (this.state.settings.logic !== 'none') {
      this.registerStates();
      this.makePaths();
      this.cleanPaths();
    }

    return { analysis: { ...this.state.analysis, paths: this.paths } };
  }
}

