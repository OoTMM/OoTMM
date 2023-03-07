import { makeTestSeed } from "../helper";

test("Can make a seed - MQ", () => {
  makeTestSeed("MQ", {
    mq: {
      DT: true,
      DC: true,
      JJ: true,
      Forest: true,
      Fire: true,
      Water: true,
      Spirit: true,
      Shadow: true,
      BotW: true,
      IC: true,
      GTG: true,
      Ganon: true,
    }
  });
});
