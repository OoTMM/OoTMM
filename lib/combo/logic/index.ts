import { Random } from '../random';
import { ItemPlacement, solve } from './solve';
import { createWorld, WorldCheck } from './world';
import { spoiler } from './spoiler';
import { LogicSeedError } from './error';
import { Options } from '../options';
import { hints, Hints } from './hints';
import { alterWorld, configFromSettings } from './settings';
import { playthrough } from './playthrough';

export type LogicResult = {
  items: WorldCheck[];
  log: string;
  hints: Hints;
  config: Set<string>;
};

export const logic = (opts: Options): LogicResult => {
  const config = configFromSettings(opts.settings);
  const world = createWorld(opts.settings);
  alterWorld(world, opts.settings, config);
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

  let spheres: string[][] = [];
  if (!opts.settings.noLogic) {
    spheres = playthrough(random, world, placement);
  }
  const log = spoiler(world, placement, spheres, opts);

  const items: WorldCheck[] = [];
  for (const loc in placement) {
    const check = world.checks[loc];
    items.push({ ...check, item: placement[loc] });
  }
  const h = hints(random, world, placement, spheres);

  return { items, log, hints: h, config };
};
