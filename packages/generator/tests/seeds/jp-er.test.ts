import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - JP Layout + ER", async () => {
  const res = await makeTestSeed("JP", {
    jpLayouts: { type: 'all' },
    erGrottos: 'full',
  });
  expect(res).toBeDefined();
});
