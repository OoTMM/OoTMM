import { makeTestSeed } from '../helper';

test("Can make a seed - No Logic", async () => {
  await makeTestSeed("NOLOGIC", {
    logic: 'none'
  });
});
