import { Random, shuffle } from '../random';
import { Settings } from '../settings';
import { addItem, isBossKey, isDungeonReward, isToken, isItemMajor, isSmallKey } from './items';
import { pathfind } from './pathfind';
import { ItemPlacement } from './solve';
import { Items } from './state';
import { World } from './world';

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

export const findSpheres = (settings: Settings, world: World, placement: ItemPlacement, restrict?: Set<string>, forbid?: Set<string>) => {
  const locations = new Set<string>();
  const items: Items = { ...settings.startingItems };
  const spheres: string[][] = [];
  let reachable = pathfind(world, items, false);

  for (;;) {
    reachable = pathfind(world, items, false, reachable);
    const sphere = [];
    for (const loc of reachable.locations) {
      if (locations.has(loc)) {
        continue;
      }
      if (restrict && !restrict.has(loc)) {
        continue;
      }
      if (forbid && forbid.has(loc)) {
        continue;
      }
      const item = placement[loc];
      locations.add(loc);
      addItem(items, item);
      sphere.push(loc);
    }
    if (sphere.length !== 0) {
      spheres.push(sphere);
    }
    if (reachable.events.has('OOT_GANON') && reachable.events.has('MM_MAJORA')) {
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

export const playthrough = (settings: Settings, random: Random, world: World, placement: ItemPlacement) => {
  const spheres = findMinimalSpheres(settings, random, world, placement);
  return spheres.map(sphere => sphere.filter(item => isItemImportant(settings, placement[item]))).filter(sphere => sphere.length !== 0);
}
