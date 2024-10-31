import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';


test("Can make a seed - Boss/Dungeon ER", async () => {
  const res = await makeTestSeed("Boss/Dungeon ER", {
    songs: 'anywhere',
    erBoss: 'full',
    erDungeons: 'full',
    erMajorDungeons: true,
    erMinorDungeons: true,
    erSpiderHouses: true,
    erIkanaCastle: true,
  });
  expect(res).toBeDefined();
});
