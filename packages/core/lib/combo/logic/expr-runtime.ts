import { Item, ItemGroups, Items, ItemsCount } from '../items';
import { Settings } from '../settings';
import { Age } from './pathfind';
import { World } from './world';
import { Expr, ExprEvent, ExprHas, ExprLicense, ExprMasks, ExprPrice, ExprRenewable, ExprSpecial, ExprTimeMm, ExprTimeOot } from './expr';

type RecursiveArray<T> = Array<T | RecursiveArray<T>>;

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
