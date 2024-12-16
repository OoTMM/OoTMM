import { ENTRANCES } from '@ootmm/data';

import { Random, sample } from '../random';
import { Settings } from '../settings';
import { DUNGEONS_REGIONS, World, WorldArea, cloneWorld, BOSS_INDEX_BY_DUNGEON } from './world';
import { Pathfinder } from './pathfind';
import { Monitor } from '../monitor';
import { LogicEntranceError, LogicError } from './error';
import { Expr, exprAge, exprAnd, exprEvent, exprFalse, exprOr, exprTrue } from './expr';
import { Location, makeLocation } from './locations';
import { LogicPassSolver } from './solve';
import { PlayerItems } from '../items';
import { ItemProperties } from './item-properties';
import { Region } from './regions';
import { AGE_ADULT, AGE_CHILD } from './constants';
import { BOSS_METADATA_BY_ENTRANCE } from './boss';

type EntrancePolarity = 'in' | 'out' | 'any';
type Entrance = keyof typeof ENTRANCES;

type PlaceOpts = {
  ownGame?: boolean;
  alias?: boolean;
};

type EntrancePoolDescr = {
  src: string[];
  dst: string[];
  opts: PlaceOpts;
};

type EntrancePool = {
  src: Set<Entrance>;
  dst: Set<Entrance>;
  opts: PlaceOpts;
};

type EntrancePoolDescrs = {[k: string]: EntrancePoolDescr};
type EntrancePools = {[k: string]: EntrancePool};

type EntranceOverrides = {[k in Entrance]?: Entrance};

const POLARITY_ANY_OVERWORLD = new Set<string>([
  'region',
  'region-extra',
  'region-shortcut',
  'region-exit',
]);

const POLARITY_IN = new Set<string>([
  'boss',
  'dungeon',
  'dungeon-minor',
  'dungeon-ganon',
  'dungeon-ganon-tower',
  'dungeon-sh',
  'dungeon-pf',
  'dungeon-btw',
  'dungeon-acoi',
  'dungeon-ss',
  'dungeon-ctr',
  'wallmaster',
  'region',
  'region-extra',
  'region-shortcut',
  'indoors',
  'indoors-pf',
  'indoors-extra',
  'indoors-special',
  'grotto',
  'grave',
]);

const POLARITY_OUT = new Set<string>([
  'dungeon-exit',
  'indoors-exit',
  'region-exit',
  'grotto-exit',
  'grave-exit',
]);

export const DUNGEON_ENTRANCES: readonly Entrance[] = [
  'OOT_DEKU_TREE',
  'OOT_DODONGO_CAVERN',
  'OOT_JABU_JABU',
  'OOT_TEMPLE_FOREST',
  'OOT_TEMPLE_FIRE',
  'OOT_TEMPLE_WATER',
  'OOT_TEMPLE_SHADOW',
  'OOT_TEMPLE_SPIRIT',
  'MM_TEMPLE_WOODFALL',
  'MM_TEMPLE_SNOWHEAD',
  'MM_TEMPLE_GREAT_BAY',
  'MM_TEMPLE_STONE_TOWER_INVERTED',
  'MM_TEMPLE_STONE_TOWER',
  'MM_SPIDER_HOUSE_SWAMP',
  'MM_SPIDER_HOUSE_OCEAN',
  'OOT_BOTTOM_OF_THE_WELL',
  'OOT_ICE_CAVERN',
  'OOT_GERUDO_TRAINING_GROUNDS',
  'MM_BENEATH_THE_WELL',
  'MM_IKANA_CASTLE',
  'MM_SECRET_SHRINE',
  'MM_BENEATH_THE_WELL_BACK',
  'MM_PIRATE_FORTRESS',
  'OOT_GANON_CASTLE',
  'OOT_GANON_TOWER',
  'MM_MOON',
];

class WorldShuffler {
  private world: World;
  private worldChanged: boolean;
  private overrides: EntranceOverrides;
  private backtrackCount: number;
  private readonly allEntrances: Entrance[];
  private usedEntrancesSrc: Set<Entrance>;
  private usedEntrancesDst: Set<Entrance>;
  private sotExpr: Expr;

  constructor(
    private random: Random,
    private worldId: number,
    private worlds: World[],
    private settings: Settings,
    private startingItems: PlayerItems,
    private allItems: PlayerItems,
  ) {
    this.backtrackCount = 0;
    this.world = worlds[worldId];
    this.worldChanged = false;
    this.overrides = {};
    this.allEntrances = this.computeAllEntrances();
    this.usedEntrancesSrc = new Set();
    this.usedEntrancesDst = new Set();
    this.sotExpr = this.world.exprParsers.mm.parse('can_reset_time');
  }

