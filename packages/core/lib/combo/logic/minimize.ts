import { ItemHelpers, Items, PlayerItems, makePlayerItem } from '../items';
import { Settings } from '../settings';
import { Pathfinder } from './pathfind';
import { Location, isLocationFullyShuffled, locationData, makeLocation } from './locations';
import { World } from './world';
import { ItemPlacement } from './solve';
import { Random, shuffle } from '../random';
import { Monitor } from '../monitor';

export class LogicPassMinimize {
  private pathfinder!: Pathfinder;
  private locations!: Location[];

  constructor(
    private readonly state: {
      settings: Settings;
      fixedLocations: Set<Location>;
      startingItems: PlayerItems;
      worlds: World[];
      items: ItemPlacement;
      random: Random;
      monitor: Monitor;
    }
  ) {
  }

  run() {
    if (this.state.settings.itemPool !== 'barren' || this.state.settings.logic === 'none') {
      return {};
    }

    this.state.monitor.log('Logic - Minimize');

    this.pathfinder = new Pathfinder(this.state.worlds, this.state.settings, this.state.startingItems);
    this.locations = this.state.worlds.map((x, i) => [...x.locations].map(l => makeLocation(l, i))).flat();

    /* Extract locs to test */
    let locsToTest: Location[] = [];
    for (const loc of this.locations) {
      if (isLocationFullyShuffled(this.state.settings, this.state.fixedLocations, this.state.items, loc, { songs: true, noPlando: true })) {
        if (!ItemHelpers.isTriforcePiece(this.state.items.get(loc)!.item)) {
          locsToTest.push(loc);
        }
      }
    }
    locsToTest = shuffle(this.state.random, locsToTest);

    const countTotal = locsToTest.length;
    let countCurrent = 0;
    while (locsToTest.length > 0) {
      this.state.monitor.setProgress(countCurrent++, countTotal);
      const l = locsToTest.pop()!;
      const item = this.state.items.get(l)!;
      const locD = locationData(l);
      this.state.items.set(l, makePlayerItem(Items.NOTHING, locD.world!));

      const pathfinderState = this.pathfinder.run(null, {
        recursive: true,
        stopAtGoal: this.state.settings.logic === 'beatable',
        items: this.state.items,
      });
      let result = pathfinderState.goal;
      if (result && this.state.settings.logic === 'allLocations') {
        if (pathfinderState.locations.size < this.locations.length) {
          result = false;
        }
      }

      if (!result) {
        this.state.items.set(l, item);
      }
    }

    return {};
  }
}
