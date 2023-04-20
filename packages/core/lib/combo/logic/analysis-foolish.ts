import { Settings } from '../settings';
import { isItemConsumable } from './items';
import { isLocationRenewable } from './locations';
import { Pathfinder, PathfinderState } from './pathfind';
import { ItemPlacement } from './solve';
import { World } from './world';
import { Random, sample } from '../random';
import { Analysis } from './analysis';
import { Monitor } from '../monitor';

type ZigZagState = {
  allowed: Set<string>;
  forbidden: Set<string>;
}

export class LogicPassAnalysisFoolish {
  private pathfinder: Pathfinder;
  private conditionallyRequiredLocations: Set<string>;

  constructor(
    private state: {
      monitor: Monitor;
      random: Random;
      world: World;
      settings: Settings;
      items: ItemPlacement;
      analysis: Analysis;
    }
  ) {
    this.pathfinder = new Pathfinder(this.state.world, this.state.settings);
    this.conditionallyRequiredLocations = new Set();
  }

  private markAsSometimesRequired(loc: string) {
    if (!this.conditionallyRequiredLocations.has(loc)) {
      this.state.monitor.debug("Foolish Analysis - Sometimes Required: " + loc + "(" + this.state.items[loc] + ")");
      this.conditionallyRequiredLocations.add(loc);
    }
  }

  private monteCarloZigZagDown(zz: ZigZagState) {
    const locations = new Set(zz.allowed);
    let allowed = new Set(zz.allowed);
    const forbidden = new Set(zz.forbidden);
    let lastBanished: string | null = null;

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
    let lastAdded: string | null = null;
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
      const newPathfinderState = this.pathfinder.run(pathfinderState, { recursive: true, items: this.state.items, forbiddenLocations: forbidden, stopAtGoal: true, includeForbiddenReachable: true });
      if (newPathfinderState.goal) {
        if (!this.conditionallyRequiredLocations.has(loc)) {
          this.markAsSometimesRequired(loc);
          lastAdded = loc;
        }
        allowed.delete(loc);
        forbidden.add(loc);
      } else {
        pathfinderState = newPathfinderState;
      }
    }

    if (!lastAdded) return null;

    /* Re-allow the last added loc and merge forbidden */
    allowed.add(lastAdded);
    forbidden.delete(lastAdded);

    return { allowed, forbidden };
  }

  private monteCarloZigZag(locations: Set<string>) {
    const allowed = new Set(locations);
    const forbidden = new Set<string>();
    let zz: ZigZagState = { allowed, forbidden };

    const step = this.monteCarloZigZagDown(zz);
    if (!step) return false;
    zz = step;

    for (;;) {
      const stepUp = this.monteCarloZigZagUp(zz);
      if (!stepUp) break;
      zz = stepUp;

      const stepDown = this.monteCarloZigZagDown(zz);
      if (!stepDown) break;
      zz = stepDown;
    }

    return true;
  }

  private uselessLocs() {
    const locs = new Set<string>();
    for (const loc in this.state.world.checks) {
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

    this.state.monitor.log("Logic: Probabilistic Foolish Analysis (can take up to a few minutes...)");

    /* Mark playthrough locs as conditionally required */
    let atLeastOneConditionallyRequired = false;
    for (const sphere of this.state.analysis.spheres) {
      for (const loc of sphere) {
        if (!this.state.analysis.required.has(loc)) {
          this.conditionallyRequiredLocations.add(loc);
          atLeastOneConditionallyRequired = true;
        }
      }
    }
    if (!atLeastOneConditionallyRequired) {
      return {};
    }

    /* Get all candidates */
    const locsSet = new Set<string>();
    for (const loc in this.state.world.checks) {
      if (this.state.analysis.required.has(loc)) continue;
      if (this.state.analysis.unreachable.has(loc)) continue;
      if (this.state.analysis.useless.has(loc)) continue;
      const item = this.state.items[loc];
      if (isItemConsumable(item) && !isLocationRenewable(this.state.world, loc)) continue;
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
      if (failures === 10) {
        break;
      }
    }

    const useless = this.uselessLocs();

    return { analysis: { ...this.state.analysis, useless } };
  }
}
