import { describe, it, expect } from 'vitest';
import { Items, DEFAULT_SETTINGS, makeSettings } from '@ootmm/core';

import { ExprParser } from '../../src/expr/parser';
import {
  exprTrue, exprFalse, exprAnd, exprOr,
  exprHas, exprEvent, exprAgeString,
  exprRenewable, exprLicense, exprMasks,
  exprOotTime, exprMmTime, exprFlagOn, exprFlagOff,
  exprSongEvent,
} from '../../src/expr/builder';
import { CONSTRAINT_FLAGS } from '../../src/expr/data';
import type { ResolvedWorldFlags } from '../../src/world/flags';

const NO_FLAGS = new Proxy({} as ResolvedWorldFlags, {
  get: () => ({ has: () => false }),
});

function makeParser(game: 'oot' | 'mm' = 'oot') {
  return new ExprParser(DEFAULT_SETTINGS, NO_FLAGS, game);
}

function parse(input: string, game: 'oot' | 'mm' = 'oot') {
  return makeParser(game).parse(input);
}

describe('constants', () => {
  it('parses true', () => {
    expect(parse('true')).toBe(exprTrue());
  });

  it('parses false', () => {
    expect(parse('false')).toBe(exprFalse());
  });
});

describe('has()', () => {
  it('parses has with implicit count 1', () => {
    expect(parse('has(OOT_SWORD)')).toBe(exprHas(Items.OOT_SWORD, 1));
  });

  it('parses has with explicit count', () => {
    expect(parse('has(OOT_SWORD, 3)')).toBe(exprHas(Items.OOT_SWORD, 3));
  });

  it('prefixes bare item names with game', () => {
    expect(parse('has(SWORD)', 'oot')).toBe(exprHas(Items.OOT_SWORD, 1));
    expect(parse('has(SWORD)', 'mm')).toBe(exprHas(Items.MM_SWORD, 1));
  });

  it('does not re-prefix already-prefixed item names', () => {
    expect(parse('has(OOT_SWORD)', 'mm')).toBe(exprHas(Items.OOT_SWORD, 1));
  });

  it('parses renewable()', () => {
    expect(parse('renewable(OOT_SWORD)')).toBe(exprRenewable(Items.OOT_SWORD));
  });

  it('parses license()', () => {
    expect(parse('license(OOT_SWORD)')).toBe(exprLicense(Items.OOT_SWORD));
  });
});

describe('event()', () => {
  it('parses event and prefixes with game', () => {
    expect(parse('event(MY_EVENT)', 'oot')).toBe(exprEvent('OOT_MY_EVENT'));
    expect(parse('event(MY_EVENT)', 'mm')).toBe(exprEvent('MM_MY_EVENT'));
  });

  it('does not re-prefix already-prefixed events', () => {
    expect(parse('event(OOT_MY_EVENT)', 'mm')).toBe(exprEvent('OOT_MY_EVENT'));
  });
});

describe('age()', () => {
  it('parses age(child)', () => {
    expect(parse('age(child)')).toBe(exprAgeString('child'));
  });

  it('parses age(adult)', () => {
    expect(parse('age(adult)')).toBe(exprAgeString('adult'));
  });

  it('throws on invalid age', () => {
    expect(() => parse('age(infant)')).toThrow();
  });
});

describe('masks()', () => {
  it('parses masks', () => {
    expect(parse('masks(3)')).toBe(exprMasks(3));
  });
});

describe('&&  ||', () => {
  it('parses a && b', () => {
    expect(parse('has(OOT_SWORD) && has(OOT_BOW)')).toBe(
      exprAnd([exprHas(Items.OOT_SWORD, 1), exprHas(Items.OOT_BOW, 1)])
    );
  });

  it('parses a || b', () => {
    expect(parse('has(OOT_SWORD) || has(OOT_BOW)')).toBe(
      exprOr([exprHas(Items.OOT_SWORD, 1), exprHas(Items.OOT_BOW, 1)])
    );
  });

  it('returns a single expr without a wrapper for a lone operand', () => {
    expect(parse('has(OOT_SWORD)')).toBe(exprHas(Items.OOT_SWORD, 1));
  });

  it('handles parenthesised grouping', () => {
    const inner = exprOr([exprHas(Items.OOT_SWORD, 1), exprHas(Items.OOT_BOW, 1)]);
    expect(parse('(has(OOT_SWORD) || has(OOT_BOW)) && has(OOT_BOMB)')).toBe(
      exprAnd([inner, exprHas(Items.OOT_BOMB, 1)])
    );
  });

  it('throws when && follows || without parens (ambiguous)', () => {
    expect(() => parse('has(OOT_SWORD) || has(OOT_BOW) && has(OOT_BOMB)')).toThrow();
  });

  it('throws when || follows && without parens (ambiguous)', () => {
    expect(() => parse('has(OOT_SWORD) && has(OOT_BOW) || has(OOT_BOMB)')).toThrow();
  });
});

