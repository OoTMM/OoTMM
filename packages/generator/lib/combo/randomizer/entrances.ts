import type { LogicResult, LogicResultWorld } from '@ootmm/logic';

import { ENTRANCES } from '@ootmm/data';
import { padBuffer16, toU32Buffer } from '../util';

function entranceId(name: string) {
  const entr = ENTRANCES[name as keyof typeof ENTRANCES];
  if (entr === undefined) {
    throw new Error(`Unknown entrance ${name}`);
  }
  let data = entr.id;
  if (entr.game === 'mm') {
    data = (data | 0x80000000) >>> 0;
  }
  return data;
}

type RandomizerPatcherEntrancesContext = {
  worldId: number;
  logic: LogicResult;
};

export class RandomizerPatcherEntrances {
  private world: LogicResultWorld;

  constructor(ctx: RandomizerPatcherEntrancesContext) {
    this.world = ctx.logic.worlds[ctx.worldId];
  }

  public static run(ctx: RandomizerPatcherEntrancesContext) {
    return (new RandomizerPatcherEntrances(ctx)).execute();
  }

  private execute(): Uint8Array {
    const data: number[][] = [];
    for (const [src, dst] of this.world.entranceOverrides) {
      const srcId = entranceId(src);
      const dstId = entranceId(dst);
      data.push([srcId, dstId]);
    }
    /* Sort ascending */
    data.sort((a, b) => a[0] - b[0]);
    data.push([0xffffffff, 0xffffffff]);
    return padBuffer16(toU32Buffer(data.flat()));
  }
}
