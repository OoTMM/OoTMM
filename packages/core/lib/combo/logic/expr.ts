import { Item, ItemGroups, Items, ItemsCount } from '../items';
import { ItemID } from '../items/defs';
import { GLITCHES, SETTINGS, Settings, TRICKS } from '../settings';
import { Age } from './pathfind';
import { PRICE_RANGES } from './price';
import { ResolvedWorldFlags, WORLD_FLAGS, World } from './world';

export const MM_TIME_SLICES = [
  'DAY1_AM_06_00',
  'DAY1_AM_07_00',
  'DAY1_AM_08_00',
  'DAY1_AM_10_00',
  'DAY1_PM_01_45',
  'DAY1_PM_03_00',
  'DAY1_PM_04_00',
  'NIGHT1_PM_06_00',
  'NIGHT1_PM_08_00',
  'NIGHT1_PM_09_00',
  'NIGHT1_PM_10_00',
  'NIGHT1_PM_11_00',
  'NIGHT1_AM_12_00',
  'NIGHT1_AM_02_30',
  'NIGHT1_AM_04_00',
  'NIGHT1_AM_05_00',
  'DAY2_AM_06_00',
  'DAY2_AM_07_00',
  'DAY2_AM_08_00',
  'DAY2_AM_10_00',
  'DAY2_AM_11_30',
  'DAY2_PM_02_00',
  'NIGHT2_PM_06_00',
  'NIGHT2_PM_08_00',
  'NIGHT2_PM_09_00',
  'NIGHT2_PM_10_00',
  'NIGHT2_PM_11_00',
  'NIGHT2_AM_12_00',
  'NIGHT2_AM_04_00',
  'NIGHT2_AM_05_00',
  'DAY3_AM_06_00',
  'DAY3_AM_07_00',
  'DAY3_AM_08_00',
  'DAY3_AM_10_00',
  'DAY3_AM_11_30',
  'DAY3_PM_01_00',
  'NIGHT3_PM_06_00',
  'NIGHT3_PM_08_00',
  'NIGHT3_PM_09_00',
  'NIGHT3_PM_10_00',
  'NIGHT3_PM_11_00',
  'NIGHT3_AM_12_00',
  'NIGHT3_AM_04_00',
  'NIGHT3_AM_05_00',
];

//export type Expr = (state: State) => ExprResult;

type ExprBase = { readonly key: string; readonly type: string };
export type Expr = ExprTrue | ExprFalse | ExprAnd | ExprOr | ExprAge | ExprHas | ExprRenewable | ExprLicense | ExprEvent | ExprMasks | ExprSpecial | ExprTimeOot | ExprTimeMm | ExprPrice;
export type ExprTrue = ExprBase & { readonly type: 'true' };
export type ExprFalse = ExprBase & { readonly type: 'false' };
export type ExprAnd = ExprBase & { readonly type: 'and'; readonly exprs: Expr[] };
export type ExprOr = ExprBase & { readonly type: 'or'; readonly exprs: Expr[] };
export type ExprAge = ExprBase & { readonly type: 'age'; readonly age: Age };
export type ExprHas = ExprBase & { readonly type: 'has'; readonly item: Item; readonly count: number };
export type ExprRenewable = ExprBase & { readonly type: 'renewable'; readonly item: Item };
export type ExprLicense = ExprBase & { readonly type: 'license'; readonly item: Item };
export type ExprEvent = ExprBase & { readonly type: 'event'; readonly event: string };
export type ExprMasks = ExprBase & { readonly type: 'masks'; readonly count: number };
export type ExprSpecial = ExprBase & { readonly type: 'special'; readonly special: string };
export type ExprTimeOot = ExprBase & { readonly type: 'time-oot'; readonly time: 'day' | 'night' };
export type ExprTimeMm = ExprBase & { readonly type: 'time-mm'; readonly value: number; readonly value2: number };
export type ExprPrice = ExprBase & { readonly type: 'price'; readonly slot: number; readonly max: number };

type Keyless<T> = T extends any ? Omit<T, 'key'> : never;
type ExprKeyless = Keyless<Expr>;

const exprMap = new Map<string, Expr>();
const exprKeyId = new Map<string, number>();

function exprBuild(key: string, fn: () => ExprKeyless): Expr {
  if (exprMap.has(key)) {
    return exprMap.get(key)!;
  }

  const exprWithoutKey = fn();
  const expr: Expr = { ...exprWithoutKey, key };
  exprMap.set(key, expr);
  exprKeyId.set(key, exprKeyId.size);
  return expr;
}

