import { cloneDeep } from 'lodash';

import { Random, sample, shuffle } from '../random';
import { Settings } from '../settings';
import { DUNGEONS_REGIONS, ExprMap, ExprParsers, World, WorldEntrance } from './world';
import { Pathfinder } from './pathfind';
import { Monitor } from '../monitor';
import { LogicEntranceError, LogicError } from './error';
import { Expr, exprAnd, exprTrue } from './expr';
import { makeLocation } from './locations';
import { LogicPassSolver } from './solve';
import { ItemsCount } from '../items';

export type EntranceShuffleResult = {
  overrides: Map<string, string>;
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
  Ganon: 23,
  Tower: 24,
} as {[k: string]: number};;

type PlaceOpts = {
  overworld?: boolean;
  noSongOfTime?: boolean;
  ownGame?: boolean;
};

export class LogicPassEntrances {
  private pathfinder!: Pathfinder;
  private world!: World;

  constructor(
    private readonly input: {
      world: World;
      exprParsers: ExprParsers;
      settings: Settings;
      random: Random;
      monitor: Monitor;
      fixedLocations: Set<string>,
      pool: ItemsCount;
      renewableJunks: ItemsCount;
    },
  ) {
  }

  private result!: EntranceShuffleResult;

  private getExpr(original: string) {
    const entrance = this.world.entrances.get(original)!;
    const from = this.input.world.areas[entrance.from];
    const expr = from.exits[entrance.to];
    if (!expr) {
      throw new Error(`No expr for ${original}`);
    }
    return expr;
  }

  private isAssignableNew(original: string, replacement: string, opts?: { ownGame?: boolean, locations?: string[] }) {
    const originalEntrance = this.world.entrances.get(original)!;
    const replacementEntrance = this.world.entrances.get(replacement)!;
    const dungeon = this.input.world.areas[replacementEntrance.to].dungeon!;

    /* Reject wrong game */
    if (opts?.ownGame) {
      if (originalEntrance.game !== replacementEntrance.game) {
        return false;
      }
    }

    /* Not all locations */
    if (this.input.settings.logic !== 'allLocations') {
      return true;
    }

    /* Apply an override */
    this.world.areas[originalEntrance.from].exits[replacementEntrance.to] = this.getExpr(original);

    /* If the dungeon is ST or IST, we need to allow the other dungeon */
    if (dungeon === 'ST') {
      this.world.areas['OOT SPAWN'].exits['MM Stone Tower Temple Inverted'] = exprTrue();
    }
    if (dungeon === 'IST') {
      this.world.areas['OOT SPAWN'].exits['MM Stone Tower Temple'] = exprTrue();
    }

    /* Check if the new world is valid */
    const pathfinderState = this.pathfinder.run(null, { singleWorld: true, ignoreItems: true, recursive: true });

    /* Restore the override */
    delete this.world.areas[originalEntrance.from].exits[replacementEntrance.to]
    delete this.world.areas['OOT SPAWN'].exits['MM Stone Tower Temple Inverted'];
    delete this.world.areas['OOT SPAWN'].exits['MM Stone Tower Temple'];

    /* Get the list of required locations */
    let locations: string[];
    if (opts?.locations) {
      locations = opts.locations;
    } else if (['ST', 'IST'].includes(dungeon)) {
      locations = [...this.world.dungeons['ST'], ...this.world.dungeons['IST']];
    } else {
      locations = Array.from(this.world.dungeons[dungeon]);
    }

    /* Turn into world 0 locations */
    const worldLocs = locations.map(l => makeLocation(l, 0));

    /* Check if the new world is valid */
    if (!(worldLocs.every(l => pathfinderState.locations.has(l))))
      return false;

    /* Ganon's tower check */
    if (dungeon === 'Tower' && ['ganon', 'both'].includes(this.input.settings.goal) && !pathfinderState.ws[0].events.has('OOT_GANON'))
      return false;

    return true;
  }

