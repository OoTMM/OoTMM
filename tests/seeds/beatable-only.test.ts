import { makeTestSeed } from '../helper';

test("Can make a seed - Beatable Only", () => {
  makeTestSeed("BEATABLE ONLY", {
    logic: 'beatable'
  });
});
