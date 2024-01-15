import { Item, ItemGroups, Items, ItemsCount } from '../items';
import { ItemID } from '../items/defs';
import { GLITCHES, SETTINGS, Settings, TRICKS } from '../settings';
import { Age } from './pathfind';
import { PRICE_RANGES } from './price';
import { ResolvedWorldFlags, WORLD_FLAGS, World } from './world';

type RecursiveArray<T> = Array<T | RecursiveArray<T>>;

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

type ExprResultFalse = {
  result: false;
  depItems: RecursiveArray<Item>;
  depEvents: RecursiveArray<string>;
}

type ExprRestrictions = {
  oot: {
    day: boolean;
    night: boolean;
  },
  mmTime: number;
  mmTime2: number;
};

export const defaultRestrictions = (): ExprRestrictions => ({
  oot: {
    day: false,
    night: false,
  },
  mmTime: 0,
  mmTime2: 0,
});

export const maxRestrictions = (): ExprRestrictions => ({
  oot: {
    day: true,
    night: true,
  },
  mmTime: 0xffffffff,
  mmTime2: 0xffffffff,
});

export const isDefaultRestrictions = (r: ExprRestrictions): boolean => {
  return r.oot.day === false &&
    r.oot.night === false &&
    r.mmTime === 0 &&
    r.mmTime2 === 0;
};

type ExprResultTrue = {
  result: true;
  depItems: RecursiveArray<Item>;
  depEvents: RecursiveArray<string>;
  restrictions?: ExprRestrictions;
}

export type ExprResult = ExprResultFalse | ExprResultTrue;

export type AreaData = {
  oot: {
    day: boolean;
    night: boolean;
  },
  mmTime: number;
  mmTime2: number;
};

type State = {
  items: ItemsCount;
  renewables: ItemsCount;
  licenses: ItemsCount;
  age: Age;
  events: Set<string>;
  ignoreItems: boolean;
  areaData: AreaData;
  world: World;
  settings: Settings;
};

/* AND the restrictions together */
/* This is implemented with an OR because restrictions are negations */
export const exprRestrictionsAnd = (exprs: ExprResult[]): ExprRestrictions => {
  const restrictions = defaultRestrictions();

  for (const expr of exprs) {
    if (!expr.result) continue;
    if (!expr.restrictions) continue;
    restrictions.oot.day = restrictions.oot.day || expr.restrictions.oot.day;
    restrictions.oot.night = restrictions.oot.night || expr.restrictions.oot.night;
    restrictions.mmTime = (restrictions.mmTime | expr.restrictions.mmTime) >>> 0;
    restrictions.mmTime2 = (restrictions.mmTime2 | expr.restrictions.mmTime2) >>> 0;
  }

  return restrictions;
};

export const exprRestrictionsOr = (exprs: ExprResult[]): ExprRestrictions => {
  const restrictions = maxRestrictions();

  for (const expr of exprs) {
    if (!expr.result) continue;
    if (!expr.restrictions)
      return defaultRestrictions();
    restrictions.oot.day = restrictions.oot.day && expr.restrictions.oot.day;
    restrictions.oot.night = restrictions.oot.night && expr.restrictions.oot.night;
    restrictions.mmTime = (restrictions.mmTime & expr.restrictions.mmTime) >>> 0;
    restrictions.mmTime2 = (restrictions.mmTime2 & expr.restrictions.mmTime2) >>> 0;
  }

  return restrictions;
};

const itemCount = (state: State, item: Item): number => state.items.get(item) || 0;
const itemsCount = (state: State, items: Item[]): number => items.reduce((acc, item) => acc + itemCount(state, item), 0);

function resolveSpecialCond(settings: Settings, state: State, special: string): ExprResult {
  const { specialConds } = settings;
  if (!specialConds.hasOwnProperty(special)) {
    throw new Error(`Unknown special condition: ${special}`);
  }

  if (state.ignoreItems) {
    return { result: true, depItems: [], depEvents: [] };
  }

  let items = new Set<Item>();
  let itemsUnique = new Set<Item>();
  const cond = specialConds[special as keyof typeof specialConds];

  if (cond.stones) itemsUnique = new Set([...itemsUnique, ...ItemGroups.STONES]);
  if (cond.medallions) itemsUnique = new Set([...itemsUnique, ...ItemGroups.MEDALLIONS]);
  if (cond.remains) itemsUnique = new Set([...itemsUnique, ...ItemGroups.REMAINS]);
  if (cond.skullsGold) items.add(Items.OOT_GS_TOKEN);
  if (cond.skullsSwamp) items.add(Items.MM_GS_TOKEN_SWAMP);
  if (cond.skullsOcean) items.add(Items.MM_GS_TOKEN_OCEAN);
  if (cond.fairiesWF) items.add(Items.MM_STRAY_FAIRY_WF);
  if (cond.fairiesSH) items.add(Items.MM_STRAY_FAIRY_SH);
  if (cond.fairiesGB) items.add(Items.MM_STRAY_FAIRY_GB);
  if (cond.fairiesST) items.add(Items.MM_STRAY_FAIRY_ST);
  if (cond.fairyTown) items.add(Items.MM_STRAY_FAIRY_TOWN);
  if (cond.masksRegular) itemsUnique = new Set([...itemsUnique, ...ItemGroups.MASKS_REGULAR]);
  if (cond.masksTransform) itemsUnique = new Set([...itemsUnique, ...ItemGroups.MASKS_TRANSFORM]);
  if (cond.masksOot) itemsUnique = new Set([...itemsUnique, ...ItemGroups.MASKS_OOT]);
  if (cond.triforce) items.add(Items.SHARED_TRIFORCE);
  if (cond.coinsRed) items.add(Items.OOT_COIN_RED);
  if (cond.coinsGreen) items.add(Items.OOT_COIN_GREEN);
  if (cond.coinsBlue) items.add(Items.OOT_COIN_BLUE);
  if (cond.coinsYellow) items.add(Items.OOT_COIN_YELLOW);

  const countUnique = [...itemsUnique].filter(item => itemCount(state, item) > 0).length;
  const result = (itemsCount(state, [...items]) + countUnique) >= cond.count;

  return { result, depEvents: [], depItems: [...items, ...itemsUnique] };
}

