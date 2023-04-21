import { makeSettings, Settings } from './settings';
import { randString } from './random';

export type Options = {
  debug: boolean;
  seed: string;
  settings: Settings;
  patch?: Buffer;
};

export type OptionsInput = Partial<Pick<Options, 'debug' | 'seed' | 'patch'> & { settings: Partial<Settings> }>;

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

  return newOpts as Options;
};
