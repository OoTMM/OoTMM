import { test, expect } from 'vitest';

import { makeTestSeed } from '../helpers';

test("Can make a seed - Cow Shuffle", async () => {
  const res = await makeTestSeed("COWS", {
    cowShuffleOot: true,
    cowShuffleMm: true,
  });
  expect(res).toBeDefined();
});