  private entrancePolarity(entrance: Entrance): EntrancePolarity {
    const type = ENTRANCES[entrance].type;
    if (this.settings.erOverworld !== 'none' && POLARITY_ANY_OVERWORLD.has(type)) {
      return 'any';
    }
    if (POLARITY_IN.has(type)) {
      return 'in';
    }
    if (POLARITY_OUT.has(type)) {
      return 'out';
    }
    return 'any';
  }

  private entrancePolarityMatch(a: Entrance, b: Entrance) {
    if (this.settings.erNoPolarity) {
      return true;
    }
    const polA = this.entrancePolarity(a);
    const polB = this.entrancePolarity(b);
    if (polA === 'any' || polB === 'any') {
      return true;
    }
    return polA === polB;
  }

  private getExpr(original: Entrance) {
    const eData = ENTRANCES[original];
    const from = this.world.areas[eData.from];
    const expr = from.exits[eData.to];
    if (!expr) {
      throw new Error(`No expr for ${original}`);
    }
    return expr;
  }

  private songOfTime(e: Expr): Expr {
    return exprAnd([e, this.sotExpr]);
  }

  private computeAllEntrances() {
    let entrances: Entrance[] = [];
    for (const key in ENTRANCES) {
      const name = key as Entrance;
      const e = ENTRANCES[name];

      if (this.settings.games !== 'ootmm' && e.game !== this.settings.games) {
        continue;
      }

      if (e.from !== 'NONE' && !this.world.areas.hasOwnProperty(e.from)) {
        continue;
      }

      if (e.to !== 'NONE' && !this.world.areas.hasOwnProperty(e.to)) {
        continue;
      }

      entrances.push(name);
    }

    return entrances;
  }

  private placeSingleExpr(world: World, original: Entrance, replacement: Entrance) {
    const entranceOriginal = ENTRANCES[original];
    const entranceReplacement = ENTRANCES[replacement];
    const areaFrom = world.areas[entranceOriginal.from];

    /* Change the world */
    let expr = this.getExpr(original);
    if (entranceReplacement.game === 'mm') {
      if (!(entranceReplacement.flags.includes('no-global'))) {
        areaFrom.exits['MM GLOBAL'] = expr;
      } else {
        areaFrom.exits['MM ACCESS'] = expr;
      }
      if (!(entranceReplacement.flags.includes('no-sot'))) {
        expr = this.songOfTime(expr);
      }
    }
    if (areaFrom.exits[entranceReplacement.to]) {
      expr = exprOr([expr, areaFrom.exits[entranceReplacement.to]]);
    }
    areaFrom.exits[entranceReplacement.to] = expr;
  }

  private placeSingle(world: World, original: Entrance, replacement: Entrance) {
    this.placeSingleExpr(world, original, replacement);

    /* Mark the override */
    world.entranceOverrides.set(original, replacement);
    world.entranceOverridesRev.set(replacement, [ ...world.entranceOverridesRev.get(replacement) || [], original ]);

    /* Track dungeons replacing dungeons */
    if (DUNGEON_ENTRANCES.includes(original) && DUNGEON_ENTRANCES.includes(replacement)) {
      world.dungeonsEntrances.set(replacement, { type: 'replace', entrance: original });
    }

    /* Boss needs special handling */
    const bossSrc = BOSS_METADATA_BY_ENTRANCE.get(original);
    const bossDst = BOSS_METADATA_BY_ENTRANCE.get(replacement);
    if (bossSrc && bossDst) {
      /* Set the boss ID */
      const bossIndexSrc = BOSS_INDEX_BY_DUNGEON[bossSrc.dungeon];
      const bossIndexDst = BOSS_INDEX_BY_DUNGEON[bossDst.dungeon];
      world.bossIds[bossIndexDst] = bossIndexSrc;

      /* Set the boss event */
      if (this.settings.regionState === 'dungeonBeaten') {
        const eventClear = bossSrc.eventClear;
        if (eventClear) {
          world.areas['OOT SPAWN'].events[eventClear] = exprEvent(bossDst.event);
        }
      }

      /* Collect areas */
      const areas = this.world.dungeonsBossAreas[bossDst.dungeon];
      for (const a of areas) {
        /* Patch dungeon and region */
        world.areas[a].dungeon = bossSrc.dungeon;
        world.areas[a].region = DUNGEONS_REGIONS[bossSrc.dungeon];

        /* Patch locations */
        for (const loc of Object.keys(world.areas[a].locations)) {
          world.dungeons[bossDst.dungeon].delete(loc);
          world.dungeons[bossSrc.dungeon].add(loc);
          world.regions[loc] = DUNGEONS_REGIONS[bossSrc.dungeon];
        }
      }
    }
  }

