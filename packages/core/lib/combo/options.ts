import { makeSettings, Settings } from './settings';
import { randString } from './random';
import { Cosmetics, makeCosmetics } from './cosmetics';
import { makeRandomSettings, OptionRandomSettings } from './settings/random';

export type Options = {
  debug: boolean;
  seed: string;
  settings: Settings;
  cosmetics: Cosmetics;
  random: OptionRandomSettings;
  patch?: Buffer;
};

export type OptionsInput =
  Partial<
    Pick<Options, 'debug' | 'seed' | 'patch'>
    & { settings: Partial<Settings> }
    & { cosmetics: Partial<Cosmetics> }
    & { random: Partial<OptionRandomSettings> }
  >;

export const options = (opts: OptionsInput): Options => {
  const newOpts: Partial<Options> = {};

  newOpts.debug = !!opts.debug;
  newOpts.patch = opts.patch;

  if (opts.seed) {
    newOpts.seed = opts.seed;
  } else {
    if (newOpts.debug) {
      newOpts.seed = 'DEBUG';
    } else {
      newOpts.seed = randString();
    }
  }

  newOpts.settings = makeSettings(opts.settings || {});
  newOpts.cosmetics = makeCosmetics(opts.cosmetics || {});
  newOpts.random = makeRandomSettings(opts.random || {});

  return newOpts as Options;
};
