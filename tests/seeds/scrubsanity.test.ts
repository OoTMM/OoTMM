import { makeTestSeed } from "../helper";

test("Can make a seed - scrubsanity", () => {
  makeTestSeed("SCRUBS", {
    scrubShuffleOot: true,
  });
});
