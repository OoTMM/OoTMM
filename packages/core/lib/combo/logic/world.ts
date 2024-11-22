import { cloneDeep, mapValues } from 'lodash';
import { MACROS, WORLD, REGIONS, POOL, ENTRANCES, Entrance } from '@ootmm/data';

import { Game, GAMES } from '../config';
import { gameId } from '../util';
import { Expr, exprTrue, MM_TIME_SLICES } from './expr';
import { ExprParser } from './expr-parser';
import { DATA_HINTS_POOL } from '../data';
import { SETTINGS, Settings } from '../settings';
import { Monitor } from '../monitor';
import { defaultPrices } from './price';
import { Item, itemByID, ItemHelpers, Items } from '../items';
import { Random } from '../random';
import { Region } from './regions';

export const WORLD_FLAGS = [
  'ganonTrials',
  'smallKeyRingOot',
  'smallKeyRingMm',
  'silverRupeePouches',
  'openDungeonsMm',
  'openDungeonsOot',
  'mmPreActivatedOwls',
  'mqDungeons',
  'jpLayouts',
] as const;

type WorldFlag = typeof WORLD_FLAGS[number];

class ResolvedWorldFlag {
  private values: Set<string>;
  constructor(
    public readonly setting: keyof Settings,
    public readonly value: 'all' | 'none' | 'specific'
  ) {
    this.values = new Set;
  }

  add(value: string) {
    this.values.add(value);
  }

  has(value: string) {
    if (this.value === 'all') {
      return true;
    }
    if (this.value === 'none') {
      return false;
    }
    return this.values.has(value);
  }
}

export type ResolvedWorldFlags = {[k in WorldFlag]: ResolvedWorldFlag};

function resolveWorldFlag<T extends WorldFlag>(settings: Settings, random: Random, flag: T): ResolvedWorldFlag {
  const v = settings[flag];
  let wf: ResolvedWorldFlag;
  if (v.type === 'random-mixed' || v.type === 'random') {
    const setting = SETTINGS.find(x => x.key === flag)!;
    const values = ((setting as any).values as any[]).map(x => x.value) as string[];
    let set: string[] = [];
    let unset: string[] = [];
    if (v.type === 'random-mixed') {
      set = v.set;
      unset = v.unset;
    }
    wf = new ResolvedWorldFlag(flag, 'specific');
    for (const v of values) {
      if (set.includes(v)) {
        wf.add(v);
      } else if (!unset.includes(v) && random.next() & 0x1000) {
        wf.add(v);
      }
    }
  } else {
    wf = new ResolvedWorldFlag(flag, v.type);
    if (v.type === 'specific') {
      for (const k of v.values) {
        wf.add(k);
      }
    }
  }

  return wf;
}

function resolveWorldFlags(settings: Settings, random: Random): ResolvedWorldFlags {
  const result = {} as ResolvedWorldFlags;
  for (const flag of WORLD_FLAGS) {
    result[flag] = resolveWorldFlag(settings, random, flag);
  }
  return result;
}

export const BOSS_INDEX_BY_DUNGEON = {
  DT: 0,
  DC: 1,
  JJ: 2,
  Forest: 3,
  Fire: 4,
  Water: 5,
  Shadow: 6,
  Spirit: 7,
  WF: 8,
  SH: 9,
  GB: 10,
  IST: 11,
} as {[k: string]: number};

export type ExprMap = {
  [k: string]: Expr;
}

export type WorldAreaExprs = {
  exits: ExprMap;
  events: ExprMap;
  locations: ExprMap;
  gossip: ExprMap;
  stay: Expr[] | null;
};

export type WorldArea = WorldAreaExprs & {
  game: Game;
  boss: boolean;
  ageChange: boolean;
  dungeon: string | null;
  time: 'still' | 'day' | 'night' | 'flow';
  region: string;
};

