import type { EnemyPlan, EnemyPlanCompatibility, EnemyDefinition, Options } from '@ootmm/core';
import type { Item, PlayerItem, PlayerItems } from './items';
import type { ItemPlacement } from './types';
import type { Hints } from './hints';
import type { Analysis } from './analysis';
import type { DungeonEntrance, ResolvedWorldFlags, World } from './world';
import type { Location } from './types';

import { Monitor, Random, enemyDefinition, enemyWeight, enemyPlacementLogicName, enemyLogicMacros } from '@ootmm/core';
import { MACROS } from '@ootmm/data';
import { ExprParser, exprAnd, exprOr, exprTrue, type Expr } from './expr';
import { ItemGroups } from './items';
import { EnemyCompatibilityError } from './error';
import { logicPassConfig, logicPassFixer, logicPassHash, logicPassSongEvents } from './misc';
import { logicPassItemProperties } from './items/properties';
import { logicPassWorld, logicPassWorldTransform } from './world';
import { logicPassCloak, logicPassEntrances, logicPassMinimize, logicPassSolver } from './solver';
import { logicPassAnalysis, logicPassAnalysisFoolish, logicPassAnalysisPaths } from './analysis';
import { logicPassHints } from './hints';
import { logicPassPrice } from './price';

type LogicPassFunc<State, Out> = (state: State) => Out;

class LogicPipeline<State> {
  constructor(private readonly state: State) {
  }

  apply<Out>(pass: LogicPassFunc<State, Out>): LogicPipeline<State & Out> {
    const newState = { ...this.state, ...pass(this.state) };
    return new LogicPipeline(newState);
  }

  exec(): State {
    return this.state;
  }
};

export type LogicResultWorld = {
  resolvedFlags: ResolvedWorldFlags;
  enemyPlan: EnemyPlan;
  prices: number[];
  entranceOverrides: Map<string, string>;
  bossIds: number[];
  songEventsOot: number[];
  songEventsMm: number[];
  dungeonsEntrances: Map<string, DungeonEntrance>;
  preCompleted: Set<string>;
  locations: Set<string>;
  regions: { [k: string]: string };
  dungeons: { [k: string]: Set<string> };
};

export type LogicResult = {
  worlds: LogicResultWorld[];
  startingItems: PlayerItems;
  plandoLocations: Map<Location, PlayerItem>;
  items: ItemPlacement;
  itemCloaks: Map<Location, Item>;
  analysis: Analysis;
  hints: Hints;
  hash: string;
};

function pipeline<State>(state: State): LogicPipeline<State> {
  return new LogicPipeline(state);
}

export const worldState = async (monitor: Monitor, opts: Options, compatibility: readonly EnemyPlanCompatibility[] = [],
  attempt = 0, resolvedWorldFlags?: readonly ResolvedWorldFlags[]) => {
  const random = new Random();
  const seedKey = [opts.seed.length.toString(16), opts.seed, opts.settings.generateSpoilerLog ? '1' : '0'].join('\0');
  await random.seed(attempt ? seedKey + '\0enemy-compatibility-' + attempt : seedKey);
  const state = { monitor, opts, settings: opts.settings, random };

  return pipeline(state)
    .apply(logicPassConfig)
    .apply(logicPassItemProperties)
    .apply(state => logicPassWorld({ ...state, enemyCompatibility: compatibility, resolvedWorldFlags }))
    .apply(logicPassFixer)
    .apply(logicPassWorldTransform)
    .exec();
};

