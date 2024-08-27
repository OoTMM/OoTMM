import { Random, randString, randomInt } from "../random";
import { Settings } from "./type";
import { makeSettings, validateSettings } from "./util";
import { Game } from '../config';

function hasGame(x: any, g: Game) {
  return x.games === g || x.games === 'ootmm';
}

const hasOoTMM = (x: any) => x.games === 'ootmm';
const hasOoT = (x: any) => hasGame(x, 'oot');
const hasMM = (x: any) => hasGame(x, 'mm');

function sampleWeighted<T extends string>(rng: Random, args: {[k in T]: number}) {
  /* Computet the sum */
  let sum = 0;
  for (const key in args) {
    sum += args[key];
  }

  /* Pick a random number */
  const n = randomInt(rng, sum);
  let acc = 0;
  for (const key in args) {
    acc += args[key];
    if (n < acc) {
      return key as T;
    }
  }
  throw new Error('Unreachable');
}

function booleanWeighted(rng: Random, chance: number): boolean {
  const mul = 65536;
  return randomInt(rng, mul) < (chance * mul);
}

export function applyIndividualRandomSettings(oldSettings: Settings): Settings {
  /* Get a new RNG */
  const random = new Random();
  random.seed(randString());
  
  const base = makeSettings({});

  if (oldSettings.individualRandom.randomSettings.includes('games')) {
    base.games = sampleWeighted(random, { oot: 2, mm: 2, ootmm: 10 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('goal')) {
    if (hasOoTMM(base)) {
      base.goal = sampleWeighted(random, { any: 3, ganon: 3, majora: 3, both: 25, triforce: 3, triforce3: 3 });
    }
    else if (hasOoT(base)) {
      base.goal = sampleWeighted(random, { any: 3, ganon: 10, triforce: 3, triforce3: 3 });
    } else {
      base.goal = sampleWeighted(random, { any: 3, majora: 10, triforce: 3, triforce3: 3 });
    }
    
    if (base.goal === 'triforce') {
      base.triforcePieces = randomInt(random, 48) + 2;
      base.triforceGoal = randomInt(random, base.triforcePieces - 1) + 1;
    }
  }

  if (oldSettings.individualRandom.randomSettings.includes('logic')) {
    base.logic = sampleWeighted(random, { allLocations: 10, beatable: 5 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('itemPool')) {
    base.itemPool = sampleWeighted(random, { normal: 10, plentiful: 2, scarce: 2, minimal: 1 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('songs')) {
    base.songs = sampleWeighted(random, { songLocations: 6, anywhere: 4 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('goldSkulltulaTokens') && hasOoT(base)) {
    base.goldSkulltulaTokens = sampleWeighted(random, { none: 5, dungeons: 3, overworld: 3, all: 2 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('housesSkulltulaTokens') && hasMM(base)) {
    base.housesSkulltulaTokens = sampleWeighted(random, { none: 5, all: 3 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('tingleShuffle') && hasMM(base)) {
    base.tingleShuffle = sampleWeighted(random, { vanilla: 10, anywhere: 7, starting: 7, removed: 5 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('mapCompassShuffle')) {
    base.mapCompassShuffle = sampleWeighted(random, { ownDungeon: 10, anywhere: 7, starting: 7, removed: 5 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('smallKeyShuffleOot') && hasOoT(base)) {
    base.smallKeyShuffleOot = sampleWeighted(random, { ownDungeon: 10, anywhere: 8, removed: 4 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('smallKeyShuffleMm') && hasMM(base)) {
    base.smallKeyShuffleMm = sampleWeighted(random, { ownDungeon: 10, anywhere: 8, removed: 4 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('smallKeyShuffleHideout') && hasOoT(base)) {
    base.smallKeyShuffleHideout = sampleWeighted(random, { ownDungeon: 10, anywhere: 7 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('smallKeyShuffleChestGame') && hasOoT(base)) {
    base.smallKeyShuffleChestGame = sampleWeighted(random, { vanilla: 10, ownDungeon: 7, anywhere: 5 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('bossKeyShuffleOot') && hasOoT(base)) {
    base.bossKeyShuffleOot = sampleWeighted(random, { ownDungeon: 10, anywhere: 8, removed: 4 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('bossKeyShuffleMm') && hasMM(base)) {
    base.bossKeyShuffleMm = sampleWeighted(random, { ownDungeon: 10, anywhere: 8, removed: 4 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('silverRupeeShuffle') && hasOoT(base)) {
    base.silverRupeeShuffle = sampleWeighted(random, { ownDungeon: 10, anywhere: 7, vanilla: 3 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('townFairyShuffle') && hasMM(base)) {
    base.townFairyShuffle = sampleWeighted(random, { anywhere: 5, vanilla: 5 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('strayFairyChestShuffle') && hasMM(base)) {
    base.strayFairyChestShuffle = sampleWeighted(random, { anywhere: 7, vanilla: 3, ownDungeon: 5, starting: 6 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('strayFairyOtherShuffle') && hasMM(base)) {
    base.strayFairyOtherShuffle = sampleWeighted(random, { anywhere: 7, vanilla: 3, ownDungeon: 5, starting: 3, removed: 3 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('ganonBossKey') && base.goal !== 'triforce' && base.goal !== 'triforce3' && hasOoT(base)) {
    base.ganonBossKey = sampleWeighted(random, { removed: 10, vanilla: 5, anywhere: 7, ganon: 5 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('dungeonRewardShuffle')) {
    base.dungeonRewardShuffle = sampleWeighted(random, { dungeonBlueWarps: 6, dungeons: 2, dungeonsLimited: 3, anywhere: 2 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('scrubShuffleOot') && hasOoT(base)) {
    base.scrubShuffleOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('scrubShuffleMm') && hasMM(base)) {
    base.scrubShuffleMm = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('cowShuffleOot') && hasOoT(base)) {
    base.cowShuffleOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('cowShuffleMm') && hasMM(base)) {
    base.cowShuffleMm = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shopShuffleOot') && hasOoT(base)) {
    base.shopShuffleOot = sampleWeighted(random, { none: 10, full: 7 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('shopShuffleMm') && hasMM(base)) {
    base.shopShuffleMm = sampleWeighted(random, { none: 10, full: 7 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('owlShuffle') && hasMM(base)) {
    base.owlShuffle = sampleWeighted(random, { none: 10, anywhere: 7 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('shufflePotsOot') && hasOoT(base)) {
    base.shufflePotsOot = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shufflePotsMm') && hasMM(base)) {
    base.shufflePotsMm = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleCratesOot') && hasOoT(base)) {
    base.shuffleCratesOot = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleCratesMm') && hasMM(base)) {
    base.shuffleCratesMm = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleBarrelsMm') && hasMM(base)) {
    base.shuffleBarrelsMm = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleHivesOot') && hasOoT(base)) {
    base.shuffleHivesOot = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleHivesMm') && hasMM(base)) {
    base.shuffleHivesMm = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleGrassOot') && hasOoT(base)) {
    base.shuffleGrassOot = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleGrassMm') && hasMM(base)) {
    base.shuffleGrassMm = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleFreeRupeesOot') && hasOoT(base)) {
    base.shuffleFreeRupeesOot = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleFreeRupeesMm') && hasMM(base)) {
    base.shuffleFreeRupeesMm = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleFreeHeartsOot') && hasOoT(base)) {
    base.shuffleFreeHeartsOot = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleFreeHeartsMm') && hasMM(base)) {
    base.shuffleFreeHeartsMm = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleWonderItemsOot') && hasOoT(base)) {
    base.shuffleWonderItemsOot = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleWonderItemsMm') && hasMM(base)) {
    base.shuffleWonderItemsMm = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleSnowballsMm') && hasMM(base)) {
    base.shuffleSnowballsMm = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleOcarinasOot') && hasOoT(base)) {
    base.shuffleOcarinasOot = booleanWeighted(random, 0.7);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleMasterSword') && hasOoT(base)) {
    base.shuffleMasterSword = booleanWeighted(random, 0.4);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleGerudoCard') && hasOoT(base)) {
    base.shuffleGerudoCard = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleMaskTrades') && hasOoT(base)) {
    base.shuffleMaskTrades = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shuffleMerchantsMm') && hasMM(base)) {
    base.shuffleMerchantsMm = booleanWeighted(random, 0.50);
  }

  if (oldSettings.individualRandom.randomSettings.includes('pondFishShuffle') && hasOoT(base)) {
    base.pondFishShuffle = booleanWeighted(random, 0.50);
  }

  if (oldSettings.individualRandom.randomSettings.includes('divingGameRupeeShuffle') && hasOoT(base)) {
    base.divingGameRupeeShuffle = booleanWeighted(random, 0.50);
  }

  if (oldSettings.individualRandom.randomSettings.includes('fairyFountainFairyShuffleOot') && hasOoT(base)) {
    base.fairyFountainFairyShuffleOot = booleanWeighted(random, 0.50);
  }

  if (oldSettings.individualRandom.randomSettings.includes('fairyFountainFairyShuffleMm') && hasMM(base)) {
    base.fairyFountainFairyShuffleMm = booleanWeighted(random, 0.50);
  }

  if (oldSettings.individualRandom.randomSettings.includes('fairySpotShuffleOot') && hasOoT(base)) {
    base.fairySpotShuffleOot = booleanWeighted(random, 0.50);
  }

  if (oldSettings.individualRandom.randomSettings.includes('eggShuffle') && hasOoT(base)) {
    base.eggShuffle = booleanWeighted(random, 0.50);
  }

  if (oldSettings.individualRandom.randomSettings.includes('priceOotShops') && hasOoT(base)) {
    base.priceOotShops = sampleWeighted(random, { vanilla: 10, affordable: 10, weighted: 10 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('priceOotScrubs') && hasOoT(base)) {
    base.priceOotScrubs = sampleWeighted(random, { vanilla: 10, affordable: 10, weighted: 10 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('priceMmShops') && hasMM(base)) {
    base.priceMmShops = sampleWeighted(random, { vanilla: 10, affordable: 10, weighted: 10 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('priceMmTingle') && hasMM(base)) {
    base.priceMmTingle = sampleWeighted(random, { vanilla: 10, affordable: 10, weighted: 10 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('moonCrash') && hasMM(base)) {
    base.moonCrash = sampleWeighted(random, { reset: 10, cycle: 10 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('swordlessAdult') && hasOoT(base)) {
    base.swordlessAdult = booleanWeighted(random, 0.50);
  }

  if (oldSettings.individualRandom.randomSettings.includes('timeTravelSword') && base.swordlessAdult && hasOoT(base)) {
    base.timeTravelSword = booleanWeighted(random, 0.50);
  }

  if (oldSettings.individualRandom.randomSettings.includes('doorOfTime') && hasOoT(base)) {
    base.doorOfTime = sampleWeighted(random, { closed: 10, open: 7 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('ageChange') && hasOoT(base)) {
    base.ageChange = sampleWeighted(random, { oot: 10, none: 10, always: 10 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('dekuTree') && hasOoT(base)) {
    base.dekuTree = sampleWeighted(random, { open: 10, closed: 7 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('clearStateDungeonsMm') && hasMM(base)) {
    base.clearStateDungeonsMm = sampleWeighted(random, { none: 5, WF: 1, GB: 1, both: 2 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('beneathWell') && hasMM(base)) {
    base.beneathWell = sampleWeighted(random, { vanilla: 5, open: 3, remorseless: 1 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('kakarikoGate') && hasOoT(base)) {
    base.kakarikoGate = sampleWeighted(random, { closed: 10, open: 7 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('openZdShortcut') && hasOoT(base)) {
    base.openZdShortcut = booleanWeighted(random, 0.3);
  }

  if (oldSettings.individualRandom.randomSettings.includes('zoraKing') && hasOoT(base)) {
    base.zoraKing = sampleWeighted(random, { vanilla: 10, open: 5, adult: 5 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('gerudoFortress') && hasOoT(base)) {
    base.gerudoFortress = sampleWeighted(random, { vanilla: 1, single: 1, open: 1 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('skipZelda') && hasOoT(base)) {
    base.skipZelda = booleanWeighted(random, 0.3);
  }

  if (oldSettings.individualRandom.randomSettings.includes('openMoon') && hasMM(base)) {
    base.openMoon = booleanWeighted(random, 0.3);
  }

  if (oldSettings.individualRandom.randomSettings.includes('rainbowBridge') && hasOoT(base)) {
    base.rainbowBridge = sampleWeighted(random, { vanilla: 1, open: 1, medallions: 1 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('bossWarpPads') && hasMM(base)) {
    base.bossWarpPads = sampleWeighted(random, { bossBeaten: 10, remains: 4 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('freeScarecrowOot') && hasOoT(base)) {
    base.freeScarecrowOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('freeScarecrowMm') && hasMM(base)) {
    base.freeScarecrowMm = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('openMaskShop') && hasOoTMM(base)) {
    base.openMaskShop = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('ootPreplantedBeans') && hasOoT(base)) {
    base.ootPreplantedBeans = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('crossAge') && hasOoTMM(base)) {
    base.crossAge = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('crossWarpOot') && hasOoTMM(base)) {
    base.crossWarpOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('crossWarpMm') && hasOoTMM(base)) {
    base.crossWarpMm = sampleWeighted(random, { none: 10, full: 7, childOnly: 3 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('spellFireMm') && hasMM(base)) {
    base.spellFireMm = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('spellWindMm') && hasMM(base)) {
    base.spellWindMm = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('spellLoveMm') && hasMM(base)) {
    base.spellLoveMm = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('crossGameFw') && base.spellWindMm && hasOoTMM(base)) {
    base.crossGameFw = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('csmc')) {
    base.csmc = sampleWeighted(random, { always: 18, agony: 1, never: 1 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('csmcHearts') && base.csmc !== 'never') {
    base.csmcHearts = booleanWeighted(random, 0.75);
  }

  if (oldSettings.individualRandom.randomSettings.includes('csmcMapCompass') && base.csmc !== 'never') {
    base.csmcMapCompass = booleanWeighted(random, 0.75);
  }

  if (oldSettings.individualRandom.randomSettings.includes('csmcSkulltula') && base.csmc !== 'never') {
    base.csmcSkulltula = booleanWeighted(random, 0.75);
  }

  if (oldSettings.individualRandom.randomSettings.includes('csmcCow') && base.csmc !== 'never') {
    base.csmcCow = booleanWeighted(random, 0.75);
  }

  if (oldSettings.individualRandom.randomSettings.includes('blastMaskOot') && hasOoT(base)) {
    base.blastMaskOot = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('blastMaskCooldown') && (base.blastMaskOot || hasMM(base))) {
    base.blastMaskCooldown = sampleWeighted(random, { default: 10, instant: 2, veryshort: 3, short: 3, long: 2, verylong: 2 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('clockSpeed') && hasMM(base)) {
    base.clockSpeed = sampleWeighted(random, { default: 10, veryslow: 3, slow: 3, fast: 2, veryfast: 1, superfast: 1 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('autoInvert') && hasMM(base)) {
    base.autoInvert = sampleWeighted(random, { never: 10, always: 5 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('keepItemsReset') && hasMM(base)) {
    base.keepItemsReset = booleanWeighted(random, 0.75);
  }

  if (oldSettings.individualRandom.randomSettings.includes('fastMasks') && hasMM(base)) {
    base.fastMasks = booleanWeighted(random, 0.75);
  }

  if (oldSettings.individualRandom.randomSettings.includes('fierceDeityAnywhere') && hasMM(base)) {
    base.fierceDeityAnywhere = booleanWeighted(random, 0.75);
  }

  if (oldSettings.individualRandom.randomSettings.includes('hookshotAnywhereOot') && hasOoT(base)) {
    base.hookshotAnywhereOot = sampleWeighted(random, { off: 10, enabled: 3, logical: 2 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('hookshotAnywhereMm') && hasMM(base)) {
    base.hookshotAnywhereMm = sampleWeighted(random, { off: 10, enabled: 3, logical: 2 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('climbMostSurfacesOot') && hasOoT(base)) {
    base.climbMostSurfacesOot = sampleWeighted(random, { off: 10, enabled: 3, logical: 2 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('climbMostSurfacesMm') && hasMM(base)) {
    base.climbMostSurfacesMm = booleanWeighted(random, 0.3);
  }

  if (oldSettings.individualRandom.randomSettings.includes('fastBunnyHood') && hasOoT(base)) {
    base.fastBunnyHood = booleanWeighted(random, 0.75);
  }

  if (oldSettings.individualRandom.randomSettings.includes('critWiggleDisable')) {
    base.critWiggleDisable = booleanWeighted(random, 0.75);
  }

  if (oldSettings.individualRandom.randomSettings.includes('restoreBrokenActors') && hasOoT(base)) {
    base.restoreBrokenActors = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('alterLostWoodsExits') && hasOoT(base)) {
    base.alterLostWoodsExits = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('voidWarpMm') && hasMM(base)) {
    base.voidWarpMm = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shadowFastBoat') && hasOoT(base)) {
    base.shadowFastBoat = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('fillWallets')) {
    base.fillWallets = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('progressiveShieldsOot') && hasOoT(base)) {
    base.progressiveShieldsOot = sampleWeighted(random, { separate: 10, progressive: 10 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('progressiveSwordsOot') && hasOoT(base)) {
    base.progressiveSwordsOot = sampleWeighted(random, { separate: 10, progressive: 10, goron: 5 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('progressiveShieldsMm') && hasMM(base)) {
    base.progressiveShieldsMm = sampleWeighted(random, { separate: 10, progressive: 10 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('progressiveGFS') && hasMM(base)) {
    base.progressiveGFS = sampleWeighted(random, { separate: 10, progressive: 10 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('progressiveGoronLullaby') && hasMM(base)) {
    base.progressiveGoronLullaby = sampleWeighted(random, { progressive: 10, single: 10 });
  }  

  if (oldSettings.individualRandom.randomSettings.includes('clocks') && hasMM(base)) {
    base.clocks = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('progressiveClocks') && base.clocks) {
    base.progressiveClocks = sampleWeighted(random, { separate: 10, ascending: 10, descending: 10 });
  }

  if (oldSettings.individualRandom.randomSettings.includes('bottleContentShuffle')) {
    base.bottleContentShuffle = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sunSongMm') && hasMM(base)) {
    base.sunSongMm = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('fairyOcarinaMm') && hasMM(base)) {
    base.fairyOcarinaMm = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('blueFireArrows') && hasOoT(base)) {
    base.blueFireArrows = booleanWeighted(random, 0.75);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sunlightArrows') && hasOoT(base)) {
    base.sunlightArrows = booleanWeighted(random, 0.75);
  }

  if (oldSettings.individualRandom.randomSettings.includes('shortHookshotMm') && hasMM(base)) {
    base.shortHookshotMm = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('childWallets')) {
    base.childWallets = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('colossalWallets')) {
    base.colossalWallets = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('bottomlessWallets') && base.colossalWallets) {
    base.bottomlessWallets = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('rupeeScaling') && base.colossalWallets) {
    base.rupeeScaling = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('skeletonKeyOot') && hasOoT(base)) {
    base.skeletonKeyOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('skeletonKeyMm') && hasMM(base)) {
    base.skeletonKeyMm = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('magicalRupee') && base.silverRupeeShuffle !== 'vanilla') {
    base.magicalRupee = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('bombchuBagOot') && hasOoT(base)) {
    base.bombchuBagOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('bombchuBagMm') && hasMM(base)) {
    base.bombchuBagMm = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('bootsIronMm') && hasMM(base)) {
    base.bootsIronMm = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('bootsHoverMm') && hasMM(base)) {
    base.bootsHoverMm = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('tunicGoronMm') && hasMM(base)) {
    base.tunicGoronMm = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('tunicZoraMm') && hasMM(base)) {
    base.tunicZoraMm = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('scalesMm') && hasMM(base)) {
    base.scalesMm = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('strengthMm') && hasMM(base)) {
    base.strengthMm = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('extraChildSwordsOot') && base.progressiveSwordsOot !== 'progressive' && hasOoT(base)) {
    base.extraChildSwordsOot = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('stoneMaskOot') && hasOoT(base)) {
    base.stoneMaskOot = booleanWeighted(random, 0.25);
  }

  if (oldSettings.individualRandom.randomSettings.includes('elegyOot') && hasOoT(base)) {
    base.elegyOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('ocarinaButtonsShuffleOot') && hasOoT(base)) {
    base.ocarinaButtonsShuffleOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('ocarinaButtonsShuffleMm') && hasMM(base)) {
    base.ocarinaButtonsShuffleMm = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('soulsEnemyOot') && hasOoT(base)) {
    base.soulsEnemyOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('soulsEnemyMm') && hasMM(base)) {
    base.soulsEnemyMm = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('soulsBossOot') && hasOoT(base)) {
    base.soulsBossOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('soulsBossMm') && hasMM(base)) {
    base.soulsBossMm = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('soulsNpcOot') && hasOoT(base)) {
    base.soulsNpcOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('soulsNpcMm') && hasMM(base)) {
    base.soulsNpcMm = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('soulsMiscOot') && hasOoT(base)) {
    base.soulsMiscOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('soulsMiscMm') && hasMM(base)) {
    base.soulsMiscMm = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('lenientSpikes') && hasMM(base)) {
    base.lenientSpikes = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('menuNotebook') && hasMM(base)) {
    base.menuNotebook = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('trapRupoor')) {
    base.trapRupoor = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('songOfDoubleTimeOot') && hasOoT(base)) {
    base.songOfDoubleTimeOot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedNutsSticks') && hasOoTMM(base)) {
    base.sharedNutsSticks = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedBows') && hasOoTMM(base)) {
    base.sharedBows = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedBombBags') && hasOoTMM(base)) {
    base.sharedBombBags = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedMagic') && hasOoTMM(base)) {
    base.sharedMagic = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedMagicArrowFire') && hasOoTMM(base)) {
    base.sharedMagicArrowFire = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedMagicArrowIce') && hasOoTMM(base)) {
    base.sharedMagicArrowIce = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedMagicArrowLight') && hasOoTMM(base)) {
    base.sharedMagicArrowLight = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSongEpona') && hasOoTMM(base)) {
    base.sharedSongEpona = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSongStorms') && hasOoTMM(base)) {
    base.sharedSongStorms = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSongTime') && hasOoTMM(base)) {
    base.sharedSongTime = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSongSun') && base.sunSongMm && hasOoTMM(base)) {
    base.sharedSongSun = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedHookshot') && hasOoTMM(base)) {
    base.sharedHookshot = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedLens') && hasOoTMM(base)) {
    base.sharedLens = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedOcarina') && hasOoTMM(base)) {
    base.sharedOcarina = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedMaskGoron') && hasOoTMM(base)) {
    base.sharedMaskGoron = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedMaskZora') && hasOoTMM(base)) {
    base.sharedMaskZora = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedMaskBunny') && hasOoTMM(base)) {
    base.sharedMaskBunny = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedMaskKeaton') && hasOoTMM(base)) {
    base.sharedMaskKeaton = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedMaskTruth') && hasOoTMM(base)) {
    base.sharedMaskTruth = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedMaskBlast') && base.blastMaskOot && hasOoTMM(base)) {
    base.sharedMaskBlast = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedMaskStone') && base.stoneMaskOot && hasOoTMM(base)) {
    base.sharedMaskStone = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSongElegy') && base.elegyOot && hasOoTMM(base)) {
    base.sharedSongElegy = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedWallets') && hasOoTMM(base)) {
    base.sharedWallets = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedHealth') && hasOoTMM(base)) {
    base.sharedHealth = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSwords') && base.extraChildSwordsOot && base.progressiveGFS !== 'progressive' && hasOoTMM(base)) {
    base.sharedSwords = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedShields') && base.progressiveShieldsOot === base.progressiveShieldsMm && hasOoTMM(base)) {
    base.sharedShields = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSoulsEnemy') && base.soulsEnemyOot && base.soulsEnemyMm && hasOoTMM(base)) {
    base.sharedSoulsEnemy = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSoulsNpc') && base.soulsNpcOot && base.soulsNpcMm && hasOoTMM(base)) {
    base.sharedSoulsNpc = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSoulsMisc') && base.soulsMiscOot && base.soulsMiscMm && hasOoTMM(base)) {
    base.sharedSoulsMisc = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedOcarinaButtons') && base.ocarinaButtonsShuffleOot && base.ocarinaButtonsShuffleMm && hasOoTMM(base)) {
    base.sharedOcarinaButtons = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSkeletonKey') && base.skeletonKeyOot && base.skeletonKeyMm && hasOoTMM(base)) {
    base.sharedSkeletonKey = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedBombchuBags') && base.bombchuBagOot && base.bombchuBagMm && hasOoTMM(base)) {
    base.sharedBombchuBags = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSpellFire') && base.spellFireMm && hasOoTMM(base)) {
    base.sharedSpellFire = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSpellWind') && base.spellWindMm && hasOoTMM(base)) {
    base.sharedSpellWind = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedSpellLove') && base.spellLoveMm && hasOoTMM(base)) {
    base.sharedSpellLove = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedBootsIron') && base.bootsIronMm && hasOoTMM(base)) {
    base.sharedBootsIron = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedBootsHover') && base.bootsHoverMm && hasOoTMM(base)) {
    base.sharedBootsHover = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedTunicGoron') && base.tunicGoronMm && hasOoTMM(base)) {
    base.sharedTunicGoron = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedTunicZora') && base.tunicZoraMm && hasOoTMM(base)) {
    base.sharedTunicZora = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedScales') && base.scalesMm && hasOoTMM(base)) {
    base.sharedScales = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('sharedStrength') && base.strengthMm && hasOoTMM(base)) {
    base.sharedStrength = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('agelessSwords') && hasOoT(base)) {
    base.agelessSwords = booleanWeighted(random, 0.1);
  }

  if (oldSettings.individualRandom.randomSettings.includes('agelessShields') && hasOoT(base)) {
    base.agelessShields = booleanWeighted(random, 0.1);
  }

  if (oldSettings.individualRandom.randomSettings.includes('agelessTunics') && hasOoT(base)) {
    base.agelessTunics = booleanWeighted(random, 0.1);
  }

  if (oldSettings.individualRandom.randomSettings.includes('agelessBoots') && hasOoT(base)) {
    base.agelessBoots = booleanWeighted(random, 0.1);
  }

  if (oldSettings.individualRandom.randomSettings.includes('agelessSticks') && hasOoT(base)) {
    base.agelessSticks = booleanWeighted(random, 0.1);
  }

  if (oldSettings.individualRandom.randomSettings.includes('agelessBoomerang') && hasOoT(base)) {
    base.agelessBoomerang = booleanWeighted(random, 0.1);
  }

  if (oldSettings.individualRandom.randomSettings.includes('agelessHammer') && hasOoT(base)) {
    base.agelessHammer = booleanWeighted(random, 0.1);
  }

  if (oldSettings.individualRandom.randomSettings.includes('agelessHookshot') && hasOoT(base)) {
    base.agelessHookshot = booleanWeighted(random, 0.1);
  }

  if (oldSettings.individualRandom.randomSettings.includes('agelessSlingshot') && hasOoT(base)) {
    base.agelessSlingshot = booleanWeighted(random, 0.1);
  }

  if (oldSettings.individualRandom.randomSettings.includes('agelessBow') && hasOoT(base)) {
    base.agelessBow = booleanWeighted(random, 0.1);
  }

  if (oldSettings.individualRandom.randomSettings.includes('agelessChildTrade') && hasOoT(base)) {
    base.agelessChildTrade = booleanWeighted(random, 0.1);
  }

  if (oldSettings.individualRandom.randomSettings.includes('agelessStrength') && hasOoT(base)) {
    base.agelessStrength = booleanWeighted(random, 0.1);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erBoss')) {
    if (hasOoTMM(base)) {
      base.erBoss = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
    }
    else {
      base.erBoss = sampleWeighted(random, { none: 10, full: 10 });
    }
  }

  if (oldSettings.individualRandom.randomSettings.includes('erDungeons')) {
    if (hasOoTMM(base)) {
      base.erDungeons = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
    }
    else {
      base.erDungeons = sampleWeighted(random, { none: 10, full: 10 });
    }
  }

  if (oldSettings.individualRandom.randomSettings.includes('erOverworld')) {
    if (hasOoTMM(base)) {
      base.erOverworld = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
    }
    else {
      base.erOverworld = sampleWeighted(random, { none: 10, full: 10 });
    }
  }

  if (oldSettings.individualRandom.randomSettings.includes('erIndoors')) {
    if (hasOoTMM(base)) {
      base.erIndoors = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
    }
    else {
      base.erIndoors = sampleWeighted(random, { none: 10, full: 10 });
    }
  }

  if (oldSettings.individualRandom.randomSettings.includes('erRegions') && base.erOverworld === 'none') {
    if (hasOoTMM(base)) {
      base.erRegions = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
    }
    else {
      base.erRegions = sampleWeighted(random, { none: 10, full: 10 });
    }
  }

  if (oldSettings.individualRandom.randomSettings.includes('erGrottos')) {
    if (hasOoTMM(base)) {
      base.erGrottos = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
    }
    else {
      base.erGrottos = sampleWeighted(random, { none: 10, full: 10 });
    }
  }

  if (oldSettings.individualRandom.randomSettings.includes('erDecoupled')) {
    base.erDecoupled = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erSelfLoops') && !base.erDecoupled) {
    base.erSelfLoops = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erMixed')) {
    if (hasOoTMM(base)) {
      base.erMixed = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
    }
    else {
      base.erMixed = sampleWeighted(random, { none: 10, full: 10 });
    }
  }

  if (oldSettings.individualRandom.randomSettings.includes('erMixedDungeons') && base.erMixed !== 'none' && base.erMixed === base.erDungeons) {
    base.erMixedDungeons = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erMixedRegions') && base.erMixed !== 'none' && base.erMixed === base.erRegions) {
    base.erMixedRegions = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erMixedOverworld') && base.erMixed !== 'none' && base.erMixed === base.erOverworld) {
    base.erMixedOverworld = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erMixedIndoors') && base.erMixed !== 'none' && base.erMixed === base.erIndoors) {
    base.erMixedIndoors = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erMixedGrottos') && base.erMixed !== 'none' && base.erMixed === base.erGrottos) {
    base.erMixedGrottos = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erWallmasters')) {
    if (hasOoTMM(base)) {
      base.erWallmasters = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
    }
    else {
      base.erWallmasters = sampleWeighted(random, { none: 10, full: 10 });
    }
  }

  if (oldSettings.individualRandom.randomSettings.includes('erSpawns') && hasOoT(base)) {
    base.erSpawns = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erMajorDungeons') && base.erDungeons !== 'none') {
    base.erMajorDungeons = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erMinorDungeons') && base.erDungeons !== 'none' && hasOoT(base)) {
    base.erMinorDungeons = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erGanonCastle') && base.erDungeons !== 'none' && hasOoT(base)) {
    base.erGanonCastle = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erGanonTower') && base.erDungeons !== 'none' && hasOoT(base)) {
    base.erGanonTower = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erMoon') && base.erDungeons !== 'none' && hasMM(base)) {
    base.erMoon = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erSpiderHouses') && base.erDungeons !== 'none' && hasMM(base)) {
    base.erSpiderHouses = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erPirateFortress') && base.erDungeons !== 'none' && hasMM(base)) {
    base.erPirateFortress = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erBeneathWell') && base.erDungeons !== 'none' && hasMM(base)) {
    base.erBeneathWell = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erIkanaCastle') && base.erDungeons !== 'none' && hasMM(base)) {
    base.erIkanaCastle = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erSecretShrine') && base.erDungeons !== 'none' && hasMM(base)) {
    base.erSecretShrine = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erRegionsExtra') && base.erRegions !== 'none' && hasOoT(base)) {
    base.erRegionsExtra = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erRegionsShortcuts') && base.erRegions !== 'none' && hasOoT(base)) {
    base.erRegionsShortcuts = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erIndoorsMajor') && base.erIndoors !== 'none') {
    base.erIndoorsMajor = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erIndoorsExtra') && base.erIndoors !== 'none') {
    base.erIndoorsExtra = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erOneWays')) {
    if (hasOoTMM(base)) {
      base.erOneWays = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
    }
    else {
      base.erOneWays = sampleWeighted(random, { none: 10, full: 10 });
    }
  }

  if (oldSettings.individualRandom.randomSettings.includes('erOneWaysMajor') && base.erOneWays !== 'none') {
    base.erOneWaysMajor = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erOneWaysIkana') && base.erOneWays !== 'none' && hasMM(base)) {
    base.erOneWaysIkana = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erOneWaysOwls') && base.erOneWays !== 'none' && hasOoT(base)) {
    base.erOneWaysOwls = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erOneWaysWoods') && base.erOneWays !== 'none' && base.alterLostWoodsExits && hasOoT(base)) {
    base.erOneWaysWoods = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erOneWaysWaterVoids') && base.erOneWays !== 'none' && hasMM(base)) {
    base.erOneWaysWaterVoids = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erOneWaysAnywhere') && base.erOneWays !== 'none') {
    base.erOneWaysAnywhere = booleanWeighted(random, 0.5);
  }

  /* TODO: not sure how to resolve cyclical dependency here, so leaving this out for now

  if (oldSettings.individualRandom.randomSettings.includes('erOneWaysSongs') && base.erOneWays !== 'none' && base.erWarps !== 'ootOnly' && base.erWarps !== 'full' && base.erWarps !== 'ownGame' && hasOoT(base)) {
    base.erOneWaysSongs = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erOneWaysStatues') && base.erOneWays !== 'none' && base.erWarps !== 'mmOnly' && base.erWarps !== 'full' && base.erWarps !== 'ownGame' && hasMM(base)) {
    base.erOneWaysStatues = booleanWeighted(random, 0.5);
  }

  if (oldSettings.individualRandom.randomSettings.includes('erWarps') && !base.erOneWaysSongs || !base.erOneWaysStatues) {
    if (hasOoTMM(base)) {
      base.erWarps = sampleWeighted(random, { none: 10, ootOnly: 10, mmOnly: 10, ownGame: 10, full: 10 });
    }
    else if (hasOoT(base)) {
      base.erWarps = sampleWeighted(random, { none: 10, ootOnly: 10, full: 10 });
    }
    else {
      base.erWarps = sampleWeighted(random, { none: 10, mmOnly: 10, full: 10 });
    }
  }*/

  return validateSettings(base);
}
