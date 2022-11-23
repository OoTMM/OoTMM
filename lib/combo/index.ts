import { Buffer } from 'buffer';

import { Options } from "./options";
import { Generator } from "./generator";
import { MonitorCallbacks } from './monitor';

type GeneratorParams = {
  oot: Buffer,
  mm: Buffer,
  opts?: Options,
  monitor?: MonitorCallbacks
};

export const generate = (params: GeneratorParams): Generator => {
  return new Generator(params.oot, params.mm, params.opts || {}, params.monitor || {});
};