const exprImplTrue = (_state: State) => ({ result: true, depItems: [], depEvents: [] });
const exprImplFalse = (_state: State) => ({ result: false, depItems: [], depEvents: [] });

//export type Expr = (state: State) => ExprResult;

export type Expr = ExprTrue | ExprFalse | ExprAnd | ExprOr | ExprAge | ExprHas | ExprRenewable | ExprLicense | ExprEvent | ExprMasks | ExprSpecial | ExprTimeOot | ExprTimeMm | ExprPrice;
type ExprTrue = { readonly type: 'true' };
type ExprFalse = { readonly type: 'false' };
type ExprAnd = { readonly type: 'and'; readonly exprs: Expr[] };
type ExprOr = { readonly type: 'or'; readonly exprs: Expr[] };
type ExprAge = { readonly type: 'age'; readonly age: Age };
type ExprHas = { readonly type: 'has'; readonly item: Item; readonly count: number };
type ExprRenewable = { readonly type: 'renewable'; readonly item: Item };
type ExprLicense = { readonly type: 'license'; readonly item: Item };
type ExprEvent = { readonly type: 'event'; readonly event: string };
type ExprMasks = { readonly type: 'masks'; readonly count: number };
type ExprSpecial = { readonly type: 'special'; readonly special: string };
type ExprTimeOot = { readonly type: 'time-oot'; readonly time: 'day' | 'night' };
type ExprTimeMm = { readonly type: 'time-mm'; readonly value: number; readonly value2: number };
type ExprPrice = { readonly type: 'price'; readonly slot: number; readonly max: number };

export const EXPR_TRUE: ExprTrue = { type: 'true' };
export const EXPR_FALSE: ExprFalse = { type: 'false' };
export const EXPR_AGE_CHILD: ExprAge = { type: 'age', age: 'child' };
export const EXPR_AGE_ADULT: ExprAge = { type: 'age', age: 'adult' };
export const EXPR_TIME_OOT_DAY: ExprTimeOot = { type: 'time-oot', time: 'day' };
export const EXPR_TIME_OOT_NIGHT: ExprTimeOot = { type: 'time-oot', time: 'night' };

export const exprTrue = () => EXPR_TRUE;
export const exprFalse = () => EXPR_FALSE;

export const exprAnd = (exprs: Expr[]): Expr => {
  /* Parse-time Optimizations */
  exprs = exprs.filter(x => x !== EXPR_TRUE);
  if (exprs.length === 0) {
    return exprTrue();
  }
  if (exprs.some(x => x === EXPR_FALSE)) {
    return exprFalse();
  }
  if (exprs.length === 1) {
    return exprs[0];
  }
  return { type: 'and', exprs };
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
  if (exprs.length === 1) {
    return exprs[0];
  }
  return { type: 'or', exprs };
};

export const exprCond = (cond: Expr, then: Expr, otherwise: Expr): Expr => {
  if (cond === EXPR_TRUE) {
    return then;
  }
  if (cond === EXPR_FALSE) {
    return otherwise;
  }

  throw new Error(`Expression is not allowed at runtime ${cond}`);
};

export const exprNot = (expr: Expr): Expr => {
  return exprCond(expr, EXPR_FALSE, EXPR_TRUE);
};

export const exprAge = (age: Age): ExprAge => {
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

  return { type: 'has', item, count };
};

export const exprRenewable = (item: Item): Expr => {
  return { type: 'renewable', item };
};

export const exprLicense = (item: Item): Expr => {
  return { type: 'license', item };
};

export const exprEvent = (event: string): Expr => {
  return { type: 'event', event };
};

export const exprMasks = (count: number): Expr => {
  return { type: 'masks', count };
};

export const exprSpecial = (special: string): Expr => {
  return { type: 'special', special };
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

  return { type: 'time-mm', value, value2 };
}

