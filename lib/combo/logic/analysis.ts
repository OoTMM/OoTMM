import { Random, shuffle } from '../random';
import { Settings } from '../settings';
import { isItemImportant } from './items';
import { ItemPlacement } from './solve';
import { World } from './world';
import { Pathfinder, PathfinderState } from './pathfind';
import { Monitor } from '../monitor';

export const findSpheres = (settings: Settings, world: World, items: ItemPlacement, restrict?: Set<string>, forbid?: Set<string>) => {
  const spheres: string[][] = [];
  const pathfinder = new Pathfinder(world, settings);
  let pathfinderState: PathfinderState | null = null;

  for (;;) {
    pathfinderState = pathfinder.run(pathfinderState, { items, restrictedLocations: restrict, forbiddenLocations: forbid });
    const sphere = Array.from(pathfinderState.newLocations);
    if (sphere.length !== 0) {
      spheres.push(sphere);
    }
    if (pathfinderState.events.has('OOT_GANON') && pathfinderState.events.has('MM_MAJORA')) {
      break;
    }
    if (sphere.length === 0) {
      return null;
    }
  }

  return spheres;
}

const findSpheresShuffled = (settings: Settings, random: Random, world: World, placement: ItemPlacement) => {
  const locations = new Set<string>();
  const spheres = findSpheres(settings, world, placement)!;
  const newSpheres = Array.from(spheres);
  newSpheres.map(s => shuffle(random, s));
  newSpheres.reverse();
  for (const sphere of newSpheres) {
    for (const loc of sphere) {
      locations.add(loc);
    }
  }
  return { locations, spheres };
}

const findMinimalSpheres = (settings: Settings, random: Random, world: World, placement: ItemPlacement) => {
  const shuffled = findSpheresShuffled(settings, random, world, placement);
  const { locations } = shuffled;
  let { spheres } = shuffled;
  let woth = new Set(locations);
  for (const loc of locations) {
    const wothMaybe = new Set(woth);
    wothMaybe.delete(loc);
    const spheresMaybe = findSpheres(settings, world, placement, wothMaybe);
    if (spheresMaybe !== null) {
      woth = wothMaybe;
      spheres = spheresMaybe;
    }
  }
  return spheres;
}

export class LogicPassAnalysis {
  private pathfinder: Pathfinder;
  private spheres: string[][] = [];
  private requiredLocs = new Set<string>();
  private uselessLocs = new Set<string>();

  constructor(
    private readonly state: {
      settings: Settings,
      random: Random,
      world: World,
      items: ItemPlacement,
      monitor: Monitor,
    },
  ){
    this.pathfinder = new Pathfinder(this.state.world, this.state.settings);
  }

  private makeSpheresRaw() {
    const spheres: string[][] = [];
    let pathfinderState: PathfinderState | null = null;

    do {
      pathfinderState = this.pathfinder.run(pathfinderState, { items: this.state.items, stopAtGoal: true });
      const sphere = Array.from(pathfinderState.newLocations).filter(x => isItemImportant(this.state.items[x]));
      if (sphere.length !== 0) {
        spheres.push(shuffle(this.state.random, sphere));
      }
    } while (!pathfinderState.goal);

    return spheres;
  }

  private makeSpheres() {
    const spheres: string[][] = [];
    const rawSpheres = this.makeSpheresRaw().reverse();
    const spheresLocs = new Set(rawSpheres.flat());

    for (const sphere of rawSpheres) {
      const filteredSphere: string[] = [];
      for (const loc of sphere) {
        spheresLocs.delete(loc);
        const pathfinderState = this.pathfinder.run(null, { items: this.state.items, restrictedLocations: spheresLocs, recursive: true, stopAtGoal: true });
        if (!pathfinderState.goal) {
          spheresLocs.add(loc);
          filteredSphere.push(loc);
        }
      }
      if (filteredSphere.length !== 0) {
        spheres.push(filteredSphere);
      }
    }

    this.spheres = spheres.reverse();
  }

  run() {
    this.state.monitor.log('Logic: Analysis');
    if (!this.state.settings.noLogic) {
      this.makeSpheres();
    }

    const analysis = {
      spheres: this.spheres,
    }

    return { analysis };
  }
}

export type Analysis = ReturnType<LogicPassAnalysis['run']>['analysis'];
