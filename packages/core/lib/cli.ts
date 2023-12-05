import fs from 'fs/promises';
import YAML from 'yaml';

import { SETTINGS, generate, importSettings } from "./combo";
import { OptionsInput } from "./combo/options";
import { makeSettings } from './combo';

function parseSettings(data: any): any {
  const result = {} as any;
  for (const key in data) {
    if (['startingItems', 'tricks', 'glitches', 'junkLocations', 'dungeon', 'specialConds', 'plando', 'hints'].includes(key)) {
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
    case "--debug":
      opts.debug = true;
      break;
    case "--seed":
      opts.seed = args[++i];
      break;
    case "--random":
      opts.random = { enabled: true };
      break;
    case "--config": {
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
    case "--patch": {
      const patch = await fs.readFile(args[++i]);
      opts.patch = patch;
      break;
    }
    case "--settings": {
      opts.settings = makeSettings(importSettings(args[++i]));
      break;
    }
    default:
      throw new Error(`Unknown option: ${opt}`);
    }
  }
  return opts;
};

function writeFiles(data: (Buffer | string)[], name: string, ext: string) {
  if (data.length === 1) {
    return [fs.writeFile(`${name}.${ext}`, data[0])];
  }

  return data.map((x, i) => {
    return fs.writeFile(`${name}-${i+1}.${ext}`, x);
  });
}

const main = async () => {
  const opts = await makeOptions(process.argv.slice(2));
  const [oot, mm] = await Promise.all([
    fs.readFile('../../roms/oot.z64'),
    fs.readFile('../../roms/mm.z64'),
  ]);
  const gen = generate({ oot, mm, opts });
  const { roms, log, hash, patches } = await gen.run();
  await fs.mkdir('out', { recursive: true });
  const promises: Promise<unknown>[] = [];
  const suffix =  opts.debug ? "" : `-${hash}`

  promises.push(...writeFiles(roms, `out/OoTMM${suffix}`, 'z64'));
  promises.push(...writeFiles(patches, `out/OoTMM-Patch${suffix}`, 'ootmm'));
  if (log)
    promises.push(...writeFiles([log], `out/OoTMM-spoiler${suffix}`, 'txt'));

  return Promise.all(promises);
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});

