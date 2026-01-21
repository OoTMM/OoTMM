import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - Double Song of Time for GS", async () => {
  const res = await makeTestSeed("DoubleSoT", {
    songOfDoubleTimeOot: true,
    plando: {
      locations: {
        "OOT Water Temple GS Large Pit": "OOT_SONG_SUN",
        "OOT Kakariko GS Ladder": "OOT_HOOKSHOT",
        "OOT Kokiri Forest GS Night Adult": "OOT_HOOKSHOT"
      }
    },
    startingItems: {
      OOT_SONG_TIME: 1,
    },
    tricks: [],
  });

  expect(res).toBeDefined();
})