function enemyCompatibilityRules(world: World, settings: Options['settings']) {
  const expressions = new Map<string, Expr>();
  return (name: string, allows: (candidate: Expr) => boolean) => {
    const row = world.enemyPlan.find(row => enemyPlacementLogicName(row) === name);
    if (!row) throw new Error('Missing incompatible enemy placement: ' + name);
    const host = enemyDefinition(row.source).game;
    const cache = new Map<string, boolean>();
    return (target: EnemyDefinition): boolean => {
      if (enemyWeight(settings, host, target) <= 0) return false;
      const key = host + '/' + target.key + '/' + target.spawnParams;
      let result = cache.get(key);
      if (result !== undefined) return result;
      let expr = expressions.get(key);
      if (!expr) {
        const probe = { source: row.source, target: target.key, kind: 'gold' as const };
        const macros = enemyLogicMacros({ ...MACROS.common, ...MACROS[host] }, [probe], host);
        const parser = new ExprParser(settings, world.resolvedFlags, host);
        for (const [signature, body] of Object.entries(macros)) {
          const [macro, ...args] = signature.replace(/[(),]/g, ' ').split(/\s+/).filter(Boolean);
          parser.addMacro(macro, args, body);
        }
        const assumeSoul = (value: Expr): Expr => {
          if (value.type === 'item' && ItemGroups.SOULS_ENEMY.has(value.item)) return exprTrue();
          if (value.type === 'and') return exprAnd(value.exprs.map(assumeSoul));
          if (value.type === 'or') return exprOr(value.exprs.map(assumeSoul));
          return value;
        };
        expr = assumeSoul(parser.parse(enemyPlacementLogicName(probe)!));
        expressions.set(key, expr);
      }
      result = allows(expr);
      cache.set(key, result);
      return result;
    };
  };
}

export const solvedWorldState = async (monitor: Monitor, opts: Options) => {
  const compatibility = Array.from({ length: opts.settings.players }, () => new Map<string, (target: EnemyDefinition) => boolean>());
  let resolvedWorldFlags: ResolvedWorldFlags[] | undefined;
  let retryWorlds: World[] | undefined;
  for (let attempt = 0; ; ++attempt) {
    const state = await worldState(monitor, opts, compatibility, attempt, resolvedWorldFlags);
    resolvedWorldFlags ??= state.worlds.map(world => world.resolvedFlags);
    try {
      return pipeline(state)
        .apply(logicPassPrice)
        .apply(logicPassSongEvents)
        .apply(state => {
          if (retryWorlds) state.worlds.forEach((world, i) => {
            world.prices = [...retryWorlds![i].prices];
            world.songEventsOot = [...retryWorlds![i].songEventsOot];
            world.songEventsMm = [...retryWorlds![i].songEventsMm];
          });
          return logicPassEntrances({ ...state, enemyEntranceOverrides: retryWorlds?.map(world => world.entranceOverrides) });
        })
        .apply(logicPassSolver)
        .apply(logicPassMinimize)
        .exec();
    } catch (error) {
      if (!(error instanceof EnemyCompatibilityError) || opts.settings.logic !== 'allLocations') throw error;
      if (attempt >= 15) throw new Error('Could not find compatible enemy replacements after 16 attempts');
      retryWorlds = error.worlds;
      const rules = state.worlds.map(world => enemyCompatibilityRules(world, opts.settings));
      for (const failure of error.failures) {
        const previous = compatibility[failure.world].get(failure.name);
        const allowed = rules[failure.world](failure.name, failure.allows);
        compatibility[failure.world].set(failure.name, target => (!previous || previous(target)) && allowed(target));
      }
      monitor.log(`Rerolling impossible enemy fights (${error.failures.length} checks, attempt ${attempt + 2})`);
    }
  }
};

export async function logic(monitor: Monitor, opts: Options): Promise<LogicResult> {
  const state = await solvedWorldState(monitor, opts);

  const data = pipeline(state)
    .apply(logicPassCloak)
    .apply(logicPassAnalysis)
    .apply(logicPassAnalysisPaths)
    .apply(logicPassAnalysisFoolish)
    .apply(logicPassHints)
    .apply(logicPassHash)
    .exec();

    const worlds: LogicResultWorld[] = data.worlds.map(w => ({
      resolvedFlags: w.resolvedFlags,
    enemyPlan: w.enemyPlan,
      prices: w.prices,
      entranceOverrides: w.entranceOverrides,
      bossIds: w.bossIds,
      songEventsOot: w.songEventsOot,
      songEventsMm: w.songEventsMm,
      dungeonsEntrances: w.dungeonsEntrances,
      preCompleted: w.preCompleted,
      locations: w.locations,
      regions: w.regions,
      dungeons: w.dungeons,
    }));

    return {
      worlds,
      startingItems: data.startingItems,
      plandoLocations: data.plandoLocations,
      items: data.items,
      itemCloaks: data.itemCloaks,
      analysis: data.analysis,
      hints: data.hints,
      hash: data.hash,
    };
};
