import { makeSettings, Settings } from './settings';
import { randString } from './random';
import { Cosmetics, makeCosmetics } from './cosmetics';
import { makeRandomSettings, OptionRandomSettings } from './settings/random';

export type Options = {
  seed: string;
  settings: Settings;
  cosmetics: Cosmetics;
  random: OptionRandomSettings;
  patch?: Uint8Array;
};

export type OptionsInput =
  Partial<
    Pick<Options, 'seed' | 'patch'>
    & { settings: Partial<Settings> }
    & { cosmetics: Partial<Cosmetics> }
    & { random: Partial<OptionRandomSettings> }
  >;

export const options = (opts: OptionsInput): Options => {
  const newOpts: Partial<Options> = {};

  newOpts.patch = opts.patch;

  if (opts.seed) {
    newOpts.seed = opts.seed.trim();
  } else {
    newOpts.seed = randString();
  }

  newOpts.settings = makeSettings(opts.settings || {});
  newOpts.cosmetics = makeCosmetics(opts.cosmetics || {});
  newOpts.random = makeRandomSettings(opts.random || {});

  return newOpts as Options;
};
