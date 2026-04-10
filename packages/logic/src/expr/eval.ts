import { ItemGroups, Items, type CountMap, type Item, type Settings } from '@ootmm/core';
import type { Expr, ExprData, ExprDependencies, ExprFunc, ExprNode, ExprRestrictions, ExprResult, ExprState } from './types';

import { OOT_TIME_ALL } from './data';
import { memoExpr } from './memo';

const RESULT_TRUE: ExprResult = { result: true };
const RESULT_FALSE: ExprResult = { result: false };

export const defaultRestrictions = (): ExprRestrictions => ({
  ootTime: 0,
  mmTime: 0,
  mmTime2: 0,
  flagsOn: 0,
  flagsOff: 0,
});

export const maxRestrictions = (): ExprRestrictions => ({
  ootTime: OOT_TIME_ALL,
  mmTime: 0xffffffff,
  mmTime2: 0xffffffff,
  flagsOn: 0xffffffff,
  flagsOff: 0xffffffff,
});

export const isDefaultRestrictions = (r: ExprRestrictions): boolean => {
  return r.ootTime === 0 &&
    r.mmTime === 0 &&
    r.mmTime2 === 0 &&
    r.flagsOn === 0 &&
    r.flagsOff === 0;
};

function isRestrictionImpossible(r: ExprRestrictions): boolean {
  if (r.ootTime === OOT_TIME_ALL) {
    return true;
  }
  if (r.mmTime === 0xffffffff && r.mmTime2 === 0xffffffff) {
    return true;
  }
  if (r.flagsOn & r.flagsOff) {
    return true;
  }
  return false;
}

/* AND the restrictions together */
/* This is implemented with an OR because restrictions are negations */
export const exprRestrictionsAnd = (exprs: ExprResult[]): ExprRestrictions => {
  const restrictions = defaultRestrictions();

  for (const expr of exprs) {
    if (!expr.result) continue;
    if (!expr.restrictions) continue;
    restrictions.ootTime = (restrictions.ootTime | expr.restrictions.ootTime) >>> 0;
    restrictions.mmTime = (restrictions.mmTime | expr.restrictions.mmTime) >>> 0;
    restrictions.mmTime2 = (restrictions.mmTime2 | expr.restrictions.mmTime2) >>> 0;
    restrictions.flagsOn = (restrictions.flagsOn | expr.restrictions.flagsOn) >>> 0;
    restrictions.flagsOff = (restrictions.flagsOff | expr.restrictions.flagsOff) >>> 0;
  }

  return restrictions;
};

export const exprRestrictionsOr = (exprs: ExprResult[]): ExprRestrictions => {
  const restrictions = maxRestrictions();

  for (const expr of exprs) {
    if (!expr.result) continue;
    if (!expr.restrictions) return defaultRestrictions();
    restrictions.ootTime = (restrictions.ootTime & expr.restrictions.ootTime) >>> 0;
    restrictions.mmTime = (restrictions.mmTime & expr.restrictions.mmTime) >>> 0;
    restrictions.mmTime2 = (restrictions.mmTime2 & expr.restrictions.mmTime2) >>> 0;
    restrictions.flagsOn = (restrictions.flagsOn & expr.restrictions.flagsOn) >>> 0;
    restrictions.flagsOff = (restrictions.flagsOff & expr.restrictions.flagsOff) >>> 0;
  }

  return restrictions;
};

function evalOr(exprs: Expr[], state: ExprState, deps: ExprDependencies) {
  const results: ExprResult[] = [];
  let result = false;

  const indexItems = deps.items.length;
  const indexEvents = deps.events.length;

  for (const e of exprs) {
    const r = e(state, deps);
    results.push(r);
    if (r.result) {
      result = true;
      if (!r.restrictions) {
        deps.items = deps.items.slice(0, indexItems);
        deps.events = deps.events.slice(0, indexEvents);
        return RESULT_TRUE;
      }
    }
  }

  if (result) {
    const restrictions = exprRestrictionsOr(results);
    if (isDefaultRestrictions(restrictions)) {
      deps.items = deps.items.slice(0, indexItems);
      deps.events = deps.events.slice(0, indexEvents);
      return RESULT_TRUE;
    } else {
      return { result: true, restrictions };
    }
  } else {
    return RESULT_FALSE;
  }
}

function evalAnd(exprs: Expr[], state: ExprState, deps: ExprDependencies) {
  const results: ExprResult[] = [];
  const indexItems = deps.items.length;
  const indexEvents = deps.events.length;

  for (const e of exprs) {
    const r = e(state, deps);
    results.push(r);

    /* Early exit */
    if (!r.result) {
      return { result: false };
    }
  }

  const restrictions = exprRestrictionsAnd(results);
  /* Check for a contradiction (a restriction that prevents everything) */
  if (isRestrictionImpossible(restrictions)) {
    return { result: false };
  }

  if (isDefaultRestrictions(restrictions)) {
    deps.items = deps.items.slice(0, indexItems);
    deps.events = deps.events.slice(0, indexEvents);
    return RESULT_TRUE;
  } else {
    return { result: true, restrictions };
  }
}

const itemCount = (state: ExprState, item: Item): number => state.items.get(item) || 0;
const itemsCount = (state: ExprState, items: Item[]): number => items.reduce((acc, item) => acc + itemCount(state, item), 0);

function evalItem(counts: CountMap<Item>, item: Item, count: number, deps: ExprDependencies): ExprResult {
  if ((counts.get(item) || 0) >= count) {
    return RESULT_TRUE;
  } else {
    deps.items.push(item);
    return RESULT_FALSE;
  }
}

