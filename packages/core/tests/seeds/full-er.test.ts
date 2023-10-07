import { makeTestSeed } from '../helper';

test("Can make a seed - Full ER", () => {
  makeTestSeed("FULL ER", {
    songs: 'anywhere',
    erDungeons: 'full',
    erBoss: 'full',
    erRegions: 'full',
    //erRegionsExtra: true,
	erRegionsShortcuts: true,
    erIndoors: 'full',
    erIndoorsExtra: true,
    erWarps: 'full',
    erWarpsOwls: true,
    erMinorDungeons: true,
    erSpiderHouses: true,
    erPirateFortress: true,
    erBeneathWell: true,
    erIkanaCastle: true,
    erSecretShrine: true,
  });
});
