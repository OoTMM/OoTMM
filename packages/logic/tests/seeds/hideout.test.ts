import { test, expect } from 'vitest';

import { makeTestSeed } from '../helpers';

test("Can make a seed - hideout shuffle", async () => {
  const res = await makeTestSeed("HIDEOUT", {
    smallKeyShuffleHideout: 'anywhere',
  });
  expect(res).toBeDefined();
});