  private validateAgeTemple(world: World) {
    const newWorld = cloneWorld(world);
    const a = newWorld.areas['OOT SPAWN'];
    if (this.settings.startingAge === 'child') {
      a.exits['OOT SPAWN ADULT'] = exprAge(AGE_ADULT);
      a.exits['OOT SPAWN CHILD'] = exprFalse();
    } else {
      a.exits['OOT SPAWN ADULT'] = exprFalse();
      a.exits['OOT SPAWN CHILD'] = exprAge(AGE_CHILD);
    }

    const worlds = [...this.worlds];
    worlds[this.worldId] = newWorld;
    const agePathfinder = new Pathfinder(worlds, this.settings, this.startingItems);
    const pathfinderState = agePathfinder.run(null, { recursive: true, singleWorld: this.worldId });
    const target = 'OOT Temple of Time';
    if (pathfinderState.ws.some(x => !(x.ages[AGE_ADULT].areas.has(target) || x.ages[AGE_CHILD].areas.has(target)))) {
      return false;
    }
    return true;
  }

  private changeWorldAssumePools(world: World, pools: EntrancePools) {
    const assumedSets: Set<Entrance>[] = [];

    for (const pool of Object.values(pools)) {
      const cardinality = pool.src.size * pool.dst.size;
      if (cardinality > 5000 && pool.src.size > 50) {
        assumedSets.push(pool.dst);
      } else {
        for (const oldName of pool.src) {
          for (const newName of pool.dst) {
            if (this.isAssignableConstraints(oldName, newName, !!pool.opts.ownGame)) {
              this.placeSingleExpr(world, oldName, newName);
            }
          }
        }
      }
    }

    if (assumedSets) {
      const assumedExits: {[k: string]: Expr} = {};
      for (const s of assumedSets) {
        for (const e of s) {
          const ee = ENTRANCES[e];
          assumedExits[ee.to] = exprTrue();
        }
      }

      const assumedArea: WorldArea = {
        game: 'mm',
        region: 'NONE',
        exits: assumedExits,
        events: {},
        locations: {},
        gossip: {},
        dungeon: null,
        boss: false,
        stay: null,
        ageChange: false,
        time: 'still',
      };

      world.areas['ASSUMED'] = assumedArea;
      world.areas['OOT SPAWN'].exits['ASSUMED'] = exprTrue();
    }

    if (this.settings.regionState === 'dungeonBeaten') {
      const bossPool = pools.BOSS;
      if (bossPool) {
        for (const oldName of bossPool.src) {
          /* Assume we can get the event */
          const meta = BOSS_METADATA_BY_ENTRANCE.get(oldName)!;
          const eventClear = meta.eventClear;
          if (eventClear) {
            world.areas['OOT SPAWN'].events[eventClear] = exprEvent(meta.event);
          }
        }
      }
    }
  }

  private changedWorld(overrides: EntranceOverrides, pools?: EntrancePools): World {
    const newWorld = cloneWorld(this.world);

    /* Delete the overriden entrances */
    for (const oldName of Object.keys(overrides) as Entrance[]) {
      const oldE = ENTRANCES[oldName];
      delete newWorld.areas[oldE.from].exits[oldE.to];
    }

    /* Delete all pool entrances */
    if (pools) {
      for (const pool of Object.values(pools)) {
        for (const oldName of pool.src) {
          const oldE = ENTRANCES[oldName];
          delete newWorld.areas[oldE.from].exits[oldE.to];
        }
      }
    }

    /* Apply overrides */
    for (const [oldName, newName] of Object.entries(overrides)) {
      this.placeSingle(newWorld, oldName as Entrance, newName as Entrance);
    }

    /* Assume pools */
    if (pools) {
      this.changeWorldAssumePools(newWorld, pools);
    }

    /* Handle dungeons that aren't shuffled */
    for (const dungeonEntrance of DUNGEON_ENTRANCES) {
      if (!overrides.hasOwnProperty(dungeonEntrance)) {
        newWorld.dungeonsEntrances.set(dungeonEntrance, { type: 'replace', entrance: dungeonEntrance });
      }
    }

    return newWorld;
  }

  private isValidShuffle(overrides: EntranceOverrides, pools: EntrancePools) {
    if (this.settings.logic === 'none') {
      return true;
    }

    /* Build the new world list */
    const newWorld = this.changedWorld(overrides, pools);
    const worlds = [...this.worlds];
    worlds[this.worldId] = newWorld;

    /* Run the pathfinder */
    const pathfinder = new Pathfinder(worlds, this.settings, this.startingItems);
    const pathfinderState = pathfinder.run(null, { singleWorld: this.worldId, assumedItems: this.allItems, recursive: true });

    if (!pathfinderState.goal) {
      return false;
    }

    if (this.settings.logic === 'allLocations' && pathfinderState.locations.size < newWorld.locations.size) {
      return false;
    }

    /* Check ToT access */
    if (['ootmm', 'oot'].includes(this.settings.games) && !this.validateAgeTemple(newWorld)) {
      return false;
    }

    return true;
  }