describe('! and cond()', () => {
  it('! true collapses to false', () => {
    expect(parse('!true')).toBe(exprFalse());
  });

  it('! false collapses to true', () => {
    expect(parse('!false')).toBe(exprTrue());
  });

  it('cond(true, a, b) collapses to a', () => {
    expect(parse('cond(true, has(OOT_SWORD), false)')).toBe(exprHas(Items.OOT_SWORD, 1));
  });

  it('cond(false, a, b) collapses to b', () => {
    expect(parse('cond(false, has(OOT_SWORD), true)')).toBe(exprTrue());
  });
});

describe('oot_time()', () => {
  it('parses day', () => {
    expect(parse('oot_time(day)')).toBe(exprOotTime('day'));
  });

  it('parses night', () => {
    expect(parse('oot_time(night)')).toBe(exprOotTime('night'));
  });
});

describe('mm_time()', () => {
  it('parses mm_time with a single slice', () => {
    expect(parse('mm_time(at, DAY1_AM_06_00)')).toBe(exprMmTime('at', ['DAY1_AM_06_00']));
  });

  it('parses mm_time with multiple slices (between)', () => {
    expect(parse('mm_time(between, DAY1_AM_06_00, DAY1_AM_08_00)')).toBe(
      exprMmTime('between', ['DAY1_AM_06_00', 'DAY1_AM_08_00'])
    );
  });
});

describe('flag_on() / flag_off()', () => {
  const flag = CONSTRAINT_FLAGS[0];

  it('parses flag_on', () => {
    expect(parse(`flag_on(${flag})`)).toBe(exprFlagOn(flag));
  });

  it('parses flag_off', () => {
    expect(parse(`flag_off(${flag})`)).toBe(exprFlagOff(flag));
  });

  it('throws on unknown flag', () => {
    expect(() => parse('flag_on(UNKNOWN_FLAG)')).toThrow();
  });
});

describe('_song_event()', () => {
  it('parses _song_event', () => {
    expect(parse('_song_event(2, 1)')).toBe(exprSongEvent(2, 1));
  });
});

describe('setting()', () => {
  it('boolean setting: true when enabled', () => {
    const settings = makeSettings({ generateSpoilerLog: true });
    const p = new ExprParser(settings, NO_FLAGS, 'oot');
    expect(p.parse('setting(generateSpoilerLog)')).toBe(exprTrue());
  });

  it('boolean setting: false when disabled', () => {
    const settings = makeSettings({ generateSpoilerLog: false });
    const p = new ExprParser(settings, NO_FLAGS, 'oot');
    expect(p.parse('setting(generateSpoilerLog)')).toBe(exprFalse());
  });

  it('enum setting: true for matching value', () => {
    const settings = makeSettings({ goal: 'ganon' });
    const p = new ExprParser(settings, NO_FLAGS, 'oot');
    expect(p.parse('setting(goal, ganon)')).toBe(exprTrue());
  });

  it('enum setting: false for non-matching value', () => {
    const settings = makeSettings({ goal: 'ganon' });
    const p = new ExprParser(settings, NO_FLAGS, 'oot');
    expect(p.parse('setting(goal, majora)')).toBe(exprFalse());
  });
});

describe('trick()', () => {
  it('true when trick is enabled', () => {
    const settings = makeSettings({ tricks: ['OOT_LENS'] });
    const p = new ExprParser(settings, NO_FLAGS, 'oot');
    expect(p.parse('trick(OOT_LENS)')).toBe(exprTrue());
  });

  it('false when trick is disabled', () => {
    const settings = makeSettings({ tricks: [] });
    const p = new ExprParser(settings, NO_FLAGS, 'oot');
    expect(p.parse('trick(OOT_LENS)')).toBe(exprFalse());
  });

  it('throws on unknown trick', () => {
    expect(() => parse('trick(UNKNOWN_TRICK)')).toThrow();
  });
});

