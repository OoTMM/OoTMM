import { Random } from '../random';
import { solve } from './solve';
import { createWorld } from './world';
import { spoiler } from './spoiler';

export const logic = async () => {
  const world = await createWorld();
  const random = new Random();
  random.seed();

  const placement = solve(world, random);
  const log = spoiler(world, placement);
  console.log(log);
};
