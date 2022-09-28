import fs from 'fs/promises';
import path from 'path';

import glob from 'glob-promise';
import YAML from 'yaml';
import * as CSV from 'csv';

import { Game, GAMES, PATH_DATA } from '../config';
import { gameId } from '../util';
import { Expr } from './expr';
import { ExprParser } from './expr-parser';
import { createReadStream } from 'fs';

type ExprMap = {
  [k: string]: Expr;
}

type WorldRegion = {
  locations: ExprMap;
  exits: ExprMap;
  events: ExprMap;
};

export type WorldCheck = {
  game: Game;
  type: 'chest' | 'npc' | 'special' | 'collectible';
  scene: string;
  id: number;
  item: string;
};

export type World = {
  regions: {[k: string]: WorldRegion};
  checks: {[k: string]: WorldCheck};
  pool: string[];
  dungeons: {[k: string]: Set<string>};
};

const mapExprs = (exprParser: ExprParser, game: Game, data: any) => {
  const result: ExprMap = {};
  for (const [k, v] of Object.entries(data)) {
    let name = k;
    if (!(/^(MM|OOT) /.test(name))) {
      name = gameId(game, k, ' ');
    }
    result[name] = exprParser.parse(v as string);
  }
  return result;
}

const loadWorldRegions = async (world: World, game: Game, exprParser: ExprParser, filename: string) => {
  const text = await fs.readFile(filename, 'utf8');
  const data = YAML.parse(text) as any;

  for (let name in data) {
    const region = data[name];
    name = gameId(game, name, ' ');
    const dungeon = region.dungeon;
    const locations = mapExprs(exprParser, game, region.locations || {});
    const exits = mapExprs(exprParser, game, region.exits || {});
    const events = mapExprs(exprParser, game, region.events || {});

    if (name === undefined) {
      throw new Error(`Region name is undefined`);
    }

    world.regions[name] = { locations, exits, events };

    if (dungeon !== undefined) {
      if (world.dungeons[dungeon] === undefined) {
        world.dungeons[dungeon] = new Set();
      }
      const d = world.dungeons[dungeon];
      Object.keys(locations).forEach(x => d.add(x));
    }
  }
};

const loadWorldPool = async (world: World, game: Game, filename: string) => {
  const text = createReadStream(filename);
  const csvParser = CSV.parse({ columns: true, skip_empty_lines: true, trim: true });
  text.pipe(csvParser);
  for await (const record of csvParser) {
    const location = gameId(game, String(record.location), ' ');
    const type = String(record.type);
    const scene = gameId(game, String(record.scene), '_');
    const id = Number(record.id);
    const item = gameId(game, String(record.item), '_');

    const check = { game, type, scene, id, item } as WorldCheck;
    world.checks[location] = check;
    world.pool.push(item);
  }
};

const loadMacros = async (exprParser: ExprParser, filename: string) => {
  const text = await fs.readFile(filename, 'utf8');
  const data = YAML.parse(text) as any;
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

const loadWorldGame = async (world: World, game: Game) => {
  /* Create the expr parser */
  const exprParser = new ExprParser(game);
  await loadMacros(exprParser, path.join(PATH_DATA, game, 'macros.yml'));

  /* Load the world */
  const worldFiles = await glob(path.resolve(PATH_DATA, game, 'world', '*.yml'));
  await Promise.all(worldFiles.map(x => loadWorldRegions(world, game, exprParser, x)));
  const poolFile = path.resolve(PATH_DATA, game, 'pool.csv');
  await loadWorldPool(world, game, poolFile);
}

export const createWorld = async () => {
  const world: World = { regions: {}, checks: {}, pool: [], dungeons: {} };
  for (const g of GAMES) {
    await loadWorldGame(world, g);
  }
  return world;
};

