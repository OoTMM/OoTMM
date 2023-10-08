import { makeTestSeed } from '../helper';

test("Can make a seed - Dungeon ER", () => {
  makeTestSeed("DUNGEON ER", {
    songs: 'anywhere',
    erDungeons: 'full',
    erMajorDungeons: true,
  });
});
