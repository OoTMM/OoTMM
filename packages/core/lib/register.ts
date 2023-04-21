import fs from 'fs';
import YAML from 'yaml';
import * as CSV from 'csv/sync';

/* Loader hooks */
require.extensions[".yml"] = (module, filename) => {
  const content = fs.readFileSync(filename, "utf8");
  module.exports = YAML.parse(content);
};

require.extensions[".csv"] = (module, filename) => {
  const content = fs.readFileSync(filename, "utf8");
  module.exports = CSV.parse(content, { columns: true, skip_empty_lines: true, trim: true });
};
