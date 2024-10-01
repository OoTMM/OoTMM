import { makeTestSeed } from "../helper";

test("Can make a seed - scrubsanity", async () => {
  await makeTestSeed("SCRUBS", {
    scrubShuffleOot: true,
  });
});
