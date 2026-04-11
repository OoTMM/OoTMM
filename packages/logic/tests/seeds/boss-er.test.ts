import { test, expect } from 'vitest';

import { makeTestSeed } from '../helpers';

test("Can make a seed - Boss ER", async () => {
  const res = await makeTestSeed("BOSS ER", {
    songs: 'anywhere',
    erBoss: 'full',
  });
  expect(res).toBeDefined();
});
