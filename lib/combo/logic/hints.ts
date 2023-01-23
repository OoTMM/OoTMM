import { ItemPlacement } from './solve';
import { World } from './world';
import { findSpheres } from './playthrough';
import { Random, sample, shuffle } from '../random';
import { pathfind } from './pathfind';
import { Items } from './state';
import { addItem, DUNGEON_REWARDS_ORDERED, isDungeonItem, isDungeonReward, isItemMajor, isGoldToken, itemsArray, isKey, isHouseToken, isSmallKey, isGanonBossKey, isRegularBossKey, DUNGEON_REWARDS, isStrayFairy, isToken, isTownStrayFairy, isSong } from './items';
import { Settings } from '../settings';
import { Game } from '../config';
import { Monitor } from '../monitor';

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
  foolish: {[k: string]: number};
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
  'MM Clock Town Great Fairy',
  'MM Clock Town Great Fairy Alt',
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

const SIMPLE_DEPENDENCIES: {[k: string]: string[]} = {
  OOT_CHICKEN: [
    'OOT Lost Woods Gift from Saria',
    'OOT Zelda\'s Letter',
    'OOT Zelda\'s Song',
  ],
  OOT_MASK_SKULL: [
    'OOT Deku Theater Sticks Upgrade',
  ],
  OOT_MASK_TRUTH: [
    'OOT Deku Theater Nuts Upgrade'
  ],
  OOT_POCKET_CUCCO: [
    'OOT Kakariko Anju Cojiro'
  ],
  OOT_COJIRO: [
    'OOT Lost Woods Odd Mushroom'
  ],
  OOT_ODD_MUSHROOM: [
    'OOT Kakariko Potion Shop Odd Potion'
  ],
  OOT_ODD_POTION: [
    'OOT Lost Woods Poacher\'s Saw'
  ],
  OOT_POACHER_SAW: [
    'OOT Gerudo Valley Broken Goron Sword'
  ],
  OOT_BROKEN_GORON_SWORD: [
    'OOT Death Mountain Prescription'
  ],
  OOT_PRESCRIPTION: [
    'OOT Zora Domain Eyeball Frog'
  ],
  OOT_EYEBALL_FROG: [
    'OOT Laboratory Eye Drops'
  ],
  OOT_EYE_DROPS: [
    'OOT Death Mountain Claim Check'
  ],
  OOT_CLAIM_CHECK: [
    'OOT Death Mountain Biggoron Sword'
  ],
  OOT_WALLET: [
    'OOT Goron City Medigoron Giant Knife'
  ],
  OOT_SONG_SUN: [
    'OOT Graveyard ReDead Tomb',
    'OOT Zora River Frogs Game'
  ],
  OOT_STONE_EMERALD: [
    'OOT Hyrule Field Ocarina of Time',
    'OOT Hyrule Field Song of Time',
  ],
  OOT_STONE_RUBY: [
    'OOT Hyrule Field Ocarina of Time',
    'OOT Hyrule Field Song of Time',
  ],
  OOT_STONE_SAPPHIRE: [
    'OOT Hyrule Field Ocarina of Time',
    'OOT Hyrule Field Song of Time',
  ],
  OOT_SMALL_KEY_FOREST: [
    'OOT Forest Temple Map',
    'OOT Forest Temple Maze',
    'OOT Forest Temple Garden',
    'OOT Forest Temple Well',
    'OOT Forest Temple Poe Key',
    'OOT Forest Temple Bow',
    'OOT Forest Temple Compass',
    'OOT Forest Temple Checkerboard',
    'OOT Forest Temple Boss Key',
    'OOT Forest Temple Floormaster',
    'OOT Forest Temple Antichamber',
    'OOT Forest Temple GS Entrance',
    'OOT Forest Temple GS Main',
    'OOT Forest Temple GS Garden West',
    'OOT Forest Temple GS Garden East',
    'OOT Forest Temple GS Antichamber',
    'OOT Forest Temple Boss Container',
    'OOT Forest Temple Boss',
  ],
  OOT_BOSS_KEY_FOREST: [
    'OOT Forest Temple Boss Container',
    'OOT Forest Temple Boss',
  ],
  OOT_SMALL_KEY_SPIRIT: [
    'OOT Spirit Temple Child Climb 1',
    'OOT Spirit Temple Child Climb 2',
    'OOT Spirit Temple Statue Base',
    'OOT Spirit Temple Statue Hands',
    'OOT Spirit Temple Statue Upper Right',
    'OOT Spirit Temple Sun Block Room Torches',
    'OOT Spirit Temple Adult Suns on Wall 1',
    'OOT Spirit Temple Adult Suns on Wall 2',
    'OOT Spirit Temple Adult Invisible 1',
    'OOT Spirit Temple Adult Invisible 2',
    'OOT Spirit Temple Adult Late Sun on Wall',
    'OOT Spirit Temple Adult Boss Key Chest',
    'OOT Spirit Temple Adult Topmost Sun on Wall',
    'OOT Spirit Temple GS Child Climb',
    'OOT Spirit Temple GS Iron Knuckle',
    'OOT Spirit Temple GS Statue',
    'OOT Spirit Temple Silver Gauntlets',
    'OOT Spirit Temple Mirror Shield',
    'OOT Spirit Temple Boss HC',
    'OOT Spirit Temple Boss',
  ],
  OOT_BOSS_KEY_SPIRIT: [
    'OOT Spirit Temple Boss HC',
    'OOT Spirit Temple Boss',
  ],
  OOT_GS_TOKEN: [
    'OOT Skulltula House 10 Tokens',
    'OOT Skulltula House 20 Tokens',
    'OOT Skulltula House 30 Tokens',
    'OOT Skulltula House 40 Tokens',
    'OOT Skulltula House 50 Tokens',
  ],
  OOT_SMALL_KEY_GF: [
    'OOT Gerudo Fortress Jail 1',
    'OOT Gerudo Fortress Jail 2',
    'OOT Gerudo Fortress Jail 3',
    'OOT Gerudo Fortress Jail 4',
    'OOT Gerudo Member Card',
  ],
  OOT_SMALL_KEY_GTG: [
    'OOT Gerudo Training Grounds Stalfos',
    'OOT Gerudo Training Grounds Near Block',
    'OOT Gerudo Training Grounds Behind Block Invisible',
    'OOT Gerudo Training Grounds Behind Block Visible 1',
    'OOT Gerudo Training Grounds Behind Block Visible 2',
    'OOT Gerudo Training Grounds Behind Block Visible 3',
    'OOT Gerudo Training Grounds Eye Statue',
    'OOT Gerudo Training Grounds Hammer Room Switch',
    'OOT Gerudo Training Grounds Hammer Room,',
    'OOT Gerudo Training Grounds Lizalfos',
    'OOT Gerudo Training Water',
    'OOT Gerudo Training Freestanding Key',
    'OOT Gerudo Training Maze Upper Cage',
    'OOT Gerudo Training Maze Upper Fake Ceiling',
    'OOT Gerudo Training Maze Side Chest 1',
    'OOT Gerudo Training Maze Side Chest 2',
    'OOT Gerudo Training Maze Chest 1',
    'OOT Gerudo Training Maze Chest 2',
    'OOT Gerudo Training Maze Chest 3',
    'OOT Gerudo Training Maze Chest 4',
  ],
  OOT_SMALL_KEY_GANON: [
    'OOT Ganon Castle Light Chest Lullaby',
  ],
  MM_MASK_CAPTAIN: [
    'MM Moon Fierce Deity Mask',
    'MM Beneath The Graveyard Chest',
    'MM Beneath The Graveyard Song of Storms',
    'MM Beneath The Graveyard HP',
    'MM Beneath The Graveyard Dampe Chest',
    'MM Ocean Spider House Chest HP'
  ],
  MM_MASK_ALL_NIGHT: [
    'MM Moon Fierce Deity Mask',
    'MM Stock Pot Inn Grandma HP 1',
    'MM Stock Pot Inn Grandma HP 2'
  ],
  MM_MASK_BUNNY: [
    'MM Moon Fierce Deity Mask',
    'MM Post Office HP'
  ],
  MM_MASK_KEATON: [
    'MM Moon Fierce Deity Mask',
    'MM Clock Town Keaton HP'
  ],
  MM_MASK_ROMANI: [
    'MM Moon Fierce Deity Mask',
    'MM Milk Bar Troupe Leader Mask'
  ],
  MM_MASK_TROUPE_LEADER: [
    'MM Moon Fierce Deity Mask'
  ],
  MM_MASK_POSTMAN: [
    'MM Moon Fierce Deity Mask',
    'MM Clock Town Post Box'
  ],
  MM_MASK_COUPLE: [
    'MM Moon Fierce Deity Mask',
    'MM Mayor\'s Office HP'
  ],
  MM_MASK_GREAT_FAIRY: [
    'MM Moon Fierce Deity Mask',
    'MM Woodfall Great Fairy',
    'MM Snowhead Great Fairy',
    'MM Great Bay Great Fairy',
  ],
  MM_MASK_DON_GERO: [
    'MM Moon Fierce Deity Mask',
    'MM Mountain Village Frog Choir HP'
  ],
  MM_MASK_KAMARO: [
    'MM Moon Fierce Deity Mask',
    'MM Clock Town Rosa Sisters HP'
  ],
  MM_MASK_TRUTH: [
    'MM Moon Fierce Deity Mask',
    'MM Doggy Racetrack HP'
  ],
  MM_MASK_STONE: [
    'MM Moon Fierce Deity Mask'
  ],
  MM_MASK_BREMEN: [
    'MM Moon Fierce Deity Mask',
    'MM Cucco Shack Bunny Mask'
  ],
  MM_MASK_KAFEI: [
    'MM Moon Fierce Deity Mask',
    'MM Stock Pot Inn Letter to Kafei',
    'MM Stock Pot Inn Couple\'s Mask',
    'MM Milk Bar Madame Aroma Bottle'
  ],
  MM_DEED_SWAMP: [
    'MM Stock Pot Inn ??? HP',
    'MM Goron Village HP',
    'MM Goron Village Scrub Deed'
  ],
  MM_DEED_MOUNTAIN: [
    'MM Stock Pot Inn ??? HP',
    'MM Zora Hall Scrub HP',
    'MM Zora Hall Scrub Deed'
  ],
  MM_DEED_OCEAN: [
    'MM Stock Pot Inn ??? HP',
    'MM Ikana Valley Scrub Rupee',
    'MM Ikana Valley Scrub HP'
  ],
  MM_ROOM_KEY: [
    'MM Stock Pot Inn Letter to Kafei',
    'MM Stock Pot Inn Couple\'s Mask',
    'MM Stock Pot Inn Guest Room Chest'
  ],
  MM_LETTER_TO_KAFEI: [
    'MM Stock Pot Inn ??? HP',
    'MM Stock Pot Inn Couple\'s Mask',
    'MM Curiosity Shop Back Room Pendant of Memories',
    'MM Curiosity Shop Back Room Owner Reward 1',
    'MM Curiosity Shop Back Room Owner Reward 2'
  ],
  MM_PENDANT_OF_MEMORIES: [
    'MM Stock Pot Inn Couple\'s Mask'
  ],
  MM_LETTER_TO_MAMA: [
    'MM Stock Pot Inn ??? HP',
    'MM Milk Bar Madame Aroma Bottle',
    'MM Clock Town Postman Hat'
  ],
  MM_POWDER_KEG: [
    'MM Romani Ranch Epona Song',
    'MM Romani Ranch Aliens',
    'MM Romani Ranch Cremia Escort',
    'MM Ancient Castle of Ikana Song Emptiness'
  ],
  MM_SONG_HEALING: [
    'MM Termina Field Kamaro Mask',
    'MM Goron Graveyard Mask',
    'MM Great Bay Coast Zora Mask',
    'MM Music Box House Gibdo Mask'
  ],
  MM_GS_TOKEN_SWAMP: [
    'MM Swamp Spider House Mask of Truth',
  ],
  MM_GS_TOKEN_OCEAN: [
    'MM Ocean Spider House Wallet',
  ],
  MM_STRAY_FAIRY_TOWN: [
    'MM Clock Town Great Fairy',
    'MM Clock Town Great Fairy Alt',
  ],
  MM_STRAY_FAIRY_WF: [
    'MM Woodfall Great Fairy',
  ],
  MM_STRAY_FAIRY_SH: [
    'MM Snowhead Great Fairy',
  ],
  MM_STRAY_FAIRY_GB: [
    'MM Great Bay Great Fairy',
  ],
  MM_STRAY_FAIRY_ST: [
    'MM Ikana Great Fairy',
  ]
};

