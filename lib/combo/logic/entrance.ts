import { cloneDeep } from "lodash";

import { Random, sample } from "../random";
import { Settings } from "../settings";
import { DUNGEONS_REGIONS, ExprMap, World, WorldEntrance } from "./world";
import { Pathfinder, EntranceOverrides } from './pathfind';
import { Monitor } from "../monitor";
import { LogicEntranceError } from "./error";

export type EntranceShuffleResult = {
  overrides: {[k: string]: {[k:string]: { from: string, to: string }}};
  boss: number[];
  dungeons: number[];
};

const BOSS_INDEX_BY_DUNGEON = {
  DT: 0,
  DC: 1,
  JJ: 2,
  Forest: 3,
  Fire: 4,
  Water: 5,
  Shadow: 6,
  Spirit: 7,
  WF: 8,
  SH: 9,
  GB: 10,
  IST: 11,
} as {[k: string]: number};

const DUNGEON_INDEX = {
  DT: 0,
  DC: 1,
  JJ: 2,
  Forest: 3,
  Fire: 4,
  Water: 5,
  Shadow: 6,
  Spirit: 7,
  WF: 8,
  SH: 9,
  GB: 10,
  IST: 11,
  ST: 12,
  SSH: 13,
  OSH: 14,
  BotW: 15,
  IC: 16,
  GTG: 17,
  BtW: 18,
  ACoI: 19,
  SS: 20,
  BtWE: 21,
} as {[k: string]: number};;

export class LogicPassEntrances {
  private pathfinder: Pathfinder;
  private world: World;

