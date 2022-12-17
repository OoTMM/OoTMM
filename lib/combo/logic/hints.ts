import { ItemPlacement, ORDERED_DUNGEON_REWARDS } from './solve';
import { World } from './world';
import { findSpheres } from './playthrough';
import { Random, sample } from '../random';
import { pathfind } from './pathfind';
import { Items } from './state';
import { addItem } from './items';

export type HintGossipHero = {
  type: 'hero',
  location: string,
};

export type HintGossip = HintGossipHero;

export type Hints = {
  dungeonRewards: string[];
  lightArrow: string;
  oathToOrder: string;
  gossip: {[k: string]: HintGossip};
};

class HintsSolver {
  private hintedLocations = new Set<string>();
  private gossip: {[k: string]: HintGossip} = {};

  constructor(
    private random: Random,
    private world: World,
    private items: ItemPlacement,
    private spheres: string[][],
  ){
  }

  private findItem(item: string) {
    for (const loc in this.items) {
      if (this.items[loc] === item) {
        return loc;
      }
    }
    return null;
  }

  private wayOfTheHero(world: World, ItemPlacement: ItemPlacement, spheres: string[][]) {
    const woth = new Set<string>();
    for (const sphere of spheres) {
      for (const loc of sphere) {
        if (findSpheres(world, ItemPlacement, undefined, new Set([loc])) === null) {
          woth.add(loc);
        }
      }
    }
    return woth;
  }

  private findValidGossip(loc: string) {
    const items: Items = {};
    let reachable = pathfind(this.world, items, true);
    const locations = new Set<string>();
    for (;;) {
      let change = false;
      reachable = pathfind(this.world, items, true, reachable);
      for (const l of reachable.locations) {
        if (locations.has(l) || l == loc) {
          continue;
        }
        addItem(items, this.items[l]);
        locations.add(l);
        change = true;
      }
      if (!change) {
        break;
      }
    }
    const gossips = Array.from(reachable.gossip).filter(x => !this.gossip[x]);
    if (gossips.length === 0) {
      return null;
    }
    return sample(this.random, gossips);
  }

  private placeGossipHero(woth: Set<string>) {
    const locs = Array.from(woth).filter(loc => !this.hintedLocations.has(loc));
    if (locs.length === 0) {
      return false;
    }
    const loc = sample(this.random, locs);
    const gossip = this.findValidGossip(loc);
    if (gossip === null) {
      return false;
    }
    this.gossip[gossip] = { type: 'hero', location: loc };
    this.hintedLocations.add(loc);
    return true;
  }

  private placeGossips() {
    /* TODO: refactor this */
    this.hintedLocations.add(this.findItem('OOT_ARROW_LIGHT')!);
    this.hintedLocations.add(this.findItem('MM_SONG_ORDER')!);

    const woth = this.wayOfTheHero(this.world, this.items, this.spheres);
    this.placeGossipHero(woth);

    console.log(this.gossip);
  }

  run() {
    const dungeonRewards = ORDERED_DUNGEON_REWARDS.map(item => this.findItem(item)).map(loc => this.world.regions[loc!]);
    const lightArrow = this.world.regions[this.findItem('OOT_ARROW_LIGHT')!];
    const oathToOrder = this.world.regions[this.findItem('MM_SONG_ORDER')!];
    this.placeGossips();
    return { dungeonRewards, lightArrow, oathToOrder, gossip: this.gossip };
  }
}

export const hints = (random: Random, world: World, items: ItemPlacement, spheres: string[][]): Hints => {
  const solver = new HintsSolver(random, world, items, spheres);
  return solver.run();
};
