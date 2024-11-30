import { Random } from '../random';
import { LogicPassSolver } from './solve';
import { LogicPassWorld } from './world';
import { LogicPassSpoiler } from './spoiler';
import { Options } from '../options';
import { LogicPassHints } from './hints';
import { LogicPassAnalysis } from './analysis';
import { Monitor } from '../monitor';
import { LogicPassEntrances } from './entrance';
import { LogicPassHash } from './hash';
import { LogicPassConfig } from './config';
import { LogicPassWorldTransform } from './world-transform';
import { LogicPassFixer } from './fixer';
import { LogicPassAnalysisFoolish } from './analysis-foolish';
import { LogicPassPrice } from './price';
import { LogicPassItemProperties } from './item-properties';
import { LogicPassMinimize } from './minimize';
import { LogicPassAnalysisPaths } from './analysis-path';

interface LogicPass<Out> {
  run: () => Out;
}

type LogicPassConstructor<In, Out> = new (state: In) => LogicPass<Out>;

class LogicPipeline<State> {
  constructor(private readonly state: State) {
  }

  apply<Out>(pass: LogicPassConstructor<State, Out>): LogicPipeline<State & Out> {
    const passInstance = new pass(this.state);
    const newState = { ...this.state, ...passInstance.run() };
    return new LogicPipeline(newState);
  }

  exec(): State {
    return this.state;
  }
};

function pipeline<State>(state: State): LogicPipeline<State> {
  return new LogicPipeline(state);
}

export const worldState = async (monitor: Monitor, opts: Options) => {
  const random = new Random();
  const seedKey = [opts.seed.length.toString(16), opts.seed, opts.settings.generateSpoilerLog ? '1' : '0'].join('\0');
  await random.seed(seedKey);
  const state = { monitor, opts, settings: opts.settings, random, attempts: 0 };

  return pipeline(state)
    .apply(LogicPassConfig)
    .apply(LogicPassItemProperties)
    .apply(LogicPassWorld)
    .apply(LogicPassFixer)
    .apply(LogicPassWorldTransform)
    .exec();
};

export const solvedWorldState = async (monitor: Monitor, opts: Options) => {
  let state = await worldState(monitor, opts);
  return pipeline(state)
    .apply(LogicPassPrice)
    .apply(LogicPassEntrances)
    .apply(LogicPassSolver)
    .apply(LogicPassMinimize)
    .exec();
}

export const logic = async (monitor: Monitor, opts: Options) => {
  const state = await solvedWorldState(monitor, opts);

  const data = pipeline(state)
    .apply(LogicPassAnalysis)
    .apply(LogicPassAnalysisPaths)
    .apply(LogicPassAnalysisFoolish)
    .apply(LogicPassHints)
    .apply(LogicPassSpoiler)
    .apply(LogicPassHash)
    .exec();

    return data;
};

export type LogicResult = Awaited<ReturnType<typeof logic>>;
