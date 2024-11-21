import { Random, shuffle } from '../random';
import { Settings } from '../settings';
import { World } from './world';
import { Pathfinder, PathfinderState } from './pathfind';
import { Monitor } from '../monitor';
import { isLocationRenewable, Location, makeLocation, locationData } from './locations';
import { ItemPlacement } from './solve';
import { ItemHelpers, Items, PlayerItem, PlayerItems } from '../items';
import { ItemProperties } from './item-properties';

export class LogicPassAnalysis {
  private pathfinder: Pathfinder;
  private spheres: Location[][] = [];
  private requiredLocs = new Set<Location>();
  private requiredlocsByItem: Map<PlayerItem, Set<Location>> = new Map();
  private uselessLocs = new Set<Location>();
  private unreachableLocs = new Set<Location>();
  private locations: Location[];
  private paths: {
    triforcePower: Set<Location>;
    triforceCourage: Set<Location>;
    triforceWisdom: Set<Location>;
  };

  constructor(
    private readonly state: {
      settings: Settings;
      random: Random;
      worlds: World[];
      items: ItemPlacement;
      monitor: Monitor;
      startingItems: PlayerItems;
      itemProperties: ItemProperties;
    },
  ){
    this.pathfinder = new Pathfinder(this.state.worlds, this.state.settings, this.state.startingItems);
    this.locations = this.state.worlds.map((x, i) => [...x.locations].map(l => makeLocation(l, i))).flat();
    this.paths = { triforcePower: new Set, triforceCourage: new Set, triforceWisdom: new Set };
  }

  private makeSpheresRaw(restrictedLocations?: Set<Location>) {
    const spheres: Location[][] = [];
    let pathfinderState: PathfinderState | null = null;
    let count = 0;

    do {
      this.progress(count++, 10);
      pathfinderState = this.pathfinder.run(pathfinderState, { inPlace: true, items: this.state.items, stopAtGoal: true, restrictedLocations });
      const sphere = Array.from(pathfinderState.newLocations).filter(x => this.state.itemProperties.important.has(this.state.items.get(x)!.item));
      if (sphere.length !== 0) {
        spheres.push(shuffle(this.state.random, sphere));
      }
    } while (!pathfinderState.goal);

    return spheres;
  }

  private makeSpheres() {
    this.state.monitor.log('Analysis - Spheres (Raw)');

    const locs = this.makeSpheresRaw().reverse().flat();
    const spheresLocs = new Set(locs);

    this.state.monitor.log('Analysis - Spheres (Playthrough)');
    let count = 0;
    for (const loc of locs) {
      this.state.monitor.setProgress(count++, locs.length);
      spheresLocs.delete(loc);
      const pathfinderState = this.pathfinder.run(null, { items: this.state.items, restrictedLocations: spheresLocs, recursive: true, stopAtGoal: true });
      if (!pathfinderState.goal) {
        spheresLocs.add(loc);
      }
    }

    /* Recompute spheres to ensure correct order */
    this.state.monitor.log('Analysis - Spheres (Reorder)');
    this.spheres = this.makeSpheresRaw(spheresLocs);
  }

  private makePath(name: string, pred: (x: PathfinderState) => boolean) {
    const path = new Set<Location>();
    this.state.monitor.log(`Analysis - ${name}`);
    let count = 0;
    const locations = new Set(this.spheres.flat());
    for (const loc of locations) {
      this.state.monitor.setProgress(count++, locations.size);
      const pathfinderState = this.pathfinder.run(null, { items: this.state.items, forbiddenLocations: new Set([loc]), recursive: true, stopAtGoal: true });
      if (!pred(pathfinderState)) {
        path.add(loc);
      }
    }
    return path;
  }

  private makeRequiredLocs() {
    this.state.monitor.log('Analysis - WotH');
    let count = 0;
    const locations = new Set(this.spheres.flat());
    for (const loc of locations) {
      this.state.monitor.setProgress(count++, locations.size);
      const pathfinderState = this.pathfinder.run(null, { items: this.state.items, forbiddenLocations: new Set([loc]), recursive: true, stopAtGoal: true });
      if (!pathfinderState.goal) {
        this.requiredLocs.add(loc);
        const item = this.state.items.get(loc)!;
        if (!this.requiredlocsByItem.has(item)) {
          this.requiredlocsByItem.set(item, new Set());
        }
        this.requiredlocsByItem.get(item)!.add(loc);
      }
    }
  }

  private makePaths() {
    if (this.state.settings.goal === 'triforce3') {
      const locsPower = Array.from(this.state.items.entries()).filter(([_, item]) => item.item === Items.SHARED_TRIFORCE_POWER).map(([loc, _]) => loc);
      const locsCourage = Array.from(this.state.items.entries()).filter(([_, item]) => item.item === Items.SHARED_TRIFORCE_COURAGE).map(([loc, _]) => loc);
      const locsWisdom = Array.from(this.state.items.entries()).filter(([_, item]) => item.item === Items.SHARED_TRIFORCE_WISDOM).map(([loc, _]) => loc);

      this.paths.triforcePower = this.makePath('Path of Power', x => locsPower.every(l => x.locations.has(l)));
      this.paths.triforceCourage = this.makePath('Path of Courage', x => locsCourage.every(l => x.locations.has(l)));
      this.paths.triforceWisdom = this.makePath('Path of Wisdom', x => locsWisdom.every(l => x.locations.has(l)));
    }
  }

  private isLocUselessNonRenewable(loc: Location) {
    const pi = this.state.items.get(loc)!;
    const locD = locationData(loc);
    return (ItemHelpers.isItemConsumable(pi.item) && !isLocationRenewable(this.state.worlds[locD.world as number], loc) && !this.state.itemProperties.license.has(pi.item));
  }

  private makeUselessLocs() {
    for (const loc of this.locations) {
      if (this.requiredLocs.has(loc) || this.uselessLocs.has(loc) || this.unreachableLocs.has(loc)) {
        continue;
      }
      const item = this.state.items.get(loc)!;
      if (!this.state.itemProperties.important.has(item.item) || this.isLocUselessNonRenewable(loc)) {
        this.uselessLocs.add(loc);
      }
    }
  }

  private makeUnreachable() {
    const pathfinderState = this.pathfinder.run(null, { items: this.state.items, recursive: true });
    for (const loc of this.locations) {
      const item = this.state.items.get(loc)!;
      if (this.state.itemProperties.important.has(item.item) && !pathfinderState.locations.has(loc)) {
        this.state.monitor.debug(`Analysis - makeUnreachable: ${item.item.id}@${item.player}`);
        this.unreachableLocs.add(loc);
      }
    }
  }

  private progress(x: number, slope: number) {
    this.state.monitor.setProgress(x, x + slope);
  }

  run() {
    this.state.monitor.log('Logic: Analysis');
    if (this.state.settings.logic !== 'none') {
      this.makeSpheres();
      this.makeRequiredLocs();
      this.makePaths();
    }
    if (this.state.settings.logic === 'beatable') {
      this.makeUnreachable();
    }
    this.makeUselessLocs();

    const analysis = {
      spheres: this.spheres,
      required: this.requiredLocs,
      useless: this.uselessLocs,
      unreachable: this.unreachableLocs,
      paths: this.paths,
    }

    return { analysis };
  }
}

export type Analysis = ReturnType<LogicPassAnalysis['run']>['analysis'];
