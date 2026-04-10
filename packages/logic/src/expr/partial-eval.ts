import type { Age } from '../age';
import type { Expr } from './types';

import { AGE_CHILD, AGE_ADULT } from '../age';
import { exprAnd, exprFalse, exprOr, exprTrue } from './builder';

let ageCache = {
  [AGE_CHILD]: [] as Expr[],
  [AGE_ADULT]: [] as Expr[],
};

export function exprPartialEvalAge(expr: Expr, age: Age): Expr {
  const cache = ageCache[age];
  const cached = cache[expr.id];
  if (cached) {
    return cached;
  }

  let newExpr: Expr;
  switch (expr.type) {
    case 'and': newExpr = exprAnd(expr.exprs.map(e => exprPartialEvalAge(e, age))); break;
    case 'or': newExpr = exprOr(expr.exprs.map(e => exprPartialEvalAge(e, age))); break;
    case 'age': newExpr = expr.age === age ? exprTrue() : exprFalse(); break;
    default: newExpr = expr; break;
  }

  cache[expr.id] = newExpr;
  return newExpr;
}
