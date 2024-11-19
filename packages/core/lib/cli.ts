import { promises as fs } from 'fs';
import YAML from 'yaml';

import { SETTINGS, generate, importSettings, makeSettings } from './combo';
import { OptionsInput } from './combo/options';

function readFileUint8(path: string): Promise<Uint8Array> {
  return fs.readFile(path).then((x) => new Uint8Array(x.buffer, x.byteOffset, x.byteLength));
}

function parseSettings(data: any): any {
  const result = {} as any;
  for (const key in data) {
    if (['startingItems', 'tricks', 'junkLocations', 'specialConds', 'plando', 'hints'].includes(key)) {
      result[key] = data[key];
      continue;
    }
    const setting = SETTINGS.find((x) => x.key === key);
    if (!setting) {
      console.log(`warn: Unknown setting: ${key}`);
      continue;
    }
    switch (setting.type) {
    case 'set':
      if (data[key] instanceof Array) {
        result[key] = { type: 'specific', values: Array.from(new Set(data[key])) };
      } else if (data[key] instanceof Object) {
        result[key] = { ...data[key] };
      } else {
        result[key] = { type: data[key] };
      }
      break;
    default:
      result[key] = data[key];
      break;
    }
  }
  return result;
}

const makeOptions = async (args: string[]): Promise<OptionsInput> => {
  const opts: OptionsInput = {};
  opts.settings = {};

  for (let i = 0; i < args.length; i++) {
    const opt = args[i];
    switch (opt) {
    case '--seed':
      opts.seed = args[++i];
      break;
    case '--random':
      opts.random = { enabled: true };
      break;
    case '--config': {
      const configFile = await fs.readFile(args[++i]);
      const config = YAML.parse(configFile.toString());
      if (config.settings) {
        opts.settings = parseSettings(config.settings);
      }
      if (config.cosmetics) {
        opts.cosmetics = config.cosmetics;
      }
      break;
    }
    case '--patch': {
      const patch = await readFileUint8(args[++i]);
      opts.patch = patch;
      break;
    }
    case '--settings': {
      opts.settings = makeSettings(importSettings(args[++i]));
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
    readFileUint8('../../roms/oot.z64'),
    readFileUint8('../../roms/mm.z64'),
  ]);
  const gen = generate({ oot, mm, opts });
  const { files } = await gen.run();
  await fs.mkdir('out', { recursive: true });
  return Promise.all(files.map(x => fs.writeFile(`out/${x.name}`, x.data)));
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
