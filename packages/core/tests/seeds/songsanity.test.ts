import { makeTestSeed } from '../helper';

test("Can make a seed - songsanity", async () => {
  await makeTestSeed("SONGSANITY", { songs: "anywhere" });
});
