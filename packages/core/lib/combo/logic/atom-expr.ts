import { Item } from '../items';
import { Expr, ExprAnd, ExprEvent, ExprHas, ExprOr } from './expr';
import { locationData, Location } from './locations';
import { World } from './world';

type AtomTrue = { type: 'true' };
type AtomFalse = { type: 'false' };
type AtomItem = { type: 'item', item: Item, count: number };
type AtomOr = { type: 'or', atoms: number[] };
type AtomAnd = { type: 'and', atoms: number[] };
type Atom = AtomTrue | AtomFalse | AtomItem | AtomOr | AtomAnd;

const ATOM_FALSE: AtomFalse = { type: 'false' };
const ATOM_TRUE: AtomTrue = { type: 'true' };

class AtomExprSetBuilder {
  private atoms: Atom[] = [];
  private atomMap: Map<string, number> = new Map();

  constructor(
    private world: World,
  ) {
    this.atoms.push(ATOM_FALSE);
    this.atoms.push(ATOM_TRUE);
  }

  private exprSymbolicNameOr(expr: ExprOr): string {
    const atoms = expr.exprs.map(e => this.exprSymbolicName(e));
    return `OR(${atoms.sort().join(',')})`;
  }

  private exprSymbolicNameAnd(expr: ExprAnd): string {
    const atoms = expr.exprs.map(e => this.exprSymbolicName(e));
    return `AND(${atoms.sort().join(',')})`;
  }

  private exprSymbolicName(expr: Expr): string {
    switch (expr.type) {
    case 'false': return 'f';
    case 'true': return 't';
    case 'or': return this.exprSymbolicNameOr(expr);
    case 'and': return this.exprSymbolicNameAnd(expr);
    case 'event': return `E(${expr.event})`;
    case 'age': return `AGE(${expr.age})`;
    case 'renewable': return `RENEWABLE(${expr.item.id})`;
    case 'ootTime': return `OOT_TIME(${expr.time})`;
    case 'mmTime': return `MM_TIME`;
    case 'price': return `PRICE`;
    case 'license': return `LICENSE`;
    case 'masks': return `MASKS(${expr.count})`;
    case 'has': return `I(${expr.item.id}, ${expr.count})`;
    default: throw new Error(`Unknown expr type: ${expr.type}`);
    }
  }

  private exprFalseToAtom(): number {
    return 0;
  }

  private exprTrueToAtom(): number {
    return 1;
  }

  private atomKey(key: string, cb: () => Atom) {
    let atomId = this.atomMap.get(key);
    if (atomId !== undefined) {
      return atomId;
    }
    atomId = this.atoms.length;
    this.atoms.push(ATOM_FALSE);
    this.atomMap.set(key, atomId);
    const atom = cb();
    this.atoms[atomId] = atom;
    return atomId;
  }

  private exprHasToAtom(expr: ExprHas): number {
    const { item, count } = expr;
    const key = `I(${item.id}, ${count})`;
    return this.atomKey(key, () => ({
      type: 'item',
      item,
      count,
    }));
  }

  private exprOrToAtom(area: string, expr: ExprOr): number {
    const atoms: number[] = [];
    for (const e of expr.exprs) {
      atoms.push(this.exprToAtom(area, e));
    }
    return this.or(atoms);
  }

  private exprAndToAtom(area: string, expr: ExprAnd): number {
    const atoms: number[] = [];
    for (const e of expr.exprs) {
      atoms.push(this.exprToAtom(area, e));
    }
    return this.and(atoms);
  }

  private exprToAtom(area: string, expr: Expr): number {
    switch (expr.type) {
    case 'false': return this.exprFalseToAtom();
    case 'true': return this.exprTrueToAtom();
    case 'or': return this.exprOrToAtom(area, expr);
    case 'and': return this.exprAndToAtom(area, expr);
    case 'has': return this.exprHasToAtom(expr);
    case 'age': return this.exprTrueToAtom();
    case 'event': return this.exprTrueToAtom();
    case 'renewable': return this.exprTrueToAtom();
    case 'ootTime': return this.exprTrueToAtom();
    case 'mmTime': return this.exprTrueToAtom();
    case 'price': return this.exprTrueToAtom();
    case 'license': return this.exprTrueToAtom();
    case 'masks': return this.exprTrueToAtom();
    default: throw new Error(`Unknown expr type: ${expr.type}`);
    }
  }

  private or(atoms: number[]): number {
    const unique = new Set(atoms);

    if (unique.size === 0) {
      return this.exprFalseToAtom();
    }

    if (unique.size === 1) {
      return atoms[0];
    }

    const sorted = [...unique].sort((a, b) => a - b);
    const key = `OR(${sorted.join(',')})`;
    return this.atomKey(key, () => ({
      type: 'or',
      atoms: sorted,
    }));
  }

  private and(atoms: number[]): number {
    const unique = new Set(atoms);

    if (unique.size === 0) {
      return this.exprTrueToAtom();
    }

    if (unique.size === 1) {
      return atoms[0];
    }

    const sorted = [...unique].sort((a, b) => a - b);
    const key = `AND(${sorted.join(',')})`;
    return this.atomKey(key, () => ({
      type: 'and',
      atoms: sorted,
    }));
  }

  private event(event: string): number {
    const key = `E(${event})`;
    return this.atomKey(key, () => {
      return ATOM_TRUE;
    });
  }

  private area(area: string): number {
    const key = `A(${area})`;
    return this.atomKey(key, () => {
      return ATOM_TRUE;
    });
  }

  private location(loc: string): number {
    const atoms: number[] = [];
    for (const area in this.world.areas) {
      const a = this.world.areas[area];
      const expr = a.locations[loc];
      if (!expr)
        continue;
      atoms.push(this.exprToAtom(area, expr));
    }
    return this.and(atoms);
  }

  run() {
    const events = new Set<string>();
    for (const area in this.world.areas) {
      const a = this.world.areas[area];
      for (const event in a.events) {
        events.add(event);
      }
    }

    for (const area in this.world.areas) {
      this.area(area);
    }

    for (const loc in this.world.checks) {
      this.location(loc);
    }

    for (const event of events) {
      this.event(event);
    }

    for (const k of this.atomMap.keys()) {
      console.log(k, this.atomMap.get(k));
    }
    //console.log(this.atomMap);
  }
}

export function worldAtomExprSet(world: World): Atom[] {
  const builder = new AtomExprSetBuilder(world);
  builder.run();
  process.exit(0);
}
