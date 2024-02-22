import { Random, randString, randomInt } from "../random";
import { Settings } from "./type";
import { makeSettings, validateSettings } from "./util";

export type OptionRandomSettings = {
  enabled: boolean;
  mq: boolean;
  er: boolean;
};

export const DEFAULT_RANDOM_SETTINGS: OptionRandomSettings = {
  enabled: false,
  mq: false,
  er: false,
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
  if (!rnd.enabled) {
    return oldSettings;
  }

  /* Get a new RNG */
  const random = new Random();
  random.seed(randString());

  /* Get base random settings */
  const base = makeSettings({});
  base.junkLocations = oldSettings.junkLocations;
  base.tricks = oldSettings.tricks;
  base.glitches = oldSettings.glitches;
  base.hints = oldSettings.hints;

  /* Main Settings */
  base.goal = sampleWeighted(random, { both: 10, triforce: 3, triforce3: 3 });
  if (base.goal === 'triforce') {
    base.triforcePieces = randomInt(random, 48) + 2;
    base.triforceGoal = randomInt(random, base.triforcePieces - 1) + 1;
  }
  base.logic = sampleWeighted(random, { allLocations: 10, beatable: 5 });
  base.itemPool = sampleWeighted(random, { normal: 10, plentiful: 2, scarce: 2, minimal: 1 });
  base.shuffleOcarinasOot = booleanWeighted(random, 0.7);
  base.shuffleMasterSword = booleanWeighted(random, 0.4);
  base.shuffleGerudoCard = booleanWeighted(random, 0.5);

  /* Scrub shuffle - 25% disabled, 25% enabled, 50% individual */
  switch (randomInt(random, 4)) {
  case 0:
    base.scrubShuffleOot = false;
    base.scrubShuffleMm = false;
    break;
  case 1:
    base.scrubShuffleOot = true;
    base.scrubShuffleMm = true;
    break;
  default:
    base.scrubShuffleOot = booleanWeighted(random, 0.5);
    base.scrubShuffleMm = booleanWeighted(random, 0.5);
  }

  /* Cow shuffle - 25% disabled, 25% enabled, 50% individual */
  switch (randomInt(random, 4)) {
  case 0:
    base.cowShuffleOot = false;
    base.cowShuffleMm = false;
    break;
  case 1:
    base.cowShuffleOot = true;
    base.cowShuffleMm = true;
    break;
  default:
    base.cowShuffleOot = booleanWeighted(random, 0.5);
    base.cowShuffleMm = booleanWeighted(random, 0.5);
  }

  base.shuffleMerchantsMm = booleanWeighted(random, 0.50);
  base.eggShuffle = booleanWeighted(random, 0.50);
  base.songs = sampleWeighted(random, { songLocations: 6, anywhere: 4 });
  base.divingGameRupeeShuffle = booleanWeighted(random, 0.50);
  base.pondFishShuffle = booleanWeighted(random, 0.50);
  base.fairyFountainFairyShuffleOot = booleanWeighted(random, 0.50);
  base.fairyFountainFairyShuffleMm = booleanWeighted(random, 0.50);

  /* Tokens - 25% disabled, 25% enabled, 50% individual */
  switch (randomInt(random, 4)) {
  case 0:
    base.goldSkulltulaTokens = 'none';
    base.housesSkulltulaTokens = 'none';
    break;
  case 1:
    base.goldSkulltulaTokens = 'all';
    base.housesSkulltulaTokens = 'all';
    break;
  default:
    base.goldSkulltulaTokens = sampleWeighted(random, { none: 5, dungeons: 3, overworld: 3, all: 2 });
    base.housesSkulltulaTokens = sampleWeighted(random, { none: 5, all: 3 });
  }

  base.tingleShuffle = sampleWeighted(random, { vanilla: 10, anywhere: 7, starting: 7, removed: 5 });
  base.mapCompassShuffle = sampleWeighted(random, { ownDungeon: 10, anywhere: 7, starting: 7, removed: 5 });

  /* Small key shuffle */
  base.smallKeyShuffleOot = sampleWeighted(random, { ownDungeon: 10, anywhere: 8, removed: 4 });
  if (booleanWeighted(random, 0.5)) {
    base.smallKeyShuffleMm = base.smallKeyShuffleOot;
  } else {
    base.smallKeyShuffleMm = sampleWeighted(random, { ownDungeon: 10, anywhere: 8, removed: 4 });
  }
  if (booleanWeighted(random, 0.5)) {
    base.smallKeyShuffleHideout = base.smallKeyShuffleOot === 'anywhere' ? 'anywhere' : 'ownDungeon';
  } else {
    base.smallKeyShuffleHideout = sampleWeighted(random, { ownDungeon: 10, anywhere: 7 });
  }

  /* Skeleton key */
  switch (randomInt(random, 4)) {
  case 0:
    break;
  case 1:
    base.skeletonKeyOot = true;
    base.skeletonKeyMm = true;
    break;
  default:
    base.skeletonKeyOot = booleanWeighted(random, 0.5);
    base.skeletonKeyMm = booleanWeighted(random, 0.5);
    break;
  }

  /* No skeleton key if keysy */
  if (base.smallKeyShuffleOot === 'removed') {
    base.skeletonKeyOot = false;
  }
  if (base.smallKeyShuffleMm === 'removed') {
    base.skeletonKeyMm = false;
  }

  /* Boss key shuffle */
  base.bossKeyShuffleOot = sampleWeighted(random, { ownDungeon: 10, anywhere: 8, removed: 4 });
  if (booleanWeighted(random, 0.5)) {
    base.bossKeyShuffleMm = base.bossKeyShuffleOot;
  } else {
    base.bossKeyShuffleMm = sampleWeighted(random, { ownDungeon: 10, anywhere: 8, removed: 4 });
  }

  /* Ganon BK - 50% chance to follow OoT BK */
  if (base.goal !== 'triforce' && base.goal !== 'triforce3') {
    if (booleanWeighted(random, 0.5)) {
      base.ganonBossKey = base.bossKeyShuffleOot === 'ownDungeon' ? 'ganon' : base.bossKeyShuffleOot;
    } else {
      base.ganonBossKey = sampleWeighted(random, { removed: 10, vanilla: 5, anywhere: 7, ganon: 5 });
    }
  }

  /* Silver rupees */
  base.silverRupeeShuffle = sampleWeighted(random, { ownDungeon: 10, anywhere: 7, vanilla: 3 });

  /* Stray fairies - 50% together, 50% individual */
  if (booleanWeighted(random, 0.5)) {
    base.strayFairyOtherShuffle = sampleWeighted(random, { anywhere: 7, vanilla: 3, ownDungeon: 5, starting: 3, removed: 3 });
    base.strayFairyChestShuffle = base.strayFairyOtherShuffle === 'removed' ? 'starting' : base.strayFairyOtherShuffle;
    base.townFairyShuffle = base.strayFairyChestShuffle === 'anywhere' ? 'anywhere' : 'vanilla';
  } else {
    base.strayFairyChestShuffle = sampleWeighted(random, { anywhere: 7, vanilla: 3, ownDungeon: 5, starting: 6 });
    base.strayFairyOtherShuffle = sampleWeighted(random, { anywhere: 7, vanilla: 3, ownDungeon: 5, starting: 3, removed: 3 });
    base.townFairyShuffle = sampleWeighted(random, { anywhere: 5, vanilla: 5 });
  }

  /* Dungeon reward shuffle */
  base.dungeonRewardShuffle = sampleWeighted(random, { dungeonBlueWarps: 6, dungeons: 2, dungeonsLimited: 3, anywhere: 2 });

  /* Shop shuffle */
  base.shopShuffleOot = sampleWeighted(random, { none: 10, full: 7 });
  if (booleanWeighted(random, 0.5)) {
    base.shopShuffleMm = base.shopShuffleOot;
  } else {
    base.shopShuffleMm = sampleWeighted(random, { none: 10, full: 7 });
  }
  base.owlShuffle = sampleWeighted(random, { none: 10, anywhere: 7 });

  /* Soul shuffle - 25% enabled, 25% disabled, 50% individual */
  switch (randomInt(random, 4)) {
  case 0:
    break;
  case 1:
    base.soulsEnemyOot = true;
    base.soulsEnemyMm = true;
    base.soulsBossOot = true;
    base.soulsBossMm = true;
    break;
  default:
    base.soulsEnemyOot = booleanWeighted(random, 0.5);;
    base.soulsEnemyMm = booleanWeighted(random, 0.5);;
    base.soulsBossOot = booleanWeighted(random, 0.5);;
    base.soulsBossMm = booleanWeighted(random, 0.5);;
  }

  /* Dungeon age modifiers - 25% all true, 25% all false, 50% individual */
  switch (randomInt(random, 4)) {
  case 0:
    base.dekuTreeAdult = true;
    base.wellAdult = true;
    base.fireChild = true;
    break;
  case 1:
    base.dekuTreeAdult = false;
    base.wellAdult = false;
    base.fireChild = false;
    break;
  default:
    base.dekuTreeAdult = booleanWeighted(random, 0.5);
    base.wellAdult = booleanWeighted(random, 0.5);
    base.fireChild = booleanWeighted(random, 0.5);
  }

  /* Events - 25% full open, 25% full closed, 50% individual */
  switch (randomInt(random, 4)) {
  case 0:
    base.skipZelda = true;
    base.openMoon = true;
    base.openDungeonsMm = { type: 'all' };
    base.clearStateDungeonsMm = 'both';
    base.doorOfTime = 'open';
    base.dekuTree = 'open';
    base.kakarikoGate = 'open';
    base.zoraKing = 'open';
    base.gerudoFortress = 'open';
    break;
  case 1:
    base.skipZelda = false;
    base.openMoon = false;
    base.openDungeonsMm = { type: 'none' };
    base.clearStateDungeonsMm = 'none';
    base.doorOfTime = 'closed';
    base.dekuTree = 'closed';
    base.kakarikoGate = 'closed';
    base.zoraKing = 'vanilla';
    base.gerudoFortress = 'vanilla';
    break;
  default:
    base.skipZelda = booleanWeighted(random, 0.3);
    base.openMoon = booleanWeighted(random, 0.3);
    base.openDungeonsMm = { type: 'random' };
    base.clearStateDungeonsMm = sampleWeighted(random, { none: 5, WF: 1, GB: 1, both: 2 });
    base.doorOfTime = sampleWeighted(random, { closed: 10, open: 7 });
    base.dekuTree = sampleWeighted(random, { open: 10, closed: 7 });
    base.kakarikoGate = sampleWeighted(random, { closed: 10, open: 7 });
    base.zoraKing = sampleWeighted(random, { vanilla: 10, open: 5, adult: 5 });
    base.gerudoFortress = sampleWeighted(random, { vanilla: 1, single: 1, open: 1 });
  }

  base.bossWarpPads = sampleWeighted(random, { bossBeaten: 10, remains: 4 });

  /* Cross warp - 25% disabled, 25% enabled, 50% individual */
  switch (randomInt(random, 4)) {
  case 0:
    base.crossWarpOot = false;
    base.crossWarpMm = 'none';
    break;
  case 1:
    base.crossWarpOot = true;
    base.crossWarpMm = 'full';
    break;
  default:
    base.crossWarpOot = booleanWeighted(random, 0.5);
    base.crossWarpMm = sampleWeighted(random, { none: 10, full: 7, childOnly: 3 });
  }

  base.sunSongMm = booleanWeighted(random, 0.5);
  base.elegyOot = booleanWeighted(random, 0.5);
  base.csmc = sampleWeighted(random, { always: 18, agony: 1, never: 1 });
  if (base.csmc !== 'never') {
    base.csmcHearts = booleanWeighted(random, 0.75);
  }

  /* Ageless items - 50% disabled, 10% enabled, 40% individual */
  switch (randomInt(random, 4)) {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
    break;
  case 5:
    base.agelessBoomerang = true;
    base.agelessSticks = true;
    base.agelessHammer = true;
    base.agelessHookshot = true;
    base.agelessTunics = true;
    base.agelessSwords = true;
    base.agelessShields = true;
    base.agelessBoots = true;
    break;
  default:
    base.agelessBoomerang = booleanWeighted(random, 0.1);
    base.agelessSticks = booleanWeighted(random, 0.1);
    base.agelessHammer = booleanWeighted(random, 0.1);
    base.agelessHookshot = booleanWeighted(random, 0.1);
    base.agelessTunics = booleanWeighted(random, 0.1);
    base.agelessSwords = booleanWeighted(random, 0.1);
    base.agelessShields = booleanWeighted(random, 0.1);
    base.agelessBoots = booleanWeighted(random, 0.1);
  }

  base.blastMaskCooldown = sampleWeighted(random, { default: 10, instant: 2, veryshort: 3, short: 3, long: 2, verylong: 2 });

  /* Items */
  base.fillWallets = booleanWeighted(random, 0.5);

  /* Progressive items - 25% disabled, 25% enabled, 50% individual */
  switch (randomInt(random, 4)) {
  case 0:
    base.progressiveShieldsOot = 'separate';
    base.progressiveSwordsOot = 'separate';
    base.progressiveShieldsMm = 'separate';
    base.progressiveGoronLullaby = 'single';
    break;
  case 1:
    base.progressiveShieldsOot = 'progressive';
    base.progressiveSwordsOot = 'progressive';
    base.progressiveShieldsMm = 'progressive';
    base.progressiveGoronLullaby = 'progressive';
    break;
  default:
    base.progressiveShieldsOot = sampleWeighted(random, { separate: 10, progressive: 10 });
    base.progressiveSwordsOot = sampleWeighted(random, { separate: 10, progressive: 10, goron: 5 });
    base.progressiveShieldsMm = sampleWeighted(random, { separate: 10, progressive: 10 });
    base.progressiveGoronLullaby = sampleWeighted(random, { progressive: 10, single: 10 });
  }

  /* Shared items - 25% disabled, 25% enabled, 50% individual */
  switch (randomInt(random, 4)) {
  case 0:
    break;
  case 1:
    base.sharedNutsSticks = true;
    base.sharedBows = true;
    base.sharedBombBags = true;
    base.sharedMagic = true;
    base.sharedMagicArrowFire = true;
    base.sharedMagicArrowIce = true;
    base.sharedMagicArrowLight = true;
    base.sharedSongEpona = true;
    base.sharedSongStorms = true;
    base.sharedSongTime = true;
    base.sharedSongSun = true;
    base.sharedSongElegy = true;
    base.sharedHookshot = true;
    base.sharedLens = true;
    base.sharedOcarina = true;
    base.sharedMaskGoron = true;
    base.sharedMaskZora = true;
    base.sharedMaskBunny = true;
    base.sharedMaskTruth = true;
    base.sharedMaskKeaton = true;
    base.sharedMaskBlast = true;
    base.sharedMaskStone = true;
    base.sharedWallets = true;
    base.sharedHealth = true;
    base.sharedSoulsEnemy = true;
    base.sharedSkeletonKey = true;
    base.sharedSpellFire = true;
    base.sharedSpellWind = true;
    base.sharedSpellLove = true;
    base.sharedBootsIron = true;
    base.sharedBootsHover = true;
    base.sharedTunicGoron = true;
    base.sharedTunicZora = true;
    break;
  default:
    base.sharedNutsSticks = booleanWeighted(random, 0.5);
    base.sharedBows = booleanWeighted(random, 0.5);
    base.sharedBombBags = booleanWeighted(random, 0.5);
    base.sharedMagic = booleanWeighted(random, 0.5);
    base.sharedHookshot = booleanWeighted(random, 0.5);
    base.sharedLens = booleanWeighted(random, 0.5);
    base.sharedOcarina = booleanWeighted(random, 0.5);
    base.sharedWallets = booleanWeighted(random, 0.5);
    base.sharedHealth = booleanWeighted(random, 0.5);
    base.sharedSoulsEnemy = booleanWeighted(random, 0.5);
    base.sharedSkeletonKey = booleanWeighted(random, 0.5);

    /* Boots - grouping */
    switch (randomInt(random, 4)) {
    case 0:
      break;
    case 1:
      base.sharedBootsIron = true;
      base.sharedBootsHover = true;
      break;
    default:
      base.sharedBootsIron = booleanWeighted(random, 0.5);
      base.sharedBootsHover = booleanWeighted(random, 0.5);
      break;
    }

    /* Tunics - grouping */
    switch (randomInt(random, 4)) {
    case 0:
      break;
    case 1:
      base.sharedTunicGoron = true;
      base.sharedTunicZora = true;
      break;
    default:
      base.sharedTunicGoron = booleanWeighted(random, 0.5);
      base.sharedTunicZora = booleanWeighted(random, 0.5);
      break;
    }

    /* Spells - grouping */
    switch (randomInt(random, 4)) {
    case 0:
      break;
    case 1:
      base.sharedSpellFire = true;
      base.sharedSpellWind = true;
      base.sharedSpellLove = true;
      break;
    default:
      base.sharedSpellFire = booleanWeighted(random, 0.5);
      base.sharedSpellWind = booleanWeighted(random, 0.5);
      base.sharedSpellLove = booleanWeighted(random, 0.5);
      break;
    }

    /* Masks - grouping */
    switch (randomInt(random, 4)) {
    case 0:
      break;
    case 1:
      base.sharedMaskGoron = true;
      base.sharedMaskZora = true;
      base.sharedMaskBunny = true;
      base.sharedMaskTruth = true;
      base.sharedMaskKeaton = true;
      base.sharedMaskBlast = true;
      base.sharedMaskStone = true;
      break;
    default:
      base.sharedMaskGoron = booleanWeighted(random, 0.5);
      base.sharedMaskZora = booleanWeighted(random, 0.5);
      base.sharedMaskBunny = booleanWeighted(random, 0.5);
      base.sharedMaskTruth = booleanWeighted(random, 0.5);
      base.sharedMaskKeaton = booleanWeighted(random, 0.5);
      base.sharedMaskBlast = booleanWeighted(random, 0.5);
      base.sharedMaskStone = booleanWeighted(random, 0.5);
      break;
    }

    /* Songs - grouping */
    switch (randomInt(random, 4)) {
    case 0:
      break;
    case 1:
      base.sharedSongEpona = true;
      base.sharedSongStorms = true;
      base.sharedSongTime = true;
      base.sharedSongSun = true;
      base.sharedSongElegy = true;
      break;
    default:
      base.sharedSongEpona = booleanWeighted(random, 0.5);
      base.sharedSongStorms = booleanWeighted(random, 0.5);
      base.sharedSongTime = booleanWeighted(random, 0.5);
      base.sharedSongSun = booleanWeighted(random, 0.5);
      base.sharedSongElegy = booleanWeighted(random, 0.5);
      break;
    }

    /* Magical Arrows - grouping */
    switch (randomInt(random, 4)) {
    case 0:
      break;
    case 1:
      base.sharedMagicArrowFire = true;
      base.sharedMagicArrowIce = true;
      base.sharedMagicArrowLight = true;
      break;
    default:
      base.sharedMagicArrowFire = booleanWeighted(random, 0.5);
      base.sharedMagicArrowIce = booleanWeighted(random, 0.5);
      base.sharedMagicArrowLight = booleanWeighted(random, 0.5);
      break;
    }
  }

  base.fairyOcarinaMm = booleanWeighted(random, 0.5);
  base.shortHookshotMm = booleanWeighted(random, 0.25);
  base.blastMaskOot = booleanWeighted(random, 0.25);
  base.stoneMaskOot = booleanWeighted(random, 0.25);

  /* Boots - 50% disabled, 25% enabled, 25% individual */
  switch (randomInt(random, 4)) {
  case 0:
  case 1:
    base.bootsIronMm = false;
    base.bootsHoverMm = false;
    break;
  case 2:
    base.bootsIronMm = true;
    base.bootsHoverMm = true;
    break;
  default:
    base.bootsIronMm = booleanWeighted(random, 0.25);
    base.bootsHoverMm = booleanWeighted(random, 0.25);
  }

  /* Tunics - 50% disabled, 25% enabled, 25% individual */
  switch (randomInt(random, 4)) {
  case 0:
  case 1:
    base.tunicGoronMm = false;
    base.tunicZoraMm = false;
    break;
  case 2:
    base.tunicGoronMm = true;
    base.tunicZoraMm = true;
    break;
  default:
    base.tunicGoronMm = booleanWeighted(random, 0.25);
    base.tunicZoraMm = booleanWeighted(random, 0.25);
  }

  /* Scales - 50% disabled, 25% enabled, 25% individual */
  switch (randomInt(random, 4)) {
  case 0:
  case 1:
    base.scalesMm = false;
    break;
  case 2:
    base.scalesMm = true;
    break;
  default:
    base.scalesMm = booleanWeighted(random, 0.25);
  }

  /* Magic spells - 50% disabled, 25% enabled, 25% individual */
  switch (randomInt(random, 4)) {
  case 0:
  case 1:
    base.spellFireMm = false;
    base.spellWindMm = false;
    base.spellLoveMm = false;
    break;
  case 2:
    base.spellFireMm = true;
    base.spellWindMm = true;
    base.spellLoveMm = true;
    break;
  default:
    base.spellFireMm = booleanWeighted(random, 0.25);
    base.spellWindMm = booleanWeighted(random, 0.25);
    base.spellLoveMm = booleanWeighted(random, 0.25);
  }

  /* Extra Wallets - 25% none, 25% all, 50% individual */
  switch (randomInt(random, 4)) {
  case 0:
    base.childWallets = false;
    base.colossalWallets = false;
    break;
  case 1:
    base.childWallets = true;
    base.colossalWallets = true;
    break;
  default:
    base.childWallets = booleanWeighted(random, 0.25);
    base.colossalWallets = booleanWeighted(random, 0.5);
  }

  /* Price rando - 25% none, 25% all, 50% individual */
  switch (randomInt(random, 4)) {
  case 0:
    break;
  case 1:
    const option = sampleWeighted(random, { affordable: 5, weighted: 10 });
    base.priceOotShops = option;
    base.priceMmShops = option;
    base.priceOotScrubs = option;
    base.priceMmTingle = option;
    break;
  default:
    base.priceOotShops = sampleWeighted(random, { vanilla: 10, affordable: 10, weighted: 10 });
    base.priceMmShops = sampleWeighted(random, { vanilla: 10, affordable: 10, weighted: 10 });
    base.priceOotScrubs = sampleWeighted(random, { vanilla: 10, affordable: 10, weighted: 10 });
    base.priceMmTingle = sampleWeighted(random, { vanilla: 10, affordable: 10, weighted: 10 });
    break;
  }

  /* MQ - 25% Disabled, 25% Enabled, 50% Individual */
  if (rnd.mq) {
    let f: () => 'vanilla' | 'mq';

    switch (randomInt(random, 4)) {
    case 0:
      f = () => 'vanilla';
      break;
    case 1:
      f = () => 'mq';
      break;
    default:
      f = () => booleanWeighted(random, 0.5) ? 'vanilla' : 'mq';
    }

    for (let k in base.dungeon) {
      base.dungeon[k as keyof typeof base.dungeon] = f();
    }
  }

  if (rnd.er) {
    /* ER Types: 25% Disabled, 25% Full, 50% Individual */
    switch (randomInt(random, 4)) {
    case 0:
      break;
    case 1:
      base.erDungeons = 'full';
      base.erBoss = 'full';
      base.erRegions = 'full';
      base.erIndoors = 'full';
      base.erWarps = 'full';
      base.erOneWays = 'full';
      break;
    default:
      base.erDungeons = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
      base.erBoss = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
      base.erRegions = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
      base.erIndoors = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
      base.erWarps = sampleWeighted(random, { none: 10, ootOnly: 10, mmOnly: 10, ownGame: 10, full: 10 });
      base.erOneWays = sampleWeighted(random, { none: 10, full: 10, ownGame: 10 });
    }

    /* ER sub-settings - 25% all on, 25% all off, 50% individual */
    switch (randomInt(random, 4)) {
    case 0:
      break;
    case 1:
      base.erBeneathWell = true;
      base.erGanonCastle = true;
      base.erGanonTower = true;
      base.erMoon = true;
      base.erIkanaCastle = true;
      base.erMajorDungeons = true;
      base.erMinorDungeons = true;
      base.erPirateFortress = true;
      base.erSecretShrine = true;
      base.erSpiderHouses = true;
      base.erRegionsExtra = true;
      base.erRegionsShortcuts = true;
      base.erIndoorsMajor = true;
      base.erIndoorsExtra = true;
      base.erOneWaysMajor = true;
      base.erOneWaysIkana = true;
      base.erOneWaysSongs = true;
      base.erOneWaysStatues = true;
      base.erOneWaysOwls = true;
      break;
    default:
      base.erBeneathWell = booleanWeighted(random, 0.5);
      base.erGanonCastle = booleanWeighted(random, 0.5);
      base.erGanonTower = booleanWeighted(random, 0.5);
      base.erMoon = booleanWeighted(random, 0.5);
      base.erIkanaCastle = booleanWeighted(random, 0.5);
      base.erMajorDungeons = booleanWeighted(random, 0.5);
      base.erMinorDungeons = booleanWeighted(random, 0.5);
      base.erPirateFortress = booleanWeighted(random, 0.5);
      base.erSecretShrine = booleanWeighted(random, 0.5);
      base.erSpiderHouses = booleanWeighted(random, 0.5);
      base.erRegionsExtra = booleanWeighted(random, 0.5);
      base.erRegionsShortcuts = booleanWeighted(random, 0.5);
      base.erIndoorsMajor = booleanWeighted(random, 0.5);
      base.erIndoorsExtra = booleanWeighted(random, 0.5);
      base.erOneWaysMajor = booleanWeighted(random, 0.5);
      base.erOneWaysIkana = booleanWeighted(random, 0.5);
      base.erOneWaysSongs = booleanWeighted(random, 0.5);
      base.erOneWaysStatues = booleanWeighted(random, 0.5);
      base.erOneWaysOwls = booleanWeighted(random, 0.5);
    }
  }

  return validateSettings(base);
}
