import { Random } from '../random';
import { Settings } from '../settings';
import { cloneWorld, World } from './world';
import { Pathfinder, PathfinderState } from './pathfind';
import { Monitor } from '../monitor';
import { Location } from './locations';
import { ItemPlacement } from './solve';
import { Item, Items, makePlayerItem, PlayerItems } from '../items';
import { ItemProperties } from './item-properties';
import { BOSS_DUNGEONS, BOSS_METADATA_BY_DUNGEON, END_BOSS_METADATA_BY_NAME } from './boss';
import { ENTRANCES } from '@ootmm/data';
import { Analysis } from './analysis';
import { AGE_ADULT, AGE_CHILD } from './constants';

type Triforce3Type = 'Power' | 'Courage' | 'Wisdom';
const TRIFORCE3_ITEMS: { [key in Triforce3Type]: Item } = {
  Power: Items.SHARED_TRIFORCE_POWER,
  Courage: Items.SHARED_TRIFORCE_COURAGE,
  Wisdom: Items.SHARED_TRIFORCE_WISDOM,
};

export type AnalysisPathTypeWotH = { type: 'woth' };
export type AnalysisPathTypeTriforce = { type: 'triforce', triforce: Triforce3Type };
export type AnalysisPathTypeDungeon = { type: 'dungeon', dungeon: string };
export type AnalysisPathTypeBoss = { type: 'boss', boss: string };
export type AnalysisPathTypeEndBoss = { type: 'end-boss', boss: string };
export type AnalysisPathTypeEvent = { type: 'event', event: string };
export type AnalysisPathType =
  | AnalysisPathTypeWotH
  | AnalysisPathTypeTriforce
  | AnalysisPathTypeDungeon
  | AnalysisPathTypeBoss
  | AnalysisPathTypeEndBoss
  | AnalysisPathTypeEvent;

export type AnalysisPathBase = { locations: Set<Location> };
export type AnalysisPath = AnalysisPathBase & AnalysisPathType;
export type AnalysisPathState = {
  key: string;
  locks: string[];
  path: AnalysisPath;
  pathfinderState: PathfinderState | null;
  pred: (x: PathfinderState) => boolean;
};

type PathEventData = {
  key: string;
  name: string;
  event: string;
};

export const PATH_EVENT_DATA: PathEventData[] = [
  { key: 'TimeTravel', name: 'Time Travel', event: 'OOT_TIME_TRAVEL' },
  { key: 'RainbowBridge', name: 'Rainbow Bridge', event: 'OOT_BRIDGE_OPEN' },
  { key: 'Termina', name: 'Termina', event: 'MM_ACCESS' },
  { key: 'Moon', name: 'Moon', event: 'MM_MOON_OPEN' },
];

