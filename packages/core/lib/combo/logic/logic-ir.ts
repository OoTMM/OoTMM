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

type AreaToCompile = {
  area: string;
  ctx: IRPartialEvaluationContext;
}

class LogicIR {
  private nameToNode = new Map<string, IRNode>();
  private symbols = new Map<string, IRNode>();
  private locations = new Map<string, IRNode>();
  private areasToCompile = new Map<string, AreaToCompile>();

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

  /* Resolves a symbol, inlining whenever possible */
  private symbol(name: string): IRNode {
    const names = new Set<string>(name);
    let node: IRNode | null = null;

    for (;;) {
      const candidate = this.symbols.get(name);
      if (!candidate)
        break;
      if (candidate.type === 'or' || candidate.type === 'and')
        break;
      node = candidate;
      if (candidate.type !== 'symbolic')
        break;
      if (names.has(candidate.name)) {
        console.log(`Warn: Symbolic loop detected ${name}`);
        break;
      }
      name = candidate.name;
      names.add(name);
    }

    if (!node) {
      node = { type: 'symbolic', name };
    }

    return node;
  }

  private area(area: string, ctx: IRPartialEvaluationContext): IRNode {
    if (area === 'OOT SPAWN') {
      return IR_TRUE;
    }

    const areaName = this.areaContextName(area, ctx);
    const areaNode = this.symbol(areaName);
    if (!this.symbols.has(areaName) && !this.areasToCompile.has(areaName)) {
      this.areasToCompile.set(areaName, { area, ctx });
    }
    return areaNode;
  }

  private orRaw(nodes: IRNode[]): IRNode {
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

    return { type: 'or', children };
  }

  private or(nodes: IRNode[]): IRNode {
    return this.symbolify(this.orRaw(nodes));
  }

  private andRaw(nodes: IRNode[]): IRNode {
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

    return { type: 'and', children };
  }

  private and(nodes: IRNode[]): IRNode {
    return this.symbolify(this.andRaw(nodes));
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
    case 'event': return this.symbol(`E(${expr.event})`);
    case 'renewable': return IR_TRUE;
    case 'ootTime': return IR_TRUE;
    case 'mmTime': return IR_TRUE;
    case 'price': return IR_TRUE;
    case 'license': return IR_TRUE;
    case 'masks': return IR_TRUE;
    case 'special': return IR_TRUE;
    }
  }

  private exprAreaVariantToIR(area: string, expr: Expr, ctx: IRPartialEvaluationContext) {
    const exprNode = this.exprToIR(expr, ctx);
    if (exprNode.type === 'false') {
      return IR_FALSE;
    }
    const areaNode = this.area(area, ctx);
    return this.and([areaNode, exprNode]);
  }

  private exprAreaToIR(area: string, expr: Expr): IRNode {
    /* Handle OoT Spawn */
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
      nodes.push(this.exprAreaVariantToIR(area, expr, v));
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

  private compileArea(area: string, ctx: IRPartialEvaluationContext) {
    const name = this.areaContextName(area, ctx);
    if (this.symbols.has(name))
      return;
    const nodes: IRNode[] = [];
    for (const a in this.world.areas) {
      const aData = this.world.areas[a];
      const expr = aData.exits[area];
      if (!expr)
        continue;
      if (!ctx.age) {
        const isVariantChild = this.exprContainsAge(expr, 'child');
        const isVariantAdult = this.exprContainsAge(expr, 'adult');
        if (isVariantChild) {
          nodes.push(this.area(a, { age: 'child' }));
        }
        if (isVariantAdult) {
          nodes.push(this.area(a, { age: 'adult' }));
        }
      }
      const node = this.exprAreaVariantToIR(a, expr, ctx);
      nodes.push(node);
    }
    const node = this.or(nodes);
    this.symbols.set(name, node);
  }

  private compileAreas() {
    for (;;) {
      const toCompile = Array.from(this.areasToCompile.values());
      this.areasToCompile.clear();
      if (toCompile.length === 0)
        break;
      for (const { area, ctx } of toCompile) {
        this.compileArea(area, ctx);
      }
    }
  }

  private replaceSymbolIn(name: string, node: IRNode, map: Map<string, IRNode>) {
    for (const [sym, symNode] of map.entries()) {
      if (symNode.type === 'symbolic' && symNode.name === name) {
        map.set(sym, node);
      }
      else if (symNode.type === 'or' || symNode.type === 'and') {
        let changed = false;
        for (let i = 0; i < symNode.children.length; i++) {
          const child = symNode.children[i];
          if (child.type === 'symbolic' && child.name === name) {
            symNode.children[i] = node;
            changed = true;
            break;
          }
        }
        if (changed) {
          if (symNode.type === 'or')
            map.set(sym, this.or(symNode.children));
          else
            map.set(sym, this.and(symNode.children));
        }
      }
    }
  }

  private replaceSymbol(name: string, node: IRNode) {
    this.symbols.delete(name);
    this.replaceSymbolIn(name, node, this.symbols);
    this.replaceSymbolIn(name, node, this.locations);
  }

  private inlineAll() {
    for (;;) {
      const syms = this.symbols.keys();
      let changed = false;
      for (const s of syms) {
        const node = this.symbols.get(s)!;
        if (node.type === 'or' || node.type === 'and')
          continue;
        changed = true;
        this.replaceSymbol(s, node);
      }
      if (!changed)
        break;
    }
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

    this.compileAreas();
    this.inlineAll();

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
