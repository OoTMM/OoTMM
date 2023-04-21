import { makeTestSeed } from '../helper';

test("Can make a seed - hideout shuffle", () => {
  makeTestSeed("HIDEOUT", {
    smallKeyShuffleHideout: 'anywhere',
  });
});
