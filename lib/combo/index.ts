import { Buffer } from 'buffer';

import { options, OptionsInput } from './options';
import { Generator } from './generator';
import { MonitorCallbacks } from './monitor';
import { SETTINGS } from './settings';

type GeneratorParams = {
  oot: Buffer,
  mm: Buffer,
  opts?: OptionsInput,
  monitor?: MonitorCallbacks
};

export const generate = (params: GeneratorParams): Generator => {
  const opts = options(params.opts || {});
  return new Generator(params.oot, params.mm, opts, params.monitor || {});
};

export { SETTINGS };
