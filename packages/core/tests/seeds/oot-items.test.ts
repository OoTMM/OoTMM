import { makeTestSeed } from '../helper';

test("Can make a seed - OoT Items", () => {
  makeTestSeed("OOT ITEMS", {
    shortHookshotMm: true,
    fairyOcarinaMm: true,
  });
});
