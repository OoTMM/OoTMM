import { makeTestSeed } from '../helper';

test("Can make a seed - allsanity", () => {
  makeTestSeed("ALLSANITY", {
    songs: 'anywhere',
    goldSkulltulaTokens: 'all',
    housesSkulltulaTokens: 'all',
    strayFairyShuffle: 'anywhere',
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
    scrubShuffleOot: true,
    childWallets: true,
    colossalWallets: true,
    tingleShuffle: 'anywhere',
    owlShuffle: 'anywhere',
  });
});
