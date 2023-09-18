import { Item } from '../items';
import { Expr, ExprAnd, ExprEvent, ExprHas, ExprOr } from './expr';
import { locationData, Location } from './locations';
import { Age } from './pathfind';
import { World } from './world';

type IRNodeTrue = { type: 'true' };
type IRNodeFalse = { type: 'false' };
type IRNodeItem = { type: 'item', item: Item, count: number };
type IRNodeOr = { type: 'or', children: IRNode[] };
type IRNodeAnd = { type: 'and', children: IRNode[] };
type IRNodeSymbolic = { type: 'symbolic', name: string };
type IRNode = IRNodeTrue | IRNodeFalse | IRNodeItem | IRNodeOr | IRNodeAnd | IRNodeSymbolic;

const IR_FALSE: IRNodeFalse = { type: 'false' };
const IR_TRUE: IRNodeTrue = { type: 'true' };

type IRPartialEvaluationContext = {
  age?: Age;
}

class LogicIR {
  private nameToNode = new Map<string, IRNode>();
  private symbols = new Map<string, IRNode>();
  private locations = new Map<string, IRNode>();

  constructor(
    private world: World,
  ) {
  }

  private exprContains(expr: Expr, pred: (x: Expr) => boolean): boolean {
    if (pred(expr))
      return true;
    if (expr.type === 'or') {
      return expr.exprs.some(x => this.exprContains(x, pred));
    }
    if (expr.type === 'and') {
      return expr.exprs.some(x => this.exprContains(x, pred));
    }
    return false;
  }

  private exprContainsAge(expr: Expr, age: Age): boolean {
    return this.exprContains(expr, x => x.type === 'age' && x.age === age);
  }

  private evaluationContextName(ctx: IRPartialEvaluationContext): string {
    const parts: string[] = [];
    if (ctx.age)
      parts.push(`age=${ctx.age}`);
    return parts.join(',');
  }

  private areaContextName(area: string, ctx: IRPartialEvaluationContext): string {
    const parts: string[] = [];
    parts.push(area);
    const ctxName = this.evaluationContextName(ctx);
    if (ctxName)
      parts.push(ctxName);
    return `A(${parts.join(',')})`;
  }

  private nodeName(node: IRNode): string {
    switch (node.type) {
    case 'true': return 't';
    case 'false': return 'f';
    case 'item': return `I(${node.item.id},${node.count})`;
    case 'or': return `OR(${node.children.map(c => this.nodeName(c)).join(',')})`;
    case 'and': return `AND(${node.children.map(c => this.nodeName(c)).join(',')})`;
    case 'symbolic': return node.name;
    }
  }

  private symbolify(node: IRNode): IRNode {
    const name = this.nodeName(node);
    const oldSym = this.nameToNode.get(name);
    if (oldSym)
      return oldSym;
    const id = this.symbols.size;
    const symName = `S(${id})`;
    const sym: IRNodeSymbolic = { type: 'symbolic', name: symName };
    this.symbols.set(symName, node);
    this.nameToNode.set(name, sym);
    return sym;
  }

  private or(nodes: IRNode[]): IRNode {
    const names = new Set<string>();
    const children: IRNode[] = [];

    if (nodes.length === 0)
      return IR_FALSE;

    if (nodes.some(c => c.type === 'true')) {
      return IR_TRUE;
    }

    /* Dedupe */
    for (const node of nodes) {
      const name = this.nodeName(node);
      if (names.has(name) || node.type === 'false')
        continue;
      names.add(name);
      children.push(node);
    }

    if (children.length === 0) {
      return IR_TRUE;
    } else if (children.length === 1) {
      return children[0];
    }

    /* Sort */
    children.sort((a, b) => this.nodeName(a).localeCompare(this.nodeName(b)));

    return this.symbolify({ type: 'or', children });
  }

  private and(nodes: IRNode[]): IRNode {
    const names = new Set<string>();
    const children: IRNode[] = [];

    if (nodes.length === 0) {
      return IR_TRUE;
    }

    if (nodes.some(a => a.type === 'false')) {
      return IR_FALSE;
    }

    /* Dedupe */
    for (const node of nodes) {
      const name = this.nodeName(node);
      if (names.has(name) || node.type === 'true')
        continue;
      names.add(name);
      children.push(node);
    }

    /* Simplify */
    if (children.length === 0) {
      return IR_TRUE;
    } else if (children.length === 1) {
      return children[0];
    }

    /* Sort */
    children.sort((a, b) => this.nodeName(a).localeCompare(this.nodeName(b)));

    return this.symbolify({ type: 'and', children });
  }

