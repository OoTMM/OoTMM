import { ItemPlacement } from './solve';
import { World } from './world';
import { findSpheres } from './playthrough';
import { Random, sample, shuffle } from '../random';
import { pathfind } from './pathfind';
import { Items } from './state';
import { addItem, DUNGEON_REWARDS_ORDERED, isDungeonItem, isDungeonReward, isItemMajor, isItemMajorSometimes, isItemMajorAlways, isToken, isGoldToken, itemsArray, isKey, isHouseToken, isSmallKey, isGanonBossKey, isRegularBossKey } from './items';
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

const FIXED_HINTS_LOCATIONS = [
  'OOT Skulltula House 10 Tokens',
  'OOT Skulltula House 20 Tokens',
  'OOT Skulltula House 30 Tokens',
  'OOT Skulltula House 40 Tokens',
  'OOT Skulltula House 50 Tokens',
  'OOT Hyrule Field Ocarina of Time',
  'OOT Hyrule Field Song of Time',
  'MM Laboratory Zora Song',
  'MM Moon Fierce Deity Mask',
  'MM Woodfall Great Fairy',
  'MM Snowhead Great Fairy',
  'MM Great Bay Great Fairy',
  'MM Ikana Great Fairy',
  'MM Goron Powder Keg',
  'MM Waterfall Rapids Beaver Race 1',
  'MM Waterfall Rapids Beaver Race 2',
  'MM Swamp Spider House Mask of Truth',
  'MM Ocean Spider House Wallet',
]

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

const LOCATIONS_FOR_CHICKEN = [
  'OOT Lost Woods Gift from Saria',
  'OOT Zelda\'s Letter',
  'OOT Zelda\'s Song'
];

const LOCATIONS_FOR_SKULL_MASK = [
  'OOT Deku Theater Sticks Upgrade'
];

const LOCATIONS_FOR_TRUTH_MASK_OOT = [
  'OOT Deku Theater Nuts Upgrade'
];

const LOCATIONS_FOR_POCKET_CUCCO = [
  'OOT Kakariko Anju Cojiro'
];

const LOCATIONS_FOR_COJIRO = [
  'OOT Lost Woods Odd Mushroom'
];

const LOCATIONS_FOR_ODD_MUSHROOM = [
  'OOT Kakariko Potion Shop Odd Potion'
];

const LOCATIONS_FOR_ODD_POTION = [
  'OOT Lost Woods Poacher\'s Saw'
];

const LOCATIONS_FOR_POACHER_SAW = [
  'OOT Gerudo Valley Broken Goron Sword'
];

const LOCATIONS_FOR_BROKEN_GORON_SWORD = [
  'OOT Death Mountain Prescription'
];

const LOCATIONS_FOR_PRESCRIPTION = [
  'OOT Zora Domain Eyeball Frog'
];

const LOCATIONS_FOR_EYEBALL_FROG = [
  'OOT Laboratory Eye Drops'
];

const LOCATIONS_FOR_EYE_DROPS = [
  'OOT Death Mountain Claim Check'
];

const LOCATIONS_FOR_CLAIM_CHECK = [
  'OOT Death Mountain Biggoron Sword'
];

const LOCATIONS_FOR_WALLET_OOT = [
  'OOT Goron City Medigoron Giant Knife'
];

const LOCATIONS_FOR_SONG_SUN = [
  'OOT Graveyard ReDead Tomb',
  'OOT Zora River Frogs Game'
];

const LOCATIONS_FOR_MASK_CAPTAIN = [
  'MM Moon Fierce Deity Mask',
  'MM Beneath The Graveyard Chest',
  'MM Beneath The Graveyard Song of Storms',
  'MM Beneath The Graveyard HP',
  'MM Beneath The Graveyard Dampe Chest',
  'MM Ocean Spider House Chest HP'
];

const LOCATIONS_FOR_MASK_ALL_NIGHT = [
  'MM Moon Fierce Deity Mask',
  'MM Stock Pot Inn Grandma HP 1',
  'MM Stock Pot Inn Grandma HP 2'
];