function evalItems(counts: CountMap<Item>, items: Set<Item>, count: number, deps: ExprDependencies): ExprResult {
  let acc = 0;
  for (const i of items) {
    acc += (counts.get(i) || 0) > 0 ? 1 : 0;
    if (acc >= count) {
      return RESULT_TRUE;
    }
  }
  deps.items.push(...items);
  return RESULT_FALSE;
}

function evalEvent(state: ExprState, event: string, deps: ExprDependencies): ExprResult {
  if (state.events.has(event)) {
    return RESULT_TRUE;
  } else {
    deps.events.push(event);
    return RESULT_FALSE;
  }
}

function specialCondSets(settings: Settings, special: string) {
  const { specialConds } = settings;
  if (!specialConds.hasOwnProperty(special)) {
    throw new Error(`Unknown special condition: ${special}`);
  }
  const cond = specialConds[special as keyof typeof specialConds];
  let items = new Set<Item>();
  let itemsUnique = new Set<Item>();

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

  return { items, itemsUnique };
}

function evalSpecial(state: ExprState, special: string, deps: ExprDependencies): ExprResult {
  const { items, itemsUnique } = specialCondSets(state.settings, special);
  const { specialConds } = state.settings;
  const cond = specialConds[special as keyof typeof specialConds];

  const countUnique = [...itemsUnique].filter((item) => itemCount(state, item) > 0).length;
  const result = itemsCount(state, [...items]) + countUnique >= cond.count;

  deps.items = deps.items.concat(...items, ...itemsUnique);

  return { result };
}

type ExprEvalFunc = (state: ExprState, deps: ExprDependencies) => ExprResult;

function evalTimeOot(state: ExprState, flag: number): ExprResult {
  if (state.areaData.ootTime & flag) {
    const flagNeg = OOT_TIME_ALL & ~flag;
    const restrictions = defaultRestrictions();
    restrictions.ootTime = flagNeg;
    return { result: true, restrictions };
  } else {
    return RESULT_FALSE;
  }
}

function evalTimeMm(value: number, value2: number): ExprEvalFunc {
  const result = { ...RESULT_TRUE, restrictions: defaultRestrictions() };
  result.restrictions.mmTime = ~value >>> 0;
  result.restrictions.mmTime2 = ~value2 >>> 0;

  return (state) => {
    if (state.areaData.mmTime & value || state.areaData.mmTime2 & value2) {
      return result;
    } else {
      return RESULT_FALSE;
    }
  }
}

function evalFlagOn(flag: number): ExprEvalFunc {
  const mask = 1 << flag;
  const result = { ...RESULT_TRUE, restrictions: defaultRestrictions() };
  result.restrictions.flagsOn = (1 << flag) >>> 0;

  return (state) => {
    if (state.areaData.flagsOff & mask) {
      return RESULT_FALSE;
    }
    return result;
  }

}

function evalFlagOff(flag: number): ExprEvalFunc {
  const mask = 1 << flag;
  const result = { ...RESULT_TRUE, restrictions: defaultRestrictions() };
  result.restrictions.flagsOff = (1 << flag) >>> 0;

  return (state) => {
    if (state.areaData.flagsOn & mask) {
      return RESULT_FALSE;
    }

    return result;
  }
}

const cache = new WeakMap<Expr, ExprEvalFunc>();

function makeEvalFunc(expr: ExprData): ExprFunc {
  switch (expr.type) {
    case 'true': return () => RESULT_TRUE;
    case 'false': return () => RESULT_FALSE;
    case 'or': return (state, deps) => evalOr(expr.exprs, state, deps);
    case 'and': return (state, deps) => evalAnd(expr.exprs, state, deps);
    case 'age': return (state) => (state.age === expr.age) ? RESULT_TRUE : RESULT_FALSE;
    case 'item': return (state, deps) => evalItem(state.items, expr.item, expr.count, deps);
    case 'license': return (state, deps) => evalItem(state.licenses, expr.item, 1, deps);
    case 'renewable': return (state, deps) => evalItem(state.renewables, expr.item, 1, deps);
    case 'price': return (state) => state.world.prices[expr.slot] <= expr.max ? RESULT_TRUE : RESULT_FALSE;
    case 'song-event': return (state) => state.world.songEvents[expr.songId] === expr.cmp ? RESULT_TRUE : RESULT_FALSE;
    case 'masks': return (state, deps) => evalItems(state.items, ItemGroups.MASKS_REGULAR, expr.count, deps);
    case 'event': return (state, deps) => evalEvent(state, expr.event, deps);
    case 'special': return (state, deps) => evalSpecial(state, expr.specialId, deps);
    case 'time-oot': return (state) => evalTimeOot(state, expr.flag);
    case 'time-mm': return evalTimeMm(expr.value, expr.value2);
    case 'flag-on': return evalFlagOn(expr.flag);
    case 'flag-off': return evalFlagOff(expr.flag);
  }
}

export function evalExpr(expr: Expr, state: ExprState, deps: ExprDependencies): ExprResult {
  let f = cache.get(expr);
  if (!f) {
    f = makeEvalFunc(expr);
    cache.set(expr, f);
  }
  return f(state, deps);
}

export function compileExpr(expr: ExprNode): Expr {
  const data = memoExpr(expr);
  const func = Object.assign(makeEvalFunc(data), data);
  Object.freeze(func);
  return func;
}
