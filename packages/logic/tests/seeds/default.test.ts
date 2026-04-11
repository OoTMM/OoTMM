import { test, expect } from 'vitest';

import { makeTestSeed } from '../helpers';

test("Can make a seed - default", async () => {
  const res = await makeTestSeed("DEFAULT", {});
  expect(res).toBeDefined();
});