describe('numeric expressions in has()', () => {
  it('parses a literal count', () => {
    expect(parse('has(OOT_SWORD, 2)')).toBe(exprHas(Items.OOT_SWORD, 2));
  });

  it('parses addition', () => {
    expect(parse('has(OOT_SWORD, 1+2)')).toBe(exprHas(Items.OOT_SWORD, 3));
  });

  it('parses subtraction', () => {
    expect(parse('has(OOT_SWORD, 5-2)')).toBe(exprHas(Items.OOT_SWORD, 3));
  });

  it('parses a hex literal', () => {
    expect(parse('has(OOT_SWORD, 0x3)')).toBe(exprHas(Items.OOT_SWORD, 3));
    expect(parse('has(OOT_SWORD, 0xff)')).toBe(exprHas(Items.OOT_SWORD, 255));
  });

  it('parses a var()', () => {
    const p = makeParser();
    p.addVar('MY_COUNT', 4);
    expect(p.parse('has(OOT_SWORD, var(MY_COUNT))')).toBe(exprHas(Items.OOT_SWORD, 4));
  });

  it('throws on unknown var', () => {
    expect(() => parse('has(OOT_SWORD, var(NO_SUCH_VAR))')).toThrow();
  });

  it('parses parenthesised arithmetic', () => {
    expect(parse('has(OOT_SWORD, (1+2))')).toBe(exprHas(Items.OOT_SWORD, 3));
  });

  it('transforms a zero-item expression into true', () => {
    expect(parse('has(OOT_SWORD, 1-1)')).toBe(exprTrue());
  });
});

describe('macros', () => {
  it('expands a no-arg macro', () => {
    const p = makeParser();
    p.addMacro('HAS_SWORD', [], 'has(OOT_SWORD)');
    expect(p.parse('HAS_SWORD')).toBe(exprHas(Items.OOT_SWORD, 1));
  });

  it('expands a macro with a single identifier argument', () => {
    const p = makeParser();
    p.addMacro('HAS_ITEM', ['item'], 'has(item)');
    expect(p.parse('HAS_ITEM(OOT_SWORD)')).toBe(exprHas(Items.OOT_SWORD, 1));
  });

  it('expands a macro with a numeric argument', () => {
    const p = makeParser();
    p.addMacro('HAS_SWORDS', ['n'], 'has(OOT_SWORD, n)');
    expect(p.parse('HAS_SWORDS(3)')).toBe(exprHas(Items.OOT_SWORD, 3));
  });

  it('produces the same memoized expr for two identical macro expansions', () => {
    const p = makeParser();
    p.addMacro('HAS_SWORD', [], 'has(OOT_SWORD)');
    expect(p.parse('HAS_SWORD && HAS_SWORD')).toBe(exprHas(Items.OOT_SWORD, 1));
  });

  it('throws when wrong number of args are passed', () => {
    const p = makeParser();
    p.addMacro('HAS_ITEM', ['item'], 'has(item)');
    expect(() => p.parse('HAS_ITEM(OOT_SWORD, OOT_BOW)')).toThrow();
  });
});

describe('whitespace', () => {
  it('tolerates extra spaces', () => {
    expect(parse('  has( OOT_SWORD , 2 )  ')).toBe(exprHas(Items.OOT_SWORD, 2));
  });

  it('tolerates tabs and newlines', () => {
    expect(parse('has(OOT_SWORD)\t&&\nhas(OOT_BOW)')).toBe(
      exprAnd([exprHas(Items.OOT_SWORD, 1), exprHas(Items.OOT_BOW, 1)])
    );
  });
});

describe('error cases', () => {
  it('throws on empty input', () => {
    expect(() => parse('')).toThrow();
  });

  it('throws on unknown token', () => {
    expect(() => parse('@')).toThrow();
  });

  it('throws on unmatched paren', () => {
    expect(() => parse('(has(OOT_SWORD)')).toThrow();
  });

  it('throws on trailing junk', () => {
    expect(() => parse('has(OOT_SWORD) garbage')).toThrow();
  });
});
