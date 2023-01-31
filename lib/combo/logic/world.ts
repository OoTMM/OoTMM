import { Game, GAMES } from '../config';
import { gameId } from '../util';
import { Expr } from './expr';
import { ExprParser } from './expr-parser';
import { DATA_POOL, DATA_MACROS, DATA_WORLD, DATA_REGIONS, DATA_ENTRANCES } from '../data';
import { Settings } from '../settings';

export type ExprMap = {
  [k: string]: Expr;
}

type WorldArea = {
  boss: boolean;
  dungeon: string | null;
  exits: ExprMap;
  events: ExprMap;
  locations: ExprMap;
  gossip: ExprMap;
};

type WorldCheckNumeric = {
  type: 'chest' | 'collectible' | 'gs' | 'sf';
  id: number;
};

type WorldCheckSymbolic = {
  type: 'npc';
  id: string;
};

export type WorldCheck = {
  game: Game;
  scene: string;
  item: string;
  hint: string;
} & (WorldCheckNumeric | WorldCheckSymbolic);

export type WorldGossip = {
  game: Game;
};

export type WorldEntrance = {
  from: string;
  to: string;
};

export type World = {
  areas: {[k: string]: WorldArea};
  checks: {[k: string]: WorldCheck};
  dungeons: {[k: string]: Set<string>};
  regions: {[k: string]: string};
  gossip: {[k: string]: WorldGossip};
  checkHints: {[k: string]: string[]};
  entrances: WorldEntrance[];
};

export const DUNGEONS_REGIONS: {[k: string]: string} = {
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
  WF: "MM_TEMPLE_WOODFALL",
  SH: "MM_TEMPLE_SNOWHEAD",
  GB: "MM_TEMPLE_GREAT_BAY",
  ST: "MM_TEMPLE_STONE_TOWER",
};

const mapExprs = (exprParser: ExprParser, game: Game, char: string, data: any) => {
  const result: ExprMap = {};
  for (const [k, v] of Object.entries(data)) {
    let name = k;
    if (!(/^(MM|OOT) /.test(name))) {
      name = gameId(game, k, char);
    }
    result[name] = exprParser.parse(v as string);
  }
  return result;
}

const loadWorldAreas = (world: World, game: Game, exprParser: ExprParser) => {
  const data = DATA_WORLD[game];
  for (let name in data) {
    const area = data[name];
    name = gameId(game, name, ' ');
    const boss = area.boss || false;
    const dungeon = area.dungeon || null;
    let region = area.region;
    if (region !== 'NONE') {
      region = region ? gameId(game, region, '_') : undefined;
    }
    if (dungeon) {
      region = DUNGEONS_REGIONS[dungeon];
    }
    const locations = mapExprs(exprParser, game, ' ', area.locations || {});
    const exits = mapExprs(exprParser, game, ' ', area.exits || {});
    const events = mapExprs(exprParser, game, '_', area.events || {});
    const gossip = mapExprs(exprParser, game, ' ', area.gossip || {});

    if (name === undefined) {
      throw new Error(`Area name is undefined`);
    }

    if (region === undefined) {
      throw new Error(`Undefined region for area ${name}`);
    }

    if (DATA_REGIONS[region] === undefined) {
      throw new Error(`Unknown region ${region}`);
    }

    world.areas[name] = { boss, dungeon, exits, events, locations, gossip };

    if (dungeon) {
      if (world.dungeons[dungeon] === undefined) {
        world.dungeons[dungeon] = new Set();
      }
      const d = world.dungeons[dungeon];
      Object.keys(locations).forEach(x => d.add(x));
    }

    const worldGossip = { game };
    Object.keys(locations).forEach(x => world.regions[x] = region);
    Object.keys(gossip).forEach(x => world.gossip[x] = worldGossip);
  }
};

const loadWorldPool = (world: World, game: Game, settings: Settings) => {
  for (const record of DATA_POOL[game]) {
    const location = gameId(game, String(record.location), ' ');
    const type = String(record.type);
    const scene = gameId(game, String(record.scene), '_');
    let id = null;
    if (type === 'npc') {
      id = gameId(game, String(record.id), '_');
    } else {
      id = Number(record.id);
    }
    const item = gameId(game, String(record.item), '_');
    let hint = String(record.hint);
    if (hint !== 'NONE') {
      hint = gameId(game, hint, '_');
      if (world.checkHints[hint] === undefined) {
        world.checkHints[hint] = [];
      }
      world.checkHints[hint].push(location);
    }

    const check = { game, type, scene, id, item, hint } as WorldCheck;
    world.checks[location] = check;
  }
};

function loadWorldEntrances(world: World, game: Game) {
  for (const record of DATA_ENTRANCES[game]) {
    const from = gameId(game, String(record.from), ' ');
    const to = gameId(game, String(record.to), ' ');
    world.entrances.push({ from, to });
  }
}

const loadMacros = (exprParser: ExprParser, game: Game) => {
  const data = DATA_MACROS[game];
  for (let name in data) {
    const buffer = data[name];

    /* Horrible hack */
    name = name.replace('(', ' ');
    name = name.replace(')', ' ');
    name = name.replace(',', ' ');

    const parts = name.split(' ').filter(x => !!x);
    name = parts[0];
    const args = parts.slice(1);
    exprParser.addMacro(name, args, buffer);
  }
};

const loadWorldGame = (world: World, game: Game, settings: Settings) => {
  /* Create the expr parser */
  const exprParser = new ExprParser(settings, game);
  loadMacros(exprParser, game);
  loadWorldAreas(world, game, exprParser);
  loadWorldPool(world, game, settings);
  loadWorldEntrances(world, game);
}

export const createWorld = (settings: Settings) => {
  const world: World = { areas: {}, checks: {}, dungeons: {}, regions: {}, gossip: {}, checkHints: {}, entrances: [] };
  for (const g of GAMES) {
    loadWorldGame(world, g, settings);
  }
  return world;
};
