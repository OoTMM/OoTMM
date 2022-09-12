import { createWorld } from './world';

export const logic = async () => {
  const world = await createWorld();
  console.log(world);
};
