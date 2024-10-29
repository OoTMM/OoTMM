import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - hideout shuffle", async () => {
  const res = await makeTestSeed("HIDEOUT", {
    smallKeyShuffleHideout: 'anywhere',
  });
  expect(res).toBeDefined();
});
