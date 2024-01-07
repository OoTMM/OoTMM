import { ENTRANCES } from '@ootmm/data';
import { Random, sample, shuffle } from '../random';
import { Settings } from '../settings';
import { DUNGEONS_REGIONS, ExprMap, World, cloneWorld } from './world';
import { Pathfinder } from './pathfind';
import { Monitor } from '../monitor';
import { LogicEntranceError, LogicError } from './error';
import { Expr, exprAge, exprAnd, exprFalse, exprTrue } from './expr';
import { Location, makeLocation } from './locations';
import { LogicPassSolver } from './solve';
import { PlayerItems } from '../items';
import { ItemProperties } from './item-properties';

type Entrance = keyof typeof ENTRANCES;

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
  Moon: 25,
} as {[k: string]: number};;

type PlaceOpts = {
  overworld?: boolean;
  ownGame?: boolean;
};

export class LogicPassEntrances {
  private pathfinder!: Pathfinder;
  private worlds!: World[];

  constructor(
    private readonly input: {
      worlds: World[];
      settings: Settings;
      random: Random;
      monitor: Monitor;
      fixedLocations: Set<Location>,
      pool: PlayerItems;
      renewableJunks: PlayerItems;
      startingItems: PlayerItems;
      itemProperties: ItemProperties;
    },
  ) {
  }

  private getExpr(worldId: number, original: Entrance) {
    const world = this.worlds[worldId];
    const originalWorld = this.input.worlds[worldId];
    const eData = ENTRANCES[original];
    const from = originalWorld.areas[eData.from];
    const expr = from.exits[eData.to];
    if (!expr) {
      throw new Error(`No expr for ${original}`);
    }
    return expr;
  }

  private isAssignableNew(worldId: number, original: Entrance, replacement: Entrance, opts?: { ownGame?: boolean, locations?: string[] }) {
    const world = this.worlds[worldId];
    const originalWorld = this.input.worlds[worldId];
    const originalEntrance = ENTRANCES[original];
    const replacementEntrance = ENTRANCES[replacement];
    const dungeon = originalWorld.areas[replacementEntrance.to].dungeon!;

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
    world.areas[originalEntrance.from].exits[replacementEntrance.to] = this.getExpr(worldId, original);

    /* If the dungeon is ST or IST, we need to allow the other dungeon */
    if (dungeon === 'ST') {
      world.areas['OOT SPAWN'].exits['MM Stone Tower Temple Inverted'] = exprTrue();
    }
    if (dungeon === 'IST') {
      world.areas['OOT SPAWN'].exits['MM Stone Tower Temple'] = exprTrue();
    }

    /* Check if the new world is valid */
    const pathfinderState = this.pathfinder.run(null, { singleWorld: worldId, ignoreItems: true, recursive: true });

    /* Restore the override */
    delete world.areas[originalEntrance.from].exits[replacementEntrance.to]
    delete world.areas['OOT SPAWN'].exits['MM Stone Tower Temple Inverted'];
    delete world.areas['OOT SPAWN'].exits['MM Stone Tower Temple'];

    /* Get the list of required locations */
    let locations: string[];
    if (opts?.locations) {
      locations = opts.locations;
    } else if (['ST', 'IST'].includes(dungeon)) {
      locations = [...world.dungeons['ST'], ...world.dungeons['IST']];
    } else {
      locations = Array.from(world.dungeons[dungeon]);
    }

    /* Turn into locations */
    const worldLocs = locations.map(l => makeLocation(l, worldId));

    /* Check if the new world is valid */
    if (!(worldLocs.every(l => pathfinderState.locations.has(l))))
      return false;

    /* Ganon's tower check */
    if (dungeon === 'Tower' && ['ganon', 'both'].includes(this.input.settings.goal) && !pathfinderState.ws[worldId].events.has('OOT_GANON'))
      return false;

    return true;
  }

