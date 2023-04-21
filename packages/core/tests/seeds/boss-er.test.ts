import { makeTestSeed } from '../helper';

test("Can make a seed - Boss ER", () => {
  makeTestSeed("BOSS ER", {
    songs: 'anywhere',
    erBoss: 'full',
  });
});