  private reverseEntranceRaw(entrance: Entrance): Entrance | null {
    const e = ENTRANCES[entrance];
    const rr = (e as any).reverse as Entrance | undefined;

    if (!rr) {
      return null;
    }
    return rr;
  }

  private reverseEntrance(entrance: Entrance): Entrance | null {
    if (this.settings.erDecoupled) {
      return null;
    }

    return this.reverseEntranceRaw(entrance);
  }

  private unbalancedPool(name: string) {
    throw new LogicEntranceError(`Unbalanced pool: ${name}`);
  }

  private assumedFromPools(pools: EntrancePools, ignore: Set<Entrance>): Set<Entrance> {
    const assumed = new Set<Entrance>();

    for (const pool of Object.values(pools)) {
      for (const dst of pool.dst) {
        if (ignore.has(dst)) {
          ignore.delete(dst);
          continue;
        }
        const e = ENTRANCES[dst];
        if (!(['dungeon-exit', 'grotto-exit', 'grave-exit'].includes(e.type)) || dst === 'OOT_DESERT_COLOSSUS_FROM_TEMPLE_SPIRIT' || this.settings.erNoPolarity) {
          assumed.add(dst);
        }
      }
    }

    return assumed;
  }

  private isSelfLoop(src: Entrance, dst: Entrance) {
    const dstEntrance = ENTRANCES[dst];
    const srcEntrance = ENTRANCES[src];
    const map = srcEntrance.fromMap;
    if (map !== 'NONE') {
      if (dstEntrance.toMap !== map) {
        return false;
      }

      /* Same map, check for internal submap */
      if (srcEntrance.fromMap !== srcEntrance.toMap) {
        return true;
      }

      if (dstEntrance.fromMap !== dstEntrance.toMap) {
        return true;
      }

      /* Both entrances are internal, check the submap */
      return srcEntrance.fromSubmap === dstEntrance.toSubmap;
    }

    return false;
  }

  private isAssignableConstraints(src: Entrance, dst: Entrance, ownGame: boolean) {
    if (ownGame && ENTRANCES[src].game !== ENTRANCES[dst].game) {
      return false;
    }

    if (!this.entrancePolarityMatch(src, dst)) {
      return false;
    }

    if (!this.settings.erSelfLoops && this.isSelfLoop(src, dst)) {
      return false;
    }

    return true;
  }

  private placePoolsRecursive(pools: EntrancePools, overrides: EntranceOverrides): EntranceOverrides | null {
    if (Object.keys(pools).length === 0) {
      return overrides;
    }

    /* Select a pool */
    const poolName = sample(this.random, Object.keys(pools));
    const pool = pools[poolName];

    /* Select a source */
    const src = sample(this.random, pool.src);

    /* Build the candidates list */
    let dstCandidates = new Set([...pool.dst].filter(x => this.isAssignableConstraints(src, x, !!pool.opts.ownGame)));

    /* Try to find a match */
    let newPools: EntrancePools;

    while (dstCandidates.size > 0) {
      const dst = sample(this.random, dstCandidates);
      dstCandidates.delete(dst);
      const revSrc = this.reverseEntrance(src);
      const revDst = this.reverseEntrance(dst);
      const newOverrides = { ...overrides };
      newOverrides[src] = dst;
      if (revSrc && revDst) {
        newOverrides[revDst] = revSrc;
      }

      /* Build the new pool */
      const newSrc = new Set(pool.src);
      const newDst = new Set(pool.dst);
      const newPool = { ...pool, src: newSrc, dst: newDst };
      newPools = { ...pools, [poolName]: newPool };

      newSrc.delete(src);
      newDst.delete(dst);
      if (revSrc && revDst) {
        newSrc.delete(revDst);
        newDst.delete(revSrc);
      }

      if (!this.isValidShuffle({ ...this.overrides, ...newOverrides }, newPools)) {
        continue;
      }

      /* The match is valid */
      if (newSrc.size === 0) {
        if (newDst.size !== 0 && !pool.opts.alias) {
          this.unbalancedPool(poolName);
        }
        delete newPools[poolName];
      }
      const finalOverrides = this.placePoolsRecursive(newPools, newOverrides);
      if (finalOverrides) {
        return finalOverrides;
      } else {
        this.backtrackCount++;
        if (this.backtrackCount >= 10) {
          throw new LogicEntranceError('Too many backtracks');
        }
      }
    }

    return null;
  }

