import { makeTestSeed } from '../helper';

test("Can make a seed - hideout shuffle", async () => {
  await makeTestSeed("HIDEOUT", {
    smallKeyShuffleHideout: 'anywhere',
  });
});
