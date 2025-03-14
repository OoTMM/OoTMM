import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - allsanity", async () => {
  const res = await makeTestSeed("ALLSANITY", {
    songs: 'anywhere',
    goldSkulltulaTokens: 'all',
    housesSkulltulaTokens: 'all',
    strayFairyChestShuffle: 'anywhere',
    strayFairyOtherShuffle: 'anywhere',
    townFairyShuffle: 'anywhere',
    bossKeyShuffleOot: 'anywhere',
    bossKeyShuffleMm: 'anywhere',
    smallKeyShuffleOot: 'anywhere',
    smallKeyShuffleMm: 'anywhere',
    smallKeyShuffleHideout: 'anywhere',
    cowShuffleOot: true,
    cowShuffleMm: true,
    shopShuffleOot: "full",
    shopShuffleMm: "full",
    eggShuffle: true,
    divingGameRupeeShuffle: true,
    pondFishShuffle: true,
    fairyFountainFairyShuffleOot: true,
    fairyFountainFairyShuffleMm: true,
    scrubShuffleOot: true,
    childWallets: true,
    colossalWallets: true,
    tingleShuffle: 'anywhere',
    owlShuffle: 'anywhere',
  });
  expect(res).toBeDefined();
});