  private fixBosses() {
    const bossEntrances = [...this.world.entrances.values()].filter(e => e.type === 'boss');
    const bossEntrancesByDungeon = new Map<string, WorldEntrance>();
    const bossEvents = new Map<string, ExprMap>();
    const bossAreas = new Map<string, string[]>();
    const bossLocations = new Map<string, string[]>();

    /* Collect every boss event */
    for (const a in this.world.areas) {
      const area = this.world.areas[a];
      const dungeon = area.dungeon!;
      if (area.boss) {
        /* Events */
        if (!bossEvents.has(dungeon)) {
          bossEvents.set(dungeon, {});
        }
        const oldEvents = bossEvents.get(dungeon)!;
        bossEvents.set(dungeon, { ...oldEvents, ...area.events });

        /* Areas */
        if (!bossAreas.has(dungeon)) {
          bossAreas.set(dungeon, []);
        }
        bossAreas.get(dungeon)!.push(a);

        /* Locations */
        if (!bossLocations.has(dungeon)) {
          bossLocations.set(dungeon, []);
        }
        const locs = bossLocations.get(dungeon)!;
        for (const l in area.locations) {
          locs.push(l);
        }

        /* Remove the event */
        area.events = {};
      }
    }

    /* Collect the entrances and delete the original ones */
    for (const e of bossEntrances) {
      const areaFrom = this.world.areas[e.from];
      const areaTo = this.world.areas[e.to];

      /* We have a boss entrance */
      const dungeon = areaTo.dungeon!;
      bossEntrancesByDungeon.set(dungeon, e);

      /* Remove the entrance */
      delete areaFrom.exits[e.to];
    }

    /* Actually shuffle bosses */
    const bosses = shuffle(this.input.random, Array.from(bossEntrancesByDungeon.keys()));
    const bossesToPlace = new Set(bosses);
    for (const srcBoss of bosses) {
      const src = bossEntrancesByDungeon.get(srcBoss)!;
      const candidates = shuffle(this.input.random, Array.from(bossesToPlace));
      let dstBoss: string | null = null;
      for (const boss of candidates) {
        const dst = bossEntrancesByDungeon.get(boss)!;
        if (this.isAssignableNew(src.id, dst.id, { ownGame: this.input.settings.erBoss === 'ownGame', locations: bossLocations.get(boss)! })) {
          dstBoss = boss;
          break;
        }
      }
      if (dstBoss === null) {
        throw new LogicEntranceError(`Nowhere to place boss ${srcBoss}`);
      }
      bossesToPlace.delete(dstBoss);

      /* We found a boss - place it */
      const dst = bossEntrancesByDungeon.get(dstBoss)!;
      this.place(src.id, dst.id, { noSongOfTime: true });

      /* Mark the boss */
      this.result.boss[BOSS_INDEX_BY_DUNGEON[dstBoss]] = BOSS_INDEX_BY_DUNGEON[srcBoss];

      /* Add the events */
      const areaNames = bossAreas.get(dstBoss)!;
      const lastAreaName = areaNames[areaNames.length - 1];
      const lastArea = this.world.areas[lastAreaName];
      lastArea.events = { ...lastArea.events, ...bossEvents.get(srcBoss)! };

      /* Change the associated dungeon */
      for (const a of bossAreas.get(dstBoss)!) {
        const area = this.world.areas[a];
        area.dungeon = srcBoss;
        area.region = DUNGEONS_REGIONS[srcBoss];

        for (const loc in area.locations) {
          this.world.regions[loc] = DUNGEONS_REGIONS[srcBoss];
          this.world.dungeons[dstBoss].delete(loc);
          this.world.dungeons[srcBoss].add(loc);
        }
      }
    }
  }

