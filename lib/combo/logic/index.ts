import { Random } from '../random';
import { LogicPassSolver } from './solve';
import { createWorld, WorldCheck } from './world';
import { LogicPassSpoiler } from './spoiler';
import { Options } from '../options';
import { Hints, LogicPassHints } from './hints';
import { alterWorld, configFromSettings } from './settings';
import { LogicPassPlaythrough } from './playthrough';
import { Monitor } from '../monitor';
import { LogicPassEntrances } from './entrance';
import { LogicPassHash } from './hash';

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

const createState = (monitor: Monitor, opts: Options) => {
  const random = new Random();
  random.seed(opts.seed);
  const world = createWorld(opts.settings);
  const config = configFromSettings(opts.settings);
  alterWorld(world, opts.settings, config);

  return { opts, monitor, random, world, config, settings: opts.settings };
};

export const logic = (monitor: Monitor, opts: Options) => {
  const state = pipeline(
    createState(monitor, opts)
  ).apply(LogicPassEntrances)
  .apply(LogicPassSolver)
  .apply(LogicPassPlaythrough)
  .apply(LogicPassHints)
  .apply(LogicPassSpoiler)
  .apply(LogicPassHash)
  .exec();

  return state;
};

export type LogicResult = ReturnType<typeof logic>;
