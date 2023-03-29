import fs from 'fs/promises';
import YAML from 'yaml';

import { generate } from "./combo";
import { OptionsInput } from "./combo/options";

const makeOptions = async (args: string[]): Promise<OptionsInput> => {
  const opts: OptionsInput = {};
  opts.settings = {};

  for (let i = 0; i < args.length; i++) {
    const opt = args[i];
    switch (opt) {
    case "--debug":
      opts.debug = true;
      break;
    case "--seed":
      opts.seed = args[++i];
      break;
    case "--config": {
      const configFile = await fs.readFile(args[++i]);
      const settings = YAML.parse(configFile.toString());
      opts.settings = settings;
      break;
    }
    case "--patch": {
      const patch = await fs.readFile(args[++i]);
      opts.patch = patch;
      break;
    }
    default:
      throw new Error(`Unknown option: ${opt}`);
    }
  }
  return opts;
};

const main = async () => {
  const opts = await makeOptions(process.argv.slice(2));
  const [oot, mm] = await Promise.all([
    fs.readFile('./roms/oot.z64'),
    fs.readFile('./roms/mm.z64'),
  ]);
  const gen = generate({ oot, mm, opts });
  const { rom, log, hash, patch } = await gen.run();
  await fs.mkdir('out', { recursive: true });
  const promises: Promise<unknown>[] = [];
  const suffix =  opts.debug ? "" : `-${hash}`
  promises.push(fs.writeFile(`out/OoTMM${suffix}.z64`, rom));
  if (log)
    promises.push(fs.writeFile(`out/OoTMM-spoiler${suffix}.txt`, log));
  if (patch)
    promises.push(fs.writeFile(`out/OoTMM-Patch${suffix}.ootmm`, patch));
  return Promise.all(promises);
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});