  private fixBosses(worldId: number) {
    const world = this.worlds[worldId];
    const bossEntrances = (Object.keys(ENTRANCES) as Entrance[]).filter(x => ENTRANCES[x].type === 'boss');
    const bossEntrancesByDungeon = new Map<string, Entrance>();
    const bossEvents = new Map<string, ExprMap>();
    const bossAreas = new Map<string, string[]>();
    const bossLocations = new Map<string, string[]>();

    /* Collect every boss event */
    for (const a in world.areas) {
      const area = world.areas[a];
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
    for (const eName of bossEntrances) {
      const e = ENTRANCES[eName];
      const areaFrom = world.areas[e.from];
      const areaTo = world.areas[e.to];

      /* We have a boss entrance */
      const dungeon = areaTo.dungeon!;
      bossEntrancesByDungeon.set(dungeon, eName);

      /* Remove the entrance */
      delete areaFrom.exits[e.to];
    }

    /* Actually shuffle bosses */
    const unplacedBosses = new Set(bossEntrancesByDungeon.keys());
    const unplacedLocs = new Set(bossLocations.keys());
    const assigns = new Map<string, Set<string>>();
    while (unplacedBosses.size > 0) {
      /* Refresh the assigns */
      for (const boss of unplacedBosses) {
        if (!assigns.has(boss)) {
          assigns.set(boss, new Set());
        }
        const locs = assigns.get(boss)!;
        for (const loc of unplacedLocs) {
          if (locs.has(loc)) {
            continue;
          }
          if (this.isAssignableNew(worldId, bossEntrancesByDungeon.get(loc)!, bossEntrancesByDungeon.get(boss)!, { ownGame: this.input.settings.erBoss === 'ownGame', locations: bossLocations.get(boss)! })) {
            locs.add(loc);
          }
        }
      }

      const minSize = Math.min(...Array.from(assigns.values()).map(s => s.size));
      const bosses = Array.from(assigns.entries()).filter(([_, s]) => s.size === minSize).map(([k, _]) => k);
      const boss = sample(this.input.random, bosses);
      const locs = assigns.get(boss)!;
      if (locs.size === 0) {
        throw new LogicEntranceError(`Nowhere to place boss ${boss}`);
      }
      const loc = sample(this.input.random, Array.from(locs));

      /* Mark as placed */
      unplacedBosses.delete(boss);
      unplacedLocs.delete(loc);
      assigns.delete(boss);
      for (const l of assigns.values()) {
        l.delete(loc);
      }

      /* Place the boss */
      const src = bossEntrancesByDungeon.get(loc)!;
      const dst = bossEntrancesByDungeon.get(boss)!;
      this.place(worldId, src, dst);
      world.bossIds[BOSS_INDEX_BY_DUNGEON[boss]] = BOSS_INDEX_BY_DUNGEON[loc];

      /* Add the events */
      const areaNames = bossAreas.get(boss)!;
      const lastAreaName = areaNames[areaNames.length - 1];
      const lastArea = world.areas[lastAreaName];
      lastArea.events = { ...lastArea.events, ...bossEvents.get(loc)! };

      /* Change the associated dungeon */
      for (const a of bossAreas.get(boss)!) {
        const area = world.areas[a];
        area.dungeon = loc;
        area.region = DUNGEONS_REGIONS[loc];

        for (const l in area.locations) {
          world.regions[l] = DUNGEONS_REGIONS[loc];
          world.dungeons[boss].delete(l);
          world.dungeons[loc].add(l);
        }
      }
    }
  }

  private fixDungeons(worldId: number) {
    const world = this.worlds[worldId];
    /* Set the dungeon list */
    let shuffledDungeons = new Set(['DT', 'DC', 'JJ', 'Forest', 'Fire', 'Water', 'Shadow', 'Spirit', 'WF', 'SH', 'GB', 'ST', 'IST']);
    if (!this.input.settings.erMajorDungeons) {
      ['DT', 'DC', 'JJ', 'Forest', 'Fire', 'Water', 'Shadow', 'Spirit', 'WF', 'SH', 'GB', 'ST', 'IST'].forEach(d => shuffledDungeons.delete(d));
    }
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
    if (this.input.settings.erMoon) {
      shuffledDungeons.add('Moon');
    }

    /* Get the transitions and exprs */
    const dungeonTransitions = (Object.keys(ENTRANCES) as Entrance[])
      .filter(e => ENTRANCES[e].type === 'dungeon')
      .filter(e => shuffledDungeons.has(world.areas[ENTRANCES[e].to].dungeon!));

    const dungeonEntrances = new Map<string, Entrance>();

    for (const eName of dungeonTransitions) {
      const e = ENTRANCES[eName];
      /* Get the exit */
      const reverse = (e as any).reverse as Entrance | undefined;
      const exit = ENTRANCES[reverse!];

      /* Get the various areas */
      const entranceAreaFrom = world.areas[e.from];
      const entranceAreaTo = world.areas[e.to];
      const exitAreaFrom = world.areas[exit.from];

      /* Save the entrance */
      dungeonEntrances.set(entranceAreaTo.dungeon!, eName);

      /* Remove the transitions */
      delete entranceAreaFrom.exits[e.to];
      delete exitAreaFrom.exits[exit.to];
    }

    /* Assign the dungeons */
    const unplacedDungeons = new Set(shuffledDungeons);
    const unplacedLocs = new Set(shuffledDungeons);
    const assigns = new Map<string, Set<string>>();
    while (unplacedDungeons.size > 0) {
      /* Refresh the assigns */
      for (const dungeon of unplacedDungeons) {
        if (!assigns.has(dungeon)) {
          assigns.set(dungeon, new Set<string>());
        }
        const locs = assigns.get(dungeon)!;
        for (const loc of unplacedLocs) {
          if (locs.has(loc)) {
            continue;
          }
          if (this.isAssignableNew(worldId, dungeonEntrances.get(loc)!, dungeonEntrances.get(dungeon)!, { ownGame: this.input.settings.erDungeons === 'ownGame' })) {
            locs.add(loc);
          }
        }
      }

      const minSize = Math.min(...Array.from(assigns.values()).map(s => s.size));
      const candidates = Array.from(assigns.entries()).filter(([_, s]) => s.size === minSize).map(([k, _]) => k);
      const dungeon = sample(this.input.random, candidates);
      const locs = assigns.get(dungeon)!;
      if (locs.size === 0) {
        throw new LogicError(`Unable to assign dungeon ${dungeon}`);
      }
      const loc = sample(this.input.random, Array.from(locs));

      /* Mark the placement */
      assigns.delete(dungeon);
      for (const v of assigns.values()) {
        v.delete(loc);
      }
      unplacedDungeons.delete(dungeon);
      unplacedLocs.delete(loc);

      /* Change the world */
      const sourceEntranceId = dungeonEntrances.get(loc)!;
      const destEntranceId = dungeonEntrances.get(dungeon)!;

      this.place(worldId, sourceEntranceId, destEntranceId);

      /* Store the dungeon */
      world.dungeonIds[DUNGEON_INDEX[dungeon]] = DUNGEON_INDEX[loc];
    }
  }

  private songOfTime(worldId: number, e: Expr): Expr {
    const subcond = this.worlds[worldId].exprParsers.mm.parse('can_reset_time');
    return exprAnd([e, subcond]);
  }

  private placeSingle(worldId: number, original: Entrance, replacement: Entrance, opts?: PlaceOpts) {
    opts = opts || {};
    const world = this.worlds[worldId];
    const entranceOriginal = ENTRANCES[original];
    const entranceReplacement = ENTRANCES[replacement];

    /* Change the world */
    let expr = this.getExpr(worldId, original);
    if ((entranceOriginal.game === 'oot' || entranceOriginal.type === 'one-way-statue') && entranceReplacement.game === 'mm') {
      if (entranceOriginal.game === 'oot' && opts.overworld) {
        world.areas[entranceOriginal.from].exits['MM GLOBAL'] = expr;
      }
      if (!((entranceReplacement.flags as string[]).includes('no-sot'))) {
        expr = this.songOfTime(worldId, expr);
      }
    }
    world.areas[entranceOriginal.from].exits[entranceReplacement.to] = expr;

    /* Mark the override */
    world.entranceOverrides.set(original, replacement);
  }

  private place(worldId: number, original: Entrance, replacement: Entrance, opts?: PlaceOpts) {
    const world = this.worlds[worldId];
    const entranceOriginal = ENTRANCES[original];
    const entranceReplacement = ENTRANCES[replacement];

    this.placeSingle(worldId, original, replacement, opts);
    if ((entranceOriginal as any).reverse && (entranceReplacement as any).reverse) {
      this.placeSingle(worldId, (entranceReplacement as any).reverse, (entranceOriginal as any).reverse, opts);
    }
  }

  private placePool(worldId: number, pool: string[], opts?: PlaceOpts) {
    const world = this.worlds[worldId];
    /* Get overworld entrances */
    const entrances = new Set((Object.keys(ENTRANCES) as Entrance[]).filter(x => pool.includes(ENTRANCES[x].type)));

    /* Delete the overworld entrances from the world */
    for (const eName of entrances) {
      const e = ENTRANCES[eName];
      delete world.areas[e.from].exits[e.to];
      const reverse = (e as any).reverse as Entrance | undefined;
      if (reverse) {
        const r = ENTRANCES[reverse];
        delete world.areas[r.from].exits[r.to];
      }
    }

    /* Shuffle the entrances */
    const shuffledEntrances = shuffle(this.input.random, [...entrances]);

    /* Apply the entrances */
    for (let e of shuffledEntrances) {
      let candidates = [...entrances];
      if (opts?.ownGame) {
        candidates = candidates.filter(c => ENTRANCES[c].game === ENTRANCES[e].game);
      }
      const newE = sample(this.input.random, candidates);
      this.place(worldId, e, newE, opts);
      entrances.delete(newE);
    }
  }

  private placeRegions(worldId: number) {
    const pool = new Set(['region']);
    if (this.input.settings.erRegionsExtra) {
      pool.add('region-extra');
    }
    if (this.input.settings.erRegionsShortcuts) {
      pool.add('region-shortcut');
    }
    this.placePool(worldId, Array.from(pool), { ownGame: this.input.settings.erRegions === 'ownGame' });
  }

  private placeIndoors(worldId: number) {
    const pool = new Set(['indoors']);
    if (!this.input.settings.erIndoorsMajor) {
      pool.delete('indoors');
    }
    if (this.input.settings.erIndoorsExtra) {
      pool.add('indoors-extra');
    }
    this.placePool(worldId, Array.from(pool), { ownGame: this.input.settings.erIndoors === 'ownGame' });
  }

  private placeWarps(worldId: number) {
    const pool = new Set(['one-way-song', 'one-way-statue']);

    if (this.input.settings.erWarps === 'ootOnly') {
      pool.delete('one-way-statue');
    }
    if (this.input.settings.erWarps === 'mmOnly') {
      pool.delete('one-way-song');
    }

    this.placePool(worldId, Array.from(pool), { ownGame: this.input.settings.erWarps === 'ownGame' });
  }

  private placeOneWays(worldId: number) {
    const pool = new Set(['one-way']);

    if (!this.input.settings.erOneWaysMajor) {
      pool.delete('one-way');
    }
    if (this.input.settings.erOneWaysIkana) {
      pool.add('one-way-ikana');
    }
    if (this.input.settings.erOneWaysSongs) {
      pool.add('one-way-song');
    }
    if (this.input.settings.erOneWaysStatues) {
      pool.add('one-way-statue');
    }
    if (this.input.settings.erOneWaysOwls) {
      pool.add('one-way-owl');
    }

    this.placePool(worldId, Array.from(pool), { ownGame: this.input.settings.erOneWays === 'ownGame' });
  }

  private shuffledPools(def?: string[]) {
    let pool: string[] = [];

    if (this.input.settings.erOneWays !== 'none') {
      pool = [...pool, 'one-way', 'one-way-ikana', 'one-way-song', 'one-way-statue', 'one-way-owl'];
    }

    if (this.input.settings.erDungeons !== 'none') {
      pool = [...pool, 'dungeon'];
    }

    if (this.input.settings.erBoss !== 'none') {
      pool = [...pool, 'boss'];
    }

    if (pool.length === 0) {
      return def;
    }

    return pool;
  }

  private placeWallmasters(worldId: number) {
    const world = this.worlds[worldId];
    const pool = this.shuffledPools(['dungeon'])!;

    const entrancesSrc = new Set((Object.keys(ENTRANCES) as Entrance[]).filter(x => ENTRANCES[x].type === 'wallmaster' && world.areas.hasOwnProperty(ENTRANCES[x].from)));
    const entrancesDst = new Set((Object.keys(ENTRANCES) as Entrance[]).filter(x => pool.includes(ENTRANCES[x].type)));

    /* Sanity - TODO: base this on real ER settings */
    entrancesDst.delete('MM_CLOCK_TOWER');
    entrancesDst.delete('MM_CLOCK_TOWN_FROM_CLOCK_TOWER');
    entrancesDst.delete('OOT_GANON_CASTLE');
    entrancesDst.delete('OOT_GANON_CASTLE_EXTERIOR_FROM_CASTLE');
    entrancesDst.delete('OOT_GANON_TOWER');
    entrancesDst.delete('OOT_GANON_CASTLE_FROM_TOWER');

    while (entrancesSrc.size > 0) {
      const src = sample(this.input.random, [...entrancesSrc]);
      let dstCandidates = [...entrancesDst];
      if (this.input.settings.erWallmasters === 'ownGame') {
        dstCandidates = dstCandidates.filter(x => ENTRANCES[x].game === ENTRANCES[src].game);
      }
      const dst = sample(this.input.random, dstCandidates);
      this.place(worldId, src, dst);
      entrancesSrc.delete(src);
      entrancesDst.delete(dst);
    }
  }

  private propagateRegionsStep(worldId: number) {
    const world = this.worlds[worldId];
    let changed = false;
    for (const areaName of Object.keys(world.areas)) {
      const a = world.areas[areaName];
      if (a.region === 'NONE' || a.region === 'ENTRANCE')
        continue;
      /* We need to propagate the region */
      for (const exitName of Object.keys(a.exits)) {
        const exitArea = world.areas[exitName];
        if (exitArea == undefined) {
          throw new LogicEntranceError(`Unknown exit: ${exitName}`);
        }
        if (exitArea.region === 'ENTRANCE') {
          exitArea.region = a.region;
          for (const loc of Object.keys(exitArea.locations)) {
            if (world.regions[loc] === 'ENTRANCE') {
              world.regions[loc] = a.region;
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
    for (let i = 0; i < this.worlds.length; ++i) {
      for (;;) {
        if (!this.propagateRegionsStep(i)) {
          break;
        }
      }
    }
  }

  private validateAgeTemple() {
    const newWorlds = this.worlds.map(w => cloneWorld(w));
    for (const w of newWorlds) {
      const a = w.areas['OOT SPAWN'];
      if (this.input.settings.startingAge === 'child') {
        a.exits['OOT SPAWN ADULT'] = exprAge('adult');
        a.exits['OOT SPAWN CHILD'] = exprFalse();
      } else {
        a.exits['OOT SPAWN ADULT'] = exprFalse();
        a.exits['OOT SPAWN CHILD'] = exprAge('child');
      }
    }

    const agePathfinder = new Pathfinder(newWorlds, this.input.settings, this.input.startingItems);
    const pathfinderState = agePathfinder.run(null, { recursive: true });
    const target = 'OOT Temple of Time';
    if (pathfinderState.ws.some(x => !(x.areas.adult.has(target) || x.areas.child.has(target)))) {
      throw new LogicEntranceError('Temple of Time is unreachable from the non-starting age');
    }
  }

  private validate() {
    if (this.input.settings.logic === 'none')
      return;
    this.validateAgeTemple();
    const pathfinderState = this.pathfinder.run(null, { ignoreItems: true, recursive: true });

    /* We want to make sure everything that needs to is reachable */
    if (!pathfinderState.goal) {
      throw new LogicEntranceError('Goal is not reachable');
    }

    if (this.input.settings.logic === 'allLocations') {
      const allLocsCount = this.worlds.map(x => x.locations.size).reduce((a, b) => a + b, 0);
      if (pathfinderState.locations.size < allLocsCount) {
        const allLocations = this.worlds.map((x, i) => [...x.locations].map(y => makeLocation(y, i))).flat();
        const unreachableLocs = allLocations.filter(x => !pathfinderState.locations.has(x));
        throw new LogicEntranceError(`Unreachable locations: ${unreachableLocs.join(', ')}`);
      }
    }

    const forbiddenAreasChild = [];
    const forbiddenAreasAdult = [];

    if (this.input.settings.ageChange === 'none') {
      /* We don't want child to reach the Fairy OGC exit, and the other way around too */
      forbiddenAreasChild.push('OOT Near Fairy Fountain Defense');
      forbiddenAreasAdult.push('OOT Near Fairy Fountain Din');
    }

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
    const solver = new LogicPassSolver({ ...this.input, worlds: this.worlds });
    solver.validate();
  }

  private runAttempt() {
    /* Init */
    this.worlds = [];
    const worldsCount = this.input.settings.distinctWorlds ? this.input.worlds.length : 1;
    for (let worldId = 0; worldId < worldsCount; ++worldId) {
      const world = cloneWorld(this.input.worlds[worldId]);
      world.dungeonIds = Object.values(DUNGEON_INDEX);
      world.bossIds = Object.values(BOSS_INDEX_BY_DUNGEON);
      this.worlds.push(world);
    }
    if (!this.input.settings.distinctWorlds) {
      for (let i = 1; i < this.input.worlds.length; ++i) {
        this.worlds.push(this.worlds[0]);
      }
    }

    let anyEr = false;
    this.pathfinder = new Pathfinder(this.worlds, this.input.settings, this.input.startingItems);

    for (let i = 0; i < worldsCount; ++i) {
      if (this.input.settings.erWallmasters !== 'none') {
        anyEr = true;
        this.placeWallmasters(i);
      }

      if (this.input.settings.erRegions !== 'none') {
        anyEr = true;
        this.placeRegions(i);
      }

      if (this.input.settings.erIndoors !== 'none') {
        anyEr = true;
        this.placeIndoors(i);
      }

      if (this.input.settings.erWarps !== 'none') {
        anyEr = true;
        this.placeWarps(i);
      }

      if (this.input.settings.erOneWays !== 'none') {
        anyEr = true;
        this.placeOneWays(i);
      }

      if (this.input.settings.erDungeons !== 'none') {
        anyEr = true;
        this.fixDungeons(i);
      }

      if (this.input.settings.erBoss !== 'none') {
        anyEr = true;
        this.fixBosses(i);
      }

      if (anyEr) {
        this.validate();
      }
    }

    this.propagateRegions();

    return { worlds: this.worlds };
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
        if (e.stack) {
          this.input.monitor.debug(e.stack);
        }
        attempts++;
      }
    }
  }
};

