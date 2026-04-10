import type { PlayerItems } from '@ootmm/core';
import type { Expr } from '@ootmm/logic';
import type { World } from './world';

import { makePlayerItem } from '@ootmm/core';
import { exprAnd, exprFalse, exprOr, exprTrue } from '@ootmm/logic';

function exprTransformSingle(expr: Expr, cb: (x: Expr) => Expr | null): Expr {
  const transformed = cb(expr);
  if (transformed) {
    return transformed;
  }
  return expr;
}

function exprTransform(expr: Expr, cb: (x: Expr) => Expr | null): Expr {
  switch (expr.type) {
    case 'and': return exprTransformSingle(exprAnd(expr.exprs.map(e => exprTransform(e, cb))), cb);
    case 'or': return exprTransformSingle(exprOr(expr.exprs.map(e => exprTransform(e, cb))), cb);
    default: return exprTransformSingle(expr, cb);
  }
}

function transformEvalExprMap(exprMap: Record<string, Expr>, cb: (e: Expr) => Expr | null): Record<string, Expr> {
  const newExprMap: Record<string, Expr> = {};
  for (const k in exprMap) {
    const oldExpr = exprMap[k];
    const newExpr = exprTransform(oldExpr, cb);
    newExprMap[k] = newExpr;
  }
  return newExprMap;
}

function transformWorldExprs(world: World, cb: (e: Expr) => Expr | null): World {
  for (const name in world.areas) {
    const area = world.areas[name];
    area.exits = transformEvalExprMap(area.exits, cb);
    area.events = transformEvalExprMap(area.events, cb);
    area.gossip = transformEvalExprMap(area.gossip, cb);
    area.locations = transformEvalExprMap(area.locations, cb);

    if (area.stay) {
      area.stay = area.stay.map(x => exprTransform(x, cb));
    }
  }

  return world;
}

export function optimizeWorldStartingAndPool(world: World, worldId: number, startingItems: PlayerItems, allItems: PlayerItems): World {
  return transformWorldExprs(world, (e) => {
    if (e.type === 'item') {
      const pi = makePlayerItem(e.item, worldId);

      if ((startingItems.get(pi) || 0) >= e.count) {
        return exprTrue();
      }

      if ((allItems.get(pi) || 0) < e.count) {
        return exprFalse();
      }

      return null;
    }

    if (e.type === 'license') {
      const pi = makePlayerItem(e.item, worldId);

      if (startingItems.has(pi)) {
        return exprTrue();
      }

      if (!allItems.has(pi)) {
        return exprFalse();
      }

      return null;
    }

    if (e.type === 'renewable') {
      const pi = makePlayerItem(e.item, worldId);

      if (!allItems.has(pi)) {
        return exprFalse();
      }

      return null;
    }

    return null;
  });
}
