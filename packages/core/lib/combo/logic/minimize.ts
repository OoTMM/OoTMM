import { ItemGroups, ItemHelpers, Items, PlayerItem, PlayerItems, makePlayerItem } from '../items';
import { Settings } from '../settings';
import { Pathfinder } from './pathfind';
import { Location, isLocationFullyShuffled, locationData, makeLocation } from './locations';
import { World } from './world';
import { ItemPlacement } from './solve';
import { Random, randomFloat, sample, shuffle } from '../random';
import { Monitor } from '../monitor';
import { JUNK } from '../items/groups';
import { countMapAdd, countMapArray } from '../util';
import { TRAP_AMOUNTS } from './traps';

export class LogicPassMinimize {
  private pathfinder!: Pathfinder;
  private locations!: Location[];
  private traps: PlayerItem[];
  private trapDensity: number;

  constructor(
    private readonly state: {
      settings: Settings;
      fixedLocations: Set<Location>;
      startingItems: PlayerItems;
      worlds: World[];
      items: ItemPlacement;
      random: Random;
      monitor: Monitor;
      plandoLocations: Map<Location, PlayerItem>;
      pool: PlayerItems;
    }
  ) {
    const trapPool = new Map();
    for (const [pi, count] of this.state.pool) {
      if (ItemGroups.TRAPS.has(pi.item)) {
        trapPool.set(pi, count);
      }
    }
    if (trapPool.size === 0) {
      this.trapDensity = -1;
      this.traps = [];
    } else {
      const trapAmount = TRAP_AMOUNTS[this.state.settings.trapsQuantity];
      this.trapDensity = (trapAmount / (trapAmount + 100));
      this.traps = countMapArray(trapPool);
    }
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
      if (isLocationFullyShuffled(this.state.settings, this.state.fixedLocations, this.state.items, this.state.plandoLocations, loc, { songs: true, noPlando: true })) {
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
      } else {
        /* Roll for trap */
        const trapRoll = randomFloat(this.state.random);
        if (trapRoll < this.trapDensity) {
          const trap = sample(this.state.random, this.traps);
          this.state.items.set(l, trap);
        }
      }
    }

    return {};
  }
}