  private placePools(poolDescrs: EntrancePoolDescrs) {
    this.backtrackCount = 0;
    const overrides: EntranceOverrides = {};
    const pools: EntrancePools = {};

    /* Get entrances */
    const poolNames = new Set(Object.keys(poolDescrs));

    for (const name of poolNames) {
      const descr = poolDescrs[name];
      const pe = this.poolEntrancesForTypes(descr.src, descr.dst, !!descr.opts.alias);
      pools[name] = { src: pe.src, dst: pe.dst, opts: descr.opts };
    }

    /* Remove any empty pools */
    for (const name of poolNames) {
      if (pools[name].src.size === 0) {
        if (pools[name].dst.size !== 0 && !pools[name].opts.alias) {
          this.unbalancedPool(name);
        }
        delete pools[name];
      }
    }

    const finalOverrides = this.placePoolsRecursive(pools, overrides);
    if (!finalOverrides) {
      throw new LogicEntranceError('Unable to place pools');
    }

    for (const [src, dst] of Object.entries(finalOverrides)) {
      this.overrideEntrance(src as Entrance, dst!);
    }
  }

  private entrancesForTypes(aTypes: Iterable<string>) {
    const types = new Set(aTypes);
    const entrances = this.allEntrances.filter(x => types.has(ENTRANCES[x].type));
    const entrancesReverse = entrances.map(x => this.reverseEntranceRaw(x)).filter(x => x) as Entrance[];
    return [...entrances, ...entrancesReverse];
  }

  private poolEntrancesForTypes(srcTypes: Iterable<string>, dstTypes: Iterable<string>, alias: boolean) {
    let entrancesSrc = this.entrancesForTypes(srcTypes);
    let entrancesDst = this.entrancesForTypes(dstTypes);

    entrancesSrc = entrancesSrc.filter(x => !this.usedEntrancesSrc.has(x));
    if (!alias) {
      entrancesDst = entrancesDst.filter(x => !this.usedEntrancesDst.has(x));
    }

    const src = new Set(entrancesSrc);
    const dst = new Set(entrancesDst);

    /* Fixup for game links */
    src.delete('OOT_MARKET_FROM_MASK_SHOP');
    src.delete('MM_CLOCK_TOWN_FROM_CLOCK_TOWER');
    dst.delete('OOT_SHOP_MASKS');
    dst.delete('MM_CLOCK_TOWER_FROM_CLOCK_TOWN');

    return { src, dst };
  }

  private overrideEntrance(src: Entrance, dst: Entrance, useDst = true) {
    this.overrides[src] = dst;
    this.worldChanged = true;
    this.usedEntrancesSrc.add(src);
    if (useDst) {
      this.usedEntrancesDst.add(dst);
    }
  }

  private poolSpawns() {
    /* Compute types */
    const types = new Set(this.poolsTypesDst());
    types.delete('boss');
    types.add('spawn');
    types.add('indoors');
    types.add('one-way-song');
    types.add('region');

    let typesSrc: string[] = [];
    switch (this.settings.erSpawns) {
    case 'adult': typesSrc = ['spawn-adult']; break;
    case 'child': typesSrc = ['spawn-child']; break;
    case 'both': typesSrc = ['spawn-adult', 'spawn-child']; break;
    }

    return { src: typesSrc, dst: [...types], opts: { alias: true, ownGame: true } };
  }

  private poolWallmasters() {
    /* Compute types */
    const types = new Set(this.poolsTypesDst());
    types.add('dungeon');
    types.add('dungeon-minor');
    types.add('dungeon-sh');

    if (this.settings.erBoss !== 'none') {
      types.add('boss');
    }

    return { src: ['wallmaster'], dst: [...types], opts: { alias: true, ownGame: this.settings.erWallmasters === 'ownGame' } };
  }

  private poolBoss() {
    return { src: ['boss'], dst: ['boss'], opts: { ownGame: this.settings.erBoss === 'ownGame' } };
  }

  private poolDungeons() {
    const types: string[] = [];

    if (this.settings.erMajorDungeons) types.push('dungeon');
    if (this.settings.erMinorDungeons) types.push('dungeon-minor');
    if (this.settings.erGanonCastle) types.push('dungeon-ganon');
    if (this.settings.erGanonTower) types.push('dungeon-ganon-tower');
    if (this.settings.erSpiderHouses) types.push('dungeon-sh');
    if (this.settings.erPirateFortress) types.push('dungeon-pf');
    if (this.settings.erBeneathWell) types.push('dungeon-btw');
    if (this.settings.erIkanaCastle) types.push('dungeon-acoi');
    if (this.settings.erSecretShrine) types.push('dungeon-ss');
    if (this.settings.erMoon) types.push('dungeon-ctr');

    return { src: types, dst: types, opts: { ownGame: this.settings.erDungeons === 'ownGame' } };
  }

