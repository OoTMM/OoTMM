import { Random } from '../random';
import { solve } from './solve';
import { createWorld } from './world';

export const logic = async () => {
  const world = await createWorld();
  const random = new Random();
  random.seed();

  const placement = solve(world, random);

  console.log(placement);
};
