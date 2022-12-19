import { ItemPlacement } from './solve';
import { World } from './world';
import { findSpheres } from './playthrough';
import { Random, sample, shuffle } from '../random';
import { pathfind } from './pathfind';
import { Items } from './state';
import { addItem, DUNGEON_REWARDS_ORDERED, isDungeonItem, isDungeonReward, isItemMajor, isToken, itemsArray } from './items';
import { Settings } from '../settings';
import { CONSTRAINT_NONE, itemConstraint } from './constraints';
import { Game } from '../config';

export type HintGossipHero = {
  type: 'hero',
  region: string,
  location: string;
};

export type HintGossipFoolish = {
  type: 'foolish',
  region: string,
};

export type HintGossipItemExact = {
  type: 'item-exact',
  check: string,
  items: string[],
};

export type HintGossipItemRegion = {
  type: 'item-region',
  region: string,
  item: string;
};

export type HintGossip = { game: Game } & (HintGossipHero | HintGossipFoolish | HintGossipItemExact | HintGossipItemRegion);

export type Hints = {
  dungeonRewards: string[];
  lightArrow: string;
  oathToOrder: string;
  gossip: {[k: string]: HintGossip};
};

const HINTS_ITEMS_ALWAYS = [
  'OOT_FROGS_FINAL',
  'OOT_FISHING',
  'MM_RANCH_DEFENSE',
  'MM_BUTLER_RACE',
  'MM_COUPLE_MASK',
  'MM_DON_GERO_CHOIR',
  'MM_GORON_RACE',
  'MM_GRAVEYARD_NIGHT3',
];

const HINTS_ITEMS_SOMETIMES = [
  'OOT_RAVAGED_VILLAGE',
  'OOT_ZORA_KING',
  'OOT_GANON_FAIRY',
  'OOT_TEMPLE_FIRE_HAMMER',
  'OOT_TEMPLE_FIRE_SCARECROW',
  'OOT_GTG_WATER',
  'OOT_HAUNTED_WASTELAND',
  'OOT_GERUDO_ARCHERY',
  'MM_BANK_3',
  'MM_SOUND_CHECK',
  'MM_BOAT_ARCHERY',
  'MM_OSH_CHEST',
  'MM_PINNACLE_ROCK_HP',
  'MM_FISHERMAN_GAME',
  'MM_SONG_ELEGY',
  'MM_SECRET_SHRINE_HP',
];

class HintsSolver {
  private hintedLocations = new Set<string>();
  private gossip: {[k: string]: HintGossip} = {};

