import { Game, Settings } from '@ootmm/core';
import { ENTRANCES } from '@ootmm/data';

import { LogicResult } from '../logic';
import { World } from '../logic/world';
import { padBuffer16, toU32Buffer } from '../util';

function entranceId(srcGame: Game, dstGame: Game, name: string) {
  const entr = ENTRANCES[name as keyof typeof ENTRANCES];
  if (entr === undefined) {
    throw new Error(`Unknown entrance ${name}`);
  }
  let data = entr.id;
  if (srcGame !== dstGame) {
    data = (data | 0x80000000) >>> 0;
  }
  return data;
}

type RandomizerPatcherEntrancesContext = {
  game: Game;
  worldId: number;
  logic: LogicResult;
  settings: Settings;
};

export class RandomizerPatcherEntrances {
  private ctx: RandomizerPatcherEntrancesContext;
  private world: World;

  constructor(ctx: RandomizerPatcherEntrancesContext) {
    this.ctx = ctx;
    this.world = ctx.logic.worlds[ctx.worldId];
  }

  public static run(ctx: RandomizerPatcherEntrancesContext) {
    return (new RandomizerPatcherEntrances(ctx)).execute();
  }

  private execute(): Uint8Array {
    const data: number[] = [];
    for (const [src, dst] of this.world.entranceOverrides) {
      const srcEntrance = ENTRANCES[src as keyof typeof ENTRANCES];
      const dstEntrance = ENTRANCES[dst as keyof typeof ENTRANCES];
      if (srcEntrance.game !== this.ctx.game)
        continue;
      const srcId = entranceId(srcEntrance.game, srcEntrance.game as Game, src);
      const dstId = entranceId(srcEntrance.game, dstEntrance.game as Game, dst);
      data.push(srcId, dstId);
    }
    data.push(0xffffffff);
    data.push(0xffffffff);
    return padBuffer16(toU32Buffer(data));
  }
}
