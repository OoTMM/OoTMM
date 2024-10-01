import { makeTestSeed } from '../helper';

test("Can make a seed - Dungeon ER", async () => {
  await makeTestSeed("DUNGEON ER", {
    songs: 'anywhere',
    erDungeons: 'full',
    erMajorDungeons: true,
  });
});
