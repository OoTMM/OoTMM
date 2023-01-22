import fs from "fs";
import YAML from 'yaml';
import * as CSV from 'csv/sync';
import * as glob from 'glob';

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

import { logic } from "../lib/combo/logic";
import { DEFAULT_SETTINGS } from "../lib/combo/settings";
import { Monitor } from "../lib/combo/monitor";

for (let i = 0; i < 3; ++i) {
  test("Can randomize a seed (" + (i + 1) + ")", () => {
    const mon = new Monitor({});
    logic(mon, { debug: false, seed: "TEST" + i, settings: DEFAULT_SETTINGS });
  });
}