const LOCATIONS_FOR_MASK_BUNNY = [
  'MM Moon Fierce Deity Mask',
  'MM Post Office HP'
];

const LOCATIONS_FOR_MASK_KEATON = [
  'MM Moon Fierce Deity Mask',
  'MM Clock Town Keaton HP'
];

const LOCATIONS_FOR_MASK_ROMANI = [
  'MM Moon Fierce Deity Mask',
  'MM Milk Bar Troupe Leader Mask'
];

const LOCATIONS_FOR_MASK_TROUPE_LEADER = [
  'MM Moon Fierce Deity Mask'
];

const LOCATIONS_FOR_MASK_POSTMAN = [
  'MM Moon Fierce Deity Mask',
  'MM Clock Town Post Box'
];

const LOCATIONS_FOR_MASK_COUPLE = [
  'MM Moon Fierce Deity Mask',
  'MM Mayor\'s Office HP'
];

const LOCATIONS_FOR_MASK_GREAT_FAIRY = [
  'MM Moon Fierce Deity Mask',
  'MM Woodfall Great Fairy',
  'MM Snowhead Great Fairy',
  'MM Great Bay Great Fairy',
];

const LOCATIONS_FOR_MASK_DON_GERO = [
  'MM Moon Fierce Deity Mask',
  'MM Mountain Village Frog Choir HP'
];

const LOCATIONS_FOR_MASK_KAMARO = [
  'MM Moon Fierce Deity Mask',
  'MM Clock Town Rosa Sisters HP'
];

const LOCATIONS_FOR_MASK_TRUTH_MM = [
  'MM Moon Fierce Deity Mask',
  'MM Doggy Racetrack HP'
];

const LOCATIONS_FOR_MASK_STONE = [
  'MM Moon Fierce Deity Mask'
];

const LOCATIONS_FOR_MASK_BREMEN = [
  'MM Moon Fierce Deity Mask',
  'MM Cucco Shack Bunny Mask'
];

const LOCATIONS_FOR_MASK_KAFEI = [
  'MM Moon Fierce Deity Mask',
  'MM Stock Pot Inn Letter to Kafei',
  'MM Stock Pot Inn Couple\'s Mask',
  'MM Milk Bar Madame Aroma Bottle'
];

const LOCATIONS_FOR_DEED_SWAMP = [
  'MM Stock Pot Inn ??? HP',
  'MM Goron Village HP',
  'MM Goron Village Scrub Deed'
];

const LOCATIONS_FOR_DEED_MOUNTAIN = [
  'MM Stock Pot Inn ??? HP',
  'MM Zora Hall Scrub HP',
  'MM Zora Hall Scrub Deed'
];

const LOCATIONS_FOR_DEED_OCEAN = [
  'MM Stock Pot Inn ??? HP',
  'MM Ikana Valley Scrub Rupee',
  'MM Ikana Valley Scrub HP'
];

const LOCATIONS_FOR_ROOM_KEY = [
  'MM Stock Pot Inn Letter to Kafei',
  'MM Stock Pot Inn Couple\'s Mask',
  'MM Stock Pot Inn Guest Room Chest'
];

const LOCATIONS_FOR_LETTER_TO_KAFEI = [
  'MM Stock Pot Inn ??? HP',
  'MM Stock Pot Inn Couple\'s Mask',
  'MM Curiosity Shop Back Room Pendant of Memories',
  'MM Curiosity Shop Back Room Owner Reward 1',
  'MM Curiosity Shop Back Room Owner Reward 2'
];

const LOCATIONS_FOR_PENDANT_OF_MEMORIES = [
  'MM Stock Pot Inn Couple\'s Mask'
];

const LOCATIONS_FOR_LETTER_TO_MAMA = [
  'MM Stock Pot Inn ??? HP',
  'MM Milk Bar Madame Aroma Bottle',
  'MM Clock Town Postman Hat'
];