type WorldCheckNumeric = {
  type: 'chest' | 'collectible' | 'gs' | 'sf' | 'cow' | 'shop' | 'scrub' | 'sr' | 'pot' | 'crate' | 'barrel' | 'grass' | 'wonder' | 'fish' | 'fairy' | 'snowball' | 'hive' | 'rupee' | 'heart' | 'fairy_spot' | 'butterfly';
  id: number;
};

type WorldCheckSymbolic = {
  type: 'npc';
  id: string;
};

export type WorldCheck = {
  game: Game;
  scene: string;
  item: Item;
  hint: string;
} & (WorldCheckNumeric | WorldCheckSymbolic);

export type WorldGossip = {
  game: Game;
  type: 'gossip' | 'gossip-grotto' | 'gossip-moon';
};

export type ExprParsers = {
  oot: ExprParser;
  mm: ExprParser;
}

export type WorldAreaExprsGraph = {[k: string]: WorldAreaExprs};

export type WorldOptimized = {
  child: WorldAreaExprsGraph;
  adult: WorldAreaExprsGraph;
};

type DungeonEntranceReplace = {
  readonly type: 'replace';
  readonly entrance: Entrance;
};

type DungeonEntranceRegion = {
  readonly type: 'region';
  readonly region: Region;
};

type DungeonEntrance =
  | DungeonEntranceReplace
  | DungeonEntranceRegion
  ;

export type World = {
  areas: { [k: string]: WorldArea };
  checks: { [k: string]: WorldCheck };
  dungeons: { [k: string]: Set<string> };
  regions: { [k: string]: string };
  gossip: { [k: string]: WorldGossip };
  checkHints: { [k: string]: string[] };
  locations: Set<string>;
  songLocations: Set<string>;
  warpLocations: Set<string>;
  prices: number[];
  bossIds: number[];
  entranceOverrides: Map<string, string>;
  entranceOverridesRev: Map<string, readonly string[]>;
  preCompleted: Set<string>;
  resolvedFlags: ResolvedWorldFlags;
  exprParsers: ExprParsers;
  optimized: WorldOptimized | null;
  dungeonsEntrances: Map<string, DungeonEntrance>;
};

export const DUNGEONS_REGIONS: { [k: string]: string } = {
  DT: "OOT_DEKU_TREE",
  DC: "OOT_DODONGO_CAVERN",
  JJ: "OOT_JABU_JABU",
  Forest: "OOT_TEMPLE_FOREST",
  Fire: "OOT_TEMPLE_FIRE",
  Water: "OOT_TEMPLE_WATER",
  Spirit: "OOT_TEMPLE_SPIRIT",
  Shadow: "OOT_TEMPLE_SHADOW",
  BotW: "OOT_BOTTOM_OF_THE_WELL",
  IC: "OOT_ICE_CAVERN",
  GTG: "OOT_GERUDO_TRAINING_GROUNDS",
  GF: "OOT_THIEVES_HIDEOUT",
  Ganon: "OOT_GANON_CASTLE",
  Tower: "OOT_GANON_CASTLE_TOWER",
  WF: "MM_TEMPLE_WOODFALL",
  SH: "MM_TEMPLE_SNOWHEAD",
  GB: "MM_TEMPLE_GREAT_BAY",
  ST: "MM_TEMPLE_STONE_TOWER",
  IST: "MM_TEMPLE_STONE_TOWER_INVERTED",
  SSH: "MM_SPIDER_HOUSE_SWAMP",
  OSH: "MM_SPIDER_HOUSE_OCEAN",
  BtW: "MM_BENEATH_THE_WELL",
  ACoI: "MM_IKANA_CASTLE",
  SS: "MM_SECRET_SHRINE",
  BtWE: "MM_BENEATH_THE_WELL",
  PF: "MM_PIRATE_FORTRESS_EXTERIOR",
  Moon: "MM_CLOCK_TOWER_ROOFTOP",
  TCG: "ENTRANCE",
};

