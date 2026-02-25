import { importSettingsRaw } from '../lib/combo/settings/string';

const data = importSettingsRaw(process.argv[2]);
console.log(data);