export class LogicPassAnalysisPaths {
  private pathfinder: Pathfinder;
  private states: { [key: string]: AnalysisPathState } = {};

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
  }

  private makePathsLocations() {
    this.state.monitor.log(`Analysis - Path`);
    let count = 0;
    const locations = this.state.analysis.required;
    for (const loc of locations) {
      this.state.monitor.setProgress(count++, locations.size);
      const pathfinderState = this.pathfinder.run(null, { items: this.state.items, forbiddenLocations: new Set([loc]), recursive: true, stopAtGoal: true });
      for (const state of Object.values(this.states)) {
        if (!state.pred(pathfinderState)) {
          state.path.locations.add(loc);
        }
      }
    }
  }

  private makePath(opts: { key: string, pathType: AnalysisPathType, pathfinderState?: PathfinderState, pred: (x: PathfinderState) => boolean }) {
    const path: AnalysisPath = { ...opts.pathType, locations: new Set() };
    const state = { key: opts.key, locks: [], path, pathfinderState: opts.pathfinderState || null, pred: opts.pred };
    this.states[state.key] = state;
  }

  private pathfindNoEvent(worldId: number, event: string, noOotSpawn: boolean = false) {
    /* Create a distinct world */
    const worlds = [...this.state.worlds];
    const newWorld = cloneWorld(worlds[worldId]);
    worlds[worldId] = newWorld;

    /* Remove the event */
    for (const areaName of Object.keys(newWorld.areas)) {
      if (areaName === 'OOT SPAWN' && noOotSpawn)
        continue;
      const area = newWorld.areas[areaName];
      delete area.events[event];
    }

    /* Pathfind */
    const pathfinder = new Pathfinder(worlds, this.state.settings, this.state.startingItems);
    const pathfinderState = pathfinder.run(null, { items: this.state.items, recursive: true });

    return pathfinderState;
  }

  private dungeonsFromKey(dungeon: string) {
    const dungeons = [dungeon];
    if (dungeon === 'ST' && !this.state.settings.extraHintRegions) {
      dungeons.push('IST');
    }
    if (dungeon === 'BtW') {
      dungeons.push('BtWE');
    }
    return dungeons;
  }

  private dungeonAreas(dungeon: string, worldId: number) {
    const dungeons = this.dungeonsFromKey(dungeon);
    const areas: string[] = [];

    for (const [areaName, area] of Object.entries(this.state.worlds[worldId].areas)) {
      if (area.dungeon && dungeons.includes(area.dungeon)) {
        areas.push(areaName);
      }
    }

    return areas
  }

  private dungeonPathfindState(areas: string[], worldId: number) {
    /* Create a distinct world */
    const worlds = [...this.state.worlds];
    const newWorld = cloneWorld(worlds[worldId]);
    worlds[worldId] = newWorld;

    if (areas.length === 0)
      return null;

    /* Remove the dungeon */
    for (const areaName of areas) {
      const area = newWorld.areas[areaName];
      area.exits = {};
      area.events = {};
      area.locations = {};
    }

    /* Pathfind */
    const pathfinder = new Pathfinder(worlds, this.state.settings, this.state.startingItems);
    const pathfinderState = pathfinder.run(null, { items: this.state.items, recursive: true });

    return pathfinderState;
  }

  private makePathDungeon(dungeon: string) {
    for (let i = 0; i < this.state.worlds.length; i++) {
      const areas = this.dungeonAreas(dungeon, i);
      const pathfinderState = this.dungeonPathfindState(areas, i);
      if (!pathfinderState || pathfinderState.goal) {
        continue;
      }

      /* The dungeon is required for this player */
      const pred = (x: PathfinderState) => areas.some(a => x.ws[i].ages[AGE_CHILD].areas.has(a) || x.ws[i].ages[AGE_ADULT].areas.has(a));
      this.makePath({
        key: `dungeon.${dungeon}.${i}`,
        pathType: { type: 'dungeon', dungeon },
        pathfinderState,
        pred,
      });
    }
  }

  private makePathBoss(dungeon: string) {
    const meta = BOSS_METADATA_BY_DUNGEON.get(dungeon)!;

    for (let i = 0; i < this.state.worlds.length; i++) {
      const pathfinderState = this.pathfindNoEvent(i, meta.event, true);
      if (!pathfinderState || pathfinderState.goal) {
        continue;
      }

      /* The boss is required for this player */
      const pred = (x: PathfinderState) => x.ws[i].events.has(meta.event);
      this.makePath({
        key: `boss.${dungeon}.${i}`,
        pathType: { type: 'boss', boss: dungeon },
        pathfinderState,
        pred,
      });
    }
  }

  private makePathEndBoss(boss: string) {
    const meta = END_BOSS_METADATA_BY_NAME.get(boss)!;

    for (let i = 0; i < this.state.worlds.length; i++) {
      const pathfinderState = this.pathfindNoEvent(i, meta.event, true);
      if (!pathfinderState || pathfinderState.goal) {
        continue;
      }

      /* The boss is required for this player */
      const pred = (x: PathfinderState) => x.ws[i].events.has(meta.event);
      this.makePath({
        key: `end-boss.${boss}.${i}`,
        pathType: { type: 'end-boss', boss },
        pathfinderState,
        pred,
      });
    }
  }

  private makePathTriforce3(triforce: Triforce3Type) {
    const triforceItem = TRIFORCE3_ITEMS[triforce];

    for (let i = 0; i < this.state.worlds.length; i++) {
      const triforcePlayerItem = makePlayerItem(triforceItem, i);
      const triforcePlayerItemLocs = Array.from(this.state.items.entries()).filter(([_, item]) => item === triforcePlayerItem).map(([loc, _]) => loc);
      const pathfinder = new Pathfinder(this.state.worlds, this.state.settings, this.state.startingItems);
      const pathfinderState = pathfinder.run(null, { recursive: true, items: this.state.items, forbiddenLocations: new Set(triforcePlayerItemLocs) });
      if (!pathfinderState || pathfinderState.goal) {
        continue;
      }

      /* The piece is required for this player */
      const pred = (x: PathfinderState) => (triforcePlayerItemLocs.length === 0 || triforcePlayerItemLocs.every(l => x.locations.has(l)));
      this.makePath({
        key: `triforce3.${triforce}.${i}`,
        pathType: { type: 'triforce', triforce },
        pathfinderState,
        pred,
      });
    }
  }

  private makePathEvent(eventKey: string) {
    const meta = PATH_EVENT_DATA.find(x => x.key === eventKey)!;

    for (let i = 0; i < this.state.worlds.length; i++) {
      const pathfinderState = this.pathfindNoEvent(i, meta.event);
      if (!pathfinderState || pathfinderState.goal) {
        continue;
      }

      /* The event is required for this player */
      const pred = (x: PathfinderState) => x.ws[i].events.has(meta.event);
      this.makePath({
        key: `event.${eventKey}.${i}`,
        pathType: { type: 'event', event: eventKey },
        pathfinderState,
        pred,
      });
    }
  }

  private checkLocks() {
    for (const state of Object.values(this.states)) {
      if (state.pathfinderState) {
        for (const stateName in this.states) {
          const otherState = this.states[stateName];
          if (otherState === state)
            continue;
          if (!otherState.pred(state.pathfinderState)) {
            /* We have a lock */
            state.locks.push(otherState.key);
          }
        }
      }
    }
  }

  private makePaths() {
    /* WotH path */
    this.makePath({
      key: 'woth',
      pathType: { type: 'woth' },
      pred: _ => false,
    });

    if (this.state.settings.goal === 'triforce3') {
      this.makePathTriforce3('Power');
      this.makePathTriforce3('Courage');
      this.makePathTriforce3('Wisdom');
    }

    if (this.state.settings.hintPathDungeons) {
      const dungeons = ['DT', 'DC', 'JJ', 'Forest', 'Fire', 'Water', 'Shadow', 'Spirit', 'BotW', 'IC', 'GTG', 'WF', 'SH', 'GB', 'ST', 'SSH', 'OSH', 'BtW', 'SS', 'ACoI'];
      if (this.state.settings.extraHintRegions) {
        dungeons.push('IST');
      }
      if (!this.state.settings.erPiratesWorld) {
        dungeons.push('PF');
      }
      if (this.state.settings.erGanonCastle) {
        dungeons.push('Ganon');
      }
      if (this.state.settings.erGanonTower) {
        dungeons.push('Tower');
      }
      if (this.state.settings.erMoon) {
        dungeons.push('Moon');
      }

      for (const dungeon of dungeons) {
        this.makePathDungeon(dungeon);
      }
    }

    if (this.state.settings.hintPathBoss) {
      for (const dungeon of BOSS_DUNGEONS) {
        this.makePathBoss(dungeon);
      }
    }

    if (this.state.settings.hintPathEndBoss) {
      for (const boss of END_BOSS_METADATA_BY_NAME.keys()) {
        this.makePathEndBoss(boss);
      }
    }

    if (this.state.settings.hintPathEvents) {
      for (const ev of PATH_EVENT_DATA) {
        this.makePathEvent(ev.key);
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
      this.makePaths();
      this.makePathsLocations();
      this.checkLocks();
      this.cleanPaths();
    }

    const paths = Object.values(this.states).map(x => x.path).filter(x => x.locations.size > 0);

    return { analysis: { ...this.state.analysis, paths } };
  }
}
