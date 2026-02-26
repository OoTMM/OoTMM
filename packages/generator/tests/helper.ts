import { merge } from 'lodash';
import { PartialDeep } from 'type-fest';
import { DEFAULT_SETTINGS, Settings } from '@ootmm/core';

import { Monitor } from '@ootmm/core/src/monitor';
import { solvedWorldState } from '../../logic/src';
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
