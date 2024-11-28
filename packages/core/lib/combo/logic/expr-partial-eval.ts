import { Expr, exprAnd, ExprAnd, ExprOr, exprOr } from './expr';
import { Age } from './constants';

export function exprPartialEvalAge(expr: Expr, age: Age): Expr {
  const cached = expr._cacheAge[age];
  if (cached) {
    return cached;
  }

  let newExpr: Expr;
  if (expr instanceof ExprAnd) {
    const newExprs = expr.exprs.map(e => exprPartialEvalAge(e, age));
    newExpr = exprAnd(newExprs);
  } else if (expr instanceof ExprOr) {
    const newExprs = expr.exprs.map(e => exprPartialEvalAge(e, age));
    newExpr = exprOr(newExprs);
  } else {
    newExpr = expr;
  }
  expr._cacheAge[age] = newExpr;
  return newExpr;
}
