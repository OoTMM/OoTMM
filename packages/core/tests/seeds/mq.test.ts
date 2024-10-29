import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - MQ", async () => {
  const res = await makeTestSeed("MQ", {
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
