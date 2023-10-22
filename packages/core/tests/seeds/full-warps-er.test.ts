import { makeTestSeed } from '../helper';

test("Can make a seed - Full ER: Separated Warps", () => {
  makeTestSeed("FULL ER SEPARATED WARPS", {
    songs: 'anywhere',
    erDungeons: 'full',
    erBoss: 'full',
    erRegions: 'full',
    //erRegionsExtra: true,
	erRegionsShortcuts: true,
    erIndoors: 'full',
    erIndoorsExtra: true,
    erWarps: 'full',
    erOneWays: 'full',
    erOneWaysMajor: true,
    erOneWaysIkana: true,
    erOneWaysOwls: true,
    erMinorDungeons: true,
    erSpiderHouses: true,
    erPirateFortress: true,
    erBeneathWell: true,
    erIkanaCastle: true,
    erSecretShrine: true,
  });
});
