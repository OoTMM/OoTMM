import { EXPR_FALSE, EXPR_TRUE, Expr, ExprAge, ExprAnd, ExprContainer, ExprOr, exprAge, exprAnd, exprOr } from './expr';
import { Age } from './pathfind';

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