  private fixDungeons() {
    /* Set the dungeon list */
    let shuffledDungeons = new Set(['DT', 'DC', 'JJ', 'Forest', 'Fire', 'Water', 'Shadow', 'Spirit', 'WF', 'SH', 'GB', 'ST', 'IST']);
    if (this.input.settings.erMinorDungeons) {
      ['BotW', 'IC', 'GTG'].forEach(d => shuffledDungeons.add(d));
    }
    if (this.input.settings.erGanonCastle) {
      shuffledDungeons.add('Ganon');
    }
    if (this.input.settings.erGanonTower) {
      shuffledDungeons.add('Tower');
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
    const dungeonTransitions = [...this.world.entrances.values()]
      .filter(e => e.type === 'dungeon')
      .filter(e => shuffledDungeons.has(this.world.areas[e.from].dungeon!) || shuffledDungeons.has(this.world.areas[e.to].dungeon!));

    const dungeonEntrances = new Map<string, WorldEntrance>();

    for (const e of dungeonTransitions) {
      /* Get the exit */
      const exit = this.world.entrances.get(e.reverse!)!;

      /* Get the various areas */
      const entranceAreaFrom = this.world.areas[e.from];
      const entranceAreaTo = this.world.areas[e.to];
      const exitAreaFrom = this.world.areas[exit.from];

      /* Save the entrance */
      dungeonEntrances.set(entranceAreaTo.dungeon!, e);

      /* Remove the transitions */
      delete entranceAreaFrom.exits[e.to];
      delete exitAreaFrom.exits[exit.to];
    }

    /* Assign the dungeons */
    const dungeons = shuffle(this.input.random, Array.from(shuffledDungeons));
    const dungeonsDest = new Set(dungeons);
    while (dungeons.length > 0) {
      const dungeon = dungeons.pop()!;
      const candidates = shuffle(this.input.random, Array.from(dungeonsDest));
      let destDungeon: string | null = null;

      for (const c of candidates) {
        const src = dungeonEntrances.get(dungeon)!.id;
        const dst = dungeonEntrances.get(c)!.id;
        const assignable = this.isAssignableNew(src, dst, { ownGame: this.input.settings.erDungeons === 'ownGame' });
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
      const sourceEntranceId = dungeonEntrances.get(dungeon)!.id;
      const destEntranceId = dungeonEntrances.get(destDungeon)!.id;

      this.place(sourceEntranceId, destEntranceId, { noSongOfTime: true });

      /* Store the dungeon */
      this.result.dungeons[DUNGEON_INDEX[destDungeon]] = DUNGEON_INDEX[dungeon];
    }
  }

  private songOfTime(e: Expr): Expr {
    const subcond = this.input.exprParsers.mm.parse('can_reset_time');
    return exprAnd([e, subcond]);
  }

  private placeSingle(original: string, replacement: string, opts?: PlaceOpts) {
    opts = opts || {};
    const entranceOriginal = this.world.entrances.get(original)!;
    const entranceReplacement = this.world.entrances.get(replacement)!;

    /* Change the world */
    let expr = this.getExpr(original);
    if (entranceOriginal.game === 'oot' && entranceReplacement.game === 'mm') {
      if (opts.overworld) {
        this.world.areas[entranceOriginal.from].exits['MM GLOBAL'] = expr;
      }
      if (!opts.noSongOfTime) {
        expr = this.songOfTime(expr);
      }
    }
    this.world.areas[entranceOriginal.from].exits[entranceReplacement.to] = expr;

    /* Mark the override */
    this.result.overrides.set(original, replacement);
  }

  private place(original: string, replacement: string, opts?: PlaceOpts) {
    const entranceOriginal = this.world.entrances.get(original)!;
    const entranceReplacement = this.world.entrances.get(replacement)!;

    this.placeSingle(original, replacement, opts);
    if (entranceOriginal.reverse && entranceReplacement.reverse) {
      this.placeSingle(entranceReplacement.reverse, entranceOriginal.reverse, opts);
    }
  }

  private placePool(pool: string[], opts?: PlaceOpts) {
    /* Get overworld entrances */
    const entrances = new Set([...this.world.entrances.values()].filter(e => pool.includes(e.type)));

    /* Delete the overworld entrances from the world */
    for (const e of entrances) {
      delete this.world.areas[e.from].exits[e.to];
      const reverse = e.reverse;
      if (reverse) {
        const r = this.world.entrances.get(reverse)!;
        delete this.world.areas[r.from].exits[r.to];
      }
    }

    /* Shuffle the entrances */
    const shuffledEntrances = shuffle(this.input.random, [...entrances]);

    /* Apply the entrances */
    for (let e of shuffledEntrances) {
      let candidates = [...entrances];
      if (opts?.ownGame) {
        candidates = candidates.filter(c => c.game === e.game);
      }
      const newE = sample(this.input.random, candidates);
      this.place(e.id, newE.id, opts);
      entrances.delete(newE);
    }
  }

  private placeRegions() {
    this.placePool(['region'], { overworld: true, ownGame: this.input.settings.erRegions === 'ownGame' });
  }

  private placeIndoors() {
    const pool = ['indoors'];
    if (this.input.settings.erIndoorsExtra) {
      pool.push('indoors-extra');
    }
    this.placePool(pool, { ownGame: this.input.settings.erIndoors === 'ownGame' });
  }

  private propagateRegionsStep() {
    let changed = false;
    for (const areaName of Object.keys(this.world.areas)) {
      const a = this.world.areas[areaName];
      if (a.region === 'NONE' || a.region === 'ENTRANCE')
        continue;
      /* We need to propagate the region */
      for (const exitName of Object.keys(a.exits)) {
        const exitArea = this.world.areas[exitName];
        if (exitArea.region === 'ENTRANCE') {
          exitArea.region = a.region;
          for (const loc of Object.keys(exitArea.locations)) {
            if (this.world.regions[loc] === 'ENTRANCE') {
              this.world.regions[loc] = a.region;
            }
          }
          changed = true;
        }
      }
    }
    return changed;
  }

  private propagateRegions() {
    /* Propagate regions */
    for (;;) {
      if (!this.propagateRegionsStep()) {
        break;
      }
    }
  }

  private validate() {
    if (this.input.settings.logic === 'none')
      return;
    const pathfinderState = this.pathfinder.run(null, { singleWorld: true, ignoreItems: true, recursive: true });

    /* We want to make sure everything that needs to is reachable */
    if (!pathfinderState.goal) {
      throw new LogicEntranceError('Goal is not reachable');
    }

    if (this.input.settings.logic === 'allLocations') {
      if (pathfinderState.locations.size < this.world.locations.size) {
        throw new LogicEntranceError('Not all locations are reachable');
      }
    }

    /* We don't want child to reach the Fairy OGC exit, and the other way around too */
    const forbiddenAreasChild = ['OOT Near Fairy Fountain Defense'];
    const forbiddenAreasAdult = ['OOT Near Fairy Fountain Din'];

    for (const area of forbiddenAreasChild) {
      if (pathfinderState.ws[0].areas.child.has(area)) {
        throw new LogicEntranceError(`Child can reach ${area}`);
      }
    }

    for (const area of forbiddenAreasAdult) {
      if (pathfinderState.ws[0].areas.adult.has(area)) {
        throw new LogicEntranceError(`Adult can reach ${area}`);
      }
    }

    /* Validate using the solver */
    const solver = new LogicPassSolver({ ...this.input, world: this.world });
    solver.validate();
  }

  private runAttempt() {
    /* Init */
    this.result = {
      overrides: new Map,
      boss: Object.values(BOSS_INDEX_BY_DUNGEON),
      dungeons: Object.values(DUNGEON_INDEX),
    };
    let anyEr = false;
    this.world = { ...this.input.world, areas: cloneDeep(this.input.world.areas), regions: cloneDeep(this.input.world.regions), dungeons: cloneDeep(this.input.world.dungeons) };
    this.pathfinder = new Pathfinder(this.world, this.input.settings);

    if (this.input.settings.erRegions !== 'none') {
      anyEr = true;
      this.placeRegions();
    }

    if (this.input.settings.erIndoors !== 'none') {
      anyEr = true;
      this.placeIndoors();
    }

    if (this.input.settings.erDungeons !== 'none') {
      anyEr = true;
      this.fixDungeons();
    }

    if (this.input.settings.erBoss !== 'none') {
      anyEr = true;
      this.fixBosses();
    }

    if (anyEr) {
      this.validate();
    }

    this.propagateRegions();

    return { world: this.world, entrances: this.result };
  }

  run() {
    let attempts = 1;

    for (;;) {
      try {
        this.input.monitor.log(`Logic: Entrances (attempt ${attempts})`);
        return this.runAttempt();
      } catch (e) {
        if (!(e instanceof LogicError) || attempts >= 1000)
          throw e;
        attempts++;
      }
    }
  }
};

