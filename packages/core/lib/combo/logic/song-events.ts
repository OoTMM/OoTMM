import { Random, randomInt } from '../random';
import { Settings } from '../settings';
import { World } from './world';

export class LogicPassSongEvents {
  constructor(
    private state: {
      settings: Settings,
      worlds: World[],
      random: Random,
    }
  ) {
  }

  private shuffleSongEvents(world: World) {
    for (let i = 0; i < world.songEvents.length; ++i) {
      const v = randomInt(this.state.random, 6);
      world.songEvents[i] = v;
    }
  }

  run() {
    if (this.state.settings.songEventsShuffle) {
      for (const world of this.state.worlds) {
        this.shuffleSongEvents(world);
      }
    }

    return {};
  }
}
