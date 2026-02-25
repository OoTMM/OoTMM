import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - Dungeon ER", async () => {
  const res = await makeTestSeed("DUNGEON ER", {
    songs: 'anywhere',
    erDungeons: 'full',
    erMajorDungeons: true,
    erMinorDungeons: true,
    erSpiderHouses: true,
    erIkanaCastle: true,
  });
  expect(res).toBeDefined();
});