export const EXPR_TRUE = exprBuild('TRUE', () => ({ type: 'true' }));
export const EXPR_FALSE = exprBuild('FALSE', () => ({ type: 'false' }));
export const EXPR_AGE_CHILD = exprBuild('AGE(child)', () => ({ type: 'age', age: 'child' }));
export const EXPR_AGE_ADULT = exprBuild('AGE(adult)', () => ({ type: 'age', age: 'adult' }));
export const EXPR_TIME_OOT_DAY = exprBuild('TIME-OOT(day)', () => ({ type: 'time-oot', time: 'day' }));
export const EXPR_TIME_OOT_NIGHT = exprBuild('TIME-OOT(night)', () => ({ type: 'time-oot', time: 'night' }));

export const exprTrue = () => EXPR_TRUE;
export const exprFalse = () => EXPR_FALSE;

function subkey(exprs: Expr[]): string {
  return exprs.map(x => exprKeyId.get(x.key)).sort().join(',');
}

export const exprAnd = (exprs: Expr[]): Expr => {
  /* Parse-time Optimizations */
  exprs = exprs.filter(x => x !== EXPR_TRUE);
  if (exprs.length === 0) {
    return exprTrue();
  }
  if (exprs.some(x => x === EXPR_FALSE)) {
    return exprFalse();
  }

  exprs = [...new Set(exprs)];
  if (exprs.length === 1) {
    return exprs[0];
  }

  const sk = subkey(exprs);
  const key = `AND(${sk})`;

  return exprBuild(key, () => ({ type: 'and', exprs }));
};

export const exprOr = (exprs: Expr[]): Expr => {
  /* Optimizations */
  exprs = exprs.filter(x => x !== EXPR_FALSE);
  if (exprs.length === 0) {
    return exprFalse();
  }
  if (exprs.some(x => x === EXPR_TRUE)) {
    return exprTrue();
  }

  exprs = [...new Set(exprs)];
  if (exprs.length === 1) {
    return exprs[0];
  }

  const sk = subkey(exprs);
  const key = `OR(${sk})`;

  return exprBuild(key, () => ({ type: 'or', exprs }));
};

export const exprCond = (cond: Expr, then: Expr, otherwise: Expr): Expr => {
  if (cond === EXPR_TRUE) {
    return then;
  }
  if (cond === EXPR_FALSE) {
    return otherwise;
  }

  throw new Error(`Expression is not allowed at runtime ` + cond.type);
};

export const exprNot = (expr: Expr): Expr => {
  return exprCond(expr, EXPR_FALSE, EXPR_TRUE);
};

export const exprAge = (age: Age): Expr => {
  /* Avoids creating a new object for every call */
  switch (age) {
  case 'child': return EXPR_AGE_CHILD;
  case 'adult': return EXPR_AGE_ADULT;
  default: throw new Error(`Invalid age: ${age}`);
  }
};

export const exprHas = (item: Item, count: number): Expr => {
  if (count <= 0) {
    return EXPR_TRUE;
  }

  const key = `HAS(${item.id},${count})`;
  return exprBuild(key, () => ({ type: 'has', item, count }));
};

export const exprRenewable = (item: Item): Expr => {
  const key = `RENEWABLE(${item.id})`;
  return exprBuild(key, () => ({ type: 'renewable', item }));
};

export const exprLicense = (item: Item): Expr => {
  const key = `LICENSE(${item.id})`;
  return exprBuild(key, () => ({ type: 'license', item }));
};

export const exprEvent = (event: string): Expr => {
  const key = `EVENT(${event})`;
  return exprBuild(key, () => ({ type: 'event', event }));
};

export const exprMasks = (count: number): Expr => {
  const key = `MASKS(${count})`;
  return exprBuild(key, () => ({ type: 'masks', count }));
};

export const exprSpecial = (special: string): Expr => {
  const key = `SPECIAL(${special})`;
  return exprBuild(key, () => ({ type: 'special', special }));
};

