import { makeTestSeed } from '../helper';

test("Can make a seed - No Logic", () => {
  makeTestSeed("NOLOGIC", {
    logic: 'none'
  });
});