  private poolRegions() {
    const types = new Set(['region']);
    if (this.settings.erRegionsExtra) {
      types.add('region-extra');
    }
    if (this.settings.erRegionsShortcuts) {
      types.add('region-shortcut');
    }
    return { src: [...types], dst: [...types], opts: { ownGame: this.settings.erRegions === 'ownGame' } };
  }

  private poolOverworld() {
    const types = ['region', 'region-extra', 'region-shortcut', 'overworld'];
    if (this.settings.erPiratesWorld) types.push('overworld-pf', 'dungeon-pf');
    return { src: types, dst: types, opts: { ownGame: this.settings.erOverworld === 'ownGame' } };
  }

  private poolIndoors() {
    const types = new Set(['indoors']);
    if (!this.settings.erIndoorsMajor) {
      types.delete('indoors');
    }
    if (this.settings.erIndoorsExtra) {
      types.add('indoors-extra');
    }
    if (this.settings.erIndoorsExtra && this.settings.erPiratesWorld) {
      types.add('indoors-pf');
    }
    if (this.settings.erIndoorsGameLinks) {
      types.add('indoors-link');
    }
    return { src: [...types], dst: [...types], opts: { ownGame: this.settings.erIndoors === 'ownGame' } };
  }

  private poolWarps() {
    const types = new Set(['one-way-song', 'one-way-statue']);

    if (this.settings.erWarps === 'ootOnly' || this.settings.erOneWaysStatues) {
      types.delete('one-way-statue');
    }
    if (this.settings.erWarps === 'mmOnly' || this.settings.erOneWaysSongs) {
      types.delete('one-way-song');
    }

    return { src: [...types], dst: [...types], opts: { ownGame: this.settings.erWarps === 'ownGame' } };
  }

  private poolOneWaysAnywhere() {
    /* Compute types */
    const types = new Set(this.poolsTypesDst());
    types.add('dungeon');
    types.add('dungeon-minor');
    types.add('dungeon-sh');

    if (this.settings.erBoss !== 'none') {
      types.add('boss');
    }

    /* Compute entrances */
    const oneWays = this.poolOneWays();
    return { src: oneWays.src, dst: [...types, ...oneWays.dst], opts: { alias: true, ownGame: this.settings.erOneWays === 'ownGame' } };
  }

  private poolOneWays() {
    const types = new Set<string>();

    if (this.settings.erOneWaysMajor) {
      types.add('one-way');
    }
    if (this.settings.erOneWaysIkana) {
      types.add('one-way-ikana');
    }
    if (this.settings.erOneWaysSongs) {
      types.add('one-way-song');
    }
    if (this.settings.erOneWaysStatues) {
      types.add('one-way-statue');
    }
    if (this.settings.erOneWaysOwls) {
      types.add('one-way-owl');
    }
    if (this.settings.erOneWaysWoods) {
      types.add('one-way-woods');
    }
    if (this.settings.erOneWaysWaterVoids) {
      types.add('one-way-water-void');
    }

    return { src: [...types], dst: [...types], opts: { ownGame: this.settings.erOneWays === 'ownGame' } };
  }

  private poolGrottos() {
    const types = ['grotto', 'grave'];
    return { src: types, dst: types, opts: { ownGame: this.settings.erGrottos === 'ownGame' } };
  }

