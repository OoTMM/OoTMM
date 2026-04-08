import type { PartialDeep } from 'type-fest';
import type { Settings } from '@ootmm/core';

import { merge } from 'lodash-es';
import { DEFAULT_SETTINGS } from '@ootmm/core';

import { Monitor } from '../lib/combo/monitor';
import { solvedWorldState } from '../lib/combo/logic';
import { makeCosmetics } from '../lib/combo';
import { makeRandomSettings } from '../lib/combo/random-settings';

export const makeTestSeed = async (seed: string, settings: PartialDeep<Settings>) => {
  const monitor = new Monitor({ onLog: () => {} });
  const cosmetics = makeCosmetics({});
  const random = makeRandomSettings({});
  const s = merge({}, DEFAULT_SETTINGS, settings, {
    probabilisticFoolish: false,
  });
  const ws = await solvedWorldState(monitor, { cosmetics, seed, settings: s, random, mode: 'create' });
  return ws;
}
