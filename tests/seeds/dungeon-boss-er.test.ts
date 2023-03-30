import { makeTestSeed } from '../helper';

test("Can make a seed - Dungeon + Boss ER", () => {
  makeTestSeed("DUNGEON ER", {
    songs: 'anywhere',
    erDungeons: 'full',
    erBoss: 'full',
    erMinorDungeons: true,
    erSpiderHouses: true,
	  erBeneathWell: true,
	  erIkanaCastle: true,
	  erSecretShrine: true,
  });
});
