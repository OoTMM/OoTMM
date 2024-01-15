import { EXPR_FALSE, EXPR_TRUE, Expr, exprAge, exprAnd, exprOr } from './expr';
import { Age } from './pathfind';

type ExprPartialEvalContext = {
  age?: Age;
};

function visitExprs(expr: Expr, cb: (e: Expr) => void) {
  cb(expr);
  if (expr.type === 'or' || expr.type === 'and') {
    for (const e of expr.exprs) {
      visitExprs(e, cb);
    }
  }
}

function exprComplexity(expr: Expr) {
  let c = 0;
  visitExprs(expr, () => c++);
  return c;
}

function exprTree(op: 'and' | 'or', exprs: Expr[]): Expr {
  switch (op) {
  case 'and': return exprAnd(exprs);
  case 'or': return exprOr(exprs);
  }
}

function exprPartialEval(expr: Expr, ctx: ExprPartialEvalContext): Expr {
  if (expr.type === 'and' || expr.type === 'or') {
    const newExprs = expr.exprs.map(e => exprPartialEval(e, ctx));
    return exprTree(expr.type, newExprs);
  }

  if (expr.type === 'age' && ctx.age) {
    if (expr.age === ctx.age) {
      return EXPR_TRUE;
    } else {
      return EXPR_FALSE;
    }
  }

  return expr;
}

function exprOptBubbleAge(expr: Expr): Expr {
  if (expr.type !== 'and' && expr.type !== 'or') {
    return expr;
  }

  let hasChild = false;
  let hasAdult = false;

  visitExprs(expr, (e) => {
    if (e.type === 'age') {
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
