import { Random } from '../random';
import { solve } from './solve';
import { createWorld, WorldCheck } from './world';
import { spoiler } from './spoiler';

type LogicResult = {
  items: WorldCheck[];
  log: string;
};

export const logic = async (): Promise<LogicResult> => {
  const world = await createWorld();
  const random = new Random();
  random.seed();

  const placement = solve(world, random);
  const log = spoiler(world, placement);

  const items: WorldCheck[] = [];
  for (const loc in placement) {
    const check = world.checks[loc];
    items.push({ ...check, item: placement[loc] });
  }
  return { items, log };
};