  constructor(
    private random: Random,
    private settings: Settings,
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

  private findItems(item: string) {
    const locs: string[] = [];
    for (const loc in this.items) {
      if (this.items[loc] === item) {
        locs.push(loc);
      }
    }
    return locs;
  }

  private isItemWayOfTheHero(item: string) {
    if (itemConstraint(item, this.settings) !== CONSTRAINT_NONE) {
      return false;
    }
    if (isDungeonItem(item)) {
      return false;
    }
    if (isDungeonReward(item)) {
      return false;
    }
    if (!this.settings.shuffleGerudoCard && item == 'OOT_GERUDO_CARD') {
      return false;
    }
    return true;
  }

  private wayOfTheHero() {
    const woth = new Set<string>();
    for (const sphere of this.spheres) {
      for (const loc of sphere) {
        if (findSpheres(this.world, this.items, undefined, new Set([loc])) === null) {
          woth.add(loc);
        }
      }
    }
    const locs = Array.from(woth).filter(loc => this.isItemWayOfTheHero(this.items[loc]));
    return new Set(locs);
  }

  private findValidGossip(locs: Set<string> | string) {
    if (typeof locs === 'string') {
      locs = new Set([locs]);
    }
    const items: Items = {};
    let reachable = pathfind(this.world, items, true);
    const locations = new Set<string>();
    for (;;) {
      let change = false;
      reachable = pathfind(this.world, items, true, reachable);
      for (const l of reachable.locations) {
        if (locations.has(l) || locs.has(l)) {
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

  private wayOfTheHeroItems(woth: Set<string>) {
    const items: {[k: string]: Set<string> } = {};
    for (const loc of woth) {
      const item = this.items[loc];
      items[item] ||= new Set<string>();
      items[item].add(loc);
    }
    return items;
  }

  private majorItemFoolish(loc: string, item: string, wothItems: {[k: string]: Set<string>}) {
    /* TODO: this is fragile */
    let maximumRequired = -1;
    switch (item) {
    case 'OOT_SWORD':
      maximumRequired = 2;
      break;
    case 'OOT_OCARINA':
    case 'OOT_BOMB_BAG':
    case 'OOT_BOW':
    case 'OOT_SLINGSHOT':
    case 'MM_SWORD':
    case 'MM_BOW':
    case 'MM_BOMB_BAG':
      maximumRequired = 1;
      break;
    }
    if (maximumRequired === -1) {
      return false;
    }
    const woth = wothItems[item];
    if (!woth) {
      return false;
    }
    if (woth.size < maximumRequired) {
      return false;
    }
    if (woth.has(loc)) {
      return false;
    }
    return true;
  }

  private locationFoolish(loc: string, wothItems: {[k: string]: Set<string>}) {
    const item = this.items[loc];
    if (isDungeonItem(item) || isDungeonReward(item) || isToken(item)) {
      return 0;
    }
    if (isItemMajor(item) && !this.majorItemFoolish(loc, item, wothItems)) {
      return -1;
    }
    if (this.hintedLocations.has(loc)) {
      return 0;
    }
    return 1;
  }

  private foolishRegions(wothItems: {[k: string]: Set<string>}) {
    let regions: {[k:string]: number} = {};

    for (const location in this.world.checks) {
      const region = this.world.regions[location];
      regions[region] ||= 0;
      if (regions[region] === -1) {
        continue;
      }
      const value = this.locationFoolish(location, wothItems);
      if (value === -1) {
        regions[region] = -1;
      } else {
        regions[region] += value;
      }
    }

    for (const r in regions) {
      if (regions[r] <= 0) {
        delete regions[r];
      }
    }

    return regions;
  }

  private placeGossipItemExact(checkHint: string) {
    if (checkHint === 'NONE') {
      return false;
    }
    const locations = shuffle(this.random, this.world.checkHints[checkHint]);
    for (const l of locations) {
      if (this.hintedLocations.has(l)) {
        return false;
      }
    }
    const items = locations.map(l => this.items[l]);
    const gossip = this.findValidGossip(new Set(locations));
    if (!gossip) {
      return false;
    }
    this.gossip[gossip] = { game: this.world.gossip[gossip].game, type: 'item-exact', items, check: checkHint };
    for (const l of locations) {
      this.hintedLocations.add(l);
    }
    return true;
  }

  private placeGossipItemExactPool(pool: string[], count?: number) {
    if (count === undefined) {
      count = pool.length;
    }
    let placed = 0;
    pool = shuffle(this.random, pool);
    for (const checkHint of pool) {
      if (placed >= count) {
        break;
      }
      if (this.placeGossipItemExact(checkHint)) {
        placed++;
      }
    }
    return placed;
  }

  private placeGossipFoolish(regions: {[k: string]: number}, count: number) {
    let placed = 0;
    regions = { ...regions };
    while (placed < count) {
      const regionsArray = itemsArray(regions); /* Ugly */
      if (regionsArray.length === 0) {
        break;
      }
      const region = sample(this.random, regionsArray);
      delete regions[region];
      const gossip = sample(this.random, Object.keys(this.world.gossip).filter(x => !this.gossip[x]));
      this.gossip[gossip] = { game: this.world.gossip[gossip].game, type: 'foolish', region: region };
      placed++;
    }
    return placed;
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
    this.gossip[gossip] = { game: this.world.gossip[gossip].game, type: 'hero', region: this.world.regions[loc], location: loc };
    this.hintedLocations.add(loc);
    return true;
  }

  private placeGossipItemRegion(item: string) {
    const locations = this.findItems(item).filter(x => !this.hintedLocations.has(x));
    if (locations.length === 0) {
      return false;
    }
    const location = sample(this.random, locations);
    const hint = this.world.checks[location].hint;
    if (this.placeGossipItemExact(hint)) {
      return true;
    }
    const gossip = this.findValidGossip(location);
    if (gossip === null) {
      return false;
    }
    this.gossip[gossip] = { game: this.world.gossip[gossip].game, type: 'item-region', item, region: this.world.regions[location] };
    this.hintedLocations.add(location);
    return true;
  }

  private placeGossipItemRegionSpheres(count: number) {
    const items = shuffle(this.random, this.spheres.flat().map(x => this.items[x]));
    let placed = 0;
    for (const item of items) {
      if (placed >= count) {
        break;
      }
      if (this.placeGossipItemRegion(item)) {
        placed++;
      }
    }
    return placed;
  }

  private duplicateHints() {
    const hints = shuffle(this.random, Object.values(this.gossip).map(x => ({ ...x })));
    const locs = new Set<string>(Object.keys(this.world.gossip));
    for (const k in this.gossip) {
      locs.delete(k);
    }
    const unplacedLocs = shuffle(this.random, Array.from(locs));
    for (let i = 0; i < hints.length; ++i) {
      this.gossip[unplacedLocs[i]] = { ...hints[i], game: this.world.gossip[unplacedLocs[i]].game };
    }
  }

  private placeGossips() {
    const woth = this.wayOfTheHero();
    const wothItems = this.wayOfTheHeroItems(woth);
    const foolishRegions = this.foolishRegions(wothItems);
    let hints = 0;

    /* TODO: refactor this */
    this.hintedLocations.add(this.findItem('OOT_ARROW_LIGHT')!);
    this.hintedLocations.add(this.findItem('MM_SONG_ORDER')!);

    [
      'OOT Skulltula House 10 Tokens',
      'OOT Skulltula House 20 Tokens',
      'OOT Skulltula House 30 Tokens',
      'OOT Skulltula House 40 Tokens',
      'OOT Skulltula House 50 Tokens',
    ].forEach(x => this.hintedLocations.add(x));

    /* Place always hints */
    hints += this.placeGossipItemExactPool(HINTS_ITEMS_ALWAYS);

    /* Place 3 sometimes hints */
    hints += this.placeGossipItemExactPool(HINTS_ITEMS_SOMETIMES, 3);

    /* Place 5 foolish hints */
    const foolishHints = this.placeGossipFoolish(foolishRegions, 5);
    hints += foolishHints;

    const missingFoolish = 5 - foolishHints;
    if (missingFoolish > 0) {
      hints += this.placeGossipItemExactPool(HINTS_ITEMS_SOMETIMES, missingFoolish);
    }

    /* Place Soaring spoiler */
    if (this.placeGossipItemRegion('MM_SONG_SOARING')) {
      hints++;
    }

    /* Place sphere spoilers */
    hints += this.placeGossipItemRegionSpheres(4);

    /* Place way of the hero hints */
    let wothHints = 0;
    for (let i = 0; i < 150; ++i) {
      if (this.placeGossipHero(woth)) {
        ++wothHints;
        ++hints;
        if (wothHints >= 9) {
          break;
        }
      }
    }

    /* Place remaining hints */
    const missingHints = 34 - hints;
    if (missingHints > 0) {
      hints += this.placeGossipItemExactPool(HINTS_ITEMS_SOMETIMES, missingHints);
    }

    /* Duplicate every hint */
    this.duplicateHints();
  }

  run() {
    const dungeonRewards = DUNGEON_REWARDS_ORDERED.map(item => this.findItem(item)).map(loc => this.world.regions[loc!]);
    const lightArrow = this.world.regions[this.findItem('OOT_ARROW_LIGHT')!];
    const oathToOrder = this.world.regions[this.findItem('MM_SONG_ORDER')!];
    this.placeGossips();
    return { dungeonRewards, lightArrow, oathToOrder, gossip: this.gossip };
  }
}

export const hints = (random: Random, settings: Settings, world: World, items: ItemPlacement, spheres: string[][]): Hints => {
  const solver = new HintsSolver(random, settings, world, items, spheres);
  return solver.run();
};
