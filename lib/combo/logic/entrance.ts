import { cloneDeep } from "lodash";

import { Random, sample, shuffle } from "../random";
import { Settings } from "../settings";
import { DUNGEONS_REGIONS, ExprMap, World, WorldArea, WorldEntrance } from "./world";
import { Pathfinder, EntranceOverrides } from './pathfind';
import { Monitor } from "../monitor";
import { LogicEntranceError, LogicError } from "./error";
import { Expr, exprTrue } from "./expr";

type Entrance = {
  from: string;
  to: string;
  expr: Expr;
};

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
  PF: 22,
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
    this.pathfinder = new Pathfinder(this.world, input.settings);
  }
  private result: EntranceShuffleResult = {
    overrides: {},
    boss: Object.values(BOSS_INDEX_BY_DUNGEON),
    dungeons: Object.values(DUNGEON_INDEX),
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

  private isDungeonTransition(e: WorldEntrance) {
    const from = this.world.areas[e.from];
    const to = this.world.areas[e.to];

    /* Overworld to overworld */
    if (!from.dungeon && !to.dungeon) {
      return false;
    }

    /* Same dungeon */
    if (from.dungeon === to.dungeon) {
      return false;
    }

    /* BtW/BtWE */
    if (from.dungeon === 'BtW' && to.dungeon === 'BtWE') {
      return false;
    }
    if (from.dungeon === 'BtWE' && to.dungeon === 'BtW') {
      return false;
    }

    return true;
  }

  private isAssignableNew(from: Entrance, to: Entrance, opts?: { ownGame: boolean }) {
    const dungeon = this.world.areas[to.to].dungeon!;

    /* Reject wrong game */
    if (opts?.ownGame) {
      /* TODO: Ugly */
      const prefixA = from.from.split(' ')[0];
      const prefixB = to.to.split(' ')[0];
      if (prefixA !== prefixB) {
        return false;
      }
    }

    /* Beatable only */
    if (this.input.settings.logic === 'beatable') {
      return true;
    }

    /* Apply an override */
    this.world.areas[from.from].exits[to.to] = from.expr;

    /* If the dungeon is ST or IST, we need to allow the other dungeon */
    if (dungeon === 'ST') {
      this.world.areas['OOT SPAWN'].exits['MM Stone Tower Temple Inverted'] = exprTrue();
    }
    if (dungeon === 'IST') {
      this.world.areas['OOT SPAWN'].exits['MM Stone Tower Temple'] = exprTrue();
    }

    /* Check if the new world is valid */
    const pathfinderState = this.pathfinder.run(null, { ignoreItems: true, recursive: true });

    /* Restore the override */
    delete this.world.areas[from.from].exits[to.to];
    delete this.world.areas['OOT SPAWN'].exits['MM Stone Tower Temple Inverted'];
    delete this.world.areas['OOT SPAWN'].exits['MM Stone Tower Temple'];

    /* Get the list of required locations */
    let locations: string[];
    if (['ST', 'IST'].includes(dungeon)) {
      locations = [...this.world.dungeons['ST'], ...this.world.dungeons['IST']];
    } else {
      locations = Array.from(this.world.dungeons[dungeon]);
    }

    /* Check if the new world is valid */
    if (locations.every(l => pathfinderState.locations.has(l))) {
      return true;
    } else {
      return false;
    }
  }

  private fixDungeons() {
    /* Set the dungeon list */
    let shuffledDungeons = new Set(['DT', 'DC', 'JJ', 'Forest', 'Fire', 'Water', 'Shadow', 'Spirit', 'WF', 'SH', 'GB', 'ST', 'IST']);
    if (this.input.settings.erMinorDungeons) {
      ['BotW', 'IC', 'GTG'].forEach(d => shuffledDungeons.add(d));
    }
    if (this.input.settings.erSpiderHouses) {
      ['SSH', 'OSH'].forEach(d => shuffledDungeons.add(d));
    }
    if (this.input.settings.erPirateFortress) {
      shuffledDungeons.add('PF');
    }
    if (this.input.settings.erBeneathWell) {
      ['BtW', 'BtWE'].forEach(d => shuffledDungeons.add(d));
    }
    if (this.input.settings.erIkanaCastle) {
      shuffledDungeons.add('ACoI');
    }
    if (this.input.settings.erSecretShrine) {
      shuffledDungeons.add('SS');
    }

    /* Get the transitions and exprs */
    const dungeonTransitions = this.world.entrances
      .filter(e => this.isDungeonTransition(e))
      .filter(e => shuffledDungeons.has(this.world.areas[e.from].dungeon!) || shuffledDungeons.has(this.world.areas[e.to].dungeon!));

    const dungeonEntrances: {[k: string]: Entrance} = {};
    const dungeonExits: {[k: string]: Entrance} = {};

    for (const e of dungeonTransitions) {
      /* Get the transition exprs */
      const from = this.world.areas[e.from];
      const to = this.world.areas[e.to];
      const expr = from.exits[e.to];

      /* Entrance */
      if (to.dungeon) {
        dungeonEntrances[to.dungeon] = { from: e.from, to: e.to, expr };
      }

      /* Exit */
      if (from.dungeon) {
        dungeonExits[from.dungeon] = { from: e.from, to: e.to, expr };
      }

      /* Remove the transition */
      delete from.exits[e.to];
    }

    /* Assign the dungeons */
    const dungeons = shuffle(this.input.random, Array.from(shuffledDungeons));
    const dungeonsDest = new Set(dungeons);
    while (dungeons.length > 0) {
      const dungeon = dungeons.pop()!;
      const candidates = shuffle(this.input.random, Array.from(dungeonsDest));
      let destDungeon: string | null = null;

      for (const c of candidates) {
        const assignable = this.isAssignableNew(dungeonEntrances[dungeon], dungeonEntrances[c], { ownGame: this.input.settings.erDungeons === 'ownGame' });
        if (assignable) {
          destDungeon = c;
          break;
        }
      }

      if (!destDungeon) {
        throw new LogicError('Unable to assign a dungeon to location: ' + dungeon);
      }

      /* Change the world */
      dungeonsDest.delete(destDungeon);
      const sourceEntrance = dungeonEntrances[dungeon];
      const sourceExit = dungeonExits[dungeon];
      const destEntrance = dungeonEntrances[destDungeon];
      const destExit = dungeonExits[destDungeon];
      this.world.areas[sourceEntrance.from].exits[destEntrance.to] = sourceEntrance.expr;
      this.world.areas[destExit.from].exits[sourceExit.to] = destExit.expr;

      /* Mark the overrides */
      const entranceOverride = this.result.overrides[sourceEntrance.from] || {};
      entranceOverride[sourceEntrance.to] = { from: destEntrance.from, to: destEntrance.to };
      this.result.overrides[sourceEntrance.from] = entranceOverride;

      const exitOverride = this.result.overrides[destExit.from] || {};
      exitOverride[destExit.to] = { from: sourceExit.from, to: sourceExit.to };
      this.result.overrides[destExit.from] = exitOverride;

      /* Store the dungeon */
      this.result.dungeons[DUNGEON_INDEX[destDungeon]] = DUNGEON_INDEX[dungeon];
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

