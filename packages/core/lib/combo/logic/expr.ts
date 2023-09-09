import { Item, ItemGroups, Items, ItemsCount } from '../items';
import { GLITCHES, Settings, TRICKS } from '../settings';
import { Age } from './pathfind';
import { PRICE_RANGES } from './price';
import { World } from './world';

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

export type Expr = (state: State) => ExprResult;

export const exprTrue = (): Expr => exprImplTrue;
export const exprFalse = (): Expr => exprImplFalse;

export const exprAnd = (exprs: Expr[]): Expr => {
  /* Optimizations */
  exprs = exprs.filter(x => x !== exprImplTrue);
  if (exprs.length === 0) {
    return exprTrue();
  }
  if (exprs.some(x => x === exprImplFalse)) {
    return exprFalse();
  }
  if (exprs.length === 1) {
    return exprs[0];
  }

  return state => {
    const results: ExprResult[] = [];
    for (const e of exprs) {
      const r = e(state);
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
};

export const exprOr = (exprs: Expr[]): Expr => {
  /* Optimizations */
  exprs = exprs.filter(x => x !== exprImplFalse);
  if (exprs.length === 0) {
    return exprFalse();
  }
  if (exprs.some(x => x === exprImplTrue)) {
    return exprTrue();
  }
  if (exprs.length === 1) {
    return exprs[0];
  }

  return state => {
    const results: ExprResult[] = [];
    let result = false;

    for (const e of exprs) {
      const r = e(state);
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
};

export const exprNot = (expr: Expr): Expr => {
  if (expr === exprImplTrue) {
    return exprFalse();
  }
  if (expr === exprImplFalse) {
    return exprTrue();
  }
  return state => expr(state).result ? exprFalse()(state) : exprTrue()(state);
}

export const exprCond = (cond: Expr, then: Expr, otherwise: Expr): Expr => {
  if (cond === exprImplTrue) {
    return then;
  }
  if (cond === exprImplFalse) {
    return otherwise;
  }
  return state => cond(state).result ? then(state) : otherwise(state);
}

export const exprAge = (age: Age): Expr => state => state.age === age ? exprTrue()(state) : exprFalse()(state);

export const exprHas = (item: Item, count: number): Expr => {
  if (count <= 0) {
    return exprTrue();
  }
  const depItems = [item];
  const depEvents: string[] = [];

  return state => {
    const result = (state.ignoreItems || (itemCount(state, item) >= count));
    return { result, depItems, depEvents };
  }
};

export const exprRenewable = (item: Item): Expr => {
  const depItems = [item];
  const depEvents: string[] = [];
  return state => {
    const result = (state.ignoreItems || (state.renewables.get(item) || 0) > 0);
    return { result, depItems, depEvents };
  }
};

export const exprLicense = (item: Item): Expr => {
  const depItems = [item];
  const depEvents: string[] = [];

  return state => {
    const result = (state.ignoreItems || (state.licenses.get(item) || 0) > 0);
    return { result, depItems, depEvents };
  }
};

export const exprEvent = (event: string): Expr => state => {
  const result = state.events.has(event);
  return { result, depItems: [], depEvents: [event] };
};

export const exprMasks = (count: number): Expr => state => {
  const result = state.ignoreItems || (itemsCount(state, [...ItemGroups.MASKS_REGULAR]) >= count);
  return { result, depItems: [...ItemGroups.MASKS_REGULAR], depEvents: [] };
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
  return settings.tricks.includes(trick as keyof typeof TRICKS) ? exprTrue() : exprFalse();
};

export const exprGlitch = (settings: Settings, glitch: string): Expr => {
  if (!GLITCHES.hasOwnProperty(glitch)) {
    throw new Error(`Glitch ${glitch} not found`);
  }
  return settings.glitches.includes(glitch as keyof typeof GLITCHES) ? exprTrue() : exprFalse();
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
      return { result: true, depItems: [], depEvents: [], restrictions };
    } else {
      return {
        result: false,
        depItems: [],
        depEvents: [],
      };
    }
  };
}

export const exprMmTime = (operator: string, sliceName: string): Expr => {
  const slice = MM_TIME_SLICES.indexOf(sliceName);
  let value = 0;
  let value2 = 0;

  if (slice === -1) {
    throw new Error(`Invalid MM time slice: ${sliceName}`);
  }

  switch (operator) {
  case 'before':
    /* Time < slice */
    for (let i = 0; i < slice; ++i)
      if (i < 32) {
        value = (value | (1 << i)) >>> 0;
      } else {
        value2 = (value2 | (1 << (i - 32))) >>> 0;
      }
    break;
  case 'after':
    /* Time >= slice */
    for (let i = slice; i < MM_TIME_SLICES.length; ++i)
      if (i < 32) {
        value = (value | (1 << i)) >>> 0;
      } else {
        value2 = (value2 | (1 << (i - 32))) >>> 0;
      }
    break;
  case 'at':
    /* Time == slice */
    if (slice < 32) {
      value = (1 << slice) >>> 0;
    } else {
      value2 = (1 << (slice - 32)) >>> 0;
    }
    break;
  default:
    throw new Error(`Invalid MM time operator: ${operator}`);
  }

  return state => {
    if ((state.areaData.mmTime & value) || (state.areaData.mmTime2 & value2)) {
      const restrictions = defaultRestrictions();
      restrictions.mmTime = ~value >>> 0;
      restrictions.mmTime2 = ~value2 >>> 0;
      return { result: true, restrictions, depItems: [], depEvents: [] };
    } else {
      return {
        result: false,
        depItems: [],
        depEvents: [],
      };
    }
  };
}

export const exprPrice = (range: string, id: number, max: number): Expr => state => {
  const price = state.world.prices[id + PRICE_RANGES[range]];
  const result = price <= max;
  return { result, depItems: [], depEvents: [] };
}
