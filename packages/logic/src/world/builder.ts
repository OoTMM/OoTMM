import type { Game } from '@ootmm/data';
import type { Settings, SongEventSongs, EnemyPlanCompatibility } from '@ootmm/core';
import type { World, WorldArea, ExprMap, ExprParsers } from './types';
import type { Expr } from '../expr';

import { cloneDeep, mapValues } from 'lodash-es';
import { gameId, MACROS, WORLD, REGIONS, CHECKS, GAMES, GOSSIPS_BY_LOCATION } from '@ootmm/data';
import { Monitor, Random, SONG_EVENT_SONG_IDS, makeEnemyPlan, enemyDefinition, enemyPlacementLogicName, enemyLogicMacros, enemySoulSpecializations, enemyPlacementAreaEvents, enemyGoldLocationLogic } from '@ootmm/core';
import { itemByID, ItemHelpers } from '../items';
import { ExprParser, exprTrue, MM_TIME_SLICES } from '../expr';
import { defaultPrices } from '../price';
import { ootEnemizerWorldLogic, mmEnemyAreaLogic } from '../../../core/src/enemies';
import type { ResolvedWorldFlags } from './flags';
import { resolveWorldFlags } from './flags';

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

function cloneWorldArea(worldArea: WorldArea): WorldArea {
  return {
    exits: { ...worldArea.exits },
    events: { ...worldArea.events },
    locations: { ...worldArea.locations },
    gossip: { ...worldArea.gossip },
    stay: worldArea.stay ? [...worldArea.stay] : null,
    game: worldArea.game,
    boss: worldArea.boss,
    ageChange: worldArea.ageChange,
    dungeon: worldArea.dungeon,
    time: worldArea.time,
    region: worldArea.region,
  };
}

export function cloneWorld(world: World): World {
  return {
    areas: mapValues(world.areas, cloneWorldArea),
    dungeons: mapValues(world.dungeons, x => new Set(x)),
    dungeonsBossAreas: mapValues(world.dungeonsBossAreas, x => new Set(x)),
    regions: cloneDeep(world.regions),
    gossip: cloneDeep(world.gossip),
    locations: new Set(world.locations),
    songLocations: new Set(world.songLocations),
    warpLocations: new Set(world.warpLocations),
    enemyPlan: world.enemyPlan.map(e => ({ ...e })),
    prices: [...world.prices],
    songEventsOot: [...world.songEventsOot],
    songEventsMm: [...world.songEventsMm],
    preCompleted: new Set(world.preCompleted),
    bossIds: [...world.bossIds],
    entranceOverrides: new Map(world.entranceOverrides),
    entranceOverridesRev: new Map(world.entranceOverridesRev),
    resolvedFlags: world.resolvedFlags,
    exprParsers: world.exprParsers,
    dungeonsEntrances: new Map(world.dungeonsEntrances),
    checkItems: new Map(world.checkItems),
  };
}

type LogicPassWorldState = {
  monitor: Monitor,
  settings: Settings,
  random: Random;
  enemyCompatibility?: readonly EnemyPlanCompatibility[];
  resolvedWorldFlags?: readonly ResolvedWorldFlags[];
};

class LogicPassWorld {
  private world!: World;

  constructor(
    private readonly state: LogicPassWorldState,
  ){
  }

  run() {
    const worlds: World[] = [];
    this.state.monitor.log('Logic: World Building');

    for (let i = 0; i < this.state.settings.players; ++i) {
      worlds.push(this.createWorld(i));
    }

    return { worlds };
  }

