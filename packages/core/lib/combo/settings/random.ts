import { Random, randString, randomInt } from "../random";
import { Settings } from "./type";
import { makeSettings } from "./util";

export type OptionRandomSettings = {
  enabled: boolean;
};

export const DEFAULT_RANDOM_SETTINGS: OptionRandomSettings = {
  enabled: false,
};

export function makeRandomSettings(arg: Partial<OptionRandomSettings>): OptionRandomSettings {
  return { ...DEFAULT_RANDOM_SETTINGS, ...arg };
}

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

export function applyRandomSettings(rnd: OptionRandomSettings, oldSettings: Settings): Settings {
  /* Get a new RNG */
  const random = new Random();
  random.seed(randString());

  /* Get base random settings */
  const base = makeSettings({});
  base.junkLocations = oldSettings.junkLocations;
  base.tricks = oldSettings.tricks;

  /* Main Settings */
  base.logic = sampleWeighted(random, { allLocations: 10, beatable: 5 });
  base.itemPool = sampleWeighted(random, { normal: 10, plentiful: 2, scarce: 2, minimal: 1 });
  base.shuffleMasterSword = booleanWeighted(random, 0.4);
  base.shuffleGerudoCard = booleanWeighted(random, 0.5);
  base.scrubShuffleOot = booleanWeighted(random, 0.33);
  base.scrubShuffleMm = booleanWeighted(random, 0.33);
  base.cowShuffleOot = booleanWeighted(random, 0.40);
  base.cowShuffleMm = booleanWeighted(random, 0.40);
  base.shuffleMerchantsMm = booleanWeighted(random, 0.50);
  base.eggShuffle = booleanWeighted(random, 0.50);
  base.songs = sampleWeighted(random, { songLocations: 6, anywhere: 4 });
  base.goldSkulltulaTokens = sampleWeighted(random, { none: 5, dungeons: 3, overworld: 3, all: 2 });
  base.housesSkulltulaTokens = sampleWeighted(random, { none: 5, all: 3 });
  base.tingleShuffle = sampleWeighted(random, { vanilla: 10, anywhere: 7, starting: 7, removed: 5 });
  base.mapCompassShuffle = sampleWeighted(random, { ownDungeon: 10, anywhere: 7, starting: 7, removed: 5 });
  base.smallKeyShuffle = sampleWeighted(random, { ownDungeon: 10, anywhere: 7 });
  base.smallKeyShuffleHideout = sampleWeighted(random, { ownDungeon: 10, anywhere: 7 });
  base.bossKeyShuffle = sampleWeighted(random, { ownDungeon: 10, anywhere: 7 });
  base.townFairyShuffle = sampleWeighted(random, { vanilla: 10, anywhere: 10 });
  base.strayFairyShuffle = sampleWeighted(random, { mixed: 10, anywhere: 7, vanilla: 3, ownDungeon: 5 });
  base.ganonBossKey = sampleWeighted(random, { removed: 10, vanilla: 5, anywhere: 7, ganon: 5 });
  base.dungeonRewardShuffle = sampleWeighted(random, { dungeonBlueWarps: 6, dungeons: 2, dungeonsLimited: 3, anywhere: 2 });
  base.shopShuffleOot = sampleWeighted(random, { none: 10, full: 7 });
  base.shopShuffleMm = sampleWeighted(random, { none: 10, full: 7 });
  base.owlShuffle = sampleWeighted(random, { none: 10, anywhere: 7 });
  base.dekuTreeAdult = booleanWeighted(random, 0.3);
  base.wellAdult = booleanWeighted(random, 0.5);
  base.fireChild = booleanWeighted(random, 0.3);
  base.skipZelda = booleanWeighted(random, 0.3);
  base.doorOfTime = sampleWeighted(random, { closed: 10, open: 7 });
  base.dekuTree = sampleWeighted(random, { open: 10, closed: 7 });
  base.kakarikoGate = sampleWeighted(random, { closed: 10, open: 7 });
  base.zoraKing = sampleWeighted(random, { vanilla: 10, open: 5, adult: 5 });
  base.bossWarpPads = sampleWeighted(random, { bossBeaten: 10, remains: 4 });
  base.crossWarpOot = booleanWeighted(random, 0.5);
  base.sunSongMm = booleanWeighted(random, 0.5);
  base.crossWarpMm = sampleWeighted(random, { none: 10, full: 7, childOnly: 3 });
  base.csmc = sampleWeighted(random, { always: 18, agony: 1, never: 1 });
  base.agelessBoomerang = booleanWeighted(random, 0.05);
  base.agelessSticks = booleanWeighted(random, 0.05);
  base.agelessHammer = booleanWeighted(random, 0.05);
  base.agelessTunics = booleanWeighted(random, 0.03);
  base.agelessSwords = booleanWeighted(random, 0.02);
  base.agelessShields = booleanWeighted(random, 0.02);
  base.agelessBoots = booleanWeighted(random, 0.02);
  base.blastMaskCooldown = sampleWeighted(random, { default: 10, instant: 2, veryshort: 3, short: 3, long: 2, verylong: 2 });

  /* Items */
  base.fillWallets = booleanWeighted(random, 0.5);
  base.progressiveShieldsOot = sampleWeighted(random, { separate: 10, progressive: 10 });
  base.progressiveSwordsOot = sampleWeighted(random, { separate: 10, progressive: 10, goron: 5 });
  base.progressiveShieldsMm = sampleWeighted(random, { separate: 10, progressive: 10 });
  base.progressiveGoronLullaby = sampleWeighted(random, { progressive: 10, single: 10 });
  base.sharedNutsSticks = booleanWeighted(random, 0.1);
  base.sharedBows = booleanWeighted(random, 0.1);
  base.sharedBombBags = booleanWeighted(random, 0.1);
  base.sharedMagic = booleanWeighted(random, 0.1);
  base.sharedMagicArrowFire = booleanWeighted(random, 0.1);
  base.sharedMagicArrowIce = booleanWeighted(random, 0.1);
  base.sharedMagicArrowLight = booleanWeighted(random, 0.1);
  base.sharedSongEpona = booleanWeighted(random, 0.1);
  base.sharedSongStorms = booleanWeighted(random, 0.1);
  base.sharedSongTime = booleanWeighted(random, 0.1);
  base.sharedSongSun = booleanWeighted(random, 0.1);
  base.sharedHookshot = booleanWeighted(random, 0.1);
  base.sharedLens = booleanWeighted(random, 0.1);
  base.sharedOcarina = booleanWeighted(random, 0.1);
  base.sharedMasks = booleanWeighted(random, 0.1);
  base.sharedWallets = booleanWeighted(random, 0.1);
  base.sharedHealth = booleanWeighted(random, 0.1);
  base.fairyOcarinaMm = booleanWeighted(random, 0.5);
  base.shortHookshotMm = booleanWeighted(random, 0.25);
  base.childWallets = booleanWeighted(random, 0.25);
  base.colossalWallets = booleanWeighted(random, 0.5);

  return base;
}
