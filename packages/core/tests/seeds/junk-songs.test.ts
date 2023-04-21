import { makeTestSeed } from "../helper";

test("Can make a seed - junk song locations", () => {
  makeTestSeed("JUNKSONGS", {
    startingItems: {
      OOT_SONG_TIME: 1,
      MM_SONG_TIME: 1,
    },
    junkLocations: [
      'MM Laboratory Zora Song',
      'OOT Hyrule Field Song of Time',
    ]
  });
});
