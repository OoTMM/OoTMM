import fs from 'fs/promises';
import path from 'path';
import YAML from 'yaml';

import { PATH_DATA } from './config';

type Defines = {[k: string]: number};

const loadYML = async <T>(filename: string) => {
  const text = await fs.readFile(filename, 'utf8');
  return YAML.parse(text) as T;
};

export const DATA_GI = loadYML<Defines>(path.join(PATH_DATA, 'gi.yml'));
export const DATA_SCENES = loadYML<Defines>(path.join(PATH_DATA, 'scenes.yml'));