const mapExprs = (exprParser: ExprParser, game: Game, char: string, data: any) => {
  const result: ExprMap = {};
  for (const [k, v] of Object.entries(data)) {
    const name = gameId(game, k, char);
    result[name] = exprParser.parse(v as string);
  }
  return result;
}

function cloneChecks(checks: { [k: string]: WorldCheck }): { [k: string]: WorldCheck } {
  const result: { [k: string]: WorldCheck } = {};
  for (const [k, v] of Object.entries(checks)) {
    result[k] = { ...v };
  }
  return result;
}

function cloneWorldAreaExprs(worldArea: WorldAreaExprs): WorldAreaExprs {
  return {
    exits: { ...worldArea.exits },
    events: { ...worldArea.events },
    locations: { ...worldArea.locations },
    gossip: { ...worldArea.gossip },
    stay: worldArea.stay ? [...worldArea.stay] : null,
  };
}

function cloneWorldAreaExprsGraph(worldAreaExprsGraph: WorldAreaExprsGraph): WorldAreaExprsGraph {
  const result: WorldAreaExprsGraph = {};
  for (const [k, v] of Object.entries(worldAreaExprsGraph)) {
    result[k] = cloneWorldAreaExprs(v);
  }
  return result;
};

function cloneWorldArea(worldArea: WorldArea): WorldArea {
  return {
    ...cloneWorldAreaExprs(worldArea),
    game: worldArea.game,
    boss: worldArea.boss,
    ageChange: worldArea.ageChange,
    dungeon: worldArea.dungeon,
    time: worldArea.time,
    region: worldArea.region,
  };
}

function cloneWorldOptimized(worldOptimized: WorldOptimized | null): WorldOptimized | null {
  if (!worldOptimized) {
    return null;
  }

  return {
    child: cloneWorldAreaExprsGraph(worldOptimized.child),
    adult: cloneWorldAreaExprsGraph(worldOptimized.adult),
  };
}

export function cloneWorld(world: World): World {
  return {
    areas: mapValues(world.areas, cloneWorldArea),
    checks: cloneChecks(world.checks),
    dungeons: mapValues(world.dungeons, x => new Set(x)),
    regions: cloneDeep(world.regions),
    gossip: cloneDeep(world.gossip),
    checkHints: cloneDeep(world.checkHints),
    locations: new Set(world.locations),
    songLocations: new Set(world.songLocations),
    warpLocations: new Set(world.warpLocations),
    prices: [...world.prices],
    preCompleted: new Set(world.preCompleted),
    bossIds: [...world.bossIds],
    entranceOverrides: new Map(world.entranceOverrides),
    entranceOverridesRev: new Map(world.entranceOverridesRev),
    resolvedFlags: world.resolvedFlags,
    exprParsers: world.exprParsers,
    optimized: cloneWorldOptimized(world.optimized),
    dungeonsEntrances: new Map(world.dungeonsEntrances),
  };
}

export function optimizedWorldView(world: World): WorldOptimized {
  if (!world.optimized) {
    return {
      child: world.areas,
      adult: world.areas,
    }
  };

  return world.optimized;
}

export class LogicPassWorld {
  private world!: World;

  constructor(
    private readonly state: {
      monitor: Monitor,
      settings: Settings,
      random: Random;
    }
  ){
  }

  run() {
    const worlds: World[] = [];
    this.state.monitor.log('Logic: World Building');

    if (this.state.settings.distinctWorlds) {
      for (let i = 0; i < this.state.settings.players; ++i) {
        worlds.push(this.createWorld());
      }
    } else {
      const world = this.createWorld();
      for (let i = 0; i < this.state.settings.players; ++i) {
        worlds.push(cloneWorld(world));
      }
    }

    return { worlds };
  }

