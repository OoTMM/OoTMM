import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';


test("Can make a seed - MQ Boss/Dungeon ER", async () => {
  const res = await makeTestSeed("MQ Boss/Dungeon ER", {
    songs: 'anywhere',
    erBoss: 'full',
    erDungeons: 'full',
    erMajorDungeons: true,
    erMinorDungeons: true,
    erSpiderHouses: true,
    erIkanaCastle: true,
    mqDungeons: { type: 'all' }
  });
  expect(res).toBeDefined();
});