const LOCATIONS_FOR_POWDER_KEG = [
  'MM Romani Ranch Epona Song',
  'MM Romani Ranch Aliens',
  'MM Romani Ranch Cremia Escort',
  'MM Ancient Castle of Ikana Song Emptiness'
];

const LOCATIONS_FOR_SONG_HEALING = [
  'MM Termina Field Kamaro Mask',
  'MM Goron Graveyard Mask',
  'MM Great Bay Coast Zora Mask',
  'MM Music Box House Gibdo Mask'
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

  private isItemHintable(item: string) {
    if (isSmallKey(item) && this.settings.smallKeyShuffle === 'anywhere') {
      return true;
    }
    if (isGanonBossKey(item) && this.settings.ganonBossKey === 'anywhere') {
      return true;
    }
    if (isRegularBossKey(item) && this.settings.bossKeyShuffle === 'anywhere') {
      return true;
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
    if (isGoldToken(item)) {
      return false;
    }
    if (isHouseToken(item) && this.settings.housesSkulltulaTokens === 'none') {
      return false;
    }
    return true;
  }

  private isItemWayOfTheHero(item: string) {
    if (!this.isItemHintable(item)) {
      return false;
    }
    if (itemConstraint(item, this.settings) !== CONSTRAINT_NONE) {
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

  private playthroughItems() {
    const items = this.spheres.flat().map(x => this.items[x]).filter(x => this.isItemHintable(x));
    return shuffle(this.random, items);
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

  private limitedItemUselessHelper(list: string[]) {
    for (let place in list) {
      if (isItemMajorAlways(this.items[list[place]])) {
        //TODO: This fails to capture sometimes items gating extraneous progrssive items. This is obscure enough to let pass for now.
        return false;
      }
      else if (isItemMajorSometimes(this.items[list[place]])) {
        // This is a dangerous form of recursion, but I think I've avoided infinite loops.
        if (!this.limitedItemUseless(this.items[list[place]])) {
          return false;
        }
      }
    }
    return true;
  }

  private limitedItemUselessHelperSafe(list: string[]) {
    // This function has to have layers of additional checks to cover for somtimes required items that come in collections of optional requirements having self-gates.
    // For instance, without this safe mode, a MM HP landing in Secret Shrine or toilet hand already having a paper item would cause an infinite loop.
    const paperItems = ['MM_DEED_SWAMP', 'MM_DEED_MOUNTAIN', 'MM_DEED_OCEAN', 'MM_LETTER_TO_KAFEI', 'MM_LETTER_TO_MAMA'];
    for (let place in list) {
      if (isItemMajorAlways(this.items[list[place]])) {
        return false;
      }
      else if (isItemMajorSometimes(this.items[list[place]])) {
        if (paperItems.includes(this.items[list[place]])) {
          if (list[place] == 'MM Stock Pot Inn ??? HP') {
            return false;
          }
        }
        else if (this.items[list[place]] == 'OOT_WALLET') {
          if (list[place] == 'OOT Goron City Medigoron Giant Knife') {
            continue;
          }
        }
        if (!this.limitedItemUseless(this.items[list[place]])) {
          return false;
        }
      }
    }
    return true;
  }

  private limitedItemUseless(item: string) {
    switch (item) {
      case 'OOT_CHICKEN':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_CHICKEN);
      case 'OOT_MASK_SKULL':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_SKULL_MASK);
      case 'OOT_MASK_TRUTH':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_TRUTH_MASK_OOT);
      case 'OOT_POCKET_CUCCO':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_POCKET_CUCCO);
      case 'OOT_COJIRO':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_COJIRO);
      case 'OOT_ODD_MUSHROOM':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_ODD_MUSHROOM);
      case 'OOT_ODD_POTION':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_ODD_POTION);
      case 'OOT_POACHER_SAW':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_POACHER_SAW);
      case 'OOT_BROKEN_GORON_SWORD':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_BROKEN_GORON_SWORD);
      case 'OOT_PRESCRIPTION':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_PRESCRIPTION);
      case 'OOT_EYEBALL_FROG':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_EYEBALL_FROG);
      case 'OOT_EYE_DROPS':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_EYE_DROPS);
      case 'OOT_CLAIM_CHECK':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_CLAIM_CHECK);
      case 'OOT_WALLET':
        return this.limitedItemUselessHelperSafe(LOCATIONS_FOR_WALLET_OOT);
      case 'OOT_SONG_SUN':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_SONG_SUN);
      case 'MM_MASK_CAPTAIN':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_CAPTAIN);
      case 'MM_MASK_ALL_NIGHT':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_ALL_NIGHT);
      case 'MM_MASK_BUNNY':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_BUNNY);
      case 'MM_MASK_KEATON':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_KEATON);
      case 'MM_MASK_ROMANI':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_ROMANI);
      case 'MM_MASK_TROUPE_LEADER':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_TROUPE_LEADER);
      case 'MM_MASK_POSTMAN':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_POSTMAN);
      case 'MM_MASK_COUPLE':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_COUPLE);
      case 'MM_MASK_GREAT_FAIRY':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_GREAT_FAIRY);
      case 'MM_MASK_DON_GERO':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_DON_GERO);
      case 'MM_MASK_CAPTAIN':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_CAPTAIN);
      case 'MM_MASK_KAMARO':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_KAMARO);
      case 'MM_MASK_TRUTH':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_TRUTH_MM);
      case 'MM_MASK_STONE':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_STONE);
      case 'MM_MASK_BREMEN':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_BREMEN);
      case 'MM_MASK_KAFEI':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_MASK_KAFEI);
      case 'MM_DEED_SWAMP':
        return this.limitedItemUselessHelperSafe(LOCATIONS_FOR_DEED_SWAMP);
      case 'MM_DEED_MOUNTAIN':
        return this.limitedItemUselessHelperSafe(LOCATIONS_FOR_DEED_MOUNTAIN);
      case 'MM_DEED_OCEAN':
        return this.limitedItemUselessHelperSafe(LOCATIONS_FOR_DEED_OCEAN);
      case 'MM_ROOM_KEY':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_ROOM_KEY);
      case 'MM_LETTER_TO_KAFEI':
        return this.limitedItemUselessHelperSafe(LOCATIONS_FOR_LETTER_TO_KAFEI);
      case 'MM_PENDANT_OF_MEMORIES':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_PENDANT_OF_MEMORIES);
      case 'MM_LETTER_TO_MAMA':
        return this.limitedItemUselessHelperSafe(LOCATIONS_FOR_LETTER_TO_MAMA);
      case 'MM_POWDER_KEG':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_POWDER_KEG);
      case 'MM_SONG_HEALING':
        return this.limitedItemUselessHelper(LOCATIONS_FOR_SONG_HEALING);
    }
    return false;
  }

  private locationFoolish(loc: string, wothItems: {[k: string]: Set<string>}) {
    const item = this.items[loc];
    if (!this.isItemHintable(item)) {
      return 0;
    }
    if ((isItemMajor(item) || isKey(item)) && !this.majorItemFoolish(loc, item, wothItems) && !this.limitedItemUseless(item)) {
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
    const locations = this.world.checkHints[checkHint];
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

      /* Mark everythibng in the region as hinted */
      for (const loc in this.world.checks) {
        if (this.world.regions[loc] === region) {
          this.hintedLocations.add(loc);
        }
      }

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
    const items = this.playthroughItems();
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
    /* Set the always hinted locations */
    this.hintedLocations.add(this.findItem('OOT_ARROW_LIGHT')!);
    this.hintedLocations.add(this.findItem('MM_SONG_ORDER')!);
    FIXED_HINTS_LOCATIONS.forEach(x => this.hintedLocations.add(x));

    const woth = this.wayOfTheHero();
    const wothItems = this.wayOfTheHeroItems(woth);
    const foolishRegions = this.foolishRegions(wothItems);
    let hints = 0;

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
