import { Item, ItemGroups, ItemsCount } from '../items';
import { Expr, ExprAnd, ExprFalse, ExprHas, ExprLicense, ExprMasks, ExprOr, ExprRenewable, ExprSpecial, ExprTrue } from './expr';
import { Age } from './pathfind';
import { World } from './world';

type IRNodeSymbolic = { type: 'symbolic', name: string };
type Primitive = ExprTrue | ExprFalse | ExprHas | ExprSpecial | ExprMasks | ExprLicense | ExprRenewable;
type IRPrimitive = Primitive | IRNodeSymbolic;

type NodeOr<T> = { type: 'or', children: T[] };
type NodeAnd<T> = { type: 'and', children: T[] };
type IRNode = IRPrimitive | NodeOr<IRPrimitive> | NodeAnd<IRPrimitive>;

type State = {
  atoms: Uint8Array;
  items: ItemsCount;
  itemsLicense: ItemsCount;
  itemsRenewable: ItemsCount;
};

type Atom = (state: State) => boolean;
type AtomEmit = { atom: Atom, deps: number[], depsItems: Item[] };

const IR_FALSE: ExprFalse = { type: 'false' };
const IR_TRUE: ExprTrue = { type: 'true' };

type IRPartialEvaluationContext = {
  age?: Age;
}

type AreaToCompile = {
  area: string;
  ctx: IRPartialEvaluationContext;
}

const META_EVENTS = new Set([
  'OOT_GANON',
  'MM_MAJORA',
  'OOT_GANON_PRE_BOSS',
  'MM_MAJORA_PRE_BOSS',
]);

export type CompiledWorld = {
  atoms: Atom[];
  locations: Map<string, number>;
  events: Map<string, number>;
  atomsToLocations: Map<number, string[]>;
  atomsToEvents: Map<number, string[]>;
  atomsToAtoms: Map<number, number[]>;
  itemsToAtoms: Map<Item, number[]>;
}

