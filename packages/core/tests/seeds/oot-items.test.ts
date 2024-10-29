import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - OoT Items", async () => {
  const res = await makeTestSeed("OOT ITEMS", {
    shortHookshotMm: true,
    fairyOcarinaMm: true,
  });
  expect(res).toBeDefined();
});
