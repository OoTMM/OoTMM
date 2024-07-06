import { makeSettings, Settings } from './settings';
import { randString } from './random';
import { Cosmetics, makeCosmetics } from './cosmetics';
import { makeRandomSettings, OptionRandomSettings } from './settings/random';

type FileResolverFetchFunc = (filename: string) => Promise<Buffer>;
type FileResolverGlobFunc = (pattern: RegExp) => Promise<string[]>;

export type FileResolver = {
  fetch: FileResolverFetchFunc;
  glob: FileResolverGlobFunc;
};

export type ResolverFetchFunc = (filename: string) => Promise<Buffer>;

export type Options = {
  debug: boolean;
  seed: string;
  settings: Settings;
  cosmetics: Cosmetics;
  random: OptionRandomSettings;
  patch?: Buffer | ArrayBuffer;
  resolver?: FileResolver;
};

export type OptionsInput =
  Partial<
    Pick<Options, 'debug' | 'seed' | 'patch' | 'resolver'>
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
  newOpts.resolver = opts.resolver;

  return newOpts as Options;
};
