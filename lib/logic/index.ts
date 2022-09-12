import { Random } from '../random';
import { Solver } from './solve';
import { createWorld } from './world';

export const logic = async () => {
  const world = await createWorld();
  const random = new Random();
  random.seed();

  const solver = new Solver(world, random);
  const placement = solver.solve();

  console.log(placement);
};