  private makePoolsSimple(): EntrancePoolDescrs {
    const pools: EntrancePoolDescrs = {};

    if (this.settings.erBoss !== 'none') {
      pools.BOSS = this.poolBoss();
    }

    if (this.settings.erDungeons !== 'none') {
      pools.DUNGEONS = this.poolDungeons();
    }

    if (this.settings.erGrottos !== 'none') {
      pools.GROTTOS = this.poolGrottos();
    }

    if (this.settings.erRegions !== 'none') {
      pools.REGIONS = this.poolRegions();
    }

    if (this.settings.erOverworld !== 'none') {
      pools.OVERWORLD = this.poolOverworld();
    }

    if (this.settings.erIndoors !== 'none') {
      pools.INDOORS = this.poolIndoors();
    }

    if (this.settings.erMixed !== 'none') {
      pools.MIXED = { src: [], dst: [], opts: { ownGame: this.settings.erMixed === 'ownGame' } };

      if (this.settings.erMixedDungeons) {
        pools.MIXED.src = [...pools.MIXED.src, ...pools.DUNGEONS.src];
        pools.MIXED.dst = [...pools.MIXED.dst, ...pools.DUNGEONS.dst];
        delete pools.DUNGEONS;
      }

      if (this.settings.erMixedGrottos) {
        pools.MIXED.src = [...pools.MIXED.src, ...pools.GROTTOS.src];
        pools.MIXED.dst = [...pools.MIXED.dst, ...pools.GROTTOS.dst];
        delete pools.GROTTOS;
      }

      if (this.settings.erMixedRegions) {
        pools.MIXED.src = [...pools.MIXED.src, ...pools.REGIONS.src];
        pools.MIXED.dst = [...pools.MIXED.dst, ...pools.REGIONS.dst];
        delete pools.REGIONS;
      }

      if (this.settings.erMixedOverworld) {
        pools.MIXED.src = [...pools.MIXED.src, ...pools.OVERWORLD.src];
        pools.MIXED.dst = [...pools.MIXED.dst, ...pools.OVERWORLD.dst];
        delete pools.OVERWORLD;
      }

      if (this.settings.erMixedIndoors) {
        pools.MIXED.src = [...pools.MIXED.src, ...pools.INDOORS.src];
        pools.MIXED.dst = [...pools.MIXED.dst, ...pools.INDOORS.dst];
        delete pools.INDOORS;
      }
    }

    if (this.settings.erWarps !== 'none') {
      pools.WARPS = this.poolWarps();
    }

    if (this.settings.erOneWays !== 'none' && !this.settings.erOneWaysAnywhere) {
      pools.ONE_WAYS = this.poolOneWays();
    }

    return pools;
  }

  private makePools(): EntrancePoolDescrs {
    const pools = this.makePoolsSimple();

    if (this.settings.erSpawns !== 'none') {
      pools.SPAWNS = this.poolSpawns();
    }

    if (this.settings.erWallmasters !== 'none') {
      pools.WALLMASTERS = this.poolWallmasters();
    }

    if (this.settings.erOneWays !== 'none' && this.settings.erOneWaysAnywhere) {
      pools.ONE_WAYS = this.poolOneWaysAnywhere();
    }

    return pools;
  }

  private poolsTypesDst(): string[] {
    const pools = this.makePoolsSimple();
    const poolValues = Object.values(pools);
    const types = poolValues.map(x => x.dst).flat();
    return [...new Set(types)];
  }

  private connectGamesDefault() {
    switch (this.settings.games) {
    case 'oot':
      this.overrideEntrance('OOT_SHOP_MASKS', 'OOT_MARKET_FROM_MASK_SHOP');
      break;
    case 'mm':
      this.overrideEntrance('MM_CLOCK_TOWER_FROM_CLOCK_TOWN', 'MM_CLOCK_TOWN_FROM_CLOCK_TOWER');
      break;
    case 'ootmm':
      this.overrideEntrance('OOT_SHOP_MASKS', 'MM_CLOCK_TOWN_FROM_CLOCK_TOWER');
      this.overrideEntrance('MM_CLOCK_TOWER_FROM_CLOCK_TOWN', 'OOT_MARKET_FROM_MASK_SHOP');
      break;
    }

    this.worldChanged = false;
  }

  run() {
    this.overrides = {};

    /* Connect the games */
    if (!this.settings.erIndoorsGameLinks) {
      this.connectGamesDefault();
    }

    /* Plando */
    for (const [src, dst] of Object.entries(this.settings.plando.entrances)) {
      if (!this.allEntrances.includes(src as Entrance)
        || !this.allEntrances.includes(dst)
        || this.usedEntrancesSrc.has(src as Entrance)
        || this.usedEntrancesDst.has(dst))
        continue;

      this.overrideEntrance(src as Entrance, dst);
      const revSrc = this.reverseEntrance(src as Entrance);
      const revDst = this.reverseEntrance(dst);
      if (revSrc && revDst) {
        this.overrideEntrance(revDst, revSrc);
      }
    }

    this.placePools(this.makePools());

    let world = this.changedWorld(this.overrides);

    return { world, changed: this.worldChanged };
  }
};

export class LogicPassEntrances {
  private worlds: World[];

  constructor(
    private readonly input: {
      worlds: World[];
      settings: Settings;
      random: Random;
      monitor: Monitor;
      fixedLocations: Set<Location>,
      pool: PlayerItems;
      allItems: PlayerItems;
      renewableJunks: PlayerItems;
      startingItems: PlayerItems;
      itemProperties: ItemProperties;
    },
  ) {
    this.worlds = [];
  }

  private changeRegion(worldId: number, area: string, newRegion: string, force?: boolean) {
    const world = this.worlds[worldId];
    world.areas[area].region = newRegion;
    for (const loc of Object.keys(world.areas[area].locations)) {
      if (world.regions[loc] === 'ENTRANCE' || force) {
        world.regions[loc] = newRegion;
      }
    }
  }

