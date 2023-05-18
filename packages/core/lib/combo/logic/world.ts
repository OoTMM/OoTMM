import { Game, GAMES } from '../config';
import { gameId } from '../util';
import { Expr, exprTrue } from './expr';
import { ExprParser } from './expr-parser';
import { DATA_POOL, DATA_MACROS, DATA_WORLD, DATA_REGIONS, DATA_ENTRANCES, DATA_HINTS, DATA_HINTS_POOL } from '../data';
import { Settings } from '../settings';
import { Monitor } from '../monitor';
import { isDungeonReward, isSong } from './items';

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
  time: 'still' | 'day' | 'night' | 'flow';
};

type WorldCheckNumeric = {
  type: 'chest' | 'collectible' | 'gs' | 'sf' | 'cow' | 'shop' | 'scrub';
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
  type: 'gossip' | 'gossip-grotto' | 'gossip-moon';
};

export type WorldEntrance = {
  type: 'boss' | 'dungeon' | 'overworld';
  from: string;
  to: string;
  game: Game;
};

export type World = {
  areas: {[k: string]: WorldArea};
  checks: {[k: string]: WorldCheck};
  dungeons: {[k: string]: Set<string>};
  regions: {[k: string]: string};
  gossip: {[k: string]: WorldGossip};
  checkHints: {[k: string]: string[]};
  entrances: WorldEntrance[];
  locations: Set<string>;
  songLocations: Set<string>;
  warpLocations: Set<string>;
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

export type ExprParsers = {
  oot: ExprParser;
  mm: ExprParser;
}

export class LogicPassWorld {
  private world: World;
  private exprParsers: Partial<ExprParsers> = {};

  constructor(
    private readonly state: {
      monitor: Monitor,
      settings: Settings,
      mq: Set<string>;
    }
  ) {
    this.world = {
      areas: {},
      checks: {},
      dungeons: {},
      regions: {},
      gossip: {},
      checkHints: {},
      entrances: [],
      locations: new Set(),
      songLocations: new Set(),
      warpLocations: new Set(),
    };
  }

  run() {
    this.state.monitor.log('Logic: World Building');

    for (const g of GAMES) {
      this.loadGame(g);
    }

    return { world: this.world, exprParsers: this.exprParsers as ExprParsers };
  }

  private loadGame(game: Game) {
    /* Create the expr parser */
    const exprParser = new ExprParser(this.state.settings, game);
    this.exprParsers[game] = exprParser;
    this.loadMacros(game, exprParser);
    this.loadAreas(game, exprParser);
    this.loadPool(game);
    this.loadEntrances(game);
  }

  private loadMacros(game: Game, exprParser: ExprParser) {
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
  }

  private loadAreas(game: Game, exprParser: ExprParser) {
    const data = DATA_WORLD[game];
    for (let areaSetName in data) {
      let areaSet = (data as any)[areaSetName];
      /* Handle MQ */
      if (game === 'oot' && this.state.mq.has(areaSetName)) {
        areaSet = (DATA_WORLD.mq as any)[areaSetName];
      }
      for (let name in areaSet) {
        const area = areaSet[name];
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
        const time = area.time || 'still';

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

        if (DATA_REGIONS[region] === undefined) {
          throw new Error(`Unknown region ${region}`);
        }

        this.world.areas[name] = { game, boss, dungeon, exits, events, locations, gossip, time };

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
      const item = gameId(game, String(record.item), '_');
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

      if (isSong(item)) {
        this.world.songLocations.add(location);
      } else if (isDungeonReward(item)) {
        this.world.warpLocations.add(location);
      }
    }
  }

  private loadEntrances(game: Game) {
    for (const record of DATA_ENTRANCES[game]) {
      const from = gameId(game, String(record.from), ' ');
      const to = gameId(game, String(record.to), ' ');
      const type = String(record.type) as WorldEntrance['type'];
      this.world.entrances.push({ from, to, type, game });
    }
  }
}
