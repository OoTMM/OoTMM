import { Random } from '../random';
import { ItemPlacement, solve } from './solve';
import { createWorld, WorldCheck } from './world';
import { spoiler } from './spoiler';
import { LogicSeedError } from './error';
import { Options } from '../options';
import { hints, Hints } from './hints';
import { alterWorld, configFromSettings } from './settings';
import { playthrough } from './playthrough';
import { Monitor } from '../monitor';

export type LogicResult = {
  items: WorldCheck[];
  log: string;
  hints: Hints;
  config: Set<string>;
  hash: string;
};

const ALPHABET = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';

const seedHash = (random: Random): string => {
  const letters: string[] = [];
  for (let i = 0; i < 8; ++i) {
    letters.push(ALPHABET[random.next() % ALPHABET.length]);
  }
  return letters.join('');
};

export const logic = (monitor: Monitor, opts: Options): LogicResult => {
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
    spheres = playthrough(opts.settings, random, world, placement);
  }
  const items: WorldCheck[] = [];
  for (const loc in placement) {
    const check = world.checks[loc];
    items.push({ ...check, item: placement[loc] });
  }
  const h = hints(monitor, random, opts.settings, world, placement, spheres);
  const log = spoiler(world, placement, spheres, opts, h);
  const hash = seedHash(random);

  return { items, log, hints: h, config, hash };
};
