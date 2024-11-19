import { merge } from 'lodash';

import { Optional } from "../lib/combo/util";
import { DEFAULT_SETTINGS, Settings } from "../lib/combo/settings";
import { Monitor } from "../lib/combo/monitor";
import { solvedWorldState } from "../lib/combo/logic";
import { makeCosmetics } from "../lib/combo";
import { makeRandomSettings } from "../lib/combo/settings/random";

export const makeTestSeed = async (seed: string, settings: Optional<Settings>) => {
  const monitor = new Monitor({ onLog: () => {} });
  const cosmetics = makeCosmetics({});
  const random = makeRandomSettings({});
  const s = merge({}, DEFAULT_SETTINGS, settings, {
    probabilisticFoolish: false,
  });
  const ws = await solvedWorldState(monitor, { cosmetics, seed, settings: s, random });
  return ws;
}
