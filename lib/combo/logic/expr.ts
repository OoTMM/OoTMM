import { Settings, Trick, TRICKS } from '../settings';
import { Items, ITEMS_MASKS_OOT, ITEMS_MASKS_REGULAR, ITEMS_MASKS_TRANSFORM, ITEMS_MEDALLIONS, ITEMS_REMAINS, ITEMS_STONES } from './items';
import { Age } from './pathfind';

export const MM_TIME_SLICES = [
  'DAY1_AM_06_00',
  'NIGHT1_PM_06_00',
  'NIGHT1_AM_02_30',
  'DAY2_AM_06_00',
  'NIGHT2_PM_06_00',
  'DAY3_AM_06_00',
  'NIGHT3_PM_06_00',
];

export type ExprDependencies = {
  items?: Set<string>;
  events?: Set<string>;
};

type ExprResultFalse = {
  result: false;
  dependencies?: ExprDependencies;
}

type ExprRestrictions = {
  oot: {
    day: boolean;
    night: boolean;
  },
  mmTime: number;
};

export const defaultRestrictions = (): ExprRestrictions => ({
  oot: {
    day: false,
    night: false,
  },
  mmTime: 0,
});

export const maxRestrictions = (): ExprRestrictions => ({
  oot: {
    day: true,
    night: true,
  },
  mmTime: 0xffffffff,
});

export const isDefaultRestrictions = (r: ExprRestrictions): boolean => {
  return r.oot.day === false &&
    r.oot.night === false &&
    r.mmTime === 0;
};

type ExprResultTrue = {
  result: true;
  dependencies?: ExprDependencies;
  restrictions?: ExprRestrictions;
}

export type ExprResult = ExprResultFalse | ExprResultTrue;

export type AreaData = {
  oot: {
    day: boolean;
    night: boolean;
  },
  mmTime: number;
};

type State = {
  items: Items;
  age: Age;
  events: Set<string>;
  ignoreItems: boolean;
  areaData: AreaData;
};

export const exprDependencies = (exprs: ExprResult[]): ExprDependencies => {
  const deps: ExprDependencies = {};

  for (const expr of exprs) {
    const d = expr.dependencies;
    if (!d) continue;
    if (d.items) deps.items = new Set([...(deps.items || []), ...d.items]);
    if (d.events) deps.events = new Set([...(deps.events || []), ...d.events]);
  }

  return deps;
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
  }

  return restrictions;
};

const itemCount = (state: State, item: string): number => state.items[item] || 0;
const itemsCount = (state: State, items: string[]): number => items.reduce((acc, item) => acc + itemCount(state, item), 0);

function resolveSpecialCond(settings: Settings, state: State, special: string): ExprResult {
  const { specialConds } = settings;
  if (!specialConds.hasOwnProperty(special)) {
    throw new Error(`Unknown special condition: ${special}`);
  }
  let items = new Set<string>();
  const cond = specialConds[special as keyof typeof specialConds];

  if (cond.stones) items = new Set([...items, ...ITEMS_STONES]);
  if (cond.medallions) items = new Set([...items, ...ITEMS_MEDALLIONS]);
  if (cond.remains) items = new Set([...items, ...ITEMS_REMAINS]);
  if (cond.skullsGold) items.add('OOT_GS_TOKEN');
  if (cond.skullsSwamp) items.add('MM_GS_TOKEN_SWAMP');
  if (cond.skullsOcean) items.add('MM_GS_TOKEN_OCEAN');
  if (cond.fairiesWF) items.add('MM_STRAY_FAIRY_WF');
  if (cond.fairiesSH) items.add('MM_STRAY_FAIRY_SH');
  if (cond.fairiesGB) items.add('MM_STRAY_FAIRY_GB');
  if (cond.fairiesST) items.add('MM_STRAY_FAIRY_ST');
  if (cond.fairyTown) items.add('MM_STRAY_FAIRY_TOWN');
  if (cond.masksRegular) items = new Set([...items, ...ITEMS_MASKS_REGULAR]);
  if (cond.masksTransform) items = new Set([...items, ...ITEMS_MASKS_TRANSFORM]);
  if (cond.masksOot) items = new Set([...items, ...ITEMS_MASKS_OOT]);

  const result = itemsCount(state, [...items]) >= cond.count;
  const dependencies = { items };

  return { result, dependencies };
}

export type Expr = (state: State) => ExprResult;

