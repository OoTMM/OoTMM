import { EXPR_FALSE, EXPR_TRUE, Expr, ExprAge, ExprAnd, ExprContainer, ExprOr, exprAge, exprAnd, exprOr } from './expr';
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

function exprPartialEval(expr: Expr, ctx: ExprPartialEvalContext): Expr {
  if (expr instanceof ExprContainer) {
    const newExprs = expr.exprs.map(e => exprPartialEval(e, ctx));
    if (expr instanceof ExprAnd) {
      return exprAnd(newExprs);
    } else {
      return exprOr(newExprs);
    }
  }

  if (expr instanceof ExprAge && ctx.age) {
    if (expr.age === ctx.age) {
      return EXPR_TRUE;
    } else {
      return EXPR_FALSE;
    }
  }

  return expr;
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
