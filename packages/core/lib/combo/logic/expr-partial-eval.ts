import { Expr, EXPR_FALSE, EXPR_TRUE, ExprAge, exprAnd, ExprAnd, ExprOr, exprOr } from './expr';
import { Age } from './pathfind';

type ExprPartialEvalContext = {
  age?: Age;
};

const AGE_KEY = {
  child: 'C',
  adult: 'A',
};

function partialEvalKey(ctx: ExprPartialEvalContext): string {
  return ctx.age ? AGE_KEY[ctx.age] : 'X';
}

function exprPartialEvalImpl(expr: Expr, key: string, ctx: ExprPartialEvalContext): Expr {
  const cached = expr._cache[key];
  if (cached) {
    return cached;
  }

  let newExpr: Expr;
  if (expr instanceof ExprAnd) {
    const newExprs = expr.exprs.map(e => exprPartialEvalImpl(e, key, ctx));
    newExpr = exprAnd(newExprs);
  } else if (expr instanceof ExprOr) {
    const newExprs = expr.exprs.map(e => exprPartialEvalImpl(e, key, ctx));
    newExpr = exprOr(newExprs);
  } else if (expr instanceof ExprAge) {
    if (expr.age === ctx.age) {
      newExpr = EXPR_TRUE;
    } else {
      newExpr = EXPR_FALSE;
    }
  } else {
    newExpr = expr;
  }
  expr._cache[key] = newExpr;
  newExpr._cache[key] = newExpr;
  return newExpr;
}

export function exprPartialEval(expr: Expr, ctx: ExprPartialEvalContext): Expr {
  const key = partialEvalKey(ctx);
  return exprPartialEvalImpl(expr, key, ctx);
}
