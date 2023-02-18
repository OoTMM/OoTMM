import { Random } from '../random';
import { ItemPlacement, LogicPassSolver, solve } from './solve';
import { createWorld, World, WorldCheck } from './world';
import { spoiler } from './spoiler';
import { LogicSeedError } from './error';
import { Options } from '../options';
import { hints, Hints } from './hints';
import { alterWorld, configFromSettings } from './settings';
import { playthrough } from './playthrough';
import { Monitor } from '../monitor';
import { LogicPassEntrances } from './entrance';

export type LogicResult = {
  items: WorldCheck[];
  log: string;
  hints: Hints;
  config: Set<string>;
  hash: string;
  entrances: EntranceShuffleResult;
};

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

const ALPHABET = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';

const seedHash = (random: Random): string => {
  const letters: string[] = [];
  for (let i = 0; i < 8; ++i) {
    letters.push(ALPHABET[random.next() % ALPHABET.length]);
  }
  return letters.join('');
};

const createState = (opts: Options) => {
  const random = new Random();
  random.seed(opts.seed);
  const world = createWorld(opts.settings);
  const config = configFromSettings(opts.settings);
  alterWorld(world, opts.settings, config);

  return { random, world, config, settings: opts.settings };
};

export const logic = (monitor: Monitor, opts: Options): LogicResult => {
  const state = pipeline(
    createState(opts)
  ).apply(LogicPassEntrances)
  .apply(LogicPassSolver)
  .exec();

  let spheres: string[][] = [];
  if (!opts.settings.noLogic) {
    spheres = playthrough(opts.settings, state.random, state.world, placement);
  }
  const items: WorldCheck[] = [];
  for (const loc in placement) {
    const check = state.world.checks[loc];
    items.push({ ...check, item: placement[loc] });
  }
  const h = hints(monitor, state.random, opts.settings, state.world, placement, spheres);
  const log = spoiler(state.world, placement, spheres, opts, h, entrances);
  const hash = seedHash(state.random);

  return { items, log, hints: h, config: state.config, hash, entrances };
};
