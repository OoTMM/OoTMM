import { makeTestSeed } from '../helper';

test("Can make a seed - Cow Shuffle", async () => {
  await makeTestSeed("COWS", {
    cowShuffleOot: true,
    cowShuffleMm: true,
  });
});
