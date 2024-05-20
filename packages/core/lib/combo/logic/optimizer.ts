import { Items, ItemsCount, PlayerItems, makePlayerItem } from '../items';
import { EXPR_FALSE, EXPR_TRUE, Expr, ExprAge, ExprAnd, ExprContainer, ExprHas, ExprLicense, ExprRenewable, exprAge, exprAnd, exprOr } from './expr';
import { Age } from './pathfind';
import { World, WorldAreaExprs, WorldAreaExprsGraph } from './world';

type ExprPartialEvalContext = {
  age?: Age;
};

function exprComplexity(expr: Expr) {
  let c = 0;
  expr.visit(() => c++);
  return c;
}


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

function exprPartialEval(expr: Expr, ctx: ExprPartialEvalContext): Expr {
  return exprTransform(expr, (e) => {
    if (!(e instanceof ExprAge)) {
      return null;
    }

    if (e.age === ctx.age) {
      return EXPR_TRUE;
    } else {
      return EXPR_FALSE;
    }
  });
}

function exprOptBubbleAge(expr: Expr): Expr {
  if (!(expr instanceof ExprContainer)) {
    return expr;
  }

  let hasChild = false;
  let hasAdult = false;

  expr.visit((e) => {
    if (e instanceof ExprAge) {
      if (e.age === 'child') {
        hasChild = true;
      } else if (e.age === 'adult') {
        hasAdult = true;
      }
    }
  });

  if (!hasChild || !hasAdult) {
    return expr;
  }

  const childSubExpr = exprPartialEval(expr, { age: 'child' });
  const adultSubExpr = exprPartialEval(expr, { age: 'adult' });

  const newExpr = exprOr([
    exprAnd([exprAge('child'), childSubExpr]),
    exprAnd([exprAge('adult'), adultSubExpr]),
  ]);

  const prevComplexity = exprComplexity(expr);
  const newComplexity = exprComplexity(newExpr);

  if (prevComplexity < newComplexity) {
    return expr;
  }

  return newExpr;
}

export function optimizeExpr(e: Expr): Expr {
  e = exprOptBubbleAge(e);
  return e;
}

export function partialEvalExprMap(exprMap: Record<string, Expr>, ctx: ExprPartialEvalContext): Record<string, Expr> {
  const newExprMap: Record<string, Expr> = {};
  for (const k in exprMap) {
    const oldExpr = exprMap[k];
    const newExpr = exprPartialEval(oldExpr, ctx);
    newExprMap[k] = newExpr;
  }
  return newExprMap;
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

export function partialEvalWorldGraph(wg: WorldAreaExprsGraph, ctx: ExprPartialEvalContext) {
  const newGraph: WorldAreaExprsGraph = {};
  for (const area in wg) {
    const newExprs: WorldAreaExprs = {
      exits: partialEvalExprMap(wg[area].exits, ctx),
      events: partialEvalExprMap(wg[area].events, ctx),
      gossip: partialEvalExprMap(wg[area].gossip, ctx),
      locations: partialEvalExprMap(wg[area].locations, ctx),
      stay: null,
    };

    const oldStay = wg[area].stay;
    if (oldStay) {
      newExprs.stay = oldStay.map(x => exprPartialEval(x, ctx));
    }

    newGraph[area] = newExprs;
  }
  return newGraph;
}

export function optimizeWorld(world: World) {
  world.optimized = {
    child: partialEvalWorldGraph(world.areas, { age: 'child' }),
    adult: partialEvalWorldGraph(world.areas, { age: 'adult' }),
  };
}

function transformWorldExprs(world: World, cb: (e: Expr) => Expr | null) {
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
}

export function optimizeStartingAndPool(world: World, worldId: number, startingItems: PlayerItems, allItems: PlayerItems) {
  transformWorldExprs(world, (e) => {
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