  private createWorld(worldId: number): World {
    this.world = this.makeDefaultWorld(worldId);
    for (const g of GAMES) {
      if (this.state.settings.games === 'ootmm' || this.state.settings.games === g) {
        this.state.monitor.log(`Building ${g === 'oot' ? 'OoT' : 'MM'} world logic`);
        this.world.exprParsers[g].withMacroCache(() => this.loadGame(g));
      }
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

  private makeDefaultWorld(worldId: number): World {
    /* Resolve flags */
    const rolledFlags = resolveWorldFlags(this.state.settings, this.state.random);
    const resolvedFlags = this.state.resolvedWorldFlags?.[worldId] ?? rolledFlags;

    let lastMessage = '';
    const enemyPlan = makeEnemyPlan(this.state.settings, this.state.random, {
      compatibility: this.state.enemyCompatibility?.[worldId],
      mqDungeons: new Set(['DT', 'DC', 'JJ', 'Forest', 'Fire', 'Water', 'Spirit', 'Shadow', 'BotW', 'IC', 'GTG', 'Ganon'].filter(key => resolvedFlags.mqDungeons.has(key)))
    }, (message, completed, total) => {
      if (message !== lastMessage) {
        this.state.monitor.log(message);
        lastMessage = message;
      }
      if (completed !== undefined && total !== undefined)
        this.state.monitor.setProgress(completed, total);
    });
    this.state.monitor.log('Applying enemy requirements to logic');

    /* Create expr parsers */
    const exprParsers = {} as ExprParsers;
    for (const g of GAMES) {
      const parser = new ExprParser(this.state.settings, resolvedFlags, g);
      const macros = { ...MACROS.common, ...MACROS[g] };
      this.loadMacrosFile(parser, enemyLogicMacros(macros, enemyPlan, g));
      if (this.state.settings.logic === 'allLocations') for (const row of enemyPlan) {
        const name = enemyPlacementLogicName(row);
        if (name && enemyDefinition(row.source).game === g)
          parser.addEnemyMacro(name, row.target);
      }
      for (const specialization of enemySoulSpecializations(macros, enemyPlan, g))
        parser.addMacroSpecialization(specialization.name, specialization.args, specialization.buffer);
      exprParsers[g] = parser;
    }

    /* Expr parser settings */
    exprParsers.mm.addVar('STRAY_FAIRY_COUNT', this.state.settings.strayFairyRewardCount);

    /* Prices */
    const prices = defaultPrices(resolvedFlags);

    /* Song events */
    const songEventsOot = [
      'TIME',                /* SONG_EVENT_TEMPLE_OF_TIME                */
      'STORMS',              /* SONG_EVENT_WINDMILL                      */
      'ZELDAS_LULLABY',      /* SONG_EVENT_GRAVEYARD                     */
      'ZELDAS_LULLABY',      /* SONG_EVENT_ZORA_RIVER                    */
      'ZELDAS_LULLABY',      /* SONG_EVENT_GORON_CITY                    */
      'ZELDAS_LULLABY',      /* SONG_EVENT_GREAT_FAIRY_SPELL_WIND        */
      'ZELDAS_LULLABY',      /* SONG_EVENT_GREAT_FAIRY_SPELL_FIRE        */
      'ZELDAS_LULLABY',      /* SONG_EVENT_GREAT_FAIRY_SPELL_LOVE        */
      'ZELDAS_LULLABY',      /* SONG_EVENT_GREAT_FAIRY_UPGRADE_MAGIC     */
      'ZELDAS_LULLABY',      /* SONG_EVENT_GREAT_FAIRY_UPGRADE_MAGIC2    */
      'ZELDAS_LULLABY',      /* SONG_EVENT_GREAT_FAIRY_UPGRADE_DEFENSE   */
      'ZELDAS_LULLABY',      /* SONG_EVENT_TEMPLE_WATER                  */
      'ZELDAS_LULLABY',      /* SONG_EVENT_TEMPLE_SHADOW                 */
      'ZELDAS_LULLABY',      /* SONG_EVENT_TEMPLE_SPIRIT_STATUE          */
      'ZELDAS_LULLABY',      /* SONG_EVENT_TEMPLE_SPIRIT_LOWER           */
      'ZELDAS_LULLABY',      /* SONG_EVENT_TEMPLE_SPIRIT_HIGHER          */
      'ZELDAS_LULLABY',      /* SONG_EVENT_TEMPLE_BOTW                   */
      'ZELDAS_LULLABY',      /* SONG_EVENT_TEMPLE_GANON                  */
    ].map(x => SONG_EVENT_SONG_IDS[x as SongEventSongs]);

    const songEventsMm = [
      'SONATA',              /* SONG_EVENT_TEMPLE_WOODFALL       */
      'GORON_LULLABY',       /* SONG_EVENT_TEMPLE_SNOWHEAD       */
      'NEW_WAVE',            /* SONG_EVENT_TEMPLE_GREATBAY       */
      'HEALING',             /* SONG_EVENT_HEALING_POEHUT        */
      'HEALING',             /* SONG_EVENT_HEALING_DARMANI       */
      'HEALING',             /* SONG_EVENT_HEALING_PAMELA_FATHER */
      'HEALING',             /* SONG_EVENT_HEALING_KAMARO        */
      'HEALING',             /* SONG_EVENT_HEALING_MIKAU         */
      'SONATA',              /* SONG_EVENT_AWAKENING_KEETA       */
      'SONATA',              /* SONG_EVENT_AWAKENING_SCRUB       */
      'GORON_LULLABY_INTRO', /* SONG_EVENT_LULLABY_KID           */
      'STORMS',              /* SONG_EVENT_STORMS_COMPOSER       */
      'OATH',                /* SONG_EVENT_CLOCK_TOWER_ROOF      */
    ].map(x => SONG_EVENT_SONG_IDS[x as SongEventSongs]);

    return {
      enemyPlan,
      areas: {},
      dungeons: {},
      dungeonsBossAreas: {},
      regions: {},
      gossip: {},
      locations: new Set(),
      songLocations: new Set(),
      warpLocations: new Set(),
      prices,
      songEventsOot,
      songEventsMm,
      preCompleted: new Set(),
      bossIds: Object.values(BOSS_INDEX_BY_DUNGEON),
      entranceOverrides: new Map,
      entranceOverridesRev: new Map,
      resolvedFlags,
      exprParsers,
      dungeonsEntrances: new Map,
      checkItems: new Map,
    };
  }

  private loadGame(game: Game) {
    /* Create the expr parser */
    this.loadAreas(game, this.world.exprParsers[game]);
    this.loadChecks(game);
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
        const originalArea = areaSet[name];
        const area = game === 'oot' ? ootEnemizerWorldLogic(game, name, this.world.resolvedFlags.mqDungeons.has(areaSetName), originalArea, this.world.enemyPlan) :
          mmEnemyAreaLogic(name, originalArea, this.world.enemyPlan, areaSetName,
            this.world.resolvedFlags.jpLayouts.has(areaSetName) ? 'mm_jp' : 'mm_us');
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
        const locations = mapExprs(exprParser, game, ' ', enemyGoldLocationLogic(game, area.locations || {}, this.world.enemyPlan, originalArea.locations || {}));
        const exits = mapExprs(exprParser, game, ' ', area.exits || {});
        const events = mapExprs(exprParser, game, '_', { ...area.events, ...enemyPlacementAreaEvents(game, name.slice(game.length + 1), this.world.enemyPlan) });
        const gossip = mapExprs(exprParser, game, ' ', area.gossip || {});
        const time = area.time || 'still';
        let stay: Expr[] | null = null;

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

          if (boss) {
            if (this.world.dungeonsBossAreas[dungeon] === undefined) {
              this.world.dungeonsBossAreas[dungeon] = new Set();
            }
            this.world.dungeonsBossAreas[dungeon].add(name);
          }
        }

        for (const loc in locations) {
          const prevRegion = this.world.regions[loc];
          if (!prevRegion || prevRegion === 'NONE') {
            this.world.regions[loc] = region;
          }
          this.world.locations.add(loc);
        }

        for (const g in gossip) {
          const data = GOSSIPS_BY_LOCATION[g];
          if (!data) throw new Error(`Unknown gossip stone ${g}`);
          const worldGossip = { game, type: data.type };
          this.world.gossip[g] = worldGossip;
        }
      }
    }
  }

  private loadChecks(game: Game) {
    for (const check of CHECKS) {
      if (check.game !== game) continue;
      if (!this.world.locations.has(check.location))
        continue;

      const item = itemByID(check.item);
      if (ItemHelpers.isSong(item)) {
        this.world.songLocations.add(check.location);
      } else if (ItemHelpers.isDungeonReward(item)) {
        this.world.warpLocations.add(check.location);
      }
      this.world.checkItems.set(check.location, item);
    }
  }
}

export function logicPassWorld(state: LogicPassWorldState) {
  return new LogicPassWorld(state).run();
}
