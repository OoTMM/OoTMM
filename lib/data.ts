import fs from 'fs/promises';
import path from 'path';
import YAML from 'yaml';

import { PATH_DATA } from './config';

type DataGI = {[k: string]: number};

const loadYML = async <T>(filename: string) => {
  const text = await fs.readFile(filename, 'utf8');
  return YAML.parse(text) as T;
};

export const GI = loadYML<DataGI>(path.join(PATH_DATA, 'gi.yml'));
