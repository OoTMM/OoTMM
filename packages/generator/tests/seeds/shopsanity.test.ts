import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - shopsanity", async () => {
  const res = await makeTestSeed("SHOPSANITY", {
    shopShuffleOot: "full",
    shopShuffleMm: "full",
  });
  expect(res).toBeDefined();
});