export const exprSetting = (settings: Settings, resolvedFlags: ResolvedWorldFlags, setting: string, value: any): Expr => {
  if ((WORLD_FLAGS as readonly string[]).includes(setting)) {
    /* World flag */
    if (!resolvedFlags.hasOwnProperty(setting)) {
      throw new Error(`Unknown world flag: ${setting}`);
    }
    const f = resolvedFlags[setting as keyof typeof resolvedFlags];
    const s = SETTINGS.find(x => x.key === f.setting)!;
    if (!((s as any).values as any[]).some(x => x.value === value)) {
      throw new Error(`Invalid world flag value: ${value} (for flag: ${setting})`);
    }

    if (f.has(value)) {
      return EXPR_TRUE;
    } else {
      return EXPR_FALSE;
    }
  } else {
    /* Normal setting */
    const v = (settings as any)[setting];
    if (v === undefined) {
      throw new Error(`Setting ${setting} not found`);
    }
    return v === value ? EXPR_TRUE : EXPR_FALSE;
  }
};

export const exprTrick = (settings: Settings, trick: string): Expr => {
  if (!TRICKS.hasOwnProperty(trick)) {
    throw new Error(`Trick ${trick} not found`);
  }
  return settings.tricks.includes(trick as keyof typeof TRICKS) ? EXPR_TRUE : EXPR_FALSE;
};

export const exprGlitch = (settings: Settings, glitch: string): Expr => {
  if (!GLITCHES.hasOwnProperty(glitch)) {
    throw new Error(`Glitch ${glitch} not found`);
  }
  return settings.glitches.includes(glitch as keyof typeof GLITCHES) ? EXPR_TRUE : EXPR_FALSE;
};

export const exprOotTime = (time: string): Expr => {
  switch (time) {
  case 'day': return EXPR_TIME_OOT_DAY;
  case 'night': return EXPR_TIME_OOT_NIGHT;
  default: throw new Error(`Invalid OoT time: ${time}`);
  }
};

export const exprMmTime = (operator: string, sliceNames: string[]): Expr => {
  const slices: number[] = [];
  let value = 0;
  let value2 = 0;

  for (const s of sliceNames) {
    const sliceIndex = MM_TIME_SLICES.indexOf(s);
    if (sliceIndex === -1) {
      throw new Error(`Invalid MM time slice: ${s}`);
    }
    slices.push(sliceIndex);
  }

  switch (operator) {
  case 'before':
    /* Time < slice */
    if (slices.length !== 1) {
      throw new Error(`Wrong number of MM time slices for operator ${operator}`);
    }
    for (let i = 0; i < slices[0]; ++i)
      if (i < 32) {
        value = (value | (1 << i)) >>> 0;
      } else {
        value2 = (value2 | (1 << (i - 32))) >>> 0;
      }
    break;
  case 'after':
    /* Time >= slice */
    if (slices.length !== 1) {
      throw new Error(`Wrong number of MM time slices for operator ${operator}`);
    }
    for (let i = slices[0]; i < MM_TIME_SLICES.length; ++i)
      if (i < 32) {
        value = (value | (1 << i)) >>> 0;
      } else {
        value2 = (value2 | (1 << (i - 32))) >>> 0;
      }
    break;
  case 'at':
    /* Time == slice */
    if (slices.length !== 1) {
      throw new Error(`Wrong number of MM time slices for operator ${operator}`);
    }
    if (slices[0] < 32) {
      value = (1 << slices[0]) >>> 0;
    } else {
      value2 = (1 << (slices[0] - 32)) >>> 0;
    }
    break;
  case 'between':
    /* Time >= slice1 && Time < slice2 */
    if (slices.length !== 2) {
      throw new Error(`Wrong number of MM time slices for operator ${operator}`);
    }
    for (let i = slices[0]; i < slices[1]; ++i) {
      if (i < 32) {
        value = (value | (1 << i)) >>> 0;
      } else {
        value2 = (value2 | (1 << (i - 32))) >>> 0;
      }
    }
    break;
  default:
    throw new Error(`Invalid MM time operator: ${operator}`);
  }

  const key = `MM-TIME(${value.toString(16)},${value2.toString(16)})`;

  return exprBuild(key, () => ({ type: 'time-mm', value, value2 }));
}

export const exprPrice = (range: string, id: number, max: number): Expr => {
  const slot = id + PRICE_RANGES[range];
  const key = `PRICE(${slot},${max})`;
  return exprBuild(key, () => ({ type: 'price', slot, max }));
}

export const exprFish = (ageAndType: string, minPounds: number, maxPounds: number): Expr => {
  const items: Item[] = [];

  for (let i = minPounds; i <= maxPounds; i++) {
    const key = `OOT_FISHING_POND_${ageAndType}_${i}LBS` as ItemID;
    const item = Items[key];
    items.push(item);
  }

  const exprs = items.map(item => exprHas(item, 1));
  return exprOr(exprs);
};
