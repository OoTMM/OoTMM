import fs from 'fs';
import YAML from 'yaml';

/* Loader hooks */
require.extensions[".yml"] = (module, filename) => {
  const content = fs.readFileSync(filename, "utf8");
  module.exports = YAML.parse(content);
};

import { generate } from "./combo";
import { PATH_ROMS } from './combo/config';
import { Options } from "./combo/options";

const makeOptions = (args: string[]): Options => {
  const opts: Options = {
    debug: false
  };
  for (let i = 0; i < args.length; i++) {
    const opt = args[i];
    switch (opt) {
    case "--debug":
      opts.debug = true;
      break;
    case "--seed":
      opts.seed = args[++i];
      break;
    default:
      throw new Error(`Unknown option: ${opt}`);
    }
  }
  return opts;
};

const main = async () => {
  const opts = makeOptions(process.argv.slice(2));
  const gen = generate({
    oot: PATH_ROMS + '/oot.z64',
    mm: PATH_ROMS + '/mm.z64',
    opts
  });
  await gen.run();
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