  private exprHasToIR(expr: ExprHas): IRNode {
    const { item, count } = expr;
    return { type: 'item', item, count };
  }

  private exprOrToIR(expr: ExprOr, ctx: IRPartialEvaluationContext): IRNode {
    const children: IRNode[] = [];
    for (const e of expr.exprs) {
      children.push(this.exprToIR(e, ctx));
    }
    return this.or(children);
  }

  private exprAndToIR(expr: ExprAnd, ctx: IRPartialEvaluationContext): IRNode {
    const children: IRNode[] = [];
    for (const e of expr.exprs) {
      children.push(this.exprToIR(e, ctx));
    }
    return this.and(children);
  }

  private exprToIR(expr: Expr, ctx: IRPartialEvaluationContext): IRNode {
    switch (expr.type) {
    case 'false': return IR_FALSE;
    case 'true': return IR_TRUE;
    case 'or': return this.exprOrToIR(expr, ctx);
    case 'and': return this.exprAndToIR(expr, ctx);
    case 'has': return this.exprHasToIR(expr);
    case 'age': return expr.age === ctx.age ? IR_TRUE : IR_FALSE;
    case 'event': return { type: 'symbolic', name: `E(${expr.event})` };
    case 'renewable': return IR_TRUE;
    case 'ootTime': return IR_TRUE;
    case 'mmTime': return IR_TRUE;
    case 'price': return IR_TRUE;
    case 'license': return IR_TRUE;
    case 'masks': return IR_TRUE;
    default: throw new Error(`Unknown expr type: ${expr.type}`);
    }
  }

  private exprAreaToIR(area: string, expr: Expr): IRNode {
    const variants: IRPartialEvaluationContext[] = [];
    const nodes: IRNode[] = [];
    const isVariantChild = this.exprContainsAge(expr, 'child');
    const isVariantAdult = this.exprContainsAge(expr, 'adult');
    if (isVariantChild) {
      variants.push({ age: 'child' });
    }
    if (isVariantAdult) {
      variants.push({ age: 'adult' });
    }
    if (!(isVariantChild && isVariantAdult)) {
      variants.push({});
    }
    for (const v of variants) {
      const areaName = this.areaContextName(area, v);
      const areaNode: IRNodeSymbolic = { type: 'symbolic', name: areaName };
      const exprNode = this.exprToIR(expr, v);
      const n = this.and([areaNode, exprNode]);
      nodes.push(n);
    }
    return this.or(nodes);
  }

  private compileLocation(loc: string) {
    const nodes: IRNode[] = [];
    for (const area in this.world.areas) {
      const a = this.world.areas[area];
      const expr = a.locations[loc];
      if (!expr)
        continue;
      nodes.push(this.exprAreaToIR(area, expr));
    }
    const node = this.or(nodes);
    this.locations.set(loc, node);
  }

  private compileEvent(event: string) {
    const nodes: IRNode[] = [];
    for (const area in this.world.areas) {
      const a = this.world.areas[area];
      const expr = a.events[event];
      if (!expr)
        continue;
      nodes.push(this.exprAreaToIR(area, expr));
    }
    const node = this.or(nodes);
    const symName = `E(${event})`;
    this.symbols.set(symName, node);
  }

  run() {
    const events = new Set<string>();
    for (const loc in this.world.checks) {
      this.compileLocation(loc);
    }

    for (const a in this.world.areas) {
      const area = this.world.areas[a];
      for (const e in area.events) {
        events.add(e);
      }
    }

    for (const e of events) {
      this.compileEvent(e);
    }

    for (const [loc, node] of this.locations.entries()) {
      console.log(`${loc}: ${this.nodeName(node)}`);
    }

    for (const [sym, node] of this.symbols.entries()) {
      console.log(`${sym}: ${this.nodeName(node)}`);
    }
  }
}

export function worldIR(world: World) {
  const builder = new LogicIR(world);
  builder.run();
  process.exit(0);
}
