import { Random, randString } from '../random';
import { ItemPlacement, solve } from './solve';
import { createWorld, WorldCheck } from './world';
import { spoiler } from './spoiler';
import { LogicSeedError } from './error';
import { Options } from '../options';
import { dungeonRewardRegions } from './hints';

export type LogicResult = {
  items: WorldCheck[];
  dungeonRewards: string[];
  log: string;
};

export const logic = (opts: Options): LogicResult => {
  const world = createWorld(opts.settings);
  const random = new Random();
  random.seed(opts.seed);

  let placement: ItemPlacement = {};
  let error: Error | null = null;
  for (let i = 0; i < 100; ++i) {
    try {
      error = null;
      placement = solve(opts, world, random);
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
  const log = spoiler(world, placement, opts.seed);

  const items: WorldCheck[] = [];
  for (const loc in placement) {
    const check = world.checks[loc];
    items.push({ ...check, item: placement[loc] });
  }
  const dungeonRewards = dungeonRewardRegions(placement);
  return { items, log, dungeonRewards };
};
