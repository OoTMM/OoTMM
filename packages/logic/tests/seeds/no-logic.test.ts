import { test, expect } from 'vitest';

import { makeTestSeed } from '../helpers';

test("Can make a seed - No Logic", async () => {
  const res = await makeTestSeed("NOLOGIC", {
    logic: 'none'
  });
  expect(res).toBeDefined();
});
