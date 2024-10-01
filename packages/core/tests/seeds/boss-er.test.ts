import { makeTestSeed } from '../helper';

test("Can make a seed - Boss ER", async () => {
  await makeTestSeed("BOSS ER", {
    songs: 'anywhere',
    erBoss: 'full',
  });
});
