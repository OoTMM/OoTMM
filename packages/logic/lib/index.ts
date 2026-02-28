import { Settings } from '@ootmm/core';
import { POOL } from '@ootmm/data';

import Logic from '../build/dist/logic.js';

const LogicModulePromise = Logic();

type LogicArgs = {
  settings: Settings;
};

type LogicImplArgs = {
  settings: Settings;
  data: {
    pool: typeof POOL;
  }
};

export async function logicNew(args: LogicArgs) {
  const module: any = await LogicModulePromise;
  const implArgs: LogicImplArgs = {
    settings: args.settings,
    data: {
      pool: POOL
    }
  };

  console.log(implArgs);

  module.logicRun(implArgs);
  process.exit(0);
}