class HintsSolver {
  private hintedLocations = new Set<string>();
  private gossip: {[k: string]: HintGossip} = {};
  private foolish: {[k: string]: number} = {};
  private dependencies: typeof SIMPLE_DEPENDENCIES;

  constructor(
    private monitor: Monitor,
    private random: Random,
    private settings: Settings,
    private world: World,
    private items: ItemPlacement,
    private spheres: string[][],
  ){
    this.dependencies = JSON.parse(JSON.stringify(SIMPLE_DEPENDENCIES));
    if (!settings.tricks['OOT_NIGHT_GS']) {
      delete this.dependencies['OOT_SONG_SUN'];
    }
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

  private isLocationHintable(loc: string) {
    const item = this.items[loc];
    if (loc === 'OOT Temple of Time Master Sword' && !this.settings.shuffleMasterSword) {
      return false;
    }
    if (isSmallKey(item) && this.settings.smallKeyShuffle === 'anywhere') {
      return true;
    }
    if (isGanonBossKey(item) && this.settings.ganonBossKey === 'anywhere') {
      return true;
    }
    if (isRegularBossKey(item) && this.settings.bossKeyShuffle === 'anywhere') {
      return true;
    }
    if (isTownStrayFairy(item) && this.settings.townFairyShuffle === 'anywhere') {
      return true;
    }
    if (isStrayFairy(item) && this.settings.strayFairyShuffle === 'anywhere') {
      return true;
    }
    if (isDungeonItem(item)) {
      return false;
    }
    if (isDungeonReward(item) && this.settings.dungeonRewardShuffle !== 'anywhere') {
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

  private isLocationWayOfTheHero(loc: string) {
    const item = this.items[loc];
    if (!this.isLocationHintable(loc)) {
      return false;
    }
    if (isSong(item) && this.settings.songs !== 'anywhere') {
      return false;
    }
    return true;
  }

  private wayOfTheHero() {
    const woth = new Set<string>();
    for (const sphere of this.spheres) {
      for (const loc of sphere) {
        if (findSpheres(this.settings, this.world, this.items, undefined, new Set([loc])) === null) {
          woth.add(loc);
        }
      }
    }
    const locs = Array.from(woth).filter(loc => this.isLocationWayOfTheHero(loc));
    return new Set(locs);
  }

  private findValidGossip(locs: Set<string> | string) {
    if (typeof locs === 'string') {
      locs = new Set([locs]);
    }
    const items: Items = { ...this.settings.startingItems };
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
    const items = this.spheres.flat().filter(loc => this.isLocationHintable(loc)).map(loc => this.items[loc]);
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
    case 'OOT_WALLET':
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
    this.monitor.debug("majorItemFoolish - Purged: " + item);
    return true;
  }

  private isItemUseless(loc: string) {
    const locsToCheck = [loc];
    const locsAdded = new Set<string>(locsToCheck);

    while (locsToCheck.length > 0) {
      const l = locsToCheck.pop()!;
      const item = this.items[l];
      if (isItemMajor(item) || isDungeonReward(item) || isKey(item) || isStrayFairy(item) || isToken(item)) {
        /* May be a progression item - need to check other locations */
        const dependencies = this.dependencies[item];
        if (dependencies === undefined) {
          return false;
        } else {
          for (const d of dependencies) {
            if (!locsAdded.has(d)) {
              locsAdded.add(d);
              locsToCheck.push(d);
            }
          }
        }
      }
    }
    this.monitor.debug("isItemUseless - Purged: " + this.items[loc]);
    return true;
  }

  private locationFoolish(loc: string, wothItems: {[k: string]: Set<string>}) {
    const item = this.items[loc];
    if (!this.isLocationHintable(loc)) {
      return 0;
    }
    if ((isItemMajor(item) || isDungeonReward(item) || isKey(item) || isStrayFairy(item)) && !this.majorItemFoolish(loc, item, wothItems) && !this.isItemUseless(loc)) {
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
    FIXED_HINTS_LOCATIONS.forEach(x => this.hintedLocations.add(x));

    const woth = this.wayOfTheHero();
    const wothItems = this.wayOfTheHeroItems(woth);
    this.foolish = this.foolishRegions(wothItems);
    let hints = 0;

    /* Place always hints */
    hints += this.placeGossipItemExactPool(HINTS_ITEMS_ALWAYS);

    /* Place 3 sometimes hints */
    hints += this.placeGossipItemExactPool(HINTS_ITEMS_SOMETIMES, 3);

    /* Place 5 foolish hints */
    const foolishHints = this.placeGossipFoolish(this.foolish, 5);
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

  private locRegion(loc: string | null) {
    if (loc === null) {
      return 'NONE';
    }
    return this.world.regions[loc];
  }

  markLocation(location: string | null) {
    if (location === null) {
      return;
    }
    this.hintedLocations.add(location);
  }

  run() {
    const dungeonRewardLocations = DUNGEON_REWARDS_ORDERED.map(item => this.findItem(item));
    const lightArrowLocation = this.findItem('OOT_ARROW_LIGHT');
    const oathToOrderLocation = this.findItem('MM_SONG_ORDER');

    dungeonRewardLocations.forEach(x => this.markLocation(x));
    this.markLocation(lightArrowLocation);
    this.markLocation(oathToOrderLocation);

    const dungeonRewards = dungeonRewardLocations.map(x => this.locRegion(x));
    const lightArrow = this.locRegion(lightArrowLocation);
    const oathToOrder = this.locRegion(oathToOrderLocation);

    this.placeGossips();
    return { dungeonRewards, lightArrow, oathToOrder, foolish: this.foolish, gossip: this.gossip };
  }
}

export const hints = (monitor: Monitor, random: Random, settings: Settings, world: World, items: ItemPlacement, spheres: string[][]): Hints => {
  const solver = new HintsSolver(monitor, random, settings, world, items, spheres);
  return solver.run();
};
