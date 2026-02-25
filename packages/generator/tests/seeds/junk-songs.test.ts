import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - junk song locations", async () => {
  const res = await makeTestSeed("JUNKSONGS", {
    startingItems: {
      OOT_SONG_TIME: 1,
      MM_SONG_TIME: 1,
    },
    junkLocations: [
      'MM Laboratory Zora Song',
      'OOT Hyrule Field Song of Time',
    ]
  });
  expect(res).toBeDefined();
});
