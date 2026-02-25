import { PlayerItems, makePlayerItem } from '../items';
import { EXPR_FALSE, EXPR_TRUE, Expr, ExprAnd, ExprContainer, ExprHas, ExprLicense, ExprRenewable, exprAnd, exprOr } from './expr';
import { World } from './world';

function exprTransformSingle(expr: Expr, cb: (x: Expr) => Expr | null): Expr {
  const transformed = cb(expr);
  if (transformed) {
    return transformed;
  }
  return expr;
}

function exprTransform(expr: Expr, cb: (x: Expr) => Expr | null): Expr {
  if (expr instanceof ExprContainer) {
    const newExprs = expr.exprs.map(e => exprTransform(e, cb));
    if (expr instanceof ExprAnd) {
      return exprTransformSingle(exprAnd(newExprs), cb);
    } else {
      return exprTransformSingle(exprOr(newExprs), cb);
    }
  }

  return exprTransformSingle(expr, cb);
}

export function transformEvalExprMap(exprMap: Record<string, Expr>, cb: (e: Expr) => Expr | null): Record<string, Expr> {
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
    if (e instanceof ExprHas) {
      const pi = makePlayerItem(e.item, worldId);

      if ((startingItems.get(pi) || 0) >= e.count) {
        return EXPR_TRUE;
      }

      if ((allItems.get(pi) || 0) < e.count) {
        return EXPR_FALSE;
      }

      return null;
    }

    if (e instanceof ExprLicense) {
      const pi = makePlayerItem(e.item, worldId);

      if (startingItems.has(pi)) {
        return EXPR_TRUE;
      }

      if (!allItems.has(pi)) {
        return EXPR_FALSE;
      }

      return null;
    }

    if (e instanceof ExprRenewable) {
      const pi = makePlayerItem(e.item, worldId);

      if (!allItems.has(pi)) {
        return EXPR_FALSE;
      }

      return null;
    }

    return null;
  });
}
