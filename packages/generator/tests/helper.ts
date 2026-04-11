import type { PartialDeep } from 'type-fest';
import type { Settings } from '@ootmm/core';

import { merge } from 'lodash-es';
import { Monitor, DEFAULT_SETTINGS, makeRandomSettings, makeCosmetics } from '@ootmm/core';
import { solvedWorldState } from '../lib/combo/logic';

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