  private createWorld(): World {
    this.world = this.makeDefaultWorld();
    for (const g of GAMES) {
      if (this.state.settings.games === 'ootmm' || this.state.settings.games === g)
        this.loadGame(g);
    }

    if (this.state.settings.games === 'mm') {
      this.world.areas['OOT SPAWN'] = {
        game: 'mm',
        boss: false,
        ageChange: false,
        dungeon: null,
        exits: {
          'MM Clock Town From Clock Tower': exprTrue(),
        },
        events: {},
        locations: {},
        gossip: {},
        stay: null,
        time: 'still',
        region: 'NONE',
      };
    }
    return this.world;
  }

  private makeDefaultWorld(): World {
    /* Resolve flags */
    const resolvedFlags = resolveWorldFlags(this.state.settings, this.state.random);

    /* Create expr parsers */
    const exprParsers = {} as ExprParsers;
    for (const g of GAMES) {
      const parser = new ExprParser(this.state.settings, resolvedFlags, g);
      this.loadMacros(g, parser);
      exprParsers[g] = parser;
    }

    /* Expr parser settings */
    exprParsers.mm.addVar('STRAY_FAIRY_COUNT', this.state.settings.strayFairyRewardCount);

    /* Prices */
    const prices = defaultPrices(resolvedFlags);

    return {
      areas: {},
      checks: {},
      dungeons: {},
      regions: {},
      gossip: {},
      checkHints: {},
      locations: new Set(),
      songLocations: new Set(),
      warpLocations: new Set(),
      prices,
      preCompleted: new Set(),
      bossIds: Object.values(BOSS_INDEX_BY_DUNGEON),
      entranceOverrides: new Map,
      entranceOverridesRev: new Map,
      resolvedFlags,
      exprParsers,
      optimized: null,
      dungeonsEntrances: new Map,
    };
  }

  private loadGame(game: Game) {
    /* Create the expr parser */
    this.loadAreas(game, this.world.exprParsers[game]);
    this.loadPool(game);
  }

  private loadMacrosFile(exprParser: ExprParser, data: any) {
    for (let name in data) {
      const buffer = data[name];

      /* Horrible hack */
      name = name.replace('(', ' ');
      name = name.replace(')', ' ');
      name = name.replace(/,/g, ' ');

      const parts = name.split(' ').filter(x => !!x);
      name = parts[0];
      const args = parts.slice(1);
      exprParser.addMacro(name, args, buffer);
    }
  }

  private loadMacros(game: Game, exprParser: ExprParser) {
    this.loadMacrosFile(exprParser, MACROS.common);
    this.loadMacrosFile(exprParser, MACROS[game]);
  }