class WorldCompiler {
  private nameToNode = new Map<string, IRNodeSymbolic>();
  private symbols = new Map<string, IRNode>();
  private definedSymbols: Set<string> = new Set();
  private nextSymbolNum = 0;
  private locations = new Map<string, IRNode>();
  private events = new Map<string, IRNode>();
  private areasToCompile = new Map<string, AreaToCompile>();
  private atoms: Atom[] = [];
  private atomsCount = 0;
  private atomNames: Map<string, number> = new Map();

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
    case 'license': return `IL(${node.item.id})`;
    case 'renewable': return `IR(${node.item.id})`;
    case 'special': return `SP(${[...(node.items.map(x => x.id)), ...(node.itemsUnique.map(x => x.id)), node.count.toString()].join(',')})`;
    case 'masks': return `M(${node.count})`;
    case 'or': return `OR(${node.children.map(c => this.nodeName(c)).join(',')})`;
    case 'and': return `AND(${node.children.map(c => this.nodeName(c)).join(',')})`;
    case 'symbolic': return node.name;
    }
  }

  private symbolify(node: IRNode): IRPrimitive {
    const name = this.nodeName(node);
    const oldSym = this.nameToNode.get(name);
    if (oldSym)
      return oldSym;
    const id = this.nextSymbolNum++;
    const symName = `S(${id})`;
    const sym: IRNodeSymbolic = { type: 'symbolic', name: symName };
    this.symbols.set(symName, node);
    this.nameToNode.set(name, sym);
    return sym;
  }

  /* Resolves a symbol, inlining whenever possible */
  private symbol(name: string): IRPrimitive {
    this.definedSymbols.add(name);
    return { type: 'symbolic', name };
  }

  private area(area: string, ctx: IRPartialEvaluationContext): IRPrimitive {
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

  private orRaw(nodes: IRPrimitive[]): IRNode {
    const names = new Set<string>();
    const children: IRPrimitive[] = [];

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
      return IR_FALSE;
    } else if (children.length === 1) {
      return children[0];
    }

    /* Sort */
    children.sort((a, b) => this.nodeName(a).localeCompare(this.nodeName(b)));

    return { type: 'or', children };
  }

  private or(nodes: IRPrimitive[]): IRPrimitive {
    return this.symbolify(this.orRaw(nodes));
  }

  private andRaw(nodes: IRPrimitive[]): IRNode {
    const names = new Set<string>();
    const children: IRPrimitive[] = [];

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

  private and(nodes: IRPrimitive[]): IRPrimitive {
    return this.symbolify(this.andRaw(nodes));
  }

  private exprOrToIR(expr: ExprOr, ctx: IRPartialEvaluationContext): IRPrimitive {
    const children: IRPrimitive[] = [];
    for (const e of expr.exprs) {
      children.push(this.exprToIR(e, ctx));
    }
    return this.or(children);
  }

  private exprAndToIR(expr: ExprAnd, ctx: IRPartialEvaluationContext): IRPrimitive {
    const children: IRPrimitive[] = [];
    for (const e of expr.exprs) {
      children.push(this.exprToIR(e, ctx));
    }
    return this.and(children);
  }

  private exprToIR(expr: Expr, ctx: IRPartialEvaluationContext): IRPrimitive {
    switch (expr.type) {
    case 'false': return IR_FALSE;
    case 'true': return IR_TRUE;
    case 'or': return this.exprOrToIR(expr, ctx);
    case 'and': return this.exprAndToIR(expr, ctx);
    case 'age': return expr.age === ctx.age ? IR_TRUE : IR_FALSE;
    case 'event': return this.symbol(`E(${expr.event})`);
    case 'ootTime': return IR_TRUE;
    case 'mmTime': return IR_TRUE;
    case 'price': return IR_TRUE;
    case 'renewable':
    case 'license':
    case 'masks':
    case 'item':
    case 'special':
      return expr;
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

  private exprAreaToIR(area: string, expr: Expr): IRPrimitive {
    const variants: IRPartialEvaluationContext[] = [];
    const nodes: IRPrimitive[] = [];
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
    const nodes: IRPrimitive[] = [];
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
    const nodes: IRPrimitive[] = [];
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
    const s = this.symbol(symName);

    if (META_EVENTS.has(event)) {
      this.events.set(event, s);
    }
  }

  private compileArea(area: string, ctx: IRPartialEvaluationContext) {
    const name = this.areaContextName(area, ctx);
    if (this.symbols.has(name))
      return;
    const nodes: IRPrimitive[] = [];
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

  private replaceSymbolIn(name: string, node: IRPrimitive, map: Map<string, IRNode>) {
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
            map.set(sym, this.orRaw(symNode.children));
          else
            map.set(sym, this.andRaw(symNode.children));
        }
      }
    }
  }

  private replaceSymbol(name: string, node: IRPrimitive) {
    this.symbols.delete(name);
    this.replaceSymbolIn(name, node, this.symbols);
    this.replaceSymbolIn(name, node, this.locations);
    this.replaceSymbolIn(name, node, this.events);
  }

  private inlineAll() {
    /* Define false values for all undefined symbols */
    for (const s of this.definedSymbols.keys()) {
      if (!this.symbols.has(s)) {
        this.symbols.set(s, IR_FALSE);
      }
    }

    for (;;) {
      const syms = this.symbols.keys();
      let changed = false;
      for (const s of syms) {
        let node = this.symbols.get(s)!;
        if (node.type === 'or' || node.type === 'and')
          continue;
        /* Remove loops */
        if (node.type === 'symbolic' && node.name === s) {
          node = IR_FALSE;
        }
        changed = true;
        this.replaceSymbol(s, node);
      }
      if (!changed)
        break;
    }
  }

  private resolveAtomConstant(c: boolean): AtomEmit {
    return { atom: () => c, deps: [], depsItems: [] };
  }

  private resolveAtomItemLicense(item: Item): AtomEmit {
    const atom = (state: State) => state.itemsLicense.has(item);
    return { atom, deps: [], depsItems: [item] };
  }

  private resolveAtomItemRenewable(item: Item): AtomEmit {
    const atom = (state: State) => state.itemsRenewable.has(item);
    return { atom, deps: [], depsItems: [item] };
  }

  private resolveAtomItem(item: Item, count: number): AtomEmit {
    const atom = (state: State) => (state.items.get(item) || 0) >= count;
    return { atom, deps: [], depsItems: [item] };
  }

  private resolveAtomMasks(count: number): AtomEmit {
    const depsItems = [...ItemGroups.MASKS_REGULAR];
    const atom = (state: State) => {
      let acc = 0;
      for (const mask of ItemGroups.MASKS_REGULAR) {
        if (state.items.get(mask))
          acc++;
      }
      return acc >= count;
    };

    return { atom, deps: [], depsItems };
  }

  private resolveAtomSpecial(items: Item[], itemsUnique: Item[], count: number): AtomEmit {
    const depsItems = [...items, ...itemsUnique];
    const atom = (state: State) => {
      let acc = 0;
      for (const item of items) {
        count += state.items.get(item) || 0;
      }
      for (const item of itemsUnique) {
        if (state.items.get(item))
          acc++;
      }
      return acc >= count;
    };

    return { atom, deps: [], depsItems };
  }

  private resolveAtomOr(children: number[]): AtomEmit {
    const atom: Atom = (state) => {
      for (const c of children) {
        const bytePos = c >> 3;
        const bitPos = c & 7;
        if (state.atoms[bytePos] & (1 << bitPos))
          return true;
      }
      return false;
    };

    return { atom, deps: children, depsItems: [] };
  }

  private resolveAtomAnd(children: number[]): AtomEmit {
    const atom: Atom = (state) => {
      for (const c of children) {
        const bytePos = c >> 3;
        const bitPos = c & 7;
        if (!(state.atoms[bytePos] & (1 << bitPos)))
          return false;
      }
      return true;
    };

    return { atom, deps: children, depsItems: [] };
  }

  private resolveAtom(node: Exclude<IRNode, IRNodeSymbolic>, atomsToAtoms: Map<number, number[]>, itemsToAtoms: Map<Item, number[]>): AtomEmit {
    switch (node.type) {
    case 'true': return this.resolveAtomConstant(true);
    case 'false': return this.resolveAtomConstant(false);
    case 'item': return this.resolveAtomItem(node.item, node.count);
    case 'special': return this.resolveAtomSpecial(node.items, node.itemsUnique, node.count);
    case 'masks': return this.resolveAtomMasks(node.count);
    case 'license': return this.resolveAtomItemLicense(node.item);
    case 'renewable': return this.resolveAtomItemRenewable(node.item);
    case 'or': return this.resolveAtomOr(node.children.map(c => this.emitAtom(c, atomsToAtoms, itemsToAtoms)));
    case 'and': return this.resolveAtomAnd(node.children.map(c => this.emitAtom(c, atomsToAtoms, itemsToAtoms)));
    }
  }

  private emitAtom(node: IRNode, atomsToAtoms: Map<number, number[]>, itemsToAtoms: Map<Item, number[]>): number {
    /* Resolve */
    while (node.type === 'symbolic') {
      const n = this.symbols.get(node.name)!;
      node = n;
    }

    const str = this.nodeName(node);
    let id = this.atomNames.get(str);
    if (id !== undefined)
      return id;
    id = this.atomsCount++;
    this.atomNames.set(str, id);
    const res = this.resolveAtom(node, atomsToAtoms, itemsToAtoms);
    this.atoms[id] = res.atom;
    for (const c of res.deps) {
      const a: number[] = atomsToAtoms.get(c) || [];
      a.push(id);
      atomsToAtoms.set(c, a);
    }
    for (const i of res.depsItems) {
      const a: number[] = itemsToAtoms.get(i) || [];
      a.push(id);
      itemsToAtoms.set(i, a);
    }
    return id;
  }

  private compile(): CompiledWorld {
    const locations = new Map<string, number>();
    const events = new Map<string, number>();
    const atomsToLocations = new Map<number, string[]>();
    const atomsToEvents = new Map<number, string[]>();
    const atomsToAtoms = new Map<number, number[]>();
    const itemsToAtoms = new Map<Item, number[]>();

    /* DEBUG */
    /*
    for (const [sym, node] of this.symbols.entries()) {
      console.log(sym, this.nodeName(node));
    }
    for (const [loc, node] of this.locations.entries()) {
      console.log(loc, this.nodeName(node));
    }
    */

    this.emitAtom(IR_FALSE, atomsToAtoms, itemsToAtoms);
    this.emitAtom(IR_TRUE, atomsToAtoms, itemsToAtoms);

    for (const [loc, node] of this.locations.entries()) {
      locations.set(loc, this.emitAtom(node, atomsToAtoms, itemsToAtoms));
    }

    for (const [loc, node] of this.events.entries()) {
      events.set(loc, this.emitAtom(node, atomsToAtoms, itemsToAtoms));
    }

    const atoms = this.atoms;
    this.atoms = [];

    /* Build the dependencies */
    for (const [loc, atomId] of locations.entries()) {
      const a = atomsToLocations.get(atomId) || [];
      a.push(loc);
      atomsToLocations.set(atomId, a);
    }

    for (const [ev, atomId] of events.entries()) {
      const a = atomsToEvents.get(atomId) || [];
      a.push(ev);
      atomsToEvents.set(atomId, a);
    }

    return { atoms, locations, events, atomsToLocations, atomsToEvents, atomsToAtoms, itemsToAtoms };
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
    return this.compile();
  }
}

export function compileWorld(world: World) {
  console.log("World Compile...");
  const compiler = new WorldCompiler(world);
  const data = compiler.run();
  console.log("World Compile Done");
  return data;
}
