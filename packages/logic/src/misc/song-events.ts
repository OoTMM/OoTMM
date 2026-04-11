import type { Settings } from '@ootmm/core';
import type { World } from '../world';

import { Random, randomInt } from '@ootmm/core';

type LogicPassSongEventsState = {
  settings: Settings,
  worlds: World[],
  random: Random,
};

class LogicPassSongEvents {
  constructor(
    private readonly state: LogicPassSongEventsState,
  ) {
  }

  private shuffleSongEvents(world: World) {
    for (let i = 0; i < world.songEvents.length; ++i) {
      const v = randomInt(this.state.random, 6);
      world.songEvents[i] = v;
    }
  }

  run() {
    if (this.state.settings.songEventsShuffleOot) {
      for (const world of this.state.worlds) {
        this.shuffleSongEvents(world);
      }
    }

    return {};
  }
}

export function logicPassSongEvents(state: LogicPassSongEventsState) {
  return new LogicPassSongEvents(state).run();
}
