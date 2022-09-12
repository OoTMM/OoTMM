import { Expr, exprTrue, exprFalse, exprAnd, exprOr } from './expr';

const SIMPLE_TOKENS = ['||', '&&', '(', ')', ',', 'true', 'false'] as const;

type TokenSimple = { type: typeof SIMPLE_TOKENS[number] };
type TokenEOF = { type: 'EOF' };
type TokenIdentifier = { type: 'identifier', value: string };
type TokenNumber = { type: 'number', value: number };
type Token = TokenSimple | TokenEOF | TokenIdentifier | TokenNumber;
type TokenType = Token['type'];
type TokenOfType<T> = Extract<Token, { type: T }>;
type TokenValue<T> = TokenOfType<T> extends { value: infer TT; } ? TT : true;

export class ExprParser {
  private buffer = "";
  private cursor = 0;
  private next?: Token;

  parse(input: string) {
    this.buffer = input;
    this.cursor = 0;
    this.next = undefined;

    const expr = this.parseExpr();
    if (expr === undefined) {
      throw new Error("Expected expression");
    }
    this.expect('EOF');
    return expr;
  }

  private parseExprTrue(): Expr | undefined {
    if (this.accept('true')) {
      return exprTrue();
    }
  }

  private parseExprFalse(): Expr | undefined {
    if (this.accept('false')) {
      return exprFalse();
    }
  }

  private parseExprSingle(): Expr | undefined {
    if (this.accept('(')) {
      const expr = this.parseExpr();
      this.expect(')');
      return expr;
    }
    return this.parseExprTrue() || this.parseExprFalse();
  }

  private parseExprOr(): Expr | undefined {
    const exprs: Expr[] = [];
    let expr = this.parseExprAnd();
    if (expr === undefined) {
      return undefined;
    }
    exprs.push(expr);
    while (this.accept('||')) {
      expr = this.parseExprAnd();
      if (expr === undefined) {
        throw new Error(`Expected expression after || at ${this.cursor}`);
      }
      exprs.push(expr);
    }
    if (exprs.length === 1) {
      return exprs[0];
    } else {
      return exprOr(exprs);
    }
  }

  private parseExprAnd(): Expr | undefined {
    const exprs: Expr[] = [];
    let expr = this.parseExprSingle();
    if (expr === undefined) {
      return undefined;
    }
    exprs.push(expr);
    while (this.accept('&&')) {
      expr = this.parseExprSingle();
      if (expr === undefined) {
        throw new Error(`Expected expression after && at ${this.cursor}`);
      }
      exprs.push(expr);
    }
    if (exprs.length === 1) {
      return exprs[0];
    } else {
      return exprAnd(exprs);
    }
  }

  private parseExpr(): Expr | undefined {
    return this.parseExprOr();
  }

  private skipWhitespace() {
    for (;;) {
      const c = this.buffer[this.cursor];
      if (c !== ' ' && c !== '\t' && c !== '\n') {
        break;
      }
      this.cursor++;
    }
  }

  private expect<T extends TokenType>(t: T): TokenValue<T> {
    const token = this.accept(t);
    if (token === undefined) {
      throw new Error(`Expected token ${t} at ${this.cursor}`);
    }
    return token;
  }

  private accept<T extends TokenType>(t: T): TokenValue<T> | undefined {
    const next = this.nextToken();
    if (next.type === t) {
      this.next = undefined;
      return (next as any).value === undefined ? true : (next as any).value;
    }
    return undefined;
  }

  private nextToken(): Token {
    if (!this.next) {
      this.next = this.parseNextToken();
    }
    return this.next;
  }

  private parseNextToken(): Token {
    /* Skip whitespace */
    this.skipWhitespace();

    /* Check for EOF */
    if (this.cursor >= this.buffer.length) {
      return { type: 'EOF' };
    }

    /* Check for a simple token */
    for (const t of SIMPLE_TOKENS) {
      const str = this.buffer.substring(this.cursor, this.cursor + t.length);
      if (str === t) {
        this.cursor += t.length;
        return { type: t };
      }
    }

    /* Check for an identifier */
    if (/[a-zA-Z_]/.test(this.buffer[this.cursor])) {
      const start = this.cursor;
      while (/[a-zA-Z0-9_]/.test(this.buffer[this.cursor])) {
        this.cursor++;
      }
      const str = this.buffer.substring(start, this.cursor);
      return { type: 'identifier', value: str };
    }

    /* Check for a number */
    if (/[0-9]/.test(this.buffer[this.cursor])) {
      const start = this.cursor;
      while (/[0-9]/.test(this.buffer[this.cursor])) {
        this.cursor++;
      }
      const str = this.buffer.substring(start, this.cursor);
      return { type: 'number', value: parseInt(str) };
    }

    /* Unknown token */
    throw new Error(`Unknown token at ${this.cursor}`);
  }
};
