import { Random, shuffle } from '../random';
import { Settings } from '../settings';
import { addItem, isBossKey, isDungeonReward, isToken, isItemMajor, isSmallKey, Items } from './items';
import { ItemPlacement } from './solve';
import { World } from './world';
import { Pathfinder, PathfinderState } from './pathfind';

const isItemImportant = (settings: Settings, item: string) => {
  if (settings.smallKeyShuffle === 'anywhere' && isSmallKey(item))
    return true;
  if (isToken(item))
    return false;
  if (isDungeonReward(item))
    return true;
  if (isItemMajor(item))
    return true;
  if (isBossKey(item))
    return true;
  return false;
}

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

export class LogicPassPlaythrough {
  constructor(
    private readonly state: {
      settings: Settings,
      random: Random,
      world: World,
      items: ItemPlacement,
    },
  ){
  }

  run() {
    let spheres: string[][] = [];

    if (!this.state.settings.noLogic) {
      const rawSpheres = findMinimalSpheres(this.state.settings, this.state.random, this.state.world, this.state.items);
      spheres = rawSpheres.map(sphere => sphere.filter(item => isItemImportant(this.state.settings, this.state.items[item]))).filter(sphere => sphere.length !== 0);
    }

    return { spheres };
  }
}
