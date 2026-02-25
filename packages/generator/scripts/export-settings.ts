import { importSettingsRaw } from '@ootmm/core';

const data = importSettingsRaw(process.argv[2]);
console.log(data);
