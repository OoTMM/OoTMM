import { makeTestSeed } from '../helper';

test("Can make a seed - OoT Items", async () => {
  await makeTestSeed("OOT ITEMS", {
    shortHookshotMm: true,
    fairyOcarinaMm: true,
  });
});