  private isRegionInvalid(region: string) {
    return region === 'NONE' || region === 'ENTRANCE' || region === 'BUFFER' || region === 'BUFFER_DELAYED';
  }

  private propagateRegionsStep(worldId: number) {
    const world = this.worlds[worldId];
    let changed = false;
    for (const areaName of Object.keys(world.areas)) {
      const a = world.areas[areaName];
      if (this.isRegionInvalid(a.region))
        continue;
      /* We need to propagate the region */
      for (const exitName of Object.keys(a.exits)) {
        const exitArea = world.areas[exitName];
        if (exitArea == undefined) {
          throw new LogicEntranceError(`Unknown exit: ${exitName}`);
        }
        if (exitArea.region === 'ENTRANCE') {
          this.changeRegion(worldId, exitName, a.region);
          changed = true;
        }
      }
    }
    return changed;
  }

  private propagateDungeons(i: number) {
    const world = this.worlds[i];
    for (const dungeonEntrance of DUNGEON_ENTRANCES) {
      /* Dungeon is already marked somewhere */
      if (world.dungeonsEntrances.has(dungeonEntrance))
        continue;

      /* Dungeon is not marked, so it cannot be reached from another dungeon entrance */
      /* We need to figure our what replaces it */
      const overrides = world.entranceOverridesRev.get(dungeonEntrance) || [];
      for (const o of overrides) {
        const data = ENTRANCES[o as Entrance];
        const areaName = data.from;
        const area = world.areas[areaName];
        if (this.isRegionInvalid(area.region))
          continue;
        world.dungeonsEntrances.set(dungeonEntrance, { type: 'region', region: area.region as Region });
        break;
      }
    }
  }

  private propagateRegions(i: number) {
    for (;;) {
      if (!this.propagateRegionsStep(i)) {
        break;
      }
    }
    this.propagateDungeons(i);
  }

  private replaceAllRegions(worldId: number, oldRegion: string, newRegion: string) {
    const world = this.worlds[worldId];
    for (const areaName of Object.keys(world.areas)) {
      const a = world.areas[areaName];
      if (a.region === oldRegion) {
        this.changeRegion(worldId, areaName, newRegion, true);
      }
    }
  }

  private simplifyRegions(worldId: number) {
    this.replaceAllRegions(worldId, 'OOT_GANON_CASTLE_TOWER', 'OOT_GANON_CASTLE');
    this.replaceAllRegions(worldId, 'MM_TEMPLE_STONE_TOWER_INVERTED', 'MM_TEMPLE_STONE_TOWER');
    this.replaceAllRegions(worldId, 'MM_BUTLER_RACE', 'ENTRANCE');
    this.replaceAllRegions(worldId, 'MM_GORON_RACETRACK', 'ENTRANCE');
  }

  private processRegions() {
    for (let i = 0; i < this.worlds.length; ++i) {
      /* Simplify */
      if (!this.input.settings.extraHintRegions) {
        this.simplifyRegions(i);
      }

      this.propagateRegions(i);
      this.replaceAllRegions(i, 'BUFFER', 'ENTRANCE');
      this.propagateRegions(i);
      this.replaceAllRegions(i, 'BUFFER_DELAYED', 'ENTRANCE');
      this.propagateRegions(i);
      this.replaceAllRegions(i, 'ENTRANCE', 'NAMELESS');
      this.propagateDungeons(i);
    }
  }

  private validate() {
    if (this.input.settings.logic === 'none')
      return;

    const pathfinder = new Pathfinder(this.worlds, this.input.settings, this.input.startingItems);
    const pathfinderState = pathfinder.run(null, { assumedItems: this.input.allItems, recursive: true });

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

    /* Validate using the solver */
    const solver = new LogicPassSolver({ ...this.input, worlds: this.worlds });
    solver.validate();
  }

  private runAttempt() {
    this.worlds = [];
    let changed = false;

    if (this.input.settings.distinctWorlds) {
      /* Distinct worlds */
      for (let i = 0; i < this.input.worlds.length; ++i) {
        const shuffler = new WorldShuffler(this.input.random, i, this.input.worlds, this.input.settings, this.input.startingItems, this.input.allItems);
        const result = shuffler.run();
        this.worlds.push(result.world);
        changed = changed || result.changed;
      }
    } else {
      /* Shared world */
      const shuffler = new WorldShuffler(this.input.random, 0, this.input.worlds, this.input.settings, this.input.startingItems, this.input.allItems);
      const result = shuffler.run();
      changed = result.changed;
      for (let i = 0; i < this.input.worlds.length; ++i) {
        this.worlds.push(cloneWorld(result.world));
      }
    }

    /* Validate */
    if (changed) {
      this.validate();
    }

    this.processRegions();

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

