import { Game, GAMES } from '../config';
import { gameId } from '../util';
import { Expr, exprTrue, MM_TIME_SLICES } from './expr';
import { ExprParser } from './expr-parser';
import { DATA_POOL, DATA_MACROS, DATA_WORLD, DATA_REGIONS, DATA_ENTRANCES_POOL, DATA_HINTS_POOL } from '../data';
import { DUNGEONS, SETTINGS, Settings } from '../settings';
import { Monitor } from '../monitor';
import { defaultPrices } from './price';
import { Item, itemByID, ItemHelpers, Items } from '../items';
import { Random } from '../random';
import { cloneDeep } from 'lodash';

export type ExprMap = {
  [k: string]: Expr;
}

export type WorldArea = {
  game: Game;
  boss: boolean;
  dungeon: string | null;
  exits: ExprMap;
  events: ExprMap;
  locations: ExprMap;
  gossip: ExprMap;
  stay: Expr[] | null;
  time: 'still' | 'day' | 'night' | 'flow';
  region: string;
};

type WorldCheckNumeric = {
  type: 'chest' | 'collectible' | 'gs' | 'sf' | 'cow' | 'shop' | 'scrub' | 'sr' | 'pot' | 'grass';
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

export type WorldEntrance = {
  id: string;
  type: 'boss' | 'dungeon' | 'overworld' | 'overworld-shuffle' | 'region' | 'region-extra' | 'region-shortcut' | 'indoors' | 'indoors-extra' | 'indoors-exit' | 'indoors-special';
  from: string;
  to: string;
  reverse: string | null;
  game: Game;
};

export type World = {
  areas: { [k: string]: WorldArea };
  checks: { [k: string]: WorldCheck };
  dungeons: { [k: string]: Set<string> };
  regions: { [k: string]: string };
  gossip: { [k: string]: WorldGossip };
  checkHints: { [k: string]: string[] };
  entrances: Map<string, WorldEntrance>;
  locations: Set<string>;
  songLocations: Set<string>;
  warpLocations: Set<string>;
  prices: number[];
  mq: Set<string>;
  bossIds: number[];
  dungeonIds: number[];
  entranceOverrides: Map<string, string>;
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
  Tower: "OOT_GANON_CASTLE",
  WF: "MM_TEMPLE_WOODFALL",
  SH: "MM_TEMPLE_SNOWHEAD",
  GB: "MM_TEMPLE_GREAT_BAY",
  ST: "MM_TEMPLE_STONE_TOWER",
  IST: "MM_TEMPLE_STONE_TOWER",
  SSH: "MM_SPIDER_HOUSE_SWAMP",
  OSH: "MM_SPIDER_HOUSE_OCEAN",
  BtW: "MM_BENEATH_THE_WELL",
  ACoI: "MM_IKANA_CASTLE",
  SS: "MM_SECRET_SHRINE",
  BtWE: "MM_BENEATH_THE_WELL",
  PF: "MM_PIRATE_FORTRESS_EXTERIOR",
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

export function cloneWorld(world: World): World {
  return {
    areas: cloneDeep(world.areas),
    checks: cloneChecks(world.checks),
    dungeons: cloneDeep(world.dungeons),
    regions: cloneDeep(world.regions),
    gossip: cloneDeep(world.gossip),
    checkHints: cloneDeep(world.checkHints),
    entrances: new Map(world.entrances),
    locations: new Set(world.locations),
    songLocations: new Set(world.songLocations),
    warpLocations: new Set(world.warpLocations),
    prices: [...world.prices],
    mq: new Set(world.mq),
    bossIds: [...world.bossIds],
    dungeonIds: [...world.dungeonIds],
    entranceOverrides: new Map(world.entranceOverrides),
  };
}

export type ExprParsers = {
  oot: ExprParser;
  mm: ExprParser;
}

export class LogicPassWorld {
  private world!: World;
  private exprParsers!: ExprParsers;

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
    this.makeExprParsers();

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

    return { worlds, exprParsers: this.exprParsers };
  }

  private createWorld(): World {
    this.world = this.makeDefaultWorld();
    for (const g of GAMES) {
      this.loadGame(g);
    }
    return this.world;
  }

  private makeExprParsers() {
    this.exprParsers = {} as ExprParsers;
    for (const g of GAMES) {
      const parser = new ExprParser(this.state.settings, g);
      this.loadMacros(g, parser);
      this.exprParsers[g] = parser;
    }
  }

  private makeDefaultWorld(): World {
    /* MQ */
    const mq = new Set<string>;
    let d: keyof typeof DUNGEONS;
    for (d in DUNGEONS) {
      if (this.state.settings.dungeon[d] === 'mq') {
        mq.add(d);
      } else if (this.state.settings.dungeon[d] === 'random') {
        if (this.state.random.next() & 0x10000) {
          mq.add(d);
        }
      }
    }

    /* Prices */
    const prices = defaultPrices(mq);

    return {
      areas: {},
      checks: {},
      dungeons: {},
      regions: {},
      gossip: {},
      checkHints: {},
      entrances: new Map,
      locations: new Set(),
      songLocations: new Set(),
      warpLocations: new Set(),
      prices,
      mq,
      bossIds: [],
      dungeonIds: [],
      entranceOverrides: new Map,
    };
  }

  private loadGame(game: Game) {
    /* Create the expr parser */
    this.loadAreas(game, this.exprParsers[game]);
    this.loadPool(game);
    this.loadEntrances(game);
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
    this.loadMacrosFile(exprParser, DATA_MACROS.common);
    this.loadMacrosFile(exprParser, DATA_MACROS[game]);
  }

  private loadAreas(game: Game, exprParser: ExprParser) {
    const data = DATA_WORLD[game];
    for (let areaSetName in data) {
      let areaSet = (data as any)[areaSetName];
      /* Handle MQ */
      if (game === 'oot' && this.world.mq.has(areaSetName)) {
        areaSet = (DATA_WORLD.mq as any)[areaSetName];
      }
      for (let name in areaSet) {
        const area = areaSet[name];
        name = gameId(game, name, ' ');
        const boss = area.boss || false;
        const dungeon = area.dungeon || null;
        let region = area.region;
        if (region !== 'NONE' && region !== 'ENTRANCE') {
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

        if (region !== 'ENTRANCE' && DATA_REGIONS[region] === undefined) {
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

        this.world.areas[name] = { game, boss, dungeon, exits, events, locations, gossip, time, stay, region };

        if (dungeon) {
          if (this.world.dungeons[dungeon] === undefined) {
            this.world.dungeons[dungeon] = new Set();
          }
          const d = this.world.dungeons[dungeon];
          Object.keys(locations).forEach(x => d.add(x));
        }

        for (const loc in locations) {
          this.world.regions[loc] = region;
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
    for (const record of DATA_POOL[game]) {
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
      const item = itemByID(gameId(game, String(record.item), '_'));
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

  private loadEntrances(game: Game) {
    for (const record of DATA_ENTRANCES_POOL[game]) {
      const id = gameId(game, String(record.id), '_');
      const reverseRaw = String(record.reverse);
      let reverse: string | null = null;
      if (reverseRaw !== 'NONE') {
        reverse = gameId(game, reverseRaw, '_');
      }
      const from = gameId(game, String(record.from), ' ');
      const to = gameId(game, String(record.to), ' ');
      const type = String(record.type) as WorldEntrance['type'];
      this.world.entrances.set(id, { id, from, to, type, reverse, game });
    }
  }
}
