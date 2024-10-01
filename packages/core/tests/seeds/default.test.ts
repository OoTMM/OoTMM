import { makeTestSeed } from '../helper';

test("Can make a seed - default", async () => {
  await makeTestSeed("DEFAULT", {});
});
