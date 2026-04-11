import { test, expect } from 'vitest';

import { makeTestSeed } from '../helpers';

test("Can make a seed - Beatable Only", async () => {
  const res = await makeTestSeed("BEATABLE ONLY", {
    logic: 'beatable'
  });
  expect(res).toBeDefined();
});
