import { Random, shuffle } from '../random';
import { Settings } from '../settings';
import { World } from './world';
import { Pathfinder, PathfinderState } from './pathfind';
import { Monitor } from '../monitor';
import { cloneDeep } from 'lodash';
import { isLocationRenewable, makePlayerLocations, Location, makeLocation, locationData } from './locations';
import { ItemPlacement } from './solve';
import { ItemGroups, ItemHelpers, Items, ItemsCount, PlayerItem, PlayerItems } from '../items';

const SIMPLE_DEPENDENCIES: {[k: string]: string[]} = {
  OOT_WEIRD_EGG: [
    'OOT Hatch Chicken',
  ],
  OOT_POCKET_EGG: [
    'OOT Hatch Pocket Cucco',
  ],
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
  OOT_MEDALLION_LIGHT: [],
  OOT_MEDALLION_FOREST: [
    'OOT Temple of Time Sheik Song',
    'OOT Kakariko Song Shadow',
  ],
  OOT_MEDALLION_FIRE: [
    'OOT Kakariko Song Shadow',
  ],
  OOT_MEDALLION_WATER: [
    'OOT Kakariko Song Shadow',
  ],
  OOT_MEDALLION_SPIRIT: [
    'OOT Temple of Time Light Arrows',
  ],
  OOT_MEDALLION_SHADOW: [
    'OOT Temple of Time Light Arrows',
  ],
  /*
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
  OOT_SMALL_KEY_GF: [
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
    'OOT Gerudo Training Grounds Hammer Room',
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
  */
  OOT_GS_TOKEN: [
    'OOT Skulltula House 10 Tokens',
    'OOT Skulltula House 20 Tokens',
    'OOT Skulltula House 30 Tokens',
    'OOT Skulltula House 40 Tokens',
    'OOT Skulltula House 50 Tokens',
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
  MM_MASK_SCENTS: [
    'MM Moon Fierce Deity Mask',
    'MM Swamp Potion Shop Item 1',
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
    'MM Woodfall Temple SF Water Room Beehive',
    'MM Woodfall Temple SF Maze Bubble',
    'MM Woodfall Temple SF Pre-Boss Left',
    'MM Woodfall Temple SF Pre-Boss Pillar',
    'MM Snowhead Temple SF Bridge Pillar',
    'MM Snowhead Temple SF Bridge Under Platform',
    'MM Snowhead Temple SF Compass Room Crate',
    'MM Snowhead Temple SF Dual Switches',
    'MM Snowhead Temple SF Snow Room',
    'MM Great Bay Temple SF Water Wheel Platform',
    'MM Great Bay Temple SF Central Room Underwater Pot',
    //'MM Great Bay Temple SF Pre-Boss Above Water', #Uncomment this if a trick to reverse the water flow in Great Bay Temple without Ice Arrows is added.
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
    'MM Kafei Hideout Pendant of Memories',
    'MM Kafei Hideout Owner Reward 1',
    'MM Kafei Hideout Owner Reward 2'
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
  ],
  /* 0 check items that can be required by special conditions */
  OOT_MASK_SPOOKY: [],
  OOT_MASK_GERUDO: [],
  OOT_MASK_ZORA: [],
  OOT_MASK_GORON: [],
  OOT_MASK_BUNNY: [],
  OOT_MASK_KEATON: [],

  /* MM remains - for special conds */
  MM_REMAINS_ODOLWA: [],
  MM_REMAINS_GOHT: [],
  MM_REMAINS_GYORG: [],
  MM_REMAINS_TWINMOLD: [],
};

export class LogicPassAnalysis {
  private pathfinder: Pathfinder;
  private spheres: Location[][] = [];
  private requiredLocs = new Set<Location>();
  private requiredlocsByItem: Map<PlayerItem, Set<Location>> = new Map();
  private uselessLocs = new Set<Location>();
  private unreachableLocs = new Set<Location>();
  private dependencies: typeof SIMPLE_DEPENDENCIES = {};
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
    },
  ){
    this.pathfinder = new Pathfinder(this.state.worlds, this.state.settings, this.state.startingItems);
    this.locations = this.state.worlds.map((x, i) => [...x.locations].map(l => makeLocation(l, i))).flat();
    this.paths = { triforcePower: new Set, triforceCourage: new Set, triforceWisdom: new Set };
  }

  private makeDependencies() {
    this.dependencies = cloneDeep(SIMPLE_DEPENDENCIES);
    if (!this.state.settings.tricks.includes('OOT_NIGHT_GS')) {
      delete this.dependencies['OOT_SONG_SUN'];
    }

    if (this.state.settings.erBoss) {
      delete this.dependencies['OOT_BOSS_KEY_FOREST'];
      delete this.dependencies['OOT_BOSS_KEY_SPIRIT'];
    }

    if (this.state.settings.erBoss || this.state.settings.erDungeons) {
      delete this.dependencies['OOT_SMALL_KEY_FOREST'];
      delete this.dependencies['OOT_SMALL_KEY_SPIRIT'];
    }

    if (this.state.settings.bossWarpPads === 'remains') {
      delete this.dependencies['MM_REMAINS_ODOLWA'];
      delete this.dependencies['MM_REMAINS_GOHT'];
      delete this.dependencies['MM_REMAINS_GYORG'];
      delete this.dependencies['MM_REMAINS_TWINMOLD'];
    }

    /* Shared items */
    this.dependencies['SHARED_MASK_TRUTH'] = [...this.dependencies['MM_MASK_TRUTH'], ...this.dependencies['OOT_MASK_TRUTH']];
    this.dependencies['SHARED_MASK_KEATON'] = [...this.dependencies['MM_MASK_KEATON']];
    this.dependencies['SHARED_MASK_BUNNY'] = [...this.dependencies['MM_MASK_BUNNY']];

    if (this.dependencies.hasOwnProperty('OOT_SONG_SUN'))
      this.dependencies['SHARED_SONG_SUN'] = [...this.dependencies['OOT_SONG_SUN']];

    const conds = Object.values(this.state.settings.specialConds);
    if (conds.some(x => x.count && x.skullsGold))  delete this.dependencies['OOT_GS_TOKEN'];
    if (conds.some(x => x.count && x.skullsSwamp)) delete this.dependencies['MM_GS_TOKEN_SWAMP'];
    if (conds.some(x => x.count && x.skullsOcean)) delete this.dependencies['MM_GS_TOKEN_OCEAN'];
    if (conds.some(x => x.count && x.fairiesWF)) delete this.dependencies['MM_STRAY_FAIRY_WF'];
    if (conds.some(x => x.count && x.fairiesSH)) delete this.dependencies['MM_STRAY_FAIRY_SH'];
    if (conds.some(x => x.count && x.fairiesGB)) delete this.dependencies['MM_STRAY_FAIRY_GB'];
    if (conds.some(x => x.count && x.fairiesST)) delete this.dependencies['MM_STRAY_FAIRY_ST'];
    if (conds.some(x => x.count && x.fairyTown)) delete this.dependencies['MM_STRAY_FAIRY_TOWN'];

    if (conds.some(x => x.count && x.stones)) {
      for (const s of ItemGroups.STONES) {
        delete this.dependencies[s.id];
      }
    }

    if (conds.some(x => x.count && x.medallions)) {
      for (const m of ItemGroups.MEDALLIONS) {
        delete this.dependencies[m.id];
      }
    }

    if (conds.some(x => x.count && x.remains)) {
      for (const r of ItemGroups.REMAINS) {
        delete this.dependencies[r.id];
      }
    }

    if (conds.some(x => x.count && x.masksRegular)) {
      for (const y of ItemGroups.MASKS_REGULAR) {
        delete this.dependencies[y.id];
      }
    }

    if (conds.some(x => x.count && x.masksTransform)) {
      for (const y of ItemGroups.MASKS_TRANSFORM) {
        delete this.dependencies[y.id];
      }
    }

    if (conds.some(x => x.count && x.masksOot)) {
      for (const y of ItemGroups.MASKS_OOT) {
        delete this.dependencies[y.id];
      }
    }
  }

  private makeSpheresRaw(restrictedLocations?: Set<Location>) {
    const spheres: Location[][] = [];
    let pathfinderState: PathfinderState | null = null;
    let count = 0;

    do {
      this.progress(count++, 10);
      pathfinderState = this.pathfinder.run(pathfinderState, { inPlace: true, items: this.state.items, stopAtGoal: true, restrictedLocations });
      const sphere = Array.from(pathfinderState.newLocations).filter(x => ItemHelpers.isItemImportant(this.state.items.get(x)!.item));
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
    return (ItemHelpers.isItemConsumable(pi.item) && !isLocationRenewable(this.state.worlds[locD.world as number], loc) && !ItemHelpers.isItemLicense(pi.item));
  }

  private isLocUselessHeuristicCount(loc: Location) {
    /* TODO: this is fragile */
    const item = this.state.items.get(loc)!;
    const itemId = item.item.id;
    let maximumRequired = -1;
    switch (itemId) {
    case 'OOT_SWORD':
      maximumRequired = 2;
      break;
    case 'OOT_OCARINA':
    case 'OOT_BOMB_BAG':
    case 'OOT_BOW':
    case 'OOT_SLINGSHOT':
    case 'OOT_MAGIC_UPGRADE':
    case 'MM_SWORD':
    case 'MM_BOW':
    case 'MM_BOMB_BAG':
    case 'MM_MAGIC_UPGRADE':
    case 'SHARED_BOW':
    case 'SHARED_BOMB_BAG':
    case 'SHARED_MAGIC_UPGRADE':
      maximumRequired = 1;
      break;
    }

    if (maximumRequired === -1) {
      return false;
    }
    const req = this.requiredlocsByItem.get(item);
    if (!req) {
      return false;
    }
    if (req.size < maximumRequired) {
      return false;
    }
    if (req.has(loc)) {
      return false;
    }
    this.state.monitor.debug(`Analysis - isLocUselessHeuristicCount: ${item.item.id}@${item.player}`);
    return true;
  }

  private isLocUselessHeuristicDependencies(loc: Location) {
    const locsToCheck = [loc];
    const locsAdded = new Set<Location>(locsToCheck);

    while (locsToCheck.length > 0) {
      const l = locsToCheck.pop()!;
      const item = this.state.items.get(l);
      if (item === undefined) {
        continue;
      }
      if (ItemHelpers.isItemImportant(item.item) && !this.uselessLocs.has(l) && !this.unreachableLocs.has(l)) {
        /* May be a progression item - need to check other locations */
        const dependencies = this.dependencies[item.item.id];
        if (dependencies === undefined) {
          return false;
        } else {
          for (const d of dependencies) {
            const player = item.player;
            const loc = makeLocation(d, player);
            if (!locsAdded.has(loc)) {
              locsAdded.add(loc);
              locsToCheck.push(loc);
            }
          }
        }
      }
    }
    const item = this.state.items.get(loc)!;
    this.state.monitor.debug(`Analysis - isLocUselessHeuristicDependencies: ${item.item.id}@${item.player}`);
    return true;
  }

  private makeUselessLocs() {
    for (const loc of this.locations) {
      if (this.requiredLocs.has(loc) || this.uselessLocs.has(loc) || this.unreachableLocs.has(loc)) {
        continue;
      }
      if (!ItemHelpers.isItemImportant(this.state.items.get(loc)!.item) || this.isLocUselessNonRenewable(loc) || this.isLocUselessHeuristicCount(loc) || this.isLocUselessHeuristicDependencies(loc)) {
        this.uselessLocs.add(loc);
      }
    }
  }

  private makeUnreachable() {
    const pathfinderState = this.pathfinder.run(null, { items: this.state.items, recursive: true });
    for (const loc of this.locations) {
      const item = this.state.items.get(loc)!;
      if (ItemHelpers.isItemImportant(item.item) && !pathfinderState.locations.has(loc)) {
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
      this.makeDependencies();
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
