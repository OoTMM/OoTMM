import { test, expect } from 'vitest';

import { makeTestSeed } from '../helpers';

test("Can make a seed - MQ", async () => {
  const res = await makeTestSeed("MQ", {
    mqDungeons: { type: 'all' }
  });
  expect(res).toBeDefined();
});
