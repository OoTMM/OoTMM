import type { Settings, CountMap } from '@ootmm/core';
import type { Expr, ExprDependencies, ExprFunc, ExprNode, ExprRestrictions, ExprResult, ExprState } from './types';
import type { Item } from '../items';

import { ItemGroups, Items } from '../items';
import { OOT_TIME_ALL } from './data';
import { exprMemoKey } from './memo';

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

  let restrictions: ExprRestrictions | undefined;
  const indexItems = deps.items.length;
  const indexEvents = deps.events.length;

  for (const e of exprs) {
    const r = e.eval(state, deps);
    if (!r.result) continue;
    if (!r.restrictions) {
      deps.items = deps.items.slice(0, indexItems);
      deps.events = deps.events.slice(0, indexEvents);
      return RESULT_TRUE;
    }
    restrictions ??= maxRestrictions();
    restrictions.ootTime = (restrictions.ootTime & r.restrictions.ootTime) >>> 0;
    restrictions.mmTime = (restrictions.mmTime & r.restrictions.mmTime) >>> 0;
    restrictions.mmTime2 = (restrictions.mmTime2 & r.restrictions.mmTime2) >>> 0;
    restrictions.flagsOn = (restrictions.flagsOn & r.restrictions.flagsOn) >>> 0;
    restrictions.flagsOff = (restrictions.flagsOff & r.restrictions.flagsOff) >>> 0;
  }

  if (!restrictions) return RESULT_FALSE;
  if (isDefaultRestrictions(restrictions)) {
    deps.items = deps.items.slice(0, indexItems);
    deps.events = deps.events.slice(0, indexEvents);
    return RESULT_TRUE;
  }
  return { result: true, restrictions };
}

function evalAnd(exprs: Expr[], state: ExprState, deps: ExprDependencies) {
  let restrictions: ExprRestrictions | undefined;
  const indexItems = deps.items.length;
  const indexEvents = deps.events.length;

  for (const e of exprs) {
    const r = e.eval(state, deps);
    if (!r.result) return RESULT_FALSE;
    if (!r.restrictions) continue;
    restrictions ??= defaultRestrictions();
    restrictions.ootTime = (restrictions.ootTime | r.restrictions.ootTime) >>> 0;
    restrictions.mmTime = (restrictions.mmTime | r.restrictions.mmTime) >>> 0;
    restrictions.mmTime2 = (restrictions.mmTime2 | r.restrictions.mmTime2) >>> 0;
    restrictions.flagsOn = (restrictions.flagsOn | r.restrictions.flagsOn) >>> 0;
    restrictions.flagsOff = (restrictions.flagsOff | r.restrictions.flagsOff) >>> 0;
  }

  if (restrictions && isRestrictionImpossible(restrictions)) return RESULT_FALSE;
  if (!restrictions || isDefaultRestrictions(restrictions)) {
    deps.items = deps.items.slice(0, indexItems);
    deps.events = deps.events.slice(0, indexEvents);
    return RESULT_TRUE;
  }
  return { result: true, restrictions };
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

function evalTimeOot(flag: number): ExprFunc {
  const result = { ...RESULT_TRUE, restrictions: defaultRestrictions() };
  const flagNeg = OOT_TIME_ALL & ~flag;
  result.restrictions.ootTime = flagNeg;

  return (state) => {
    if (state.areaData.ootTime & flag) {
      return result;
    } else {
      return RESULT_FALSE;
    }
  }
}

function evalTimeMm(value: number, value2: number): ExprFunc {
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

function evalFlagOn(flag: number): ExprFunc {
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

function evalFlagOff(flag: number): ExprFunc {
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

function makeEvalFunc(expr: ExprNode): ExprFunc {
  switch (expr.type) {
    case 'true': return () => RESULT_TRUE;
    case 'false': return () => RESULT_FALSE;
    case 'or': return (state, deps) => evalOr(expr.exprs, state, deps);
    case 'and': return (state, deps) => evalAnd(expr.exprs, state, deps);
    case 'enemy': return (state, deps) => {
      if (state.ignoreEnemyCombat === true || state.ignoreEnemyCombat && state.ignoreEnemyCombat.has(expr.name)) return RESULT_TRUE;
      return expr.expr.eval(state, deps);
    };
    case 'age': return (state) => (state.age === expr.age) ? RESULT_TRUE : RESULT_FALSE;
    case 'item': return (state, deps) => evalItem(state.items, expr.item, expr.count, deps);
    case 'license': return (state, deps) => evalItem(state.licenses, expr.item, 1, deps);
    case 'renewable': return (state, deps) => evalItem(state.renewables, expr.item, 1, deps);
    case 'price': return (state) => state.world.prices[expr.slot] <= expr.max ? RESULT_TRUE : RESULT_FALSE;
    case 'song-event-oot': return (state) => state.world.songEventsOot[expr.songId] === expr.cmp ? RESULT_TRUE : RESULT_FALSE;
    case 'song-event-mm': return (state) => state.world.songEventsMm[expr.songId] === expr.cmp ? RESULT_TRUE : RESULT_FALSE;
    case 'masks': return (state, deps) => evalItems(state.items, ItemGroups.MASKS_REGULAR_VANILLA_MM, expr.count, deps);
    case 'event': return (state, deps) => evalEvent(state, expr.event, deps);
    case 'special': return (state, deps) => evalSpecial(state, expr.specialId, deps);
    case 'time-oot': return evalTimeOot(expr.flag);
    case 'time-mm': return evalTimeMm(expr.value, expr.value2);
    case 'flag-on': return evalFlagOn(expr.flag);
    case 'flag-off': return evalFlagOff(expr.flag);
  }
}

const exprAreaMasks: number[] = [];
function areaMask(expr: ExprNode): number {
  switch (expr.type) {
    case 'time-oot': return 1;
    case 'time-mm': return 2;
    case 'flag-on': case 'flag-off': return 4;
    case 'enemy': return exprAreaMasks[expr.expr.id];
    case 'and': case 'or': return expr.exprs.reduce((mask, child) => mask | exprAreaMasks[child.id], 0);
    default: return 0;
  }
}
function cachedEval(id: number, evaluate: ExprFunc): ExprFunc {
  return (state, deps) => {
    const cache = state.evalCache;
    if (cache && cache.successes[id] === cache.run) return RESULT_TRUE;
    const result = evaluate(state, deps);

    if (cache && result.result && !result.restrictions) cache.successes[id] = cache.run;
    return result;
  };
}

let nextExprId = 0;
const exprMemoIds: Record<string, number> = {};
const exprMemo: Expr[] = [];

export function compileExpr(expr: ExprNode): Expr {
  const key = exprMemoKey(expr);
  let id = exprMemoIds[key];
  if (id !== undefined) {
    return exprMemo[id];
  }
  id = nextExprId++;
  exprMemoIds[key] = id;

  const mask = exprAreaMasks[id] = areaMask(expr);
  const evaluate = makeEvalFunc(expr);
  const e = Object.assign({}, expr, {
    id,
    eval: !mask && (expr.type === 'and' || expr.type === 'or') ? cachedEval(id, evaluate) : evaluate
  });
  Object.freeze(e);
  exprMemo[id] = e;
  return e;
}
