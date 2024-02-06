import { Settings } from '../settings';
import { Location, isLocationRenewable, locationData, makeLocation, makePlayerLocations } from './locations';
import { Pathfinder, PathfinderState } from './pathfind';
import { World } from './world';
import { Random, sample } from '../random';
import { Analysis } from './analysis';
import { Monitor } from '../monitor';
import { ItemPlacement } from './solve';
import { ItemHelpers, PlayerItems } from '../items';
import { ItemProperties } from './item-properties';

type ZigZagState = {
  allowed: Set<Location>;
  forbidden: Set<Location>;
}

export class LogicPassAnalysisFoolish {
  private pathfinder: Pathfinder;
  private conditionallyRequiredLocations: Set<string>;
  private zigZagCount: number = 0;

  constructor(
    private state: {
      monitor: Monitor;
      random: Random;
      worlds: World[];
      settings: Settings;
      items: ItemPlacement;
      analysis: Analysis;
      startingItems: PlayerItems;
      itemProperties: ItemProperties;
    }
  ) {
    this.pathfinder = new Pathfinder(this.state.worlds, this.state.settings, this.state.startingItems);
    this.conditionallyRequiredLocations = new Set();
  }

  private progress() {
    this.zigZagCount++;
    this.state.monitor.setProgress(this.zigZagCount, this.zigZagCount + 30);
  }

  private markAsSometimesRequired(loc: Location) {
    if (!this.conditionallyRequiredLocations.has(loc)) {
      const item = this.state.items.get(loc)!;
      this.state.monitor.debug(`Foolish Analysis - Sometimes Required: ${loc} (${item.item.id}@${item.player})`);
      this.conditionallyRequiredLocations.add(loc);
    }
  }

  private monteCarloZigZagDown(zz: ZigZagState) {
    const locations = new Set(zz.allowed);
    let allowed = new Set(zz.allowed);
    const forbidden = new Set(zz.forbidden);
    let lastBanished: Location | null = null;

    for (;;) {
      const locs = Array.from(locations);
      if (locs.length === 0) {
        break;
      }
      const loc = sample(this.state.random, locs);
      allowed.delete(loc);
      locations.delete(loc);
      forbidden.add(loc);
      const pathfinderState = this.pathfinder.run(null, { recursive: true, items: this.state.items, forbiddenLocations: forbidden, stopAtGoal: true });
      if (!pathfinderState.goal) {
        if (!this.conditionallyRequiredLocations.has(loc)) {
          this.markAsSometimesRequired(loc);
          lastBanished = loc;
        }
        allowed.add(loc);
        forbidden.delete(loc);
      }
    }

    if (!lastBanished) return null;

    /* Re-forbid the last banished loc and merge allowed */
    forbidden.add(lastBanished);
    allowed.delete(lastBanished);

    return { allowed, forbidden };
  }

  private monteCarloZigZagUp(zz: ZigZagState) {
    const locations = new Set(zz.forbidden);
    let forbidden = new Set(zz.forbidden);
    const allowed = new Set(zz.allowed);
    let lastAdded: Location | null = null;
    let pathfinderState: PathfinderState | null = null;

    for (;;) {
      const locs = Array.from(locations);
      if (locs.length === 0) {
        break;
      }
      const loc = sample(this.state.random, locs);
      locations.delete(loc);
      forbidden.delete(loc);
      allowed.add(loc);
      pathfinderState = this.pathfinder.run(pathfinderState, { inPlace: true, recursive: true, items: this.state.items, forbiddenLocations: forbidden, stopAtGoal: true, includeForbiddenReachable: true });
      if (pathfinderState.goal) {
        if (!this.conditionallyRequiredLocations.has(loc)) {
          this.markAsSometimesRequired(loc);
          lastAdded = loc;
        }
        allowed.delete(loc);
        forbidden.add(loc);
        pathfinderState = null;
      }
    }

    if (!lastAdded) return null;

    /* Re-allow the last added loc and merge forbidden */
    allowed.add(lastAdded);
    forbidden.delete(lastAdded);

    return { allowed, forbidden };
  }

  private monteCarloZigZag(locations: Set<Location>) {
    const allowed = new Set(locations);
    const forbidden = new Set<Location>();
    let zz: ZigZagState = { allowed, forbidden };
    let zzStack: ZigZagState[] = [];
    let result = false;
    zzStack.push(zz);

    for (;;) {
      const downStates = zzStack;
      zzStack = [];
      for (const zz of downStates) {
        for (;;) {
          const step = this.monteCarloZigZagDown(zz);
          this.progress();
          if (step) {
            result = true;
            zzStack.push(step);
          } else {
            break;
          }
        }
      }

      if (zzStack.length === 0) {
        break;
      }

      const upStates = zzStack;
      zzStack = [];
      for (const zz of upStates) {
        for (;;) {
          const step = this.monteCarloZigZagUp(zz);
          this.progress();
          if (step) {
            zzStack.push(step);
          } else {
            break;
          }
        }
      }

      if (zzStack.length === 0) {
        break;
      }
    }

    return result;
  }

  private uselessLocs() {
    const allLocs = this.state.worlds.map((x, i) => [...x.locations].map(y => makeLocation(y, i))).flat();
    const locs = new Set<Location>();
    for (const loc of allLocs) {
      if (this.state.analysis.unreachable.has(loc)) continue;
      if (this.state.analysis.required.has(loc)) continue;
      if (this.conditionallyRequiredLocations.has(loc)) continue;
      if (!this.state.analysis.useless.has(loc)) {
        this.state.monitor.debug("Foolish Analysis - ZigZag Foolish: " + loc);
      }
      locs.add(loc);
    }

    return locs;
  }

  run() {
    if (!this.state.settings.probabilisticFoolish || this.state.settings.logic === 'none') {
      return {};
    }

    this.state.monitor.log("Logic: Probabilistic Foolish Analysis");

    /* Mark playthrough locs as conditionally required */
    for (const sphere of this.state.analysis.spheres) {
      for (const loc of sphere) {
        if (!this.state.analysis.required.has(loc)) {
          this.conditionallyRequiredLocations.add(loc);
        }
      }
    }

    /* Get all candidates */
    const allLocs = this.state.worlds.map((x, i) => [...x.locations].map(y => makeLocation(y, i))).flat();
    const locsSet = new Set<Location>();
    for (const loc of allLocs) {
      if (this.state.analysis.required.has(loc)) continue;
      if (this.state.analysis.unreachable.has(loc)) continue;
      if (this.state.analysis.useless.has(loc)) continue;
      const item = this.state.items.get(loc)!;
      const locD = locationData(loc);
      if (ItemHelpers.isItemConsumable(item.item) && !isLocationRenewable(this.state.worlds[locD.world as number], loc) && !this.state.itemProperties.license.has(item.item)) continue;
      locsSet.add(loc);
    }

    /* Prune */
    let failures = 0;
    for (;;) {
      if (this.monteCarloZigZag(locsSet)) {
        failures = 0;
      } else {
        failures++;
      }
      if (failures >= 3) {
        /* With the new system, a single pass seems to always gather all the items */
        /* But just in case, we run 2 more */
        break;
      }
    }

    const useless = this.uselessLocs();

    return { analysis: { ...this.state.analysis, useless } };
  }
}
