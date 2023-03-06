import { makeTestSeed } from '../helper';

test("Can make a seed - Cow Shuffle", () => {
  makeTestSeed("COWS", {
    cowShuffle: 'full',
  });
});