  constructor(
    private readonly input: {
      world: World;
      settings: Settings;
      random: Random;
      monitor: Monitor;
      attempts: number;
    },
  ) {
    this.world = cloneDeep(input.world);
    this.pathfinder = new Pathfinder(input.world, input.settings);
  }
  private result: EntranceShuffleResult = {
    overrides: {},
    boss: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11],
    dungeons: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21],
  };

  private isAssignable(src: WorldEntrance, dst: WorldEntrance, overrides: EntranceOverrides, opts?: { mergeStoneTowers?: boolean, ownGame?: boolean }) {
    if (opts?.ownGame) {
      /* Check that the entrances are from the same game */
      /* TODO: Ugly */
      const prefixA = src.from.split(' ')[0];
      const prefixB = dst.from.split(' ')[0];
      if (prefixA !== prefixB) {
        return false;
      }
    }

    /* In beatable only, we don't care about logic at this point */
    if (this.input.settings.logic === 'beatable') {
      return true;
    }

    const dungeon = this.world.areas[dst.to].dungeon!;
    let locations: string[] = [];
    if ((dungeon === 'ST' || dungeon === 'IST') && opts?.mergeStoneTowers) {
      locations = [...this.world.dungeons['ST'], ...this.world.dungeons['IST']];
    } else {
      locations = [...this.world.dungeons[dungeon]];
    }

    overrides = { ...overrides, [src.from]: { [src.to]: dst.to } };
    const extraStartAreas = new Set<string>();

    if (!opts?.mergeStoneTowers) {
      /* Stone Tower and Inverted rely on each other's events */
      if (dungeon === 'ST') {
        extraStartAreas.add("MM Stone Tower Temple Inverted");
      } else if (dungeon === 'IST') {
        extraStartAreas.add("MM Stone Tower Temple");
      }
    }

    /* Pathfind with the override */
    const pathfinderState = this.pathfinder.run(null, { recursive: true, entranceOverrides: overrides, ignoreItems: true, extraStartAreas });

    /* Check if every location is reachable */
    if (locations.some(l => !pathfinderState.locations.has(l))) {
      return false;
    }
    return true;
  }

  private fixBosses() {
    const bossEntrances = this.world.entrances.filter(e => this.world.areas[e.to].boss);
    const bossEntrancesByDungeon: {[k: string]: WorldEntrance} = {};
    const overrides: EntranceOverrides = {};
    const combinations: {[k: string]: string[]} = {};
    const placed: {[k: string]: string} = {};

    /* Set up null overrides */
    for (const e of bossEntrances) {
      const override = overrides[e.from] || {};
      override[e.to] = null;
      overrides[e.from] = override;
      const dungeon = this.world.areas[e.to].dungeon!;
      bossEntrancesByDungeon[dungeon] = e;
    }

    /* Extract dungeons and events */
    const dungeons = Object.keys(bossEntrancesByDungeon);
    const events: {[k: string]: ExprMap} = {};
    const lastAreaByDungeon: {[k: string]: string} = {};
    const dungeonsBossAreas: {[k: string]: string[]} = {};
    for (const d of dungeons) {
      events[d] = {};
      dungeonsBossAreas[d] = [];
      for (const a in this.world.areas) {
        const area = this.world.areas[a];
        if (area.dungeon === d && area.boss) {
          events[d] = { ...events[d], ...area.events };
          area.events = {};
          lastAreaByDungeon[d] = a;
          dungeonsBossAreas[d].push(a);
        }
      }
    }

    /* Set up base reachability */
    for (const dungeonSrc of dungeons) {
      for (const dungeonDst of dungeons) {
        const src = bossEntrancesByDungeon[dungeonSrc];
        const dst = bossEntrancesByDungeon[dungeonDst];
        if (this.isAssignable(src, dst, overrides, { mergeStoneTowers: true, ownGame: this.input.settings.erBoss === 'ownGame' })) {
          const combination = combinations[dungeonDst] || [];
          combination.push(dungeonSrc);
          combinations[dungeonDst] = combination;
        }
      }
    }

    for (;;) {
      const keys = Object.keys(combinations);
      if (keys.length === 0) {
        break;
      }
      const sorted = keys.sort((a, b) => combinations[a].length - combinations[b].length);
      const boss = sorted[0];
      const comb = combinations[boss];
      if (comb.length === 0) {
        throw new LogicEntranceError(`Nowhere to place boss ${boss}`);
      }
      const src = sample(this.input.random, comb);
      placed[src] = boss;
      delete combinations[boss];
      for (const k of Object.keys(combinations)) {
        combinations[k] = combinations[k].filter(s => s !== src);
      }
    }

    /* Alter the world */
    for (const srcDungeon in placed) {
      const dstDungeon = placed[srcDungeon];
      const src = bossEntrancesByDungeon[srcDungeon];
      const dst = bossEntrancesByDungeon[dstDungeon];

      /* Mark the blue warp */
      this.result.boss[BOSS_INDEX_BY_DUNGEON[dstDungeon]] = BOSS_INDEX_BY_DUNGEON[srcDungeon];

      /* Replace the entrance */
      const srcArea = this.world.areas[src.from];
      const expr = srcArea.exits[src.to];
      delete srcArea.exits[src.to];
      srcArea.exits[dst.to] = expr;

      /* Replace the dungeon tag */
      for (const a of dungeonsBossAreas[dstDungeon]) {
        const area = this.world.areas[a];
        area.dungeon = srcDungeon;

        for (const l in area.locations) {
          this.world.regions[l] = DUNGEONS_REGIONS[srcDungeon];
          this.world.dungeons[srcDungeon].add(l);
          this.world.dungeons[dstDungeon].delete(l);
        }
      }

      /* Replace the events */
      const dstBoss = this.world.areas[lastAreaByDungeon[dstDungeon]];
      dstBoss.events = events[srcDungeon];

      /* Mark the override */
      const override = this.result.overrides[src.from] || {};
      override[src.to] = { from: dst.from, to: dst.to };
      this.result.overrides[src.from] = override;
    }
  }

  private fixDungeons() {
    /* Set the dungeon list */
    const validDungeons = new Set(['DT', 'DC', 'JJ', 'Forest', 'Fire', 'Water', 'Shadow', 'Spirit', 'WF', 'SH', 'GB', 'ST', 'IST']);
    if (this.input.settings.erMinorDungeons) {
      ['BotW', 'IC', 'GTG'].forEach(d => validDungeons.add(d));
    }
    if (this.input.settings.erSpiderHouses) {
      ['SSH', 'OSH'].forEach(d => validDungeons.add(d));
    }
    if (this.input.settings.erBeneathWell) {
      ['BtW', 'BtWE'].forEach(d => validDungeons.add(d));
    }
    if (this.input.settings.erIkanaCastle) {
      ['ACoI'].forEach(d => validDungeons.add(d));
    }
    if (this.input.settings.erSecretShrine) {
      ['SS'].forEach(d => validDungeons.add(d));
    }

    const dungeonEntrances = this.world.entrances.filter(e => !this.world.areas[e.from].dungeon && this.world.areas[e.to].dungeon && validDungeons.has(this.world.areas[e.to].dungeon!));
    const dungeonExits = this.world.entrances.filter(e => this.world.areas[e.from].dungeon && !this.world.areas[e.to].dungeon && validDungeons.has(this.world.areas[e.from].dungeon!));
    const entranceByDungeon: {[k: string]: WorldEntrance} = {};
    const exitsByDungeon: {[k: string]: WorldEntrance} = {};
    const overrides: EntranceOverrides = {};
    const combinations: {[k: string]: string[]} = {};
    const placed: {[k: string]: string} = {};

    /* Set up null overrides */
    for (const e of dungeonEntrances) {
      const override = overrides[e.from] || {};
      override[e.to] = null;
      overrides[e.from] = override;
      const dungeon = this.world.areas[e.to].dungeon!;
      entranceByDungeon[dungeon] = e;
    }

    for (const e of dungeonExits) {
      const override = overrides[e.from] || {};
      override[e.to] = null;
      overrides[e.from] = override;
      const dungeon = this.world.areas[e.from].dungeon!;
      exitsByDungeon[dungeon] = e;
    }

    /* Set up base reachability */
    const dungeons = Object.keys(entranceByDungeon);
    for (const dungeonSrc of dungeons) {
      for (const dungeonDst of dungeons) {
        const src = entranceByDungeon[dungeonSrc];
        const dst = entranceByDungeon[dungeonDst];
        const combination = combinations[dungeonDst] || [];
        if (this.isAssignable(src, dst, overrides, { ownGame: this.input.settings.erDungeons === 'ownGame' })) {
          combination.push(dungeonSrc);
        }
        combinations[dungeonDst] = combination;
      }
    }

    /* Place dungeons */
    for (;;) {
      const keys = Object.keys(combinations);
      if (keys.length === 0) {
        break;
      }
      const sorted = keys.sort((a, b) => combinations[a].length - combinations[b].length);
      const dungeon = sorted[0];
      const comb = combinations[dungeon];
      if (comb.length === 0) {
        throw new LogicEntranceError(`Nowhere to place ${dungeon}`);
      }
      const src = sample(this.input.random, comb);
      placed[src] = dungeon;
      delete combinations[dungeon];
      for (const k of Object.keys(combinations)) {
        combinations[k] = combinations[k].filter(s => s !== src);
      }
    }

    /* Alter the world */
    for (const srcDungeon in placed) {
      const dstDungeon = placed[srcDungeon];
      const srcEntrance = entranceByDungeon[srcDungeon];
      const dstEntrance = entranceByDungeon[dstDungeon];
      const srcExit = exitsByDungeon[dstDungeon];
      const dstExit = exitsByDungeon[srcDungeon];

      /* Mark the dungeon */
      this.result.dungeons[DUNGEON_INDEX[dstDungeon]] = DUNGEON_INDEX[srcDungeon];

      /* Replace the entrance */
      const entranceArea = this.world.areas[srcEntrance.from];
      const entranceExpr = entranceArea.exits[srcEntrance.to];
      delete entranceArea.exits[srcEntrance.to];
      entranceArea.exits[dstEntrance.to] = entranceExpr;

      const exitArea = this.world.areas[srcExit.from];
      const exitExpr = exitArea.exits[srcExit.to];
      delete exitArea.exits[srcExit.to];
      exitArea.exits[dstExit.to] = exitExpr;

      /* Mark the overrides */
      const entranceOverride = this.result.overrides[srcEntrance.from] || {};
      entranceOverride[srcEntrance.to] = { from: dstEntrance.from, to: dstEntrance.to };
      this.result.overrides[srcEntrance.from] = entranceOverride;

      const exitOverride = this.result.overrides[srcExit.from] || {};
      exitOverride[srcExit.to] = { from: dstExit.from, to: dstExit.to };
      this.result.overrides[srcExit.from] = exitOverride;
    }
  }

  run() {
    this.input.monitor.log(`Logic: Entrances (attempt ${this.input.attempts})`);

    if (this.input.settings.erDungeons !== 'none') {
      this.fixDungeons();
    }

    if (this.input.settings.erBoss !== 'none') {
      this.fixBosses();
    }

    return { world: this.world, entrances: this.result };
  }
};

