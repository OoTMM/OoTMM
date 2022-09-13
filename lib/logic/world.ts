import fs from 'fs/promises';

import glob from 'glob-promise';
import path from 'path';

import { Game, PATH_DATA } from '../config';
import { Expr } from './expr';
import { ExprParser } from './expr-parser';

type ExprMap = {
  [k: string]: Expr;
}

type WorldRegion = {
  locations: ExprMap;
  exits: ExprMap;
  events: ExprMap;
};

type WorldCheck = {
  type: 'chest';
  sceneId: number;
  id: number;
  item: string;
};

export type World = {
  regions: {[k: string]: WorldRegion};
  checks: {[k: string]: WorldCheck};
  pool: string[];
  dungeons: {[k: string]: Set<string>};
};

const mapExprs = (exprParser: ExprParser, data: any) => {
  const result: ExprMap = {};
  for (const [k, v] of Object.entries(data)) {
    result[k] = exprParser.parse(v as string);
  }
  return result;
}

const loadWorldRegions = async (world: World, exprParser: ExprParser, filename: string) => {
  const text = await fs.readFile(filename, 'utf8');
  const data = JSON.parse(text) as any;

  for (const name in data) {
    const region = data[name];
    const dungeon = region.dungeon;
    const locations = mapExprs(exprParser, region.locations || {});
    const exits = mapExprs(exprParser, region.exits || {});
    const events = mapExprs(exprParser, region.events || {});

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

const loadWorldPool = async (world: World, filename: string) => {
  const text = await fs.readFile(filename, 'utf8');
  const data = JSON.parse(text) as any;
  for (const location in data) {
    const d = data[location];
    const item = d[3];
    const check = { type: d[0], sceneId: parseInt(d[1], 16), id: parseInt(d[2], 16), item } as WorldCheck;
    world.checks[location] = check;
    world.pool.push(item);
  }
};

const loadMacros = async (exprParser: ExprParser, filename: string) => {
  const text = await fs.readFile(filename, 'utf8');
  const data = JSON.parse(text) as any;
  for (let name in data) {
    const buffer = data[name];

    /* Horrible hack */
    name = name.replace('(', ' ');
    name = name.replace(')', ' ');
    name = name.replace(',', ' ');

    const parts = name.split(' ');
    name = parts[0];
    const args = parts.slice(1);
    exprParser.addMacro(name, args, buffer);
  }
};

const loadWorldGame = async (world: World, game: Game) => {
  /* Create the expr parser */
  const exprParser = new ExprParser();
  await loadMacros(exprParser, path.join(PATH_DATA, game, 'macros.json'));

  /* Load the world */
  const worldFiles = await glob(path.resolve(PATH_DATA, game, 'world', '*.json'));
  await Promise.all(worldFiles.map(x => loadWorldRegions(world, exprParser, x)));
  const poolFile = path.resolve(PATH_DATA, game, 'pool.json');
  await loadWorldPool(world, poolFile);
}

export const createWorld = async () => {
  const world: World = { regions: {}, checks: {}, pool: [], dungeons: {} };
  await loadWorldGame(world, 'oot');
  return world;
};

