import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - Full ER", async () => {
  const res = await makeTestSeed("FULL ER", {
    songs: 'anywhere',
    erDungeons: 'full',
    erBoss: 'full',
    erRegions: 'full',
    erRegionsShortcuts: true,
    erIndoors: 'full',
    erIndoorsMajor: true,
    erIndoorsExtra: true,
    erOneWays: 'full',
    erOneWaysMajor: true,
    erOneWaysIkana: true,
    erOneWaysSongs: true,
    erOneWaysStatues: true,
    erOneWaysOwls: true,
    erMajorDungeons: true,
    erMinorDungeons: true,
    erSpiderHouses: true,
    erPirateFortress: true,
    erBeneathWell: true,
    erIkanaCastle: true,
    erSecretShrine: true,
  });
  expect(res).toBeDefined();
});