export const exprTrue = (): Expr => state => ({result: true });
export const exprFalse = (): Expr => state => ({ result: false, dependencies: { items: new Set(), events: new Set() } });

export const exprAnd = (exprs: Expr[]): Expr => state => {
  const results: ExprResult[] = exprs.map(expr => expr(state));
  const dependencies = exprDependencies(results);
  const result = results.every(x => x.result);

  if (result) {
    const restrictions = exprRestrictionsAnd(results);
    if (isDefaultRestrictions(restrictions)) {
      return { result: true };
    } else {
      return { result: true, dependencies, restrictions };
    }
  } else {
    return { result: false, dependencies };
  }
};

export const exprOr = (exprs: Expr[]): Expr => state => {
  const results: ExprResult[] = exprs.map(expr => expr(state));
  const dependencies = exprDependencies(results);
  const result = results.some(x => x.result);

  if (result) {
    const restrictions = exprRestrictionsOr(results);
    if (isDefaultRestrictions(restrictions)) {
      return { result: true };
    } else {
      return { result: true, dependencies, restrictions };
    }
  } else {
    return { result: false, dependencies };
  }
};

export const exprNot = (expr: Expr): Expr => state => expr(state).result ? exprFalse()(state) : exprTrue()(state);
export const exprCond = (cond: Expr, then: Expr, otherwise: Expr): Expr => state => cond(state).result ? then(state) : otherwise(state);
export const exprAge = (age: Age): Expr => state => state.age === age ? exprTrue()(state) : exprFalse()(state);

export const exprHas = (item: string, itemShared: string, count: number): Expr => state => {
  const result = (state.ignoreItems || ((itemCount(state, item) + itemCount(state, itemShared)) >= count));
  const dependencies = { items: new Set([item, itemShared]) };
  return { result, dependencies };
};

export const exprEvent = (event: string): Expr => state => {
  const dependencies = { events: new Set([event]) };
  const result = state.events.has(event);
  return { result, dependencies };
};

export const exprMasks = (count: number): Expr => state => {
  const result = state.ignoreItems || (itemsCount(state, ITEMS_MASKS_REGULAR) >= count);
  const dependencies = { items: new Set(ITEMS_MASKS_REGULAR) };
  return { result, dependencies };
};

export const exprSpecial = (settings: Settings, special: string): Expr => state => resolveSpecialCond(settings, state, special);

export const exprSetting = (settings: Settings, setting: string, value: any): Expr => {
  const v = (settings as any)[setting];
  if (v === undefined) {
    throw new Error(`Setting ${setting} not found`);
  }
  return v === value ? exprTrue() : exprFalse();
};

export const exprTrick = (settings: Settings, trick: string): Expr => {
  if (!TRICKS.hasOwnProperty(trick)) {
    throw new Error(`Trick ${trick} not found`);
  }
  return settings.tricks[trick as Trick] ? exprTrue() : exprFalse();
};

export const exprOotTime = (time: string): Expr => {
  if (time !== 'day' && time !== 'night') {
    throw new Error(`Invalid OoT time: ${time}`);
  }

  const negation = time === 'day' ? 'night' : 'day';

  return state => {
    if (state.areaData.oot[time]) {
      const restrictions = defaultRestrictions();
      restrictions.oot[negation] = true;
      return { result: true, restrictions };
    } else {
      return {
        result: false,
      };
    }
  };
}

export const exprMmTime = (operator: string, sliceName: string): Expr => {
  const slice = MM_TIME_SLICES.indexOf(sliceName);
  let value = 0;

  if (slice === -1) {
    throw new Error(`Invalid MM time slice: ${sliceName}`);
  }

  switch (operator) {
  case 'before':
    /* Time < slice */
    for (let i = 0; i < slice; ++i)
      value = (value | (1 << i)) >>> 0;
    break;
  case 'after':
    /* Time >= slice */
    for (let i = slice; i < MM_TIME_SLICES.length; ++i)
      value = (value | (1 << i)) >>> 0;
    break;
  case 'at':
    /* Time == slice */
    value = (1 << slice) >>> 0;
    break;
  default:
    throw new Error(`Invalid MM time operator: ${operator}`);
  }

  return state => {
    if (state.areaData.mmTime & value) {
      const restrictions = defaultRestrictions();
      restrictions.mmTime = ~value >>> 0;
      return { result: true, restrictions };
    } else {
      return {
        result: false,
      };
    }
  };
}