  private loadAreas(game: Game, exprParser: ExprParser) {
    if (game === 'oot' && this.state.settings.games === 'mm') {
      this.world.areas['OOT SPAWN'] = {
        game: 'oot',
        boss: false,
        ageChange: false,
        dungeon: null,
        exits: {
          'MM Clock Town': exprTrue(),
        },
        events: {},
        locations: {},
        gossip: {},
        stay: null,
        time: 'still',
        region: 'NONE',
      };
      return;
    }

    const data = WORLD[game];
    for (let areaSetName in data) {
      let areaSet = (data as any)[areaSetName];
      /* Handle MQ */
      if (game === 'oot' && WORLD.mq.hasOwnProperty(areaSetName) && this.world.resolvedFlags.mqDungeons.has(areaSetName)) {
        areaSet = (WORLD.mq as any)[areaSetName];
      }
      /* Handle JP layouts */
      if (game === 'mm') {
        const areaSetUs = (WORLD.mm_us as any)[areaSetName];
        const areaSetJp = (WORLD.mm_jp as any)[areaSetName];
        if (areaSetUs !== undefined && areaSetJp !== undefined) {
          const areaSetPatch = this.world.resolvedFlags.jpLayouts.has(areaSetName) ? areaSetJp : areaSetUs;
          areaSet = { ...areaSet, ...areaSetPatch };
        }
      }
      for (let name in areaSet) {
        const area = areaSet[name];
        name = gameId(game, name, ' ');
        const boss = area.boss || false;
        const ageChange = area.age_change ?? (game === 'oot' ? true : false);
        const dungeon = area.dungeon || null;
        let region = area.region;
        if (region !== 'NONE' && region !== 'ENTRANCE' && region !== 'BUFFER' && region !== 'BUFFER_DELAYED') {
          region = region ? gameId(game, region, '_') : undefined;
        }
        if (dungeon) {
          region = DUNGEONS_REGIONS[dungeon];
        }
        const locations = mapExprs(exprParser, game, ' ', area.locations || {});
        const exits = mapExprs(exprParser, game, ' ', area.exits || {});
        const events = mapExprs(exprParser, game, '_', area.events || {});
        const gossip = mapExprs(exprParser, game, ' ', area.gossip || {});
        const time = area.time || 'still';
        let stay: Expr[] | null = null;

        /* Hack to propagate time back to OoT GLOBAL */
        if (time !== 'still') {
          exits["OOT GLOBAL"] = exprTrue();
        }

        if (name === undefined) {
          throw new Error(`Area name is undefined`);
        }

        if (region === undefined) {
          throw new Error(`Undefined region for area ${name}`);
        }

        if (region !== 'ENTRANCE' && region !== 'BUFFER' && region !== 'BUFFER_DELAYED' && REGIONS[region as keyof typeof REGIONS] === undefined) {
          throw new Error(`Unknown region ${region}`);
        }

        if (area.stay) {
          stay = Object.values(MM_TIME_SLICES).map(x => exprTrue());
          for (const [k, v] of Object.entries(area.stay)) {
            const timeId = MM_TIME_SLICES.indexOf(k as any);
            if (timeId === -1) {
              throw new Error(`Unknown time ${k}`);
            }
            stay[timeId] = exprParser.parse(v as string);
          }
        }

        this.world.areas[name] = { game, boss, ageChange, dungeon, exits, events, locations, gossip, time, stay, region };

        if (dungeon) {
          if (this.world.dungeons[dungeon] === undefined) {
            this.world.dungeons[dungeon] = new Set();
          }
          const d = this.world.dungeons[dungeon];
          Object.keys(locations).forEach(x => d.add(x));
        }

        for (const loc in locations) {
          const prevRegion = this.world.regions[loc];
          if (!prevRegion || prevRegion === 'NONE') {
            this.world.regions[loc] = region;
          }
          this.world.locations.add(loc);
        }

        for (const g in gossip) {
          const data = DATA_HINTS_POOL[game][g];
          if (!data) throw new Error(`Unknown gossip stone ${g}`);
          const worldGossip = { game, type: data.type } as WorldGossip;
          this.world.gossip[g] = worldGossip;
        }
      }
    }
  }

  private loadPool(game: Game) {
    for (const record of POOL[game]) {
      const location = gameId(game, String(record.location), ' ');
      if (!this.world.locations.has(location))
        continue;
      const type = String(record.type);
      const scene = gameId(game, String(record.scene), '_');
      let id = null;
      if (type === 'npc') {
        id = gameId(game, String(record.id), '_');
      } else {
        id = Number(record.id);
      }
      let item: Item;
      if (record.item === 'NOTHING') {
        item = Items.NOTHING;
      } else {
        item = itemByID(gameId(game, String(record.item), '_'));
      }
      let hint = String(record.hint);
      if (hint !== 'NONE') {
        hint = gameId(game, hint, '_');
        if (this.world.checkHints[hint] === undefined) {
          this.world.checkHints[hint] = [];
        }
        this.world.checkHints[hint].push(location);
      }

      const check = { game, type, scene, id, item, hint } as WorldCheck;
      this.world.checks[location] = check;

      if (ItemHelpers.isSong(item)) {
        this.world.songLocations.add(location);
      } else if (ItemHelpers.isDungeonReward(item)) {
        this.world.warpLocations.add(location);
      }
    }
  }
}
