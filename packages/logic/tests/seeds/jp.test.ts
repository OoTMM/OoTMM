import { test, expect } from 'vitest';

import { makeTestSeed } from '../helpers';

test("Can make a seed - JP Layout", async () => {
  const res = await makeTestSeed("JP", {
    jpLayouts: { type: 'all' },
  });
  expect(res).toBeDefined();
});
