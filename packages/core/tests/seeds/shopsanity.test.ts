import { makeTestSeed } from '../helper';

test("Can make a seed - shopsanity", async () => {
  await makeTestSeed("SHOPSANITY", {
    shopShuffleOot: "full",
    shopShuffleMm: "full",
  });
});
