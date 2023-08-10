import { makeTestSeed } from '../helper';

test("Can make a seed - Regional ER", () => {
  makeTestSeed("REGION ER", {
    songs: 'anywhere',
    erOverworld: 'regionsFull',
    erRegionsExtra: true,
    erRegionsExtra: true,
  });
});
