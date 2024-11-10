import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - JP Layout + ER", async () => {
  const res = await makeTestSeed("JP", {
    worldLayoutMm: 'jp',
    erGrottos: 'full',
  });
  expect(res).toBeDefined();
});
