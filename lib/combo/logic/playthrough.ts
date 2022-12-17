import { Random, shuffle } from '../random';
import { addItem } from './items';
import { pathfind } from './pathfind';
import { ItemPlacement, ITEMS_DUNGEON_REWARDS, ITEMS_REQUIRED } from './solve';
import { Items } from './state';
import { World } from './world';

const isItemImportant = (item: string) => (ITEMS_DUNGEON_REWARDS.has(item) || ITEMS_REQUIRED.has(item) || /_BOSS_KEY_/.test(item)) && !(/GS_TOKEN$/.test(item) || /MM_HEART_(PIECE|CONTAINER)$/.test(item));

export const findSpheres = (world: World, placement: ItemPlacement, restrict?: Set<string>, forbid?: Set<string>) => {
  const locations = new Set<string>();
  const items: Items = {};
  const spheres: string[][] = [];
  let reachable = pathfind(world, items);

  for (;;) {
    reachable = pathfind(world, items, reachable);
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

const findSpheresShuffled = (random: Random, world: World, placement: ItemPlacement) => {
  const locations = new Set<string>();
  const spheres = findSpheres(world, placement)!;
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

const findMinimalSpheres = (random: Random, world: World, placement: ItemPlacement) => {
  const shuffled = findSpheresShuffled(random, world, placement);
  const { locations } = shuffled;
  let { spheres } = shuffled;
  let woth = new Set(locations);
  for (const loc of locations) {
    const wothMaybe = new Set(woth);
    wothMaybe.delete(loc);
    const spheresMaybe = findSpheres(world, placement, wothMaybe);
    if (spheresMaybe !== null) {
      woth = wothMaybe;
      spheres = spheresMaybe;
    }
  }
  return spheres;
}

export const playthrough = (random: Random, world: World, placement: ItemPlacement) => {
  const spheres = findMinimalSpheres(random, world, placement);
  return spheres.map(sphere => sphere.filter(item => isItemImportant(placement[item]))).filter(sphere => sphere.length !== 0);
}
