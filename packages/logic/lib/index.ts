import { Settings } from '@ootmm/core';

import Logic from '../build/dist/logic.js';

const LogicModulePromise = Logic();

type LogicArgs = {
  settings: Settings;
};

type LogicImplArgs = {
  settings: Settings;
}

export async function logicNew(args: LogicArgs) {
  const module: any = await LogicModulePromise;
  const implArgs: LogicImplArgs = {
    settings: args.settings,
  };

  module.logicRun(implArgs);
  process.exit(0);
}
