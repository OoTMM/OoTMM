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
    dungeon: {
      DT: 'mq',
      DC: 'mq',
      JJ: 'mq',
      Forest: 'mq',
      Fire: 'mq',
      Water: 'mq',
      Spirit: 'mq',
      Shadow: 'mq',
      BotW: 'mq',
      IC: 'mq',
      GTG: 'mq',
      Ganon: 'mq',
    }
  });
  expect(res).toBeDefined();
});
