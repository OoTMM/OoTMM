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
import { Settings } from '../settings';

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

export const worldState = (monitor: Monitor, settings: Settings) => {
  const state = { monitor, settings }

  return pipeline(state)
    .apply(LogicPassConfig)
    .apply(LogicPassWorld)
    .apply(LogicPassWorldTransform)
    .exec();
};

export const logic = (monitor: Monitor, opts: Options) => {
  const random = new Random();
  random.seed(opts.seed);

  const ws = worldState(monitor, opts.settings);
  const state = { ...ws, opts, random };

  return pipeline(state)
    .apply(LogicPassEntrances)
    .apply(LogicPassSolver)
    .apply(LogicPassAnalysis)
    .apply(LogicPassHints)
    .apply(LogicPassSpoiler)
    .apply(LogicPassHash)
    .exec();
};

export type LogicResult = ReturnType<typeof logic>;