export const exprPrice = (range: string, id: number, max: number): Expr => {
  const slot = id + PRICE_RANGES[range];
  return { type: 'price', slot, max };
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

function evalExprAnd(exprs: Expr[], state: State): ExprResult {
  const results: ExprResult[] = [];
  for (const e of exprs) {
    const r = evalExpr(e, state);
    results.push(r);

    /* Early exit */
    if (!r.result) {
      return { result: false, depItems: results.map(x => x.depItems), depEvents: results.map(x => x.depEvents) };
    }
  }

  const restrictions = exprRestrictionsAnd(results);
  if (isDefaultRestrictions(restrictions)) {
    return { result: true, depItems: [], depEvents: [] };
  } else {
    return { result: true, depItems: results.map(x => x.depItems), depEvents: results.map(x => x.depEvents), restrictions };
  }
}

function evalExprOr(exprs: Expr[], state: State): ExprResult {
  const results: ExprResult[] = [];
  let result = false;

  for (const e of exprs) {
    const r = evalExpr(e, state);
    results.push(r);
    if (r.result) {
      result = true;
      if (!r.restrictions) {
        return { result: true, depItems: [], depEvents: [] };
      }
    }
  }

  if (result) {
    const restrictions = exprRestrictionsOr(results);
    if (isDefaultRestrictions(restrictions)) {
      return { result: true, depItems: [], depEvents: [] };
    } else {
      return { result: true, depItems: results.map(x => x.depItems), depEvents: results.map(x => x.depEvents), restrictions };
    }
  } else {
    return { result: false, depItems: results.map(x => x.depItems), depEvents: results.map(x => x.depEvents) };
  }
}

function evalExprHas(expr: ExprHas, state: State): ExprResult {
  const result = (state.ignoreItems || (itemCount(state, expr.item) >= expr.count));
  return { result, depItems: [expr.item], depEvents: [] };
}

function evalExprRenewable(expr: ExprRenewable, state: State): ExprResult {
  const result = (state.ignoreItems || (state.renewables.get(expr.item) || 0) > 0);
  return { result, depItems: [expr.item], depEvents: [] };
}

function evalExprLicense(expr: ExprLicense, state: State): ExprResult {
  const result = (state.ignoreItems || (state.licenses.get(expr.item) || 0) > 0);
  return { result, depItems: [expr.item], depEvents: [] };
}

function evalExprEvent(expr: ExprEvent, state: State): ExprResult {
  const result = state.events.has(expr.event);
  return { result, depItems: [], depEvents: [expr.event] };
}

function evalExprMasks(expr: ExprMasks, state: State): ExprResult {
  const result = state.ignoreItems || (itemsCount(state, [...ItemGroups.MASKS_REGULAR]) >= expr.count);
  return { result, depItems: [...ItemGroups.MASKS_REGULAR], depEvents: [] };
}

function evalExprSpecial(expr: ExprSpecial, state: State): ExprResult {
  return resolveSpecialCond(state.settings, state, expr.special);
}

function evalExprOotTime(expr: ExprTimeOot, state: State): ExprResult {
  const negation = expr.time === 'day' ? 'night' : 'day';

  if (state.areaData.oot[expr.time]) {
    const restrictions = defaultRestrictions();
    restrictions.oot[negation] = true;
    return { result: true, depItems: [], depEvents: [], restrictions };
  } else {
    return {
      result: false,
      depItems: [],
      depEvents: [],
    };
  }
}

function evalExprMmTime(expr: ExprTimeMm, state: State): ExprResult {
  if ((state.areaData.mmTime & expr.value) || (state.areaData.mmTime2 & expr.value2)) {
    const restrictions = defaultRestrictions();
    restrictions.mmTime = ~(expr.value) >>> 0;
    restrictions.mmTime2 = ~(expr.value2) >>> 0;
    return { result: true, restrictions, depItems: [], depEvents: [] };
  } else {
    return {
      result: false,
      depItems: [],
      depEvents: [],
    };
  }
}

function evalExprPrice(expr: ExprPrice, state: State): ExprResult {
  const price = state.world.prices[expr.slot];
  const result = price <= expr.max;
  return { result, depItems: [], depEvents: [] };
}

export function evalExpr(expr: Expr, state: State): ExprResult {
  switch (expr.type) {
  case 'true': return { result: true, depItems: [], depEvents: [] };
  case 'false': return { result: false, depItems: [], depEvents: [] };
  case 'and': return evalExprAnd(expr.exprs, state);
  case 'or': return evalExprOr(expr.exprs, state);
  case 'age': return state.age === expr.age ? { result: true, depItems: [], depEvents: [] } : { result: false, depItems: [], depEvents: [] };
  case 'has': return evalExprHas(expr, state);
  case 'renewable': return evalExprRenewable(expr, state);
  case 'license': return evalExprLicense(expr, state);
  case 'event': return evalExprEvent(expr, state);
  case 'masks': return evalExprMasks(expr, state);
  case 'special': return evalExprSpecial(expr, state);
  case 'time-oot': return evalExprOotTime(expr, state);
  case 'time-mm': return evalExprMmTime(expr, state);
  case 'price': return evalExprPrice(expr, state);
  }
}
