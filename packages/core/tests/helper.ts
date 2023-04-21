import fs from "fs";
import YAML from 'yaml';
import * as CSV from 'csv/sync';
import * as glob from 'glob';
import { merge } from 'lodash';

const mockCSV = (filename: string) => {
  const data = fs.readFileSync(filename, "utf8");
  const module = CSV.parse(data, { columns: true, skip_empty_lines: true, trim: true });
  jest.setMock(filename, module);
};

const mockYAML = (filename: string) => {
  const data = fs.readFileSync(filename, "utf8");
  const module = YAML.parse(data);
  jest.setMock(filename, module);
};

glob.sync(__dirname + "/../data/**/*.yml").forEach(mockYAML);
glob.sync(__dirname + "/../data/**/*.csv").forEach(mockCSV);

import { Optional } from "../lib/combo/util";
import { DEFAULT_SETTINGS, Settings } from "../lib/combo/settings";
import { Monitor } from "../lib/combo/monitor";
import { logic } from "../lib/combo/logic";

const sharedMonitor = new Monitor({ onLog: () => {} });

export const makeTestSeed = (seed: string, settings: Optional<Settings>) => {
  const s = merge({}, DEFAULT_SETTINGS, settings, {
    probabilisticFoolish: false,
  });
  return logic(sharedMonitor, { debug: false, seed, settings: s });
}
