import type { Settings } from '@ootmm/core';
import type { PartialDeep } from 'type-fest';

export const CONFIGS: Record<string, PartialDeep<Settings>> = {
  default: {},

  allsanity: {
    songs: 'anywhere',
    goldSkulltulaTokens: 'all',
    housesSkulltulaTokens: 'all',
    strayFairyChestShuffle: 'anywhere',
    strayFairyOtherShuffle: 'anywhere',
    townFairyShuffle: 'anywhere',
    bossKeyShuffleOot: 'anywhere',
    bossKeyShuffleMm: 'anywhere',
    smallKeyShuffleOot: 'anywhere',
    smallKeyShuffleMm: 'anywhere',
    smallKeyShuffleHideout: 'anywhere',
    cowShuffleOot: true,
    cowShuffleMm: true,
    shopShuffleOot: 'full',
    shopShuffleMm: 'full',
    eggShuffle: true,
    divingGameRupeeShuffle: true,
    pondFishShuffle: true,
    fairyFountainFairyShuffleOot: true,
    fairyFountainFairyShuffleMm: true,
    scrubShuffleOot: true,
    childWallets: true,
    colossalWallets: true,
    tingleShuffle: 'anywhere',
    owlShuffle: 'anywhere',
  },

  'full-er': {
    songs: 'anywhere',
    erDungeons: 'full',
    erBoss: 'full',
    erRegions: 'full',
    erRegionsShortcuts: true,
    erIndoors: 'full',
    erIndoorsMajor: true,
    erIndoorsExtra: true,
    erOneWays: 'full',
    erOneWaysMajor: true,
    erOneWaysIkana: true,
    erOneWaysSongs: true,
    erOneWaysStatues: true,
    erOneWaysOwls: true,
    erMajorDungeons: true,
    erMinorDungeons: true,
    erSpiderHouses: true,
    erPirateFortress: true,
    erBeneathWell: true,
    erIkanaCastle: true,
    erSecretShrine: true,
  },

  'mq-boss-er': {
    mqDungeons: { type: 'all' },
    erBoss: 'full',
    erDungeons: 'full',
    erMajorDungeons: true,
    erMinorDungeons: true,
  },
};

export const DEFAULT_CONFIG_NAMES = ['default', 'allsanity', 'fuller'];

export function resolveConfigs(names: string[]): [string, PartialDeep<Settings>][] {
  return names.map((name) => {
    const settings = CONFIGS[name];
    if (!settings) {
      throw new Error(`Unknown bench config '${name}'. Known: ${Object.keys(CONFIGS).join(', ')}`);
    }
    return [name, settings];
  });
}
