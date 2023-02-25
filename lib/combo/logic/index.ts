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
import { LogicError } from './error';

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

const solvedWorldState = (monitor: Monitor, opts: Options) => {
  const random = new Random();
  random.seed(opts.seed + opts.settings.generateSpoilerLog);
  let state = { ...worldState(monitor, opts.settings), attempts: 0, random, opts };

  for (;;) {
    state.attempts++;
    try {
      const newState = pipeline(state)
        .apply(LogicPassEntrances)
        .apply(LogicPassSolver)
        .exec();
      return newState;
    } catch (e) {
      if (!(e instanceof LogicError) || state.attempts >= 1000) {
        throw e;
      }
    }
  }
}

export const logic = (monitor: Monitor, opts: Options) => {
  const state = solvedWorldState(monitor, opts);

  return pipeline(state)
    .apply(LogicPassAnalysis)
    .apply(LogicPassHints)
    .apply(LogicPassSpoiler)
    .apply(LogicPassHash)
    .exec();
};

export type LogicResult = ReturnType<typeof logic>;
