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
};

export type World = {
  regions: {[k: string]: WorldRegion};
};

const mapExprs = (exprParser: ExprParser, data: any) => {
  const result: ExprMap = {};
  for (const [k, v] of Object.entries(data)) {
    result[k] = exprParser.parse(v as string);
  }
  return result;
}

const loadWorldData = async (world: World, exprParser: ExprParser, filename: string) => {
  const text = await fs.readFile(filename, 'utf8');
  const data = JSON.parse(text) as any;

  for (const name in data) {
    const region = data[name];
    const locations = mapExprs(exprParser, region.locations || {});
    const exits = mapExprs(exprParser, region.exits || {});

    if (name === undefined) {
      throw new Error(`Region name is undefined`);
    }

    world.regions[name] = { locations, exits };
  }
};

const loadWorldGame = async (world: World, game: Game) => {
  const exprParser = new ExprParser();
  const worldFiles = await glob(path.resolve(PATH_DATA, game, 'world', '*.json'));
  await Promise.all(worldFiles.map(x => loadWorldData(world, exprParser, x)));
}

export const createWorld = async () => {
  const world: World = { regions: {} };
  await loadWorldGame(world, 'oot');
  return world;
};

