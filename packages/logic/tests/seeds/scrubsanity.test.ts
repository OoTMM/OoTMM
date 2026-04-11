import { test, expect } from 'vitest';

import { makeTestSeed } from '../helpers';

test("Can make a seed - scrubsanity", async () => {
  const res = await makeTestSeed("SCRUBS", {
    scrubShuffleOot: true,
  });
  expect(res).toBeDefined();
});
