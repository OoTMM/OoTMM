import { cloneDeep } from 'lodash';

import { Random, shuffle } from '../random';
import { Settings } from '../settings';
import { DUNGEONS_REGIONS, ExprMap, ExprParsers, World, WorldEntrance } from './world';
import { Pathfinder } from './pathfind';
import { Monitor } from '../monitor';
import { LogicEntranceError, LogicError } from './error';
import { Expr, exprAnd, exprTrue } from './expr';
import { Game } from '../config';

type Entrance = {
  from: string;
  to: string;
  expr: Expr;
  game: Game;
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
      exprParsers: ExprParsers;
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

  private isAssignableNew(from: Entrance, to: Entrance, opts?: { ownGame?: boolean, locations?: string[] }) {
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
    if (opts?.locations) {
      locations = opts.locations;
    } else if (['ST', 'IST'].includes(dungeon)) {
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

  private fixBosses() {
    const bossEntrances = this.world.entrances.filter(e => e.type === 'boss');
    const bossEntrancesByDungeon: {[k: string]: Entrance[]} = {};
    const bossEvents: {[k: string]: ExprMap} = {};
    const bossAreas: {[k: string]: string[]} = {};
    const bossLocations: {[k: string]: string[]} = {};

    /* Collect every boss event */
    for (const a in this.world.areas) {
      const area = this.world.areas[a];
      const dungeon = area.dungeon;
      if (area.boss) {
        bossEvents[dungeon!] = { ...bossEvents[dungeon!], ...area.events };
        bossAreas[dungeon!] = [...(bossAreas[dungeon!] || []), a];
        bossLocations[dungeon!] = [...(bossLocations[dungeon!] || []), ...Object.keys(area.locations)];

        /* Remove the event */
        area.events = {};
      }
    }

    for (const e of bossEntrances) {
      const from = this.world.areas[e.from];
      const to = this.world.areas[e.to];

      /* We have a boss entrance */
      const dungeon = to.dungeon!;
      const entrance = { ...e, expr: from.exits[e.to] };
      bossEntrancesByDungeon[dungeon] ||= [];
      bossEntrancesByDungeon[dungeon].push(entrance);

      /* Remove the entrance */
      delete from.exits[e.to];
    }

    /* Actually shuffle bosses */
    const bosses = shuffle(this.input.random, Object.keys(bossEntrancesByDungeon));
    const bossesToPlace = new Set(bosses);
    for (const srcBoss of bosses) {
      const srcs = bossEntrancesByDungeon[srcBoss];
      const candidates = shuffle(this.input.random, Array.from(bossesToPlace));
      let dstBoss: string | null = null;
      for (const boss of candidates) {
        const dst = bossEntrancesByDungeon[boss][0];
        if (srcs.some(src => this.isAssignableNew(src, dst, { ownGame: this.input.settings.erBoss === 'ownGame', locations: bossLocations[boss] }))) {
          dstBoss = boss;
          break;
        }
      }
      if (dstBoss === null) {
        throw new LogicEntranceError(`Nowhere to place boss ${srcBoss}`);
      }
      bossesToPlace.delete(dstBoss);

      /* We found a boss - place it */
      const dst = bossEntrancesByDungeon[dstBoss][0];
      for (const src of srcs) {
        this.place(src, dst, false);
      }

      /* Mark the boss */
      this.result.boss[BOSS_INDEX_BY_DUNGEON[dstBoss]] = BOSS_INDEX_BY_DUNGEON[srcBoss];

      /* Add the events */
      const lastAreaName = bossAreas[dstBoss][bossAreas[dstBoss].length - 1];
      const lastArea = this.world.areas[lastAreaName];
      lastArea.events = { ...lastArea.events, ...bossEvents[srcBoss] };

      /* Change the associated dungeon */
      for (const a of bossAreas[dstBoss]) {
        const area = this.world.areas[a];
        area.dungeon = srcBoss;

        for (const loc in area.locations) {
          this.world.regions[loc] = DUNGEONS_REGIONS[srcBoss];
          this.world.dungeons[dstBoss].delete(loc);
          this.world.dungeons[srcBoss].add(loc);
        }
      }
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
        dungeonEntrances[to.dungeon] = { from: e.from, to: e.to, expr, game: e.game };
      }

      /* Exit */
      if (from.dungeon) {
        dungeonExits[from.dungeon] = { from: e.from, to: e.to, expr, game: e.game };
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

      this.place(sourceEntrance, destEntrance, false);
      this.place(destExit, sourceExit, false);

      /* Store the dungeon */
      this.result.dungeons[DUNGEON_INDEX[destDungeon]] = DUNGEON_INDEX[dungeon];
    }
  }

  private songOfTime(e: Expr): Expr {
    const subcond = this.input.exprParsers.mm.parse('can_reset_time');
    return exprAnd([e, subcond]);
  }

  private place(src: Entrance, dst: Entrance, overworld: boolean) {
    /* Change the world */
    let expr = src.expr;
    if (overworld && src.game === 'oot' && dst.game === 'mm') {
      this.world.areas[src.from].exits['MM GLOBAL'] = expr;
      expr = this.songOfTime(expr);
    }
    this.world.areas[src.from].exits[dst.to] = expr;

    /* Mark the override */
    this.result.overrides[src.from] = { ...this.result.overrides[src.from], [src.to]: { from: dst.from, to: dst.to } };
  }

  private placeOverworld() {
    /* Get overworld entrances */
    const entrances: Entrance[] = this.world.entrances.filter(e => e.type === 'overworld').map(e => ({
      from: e.from,
      to: e.to,
      expr: this.world.areas[e.from].exits[e.to],
      game: e.game,
    }));

    /* Delete the overworld entrances from the world */
    for (const e of entrances) {
      delete this.world.areas[e.from].exits[e.to];
    }

    /* Shuffle the entrances */
    const indices = shuffle(this.input.random, [...Array(entrances.length / 2).keys()]);

    /* Apply the entrances */
    for (let i = 0; i < indices.length; ++i) {
      const entranceSrc = entrances[i * 2 + 0];
      const entranceDst = entrances[indices[i] * 2 + 0];
      const exitSrc = entrances[i * 2 + 1];
      const exitDst = entrances[indices[i] * 2 + 1];

      this.place(entranceSrc, entranceDst, true);
      this.place(exitDst, exitSrc, true);
    }
  }

  run() {
    this.input.monitor.log(`Logic: Entrances (attempt ${this.input.attempts})`);

    if (this.input.settings.erOverworld) {
      this.placeOverworld();
    }

    if (this.input.settings.erDungeons !== 'none') {
      this.fixDungeons();
    }

    if (this.input.settings.erBoss !== 'none') {
      this.fixBosses();
    }

    return { world: this.world, entrances: this.result };
  }
};

