import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - No Logic", async () => {
  const res = await makeTestSeed("NOLOGIC", {
    logic: 'none'
  });
  expect(res).toBeDefined();
});
