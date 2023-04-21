import { makeTestSeed } from '../helper';

test("Can make a seed - open", () => {
  makeTestSeed("OPEN", {
    startingItems: {
      OOT_SONG_TIME: 1,
      MM_SONG_TIME: 1,
      OOT_OCARINA: 1,
      MM_OCARINA: 1,
      OOT_SONG_TP_LIGHT: 1,
      MM_SONG_SOARING: 1,
    },
    doorOfTime: 'open',
    shuffleMasterSword: false,
    shuffleGerudoCard: false,
  });
});
