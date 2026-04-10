import type { Expr } from './types';
import { memoExpr } from './memo';
import { AGE_ADULT, AGE_CHILD, type Age } from '../age';
import { CONSTRAINT_FLAGS, MM_TIME_SLICES, OOT_TIME } from './data';
import { Items, SETTINGS, TRICKS, type Item, type ItemID, type Settings, type TrickKey } from '@ootmm/core';
import { WORLD_FLAGS } from '../world-flags';
import { PRICE_RANGES } from '../price';
import { compileExpr } from './eval';

const EXPR_FALSE = compileExpr({ type: 'false' });
const EXPR_TRUE = compileExpr({ type: 'true' });
const EXPR_AGE_CHILD = compileExpr({ type: 'age', age: AGE_CHILD });
const EXPR_AGE_ADULT = compileExpr({ type: 'age', age: AGE_ADULT });
const EXPR_TIME_OOT_DAY = compileExpr({ type: 'time-oot', flag: OOT_TIME.DAY });
const EXPR_TIME_OOT_NIGHT = compileExpr({ type: 'time-oot', flag: OOT_TIME.NIGHT });
const EXPR_TIME_OOT_DUSK = compileExpr({ type: 'time-oot', flag: OOT_TIME.DUSK });

export function exprFalse() {
  return EXPR_FALSE;
}

export function exprTrue() {
  return EXPR_TRUE;
}

export function exprAnd(exprs: Expr[]) {
  /* Parse-time Optimizations */
  exprs = exprs.filter((x) => x !== EXPR_TRUE);
  if (exprs.length === 0) {
    return EXPR_TRUE;
  }
  if (exprs.some((x) => x === EXPR_FALSE)) {
    return EXPR_FALSE;
  }

  exprs = [...new Set(exprs)];
  if (exprs.length === 1) {
    return exprs[0];
  }

  return compileExpr({ type: 'and', exprs });
}

export function exprOr(exprs: Expr[]) {
  /* Optimizations */
  exprs = exprs.filter((x) => x !== EXPR_FALSE);
  if (exprs.length === 0) {
    return EXPR_FALSE;
  }
  if (exprs.some((x) => x === EXPR_TRUE)) {
    return EXPR_TRUE;
  }

  exprs = [...new Set(exprs)];
  if (exprs.length === 1) {
    return exprs[0];
  }

  return compileExpr({ type: 'or', exprs });
}

export function exprCond(cond: Expr, then: Expr, otherwise: Expr) {
  if (cond === EXPR_TRUE) {
    return then;
  }
  if (cond === EXPR_FALSE) {
    return otherwise;
  }

  throw new Error(`Expression is not allowed at runtime`);
}

export function exprNot(expr: Expr) {
  return exprCond(expr, EXPR_FALSE, EXPR_TRUE);
}

export function exprAge(age: Age) {
  /* Avoids creating a new object for every call */
  switch (age) {
    case AGE_CHILD:
      return EXPR_AGE_CHILD;
    case AGE_ADULT:
      return EXPR_AGE_ADULT;
    default:
      throw new Error(`Invalid age: ${age}`);
  }
}

export function exprAgeString(age: string) {
  switch (age) {
    case 'child':
      return EXPR_AGE_CHILD;
    case 'adult':
      return EXPR_AGE_ADULT;
    default:
      throw new Error(`Invalid age: ${age}`);
  }
}

export function exprHas(item: Item, count: number) {
  if (count <= 0) {
    return EXPR_TRUE;
  }

  return compileExpr({ type: 'item', item, count });
}

export function exprRenewable(item: Item) {
  return compileExpr({ type: 'renewable', item });
}

export function exprLicense(item: Item) {
  return compileExpr({ type: 'license', item });
}

export function exprEvent(event: string) {
  return compileExpr({ type: 'event', event });
}

export function exprMasks(count: number) {
  return compileExpr({ type: 'masks', count });
}

export function exprSpecial(settings: Settings, special: string) {
  return compileExpr({ type: 'special', specialId: special });
}

export function exprSetting(settings: Settings, resolvedFlags: Record<string, any>, setting: string, value: string | undefined) {
  if ((WORLD_FLAGS as readonly string[]).includes(setting)) {
    /* World flag */
    if (!resolvedFlags.hasOwnProperty(setting)) {
      throw new Error(`Unknown world flag: ${setting}`);
    }
    const f = resolvedFlags[setting as keyof typeof resolvedFlags];
    const s = SETTINGS.find((x) => x.key === f.setting)!;
    if (!((s as any).values as any[]).some((x) => x.value === value)) {
      throw new Error(`Invalid world flag value: ${value} (for flag: ${setting})`);
    }

    if (value === undefined) {
      throw new Error(`World flag ${setting} requires a value`);
    }

    if (f.has(value)) {
      return EXPR_TRUE;
    } else {
      return EXPR_FALSE;
    }
  } else {
    /* Normal setting */
    const s = SETTINGS.find((x) => x.key === setting);
    const v = (settings as any)[setting];
    if (v === undefined || s === undefined) {
      throw new Error(`Setting ${setting} not found`);
    }
    if (s.type === 'boolean') {
      if (value !== undefined) {
        throw new Error(`Setting ${setting} does not take a value`);
      }
      return v ? EXPR_TRUE : EXPR_FALSE;
    } else {
      if (value === undefined) {
        throw new Error(`Setting ${setting} requires a value`);
      }
      return v === value ? EXPR_TRUE : EXPR_FALSE;
    }
  }
}

export function exprTrick(settings: Settings, trick: string) {
  if (!TRICKS.hasOwnProperty(trick)) {
    throw new Error(`Trick ${trick} not found`);
  }
  return settings.tricks.includes(trick as TrickKey) ? EXPR_TRUE : EXPR_FALSE;
}

export function exprOotTime(time: string) {
  switch (time) {
    case 'day':
      return EXPR_TIME_OOT_DAY;
    case 'night':
      return EXPR_TIME_OOT_NIGHT;
    case 'dusk':
      return EXPR_TIME_OOT_DUSK;
    default:
      throw new Error(`Invalid OoT time: ${time}`);
  }
}

export function exprMmTime(operator: string, sliceNames: string[]) {
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

  return compileExpr({ type: 'time-mm', value, value2 });
}

export function exprPrice(range: string, id: number, max: number) {
  const slot = id + PRICE_RANGES[range];
  return compileExpr({ type: 'price', slot, max });
}

export function exprSongEvent(songId: number, cmp: number) {
  return compileExpr({ type: 'song-event', songId, cmp });
}

export function exprFish(ageAndType: string, minPounds: number, maxPounds: number) {
  const items: Item[] = [];

  for (let i = minPounds; i <= maxPounds; i++) {
    const key = `OOT_FISHING_POND_${ageAndType}_${i}LBS` as ItemID;
    const item = Items[key];
    items.push(item);
  }

  const exprs = items.map((item) => exprHas(item, 1));
  return exprOr(exprs);
}

export function exprFlagOn(flag: string) {
  const index = CONSTRAINT_FLAGS.indexOf(flag);
  if (index === -1) {
    throw new Error(`Unknown constraint flag: ${flag}`);
  }
  return compileExpr({ type: 'flag-on', flag: index });
}

export function exprFlagOff(flag: string) {
  const index = CONSTRAINT_FLAGS.indexOf(flag);
  if (index === -1) {
    throw new Error(`Unknown constraint flag: ${flag}`);
  }
  return compileExpr({ type: 'flag-off', flag: index });
}
