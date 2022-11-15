import { Random, randString } from '../random';
import { ItemPlacement, solve } from './solve';
import { createWorld, WorldCheck } from './world';
import { spoiler } from './spoiler';
import { LogicSeedError } from './error';
import { Options } from '../options';

export type LogicResult = {
  items: WorldCheck[];
  log: string;
};

const getSeed = (opts: Options): string => {
  if (opts.seed) {
    return opts.seed;
  }
  if (opts.debug) {
    return 'DEBUG';
  }
  return randString();
}

export const logic = async (opts: Options): Promise<LogicResult> => {
  const seed = getSeed(opts);
  const world = await createWorld();
  const random = new Random();
  random.seed(seed);

  let placement: ItemPlacement = {};
  let error: Error | null = null;
  for (let i = 0; i < 20; ++i) {
    try {
      error = null;
      placement = solve(world, random);
      break;
    } catch (e) {
      if (!(e instanceof LogicSeedError)) {
        throw e;
      }
      error = e;
    }
  }
  if (error) {
    throw error;
  }
  const log = spoiler(world, placement, seed);

  const items: WorldCheck[] = [];
  for (const loc in placement) {
    const check = world.checks[loc];
    items.push({ ...check, item: placement[loc] });
  }
  return { items, log };
};
