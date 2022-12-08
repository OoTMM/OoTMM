import { Settings, settings } from './settings';
import { randString } from './random';

export type Options = {
  debug: boolean;
  seed: string;
  settings: Settings;
};

export type OptionsInput = { [k in keyof Options]?: Partial<Options[k]> };

export const options = (opts: OptionsInput): Options => {
  const newOpts: Partial<Options> = {};

  newOpts.debug = !!opts.debug;

  if (opts.seed) {
    newOpts.seed = opts.seed;
  } else {
    if (newOpts.debug) {
      newOpts.seed = 'DEBUG';
    } else {
      newOpts.seed = randString();
    }
  }

  newOpts.settings = settings(opts.settings || {});

  return newOpts as Options;
};
