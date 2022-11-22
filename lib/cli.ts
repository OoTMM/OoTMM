import fs from 'fs/promises';
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
  const { rom, log } = await gen.run();
  await fs.mkdir('out', { recursive: true });
  return Promise.all([
    fs.writeFile('out/OoTMM.z64', rom),
    fs.writeFile('out/spoiler.txt', log),
  ]);
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
