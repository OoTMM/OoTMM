import { makeTestSeed } from '../helper';

test("Can make a seed - allsanity", () => {
  makeTestSeed("ALLSANITY", {
    songs: 'anywhere',
    goldSkulltulaTokens: 'all',
    housesSkulltulaTokens: 'all',
    strayFairyShuffle: 'anywhere',
    townFairyShuffle: 'anywhere',
    bossKeyShuffle: 'anywhere',
    smallKeyShuffle: 'anywhere',
    smallKeyShuffleHideout: 'anywhere',
    cowShuffleOot: true,
    cowShuffleMm: true,
    shopShuffleOot: "full",
    eggShuffle: true,
  });
});
