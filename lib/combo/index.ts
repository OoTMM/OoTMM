import { Options } from "./options";
import { Generator } from "./generator";

type GeneratorParams = {
  oot: string | Buffer,
  mm: string | Buffer,
  opts?: Options,
};

export const generate = (params: GeneratorParams): Generator => {
  return new Generator(params.oot, params.mm, params.opts || {});
};
