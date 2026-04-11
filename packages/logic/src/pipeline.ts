import type { Item, Options, PlayerItem, PlayerItems } from '@ootmm/core';
import type { ItemPlacement } from './types';
import type { Hints } from './hints';
import type { Analysis } from './analysis';
import type { DungeonEntrance, ResolvedWorldFlags, WorldCheck } from './world';
import type { Location } from './types';

import { Monitor, Random } from '@ootmm/core';
import { logicPassConfig, logicPassFixer, logicPassHash, logicPassSongEvents } from './misc';
import { logicPassItemProperties } from './item-properties';
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
  checks: { [k: string]: WorldCheck };
  prices: number[];
  entranceOverrides: Map<string, string>;
  bossIds: number[];
  songEvents: number[];
  dungeonsEntrances: Map<string, DungeonEntrance>;
  preCompleted: Set<string>;
  locations: Set<string>;
  regions: { [k: string]: string };
  dungeons: { [k: string]: Set<string> };
  checkHints: { [k: string]: string[] };
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

export const worldState = async (monitor: Monitor, opts: Options) => {
  const random = new Random();
  const seedKey = [opts.seed.length.toString(16), opts.seed, opts.settings.generateSpoilerLog ? '1' : '0'].join('\0');
  await random.seed(seedKey);
  const state = { monitor, opts, settings: opts.settings, random };

  return pipeline(state)
    .apply(logicPassConfig)
    .apply(logicPassItemProperties)
    .apply(logicPassWorld)
    .apply(logicPassFixer)
    .apply(logicPassWorldTransform)
    .exec();
};

export const solvedWorldState = async (monitor: Monitor, opts: Options) => {
  let state = await worldState(monitor, opts);
  return pipeline(state)
    .apply(logicPassPrice)
    .apply(logicPassSongEvents)
    .apply(logicPassEntrances)
    .apply(logicPassSolver)
    .apply(logicPassMinimize)
    .exec();
}

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
      checks: w.checks,
      prices: w.prices,
      entranceOverrides: w.entranceOverrides,
      bossIds: w.bossIds,
      songEvents: w.songEvents,
      dungeonsEntrances: w.dungeonsEntrances,
      preCompleted: w.preCompleted,
      locations: w.locations,
      regions: w.regions,
      dungeons: w.dungeons,
      checkHints: w.checkHints,
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
