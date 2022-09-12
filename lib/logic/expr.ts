export type ExprArgs = {};
export type Expr = (args: ExprArgs) => boolean;

export const exprTrue = (): Expr => (args: ExprArgs) => true;
export const exprFalse = (): Expr => (args: ExprArgs) => false;
export const exprAnd = (exprs: Expr[]): Expr => (args: ExprArgs) => exprs.every(expr => expr(args));
export const exprOr = (exprs: Expr[]): Expr => (args: ExprArgs) => exprs.some(expr => expr(args));
