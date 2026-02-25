import { bench } from 'vitest';

import { makeTestSeed } from '../helper';

bench("Default", async () => {
  const res = await makeTestSeed("DEFAULT", {});
});
