import { makeTestSeed } from '../helper';

test("Can make a seed - Beatable Only", async () => {
  await await makeTestSeed("BEATABLE ONLY", {
    logic: 'beatable'
  });
});
