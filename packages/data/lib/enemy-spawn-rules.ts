import type { Game } from './game';

export type EnemySpawnHabitat = 'ground' | 'air' | 'water-surface' | 'underwater';
export type EnemySpawnSetting = 'restoreBrokenActors' | 'enemyShuffleLaggySpawns';
export type EnemySpawnScalar = string | number | boolean | null;
export type EnemySpawnPredicate = boolean | string |
  { all: EnemySpawnPredicate[] } | { any: EnemySpawnPredicate[] } | { not: EnemySpawnPredicate } |
  { field: string; mask?: number; except?: EnemySpawnScalar[]; equals?: EnemySpawnScalar; oneOf?: EnemySpawnScalar[];
    contains?: EnemySpawnScalar; containsAny?: EnemySpawnScalar[]; subsetOf?: EnemySpawnScalar[];
    equalsField?: string; containsField?: string; subsetOfField?: string;
    exists?: boolean; nonempty?: boolean; min?: number; max?: number };

export type EnemySpawnRoom = {
  game: Game;
  scene?: number;
  room?: number;
  setup?: number;
  slots?: number[];
  mq?: boolean;
};

export type EnemySpawnRule = {
  rooms?: EnemySpawnRoom[];
  spawns?: string[];
  when?: EnemySpawnPredicate;
  allow?: EnemySpawnPredicate | EnemySpawnPredicate[];
  deny?: EnemySpawnPredicate | EnemySpawnPredicate[];
  preserve?: boolean;
  preserveUnless?: EnemySpawnSetting;
  restrictions?: string[];
  meets?: string[];
  explicit?: string[];
  varOverrides?: Record<string, number>;
  logicOverrides?: Record<string, string>;
  skipRaycast?: boolean;
  position?: (string | number)[][];
  maxHeightDifference?: number;
  limits?: Record<string, { when: EnemySpawnPredicate; maximum: number }[]>;
};

export type EnemySpawnRules = {
  macros: Record<string, EnemySpawnPredicate>;
  rules: Record<string, EnemySpawnRule>;
  limits?: Record<string, { when: EnemySpawnPredicate; maximum: number }[]>;
};

export type EnemyOotSpawnTargetPolicy = { restrictions: string[]; requires: string[] };
export type EnemyOotSpawnRoom = { scene: number; room: number; setup: number; slots: number[]; mq: boolean };
export type EnemyOotSpawnLocationPolicy = {
  rooms: EnemyOotSpawnRoom[];
  choices: string[];
  restrictions: string[];
  meets: string[];
  disallowed: string[];
  explicit: string[];
  varOverrides: Record<string, number>;
  logicOverrides: Record<string, string>;
  skipRaycast: boolean;
  broken: boolean;
  position: (string | number)[][];
};
export type EnemyOotSpawnTemplate = Omit<EnemyOotSpawnLocationPolicy, 'rooms'>;
export type EnemyMmSpawnActorPolicy = {
  actorId: number;
  type: number;
  stationary: boolean;
};
export type EnemySpawnActorType = { game: Game; actorId: number; type: number; stationary: boolean };
export type EnemyOotRuntimeSpawnRule = { scene: number; room: number; maxHeightDifference: number };

const fail = (where: string): never => { throw new Error('Invalid enemy spawn policy: ' + where); };
const record = (value: unknown, where: string): Record<string, unknown> => {
  if (!value || typeof value !== 'object' || Array.isArray(value)) return fail(where + ': expected mapping');
  const result = value as Record<string, unknown>;
  if (Object.keys(result).some(key => !key.trim() || ['__proto__', 'prototype', 'constructor'].includes(key)))
    fail(where + ': invalid mapping key');
  return result;
};
const object = (value: unknown, keys: readonly string[], where: string): Record<string, unknown> => {
  const result = record(value, where);
  for (const key of Object.keys(result)) if (!keys.includes(key)) fail(where + ': unknown field ' + key);
  return result;
};
const integer = (value: unknown, max: number, where: string): number => {
  if (!Number.isInteger(value) || (value as number) < 0 || (value as number) > max) fail(where + ': invalid integer');
  return value as number;
};
const numberList = (value: unknown, max: number, where: string, empty = true): number[] => {
  if (!Array.isArray(value) || (!empty && !value.length)) return fail(where + ': expected number list');
  value.forEach(entry => integer(entry, max, where));
  if (new Set(value).size !== value.length) fail(where + ': duplicate number');
  return value as number[];
};
const stringList = (value: unknown, where: string): string[] => {
  if (!Array.isArray(value) || value.some(entry => typeof entry !== 'string' || !entry.trim()))
    return fail(where + ': expected name list');
  if (new Set(value).size !== value.length) fail(where + ': duplicate name');
  return value as string[];
};
const bool = (value: unknown, where: string): boolean => {
  if (typeof value !== 'boolean') fail(where + ': expected boolean');
  return value as boolean;
};
const scalar = (value: unknown, where: string) => {
  if (value !== null && typeof value !== 'string' && typeof value !== 'boolean' &&
      !(typeof value === 'number' && Number.isFinite(value))) fail(where + ': expected scalar');
};
const fieldPath = (value: unknown, where: string) => {
  if (typeof value !== 'string' || !/^(source|target|location|settings|facts)(\.[A-Za-z_][A-Za-z0-9_]*)+$/.test(value) ||
      value.split('.').some(part => ['__proto__', 'prototype', 'constructor'].includes(part))) fail(where + ': invalid context field');
};

/* Enemy aliases are ordinary macros. Compact selectors and lists expand to the
 * same predicate language used by the planner; they do not create a second
 * registry or bypass allow/deny rules. */
function normalizePredicate(value: unknown, where: string): EnemySpawnPredicate {
  if (typeof value === 'boolean' || typeof value === 'string') return value;
  if (Array.isArray(value)) return value.length ? { any: value.map(entry => normalizePredicate(entry, where)) } : false;
  const entry = record(value, where);
  if ('all' in entry || 'any' in entry) {
    const key = 'all' in entry ? 'all' : 'any';
    object(entry, [key], where);
    if (!Array.isArray(entry[key]) || !(entry[key] as unknown[]).length) fail(where + ': empty predicate list');
    return { [key]: (entry[key] as unknown[]).map(child => normalizePredicate(child, where)) } as EnemySpawnPredicate;
  }
  if ('not' in entry) {
    object(entry, ['not'], where);
    return { not: normalizePredicate(entry.not, where) };
  }
  if ('field' in entry) return entry as EnemySpawnPredicate;
  object(entry, ['game', 'actor', 'params', 'name', 'habitat'], where);
  if (!Object.keys(entry).length) fail(where + ': empty enemy selector');
  const terms: EnemySpawnPredicate[] = [];
  for (const [key, value] of Object.entries(entry)) {
    const field = 'target.' + (key === 'actor' ? 'actorId' : key);
    if (key === 'game' && value !== 'oot' && value !== 'mm') fail(where + ': game');
    if (key === 'habitat' && !['ground', 'air', 'water-surface', 'underwater'].includes(value as string)) fail(where + ': habitat');
    if (key === 'name' && (typeof value !== 'string' || !value.trim())) fail(where + ': name');
    if (key === 'actor' || key === 'params') {
      if (Array.isArray(value)) numberList(value, 0xffff, where + ': ' + key, false);
      else integer(value, 0xffff, where + ': ' + key);
    }
    terms.push(Array.isArray(value) ? { field, oneOf: value as EnemySpawnScalar[] } : { field, equals: value as EnemySpawnScalar });
  }
  return terms.length === 1 ? terms[0] : { all: terms };
}

function normalizeLimits(value: unknown, where: string) {
  return Object.fromEntries(Object.entries(record(value, where)).map(([name, entries]) => {
    if (!Array.isArray(entries) || !entries.length) fail(where + ': empty limit ' + name);
    return [name, (entries as unknown[]).map(input => {
      const entry = object(input, ['when', 'maximum'], where + ': ' + name);
      return { when: normalizePredicate(entry.when ?? true, where), maximum: entry.maximum };
    })];
  }));
}

/* The build rejects unknown fields and unresolved/cyclic macros. Policy data
 * can change without editing the evaluator or silently ignoring a typo. */
export function parseEnemySpawnRules(value: unknown): EnemySpawnRules {
  const root = object(value, ['macros', 'rules', 'limits'], 'root');
  const macros = Object.fromEntries(Object.entries(record(root.macros, 'macros'))
    .map(([name, value]) => [name, normalizePredicate(value, 'macro ' + name)]));
  const rules = Object.fromEntries(Object.entries(record(root.rules, 'rules')).map(([name, input]) => {
    const value = record(input, 'rule ' + name);
    const result = { ...value };
    for (const field of ['allow', 'deny']) if (value[field] !== undefined)
      result[field] = Array.isArray(value[field])
        ? (value[field] as unknown[]).map(entry => normalizePredicate(entry, name + ': ' + field))
        : normalizePredicate(value[field], name + ': ' + field);
    if (value.when !== undefined) result.when = normalizePredicate(value.when, name + ': when');
    if (value.limits !== undefined) result.limits = normalizeLimits(value.limits, name + ': limits');
    return [name, result];
  }));
  const edges = new Map<string, Set<string>>();
  const predicate = (value: unknown, where: string, refs: Set<string>): void => {
    if (typeof value === 'boolean') return;
    if (typeof value === 'string') {
      if (!Object.hasOwn(macros, value)) fail(where + ': unknown macro ' + value);
      refs.add(value);
      return;
    }
    const entry = record(value, where);
    if (Object.hasOwn(entry, 'all') || Object.hasOwn(entry, 'any')) {
      const key = Object.hasOwn(entry, 'all') ? 'all' : 'any';
      object(entry, [key], where);
      if (!Array.isArray(entry[key]) || !(entry[key] as unknown[]).length) fail(where + ': empty predicate list');
      for (const child of entry[key] as unknown[]) predicate(child, where, refs);
      return;
    }
    if (Object.hasOwn(entry, 'not')) {
      object(entry, ['not'], where);
      predicate(entry.not, where, refs);
      return;
    }
    const operators = ['equals', 'oneOf', 'contains', 'containsAny', 'subsetOf', 'equalsField', 'containsField', 'subsetOfField', 'exists', 'nonempty', 'min', 'max'];
    object(entry, ['field', 'mask', 'except', ...operators], where);
    fieldPath(entry.field, where);
    const active = operators.filter(key => Object.hasOwn(entry, key));
    if (active.length !== 1) fail(where + ': a field predicate requires exactly one comparison');
    const key = active[0];
    if (entry.except !== undefined) {
      if (!['subsetOf', 'subsetOfField'].includes(key) || !Array.isArray(entry.except)) fail(where + ': except requires a subset comparison');
      for (const item of entry.except as unknown[]) scalar(item, where + ': except');
    }
    if (entry.mask !== undefined) {
      integer(entry.mask, 0xffffffff, where + ': mask');
      if (!['equals', 'oneOf', 'min', 'max', 'equalsField'].includes(key)) fail(where + ': mask needs numeric comparison');
    }
    if (['equalsField', 'containsField', 'subsetOfField'].includes(key)) fieldPath(entry[key], where);
    else if (['exists', 'nonempty'].includes(key)) bool(entry[key], where);
    else if (['min', 'max'].includes(key)) {
      if (typeof entry[key] !== 'number' || !Number.isFinite(entry[key])) fail(where + ': expected finite bound');
    } else if (['oneOf', 'containsAny', 'subsetOf'].includes(key)) {
      if (!Array.isArray(entry[key])) fail(where + ': expected scalar list');
      for (const item of entry[key] as unknown[]) scalar(item, where);
    } else scalar(entry[key], where);
  };
  for (const [name, macro] of Object.entries(macros)) {
    const refs = new Set<string>();
    predicate(macro, 'macro ' + name, refs);
    edges.set(name, refs);
  }
  const visiting = new Set<string>(), visited = new Set<string>();
  const visit = (name: string) => {
    if (visiting.has(name)) fail('cyclic macro ' + name);
    if (visited.has(name)) return;
    visiting.add(name);
    for (const child of edges.get(name) ?? []) visit(child);
    visiting.delete(name);
    visited.add(name);
  };
  Object.keys(macros).forEach(visit);
  for (const [name, value] of Object.entries(rules)) {
    const rule = object(value, ['rooms', 'spawns', 'when', 'allow', 'deny', 'preserve', 'preserveUnless',
      'restrictions', 'meets', 'explicit', 'varOverrides', 'logicOverrides', 'skipRaycast', 'position', 'maxHeightDifference', 'limits'], 'rule ' + name);
    if (rule.rooms !== undefined) {
      if (!Array.isArray(rule.rooms) || !rule.rooms.length) fail(name + ': rooms');
      for (const value of rule.rooms as unknown[]) {
        const room = object(value, ['game', 'scene', 'room', 'setup', 'slots', 'mq'], name + ': room');
        if (room.game !== 'oot' && room.game !== 'mm') fail(name + ': game');
        for (const field of ['scene', 'room', 'setup']) if (room[field] !== undefined) integer(room[field], 0xff, name + ': ' + field);
        if (room.slots !== undefined) numberList(room.slots, 0xff, name + ': slots', false);
        if (room.mq !== undefined && (typeof room.mq !== 'boolean' || room.game !== 'oot')) fail(name + ': mq');
      }
    }
    if (rule.spawns !== undefined && !stringList(rule.spawns, name + ': spawns').length) fail(name + ': empty spawns');
    if (rule.when !== undefined) predicate(rule.when, name + ': when', new Set());
    for (const field of ['allow', 'deny']) if (rule[field] !== undefined) {
      const entries = Array.isArray(rule[field]) ? rule[field] as unknown[] : [rule[field]];
      entries.forEach(entry => predicate(entry, name + ': ' + field, new Set()));
    }
    for (const field of ['preserve', 'skipRaycast']) if (rule[field] !== undefined) bool(rule[field], name + ': ' + field);
    for (const field of ['restrictions', 'meets', 'explicit']) if (rule[field] !== undefined) stringList(rule[field], name + ': ' + field);
    if (rule.varOverrides !== undefined) for (const [target, value] of Object.entries(record(rule.varOverrides, name + ': varOverrides')))
      integer(value, 0xffff, name + ': variant ' + target);
    if (rule.logicOverrides !== undefined) for (const value of Object.values(record(rule.logicOverrides, name + ': logicOverrides')))
      if (typeof value !== 'string' || !value.trim()) fail(name + ': logic override');
    if (rule.position !== undefined) {
      if (!Array.isArray(rule.position)) fail(name + ': position');
      for (const step of rule.position as unknown[]) if (!Array.isArray(step) || step.length !== 3 ||
        !['x', 'y', 'z'].includes(step[0]) || !['set', 'add'].includes(step[1]) ||
        typeof step[2] !== 'number' || !Number.isFinite(step[2])) fail(name + ': position operation');
    }
    if (rule.maxHeightDifference !== undefined && (typeof rule.maxHeightDifference !== 'number' ||
      !Number.isFinite(rule.maxHeightDifference) || rule.maxHeightDifference < 0 || rule.maxHeightDifference > 32767)) fail(name + ': maxHeightDifference');
    for (const [limit, entries] of Object.entries(record(rule.limits ?? {}, name + ': limits'))) {
      if (!Array.isArray(entries) || !entries.length) fail(name + ': empty limit ' + limit);
      for (const entry of entries as unknown[]) {
        const value = object(entry, ['when', 'maximum'], name + ': limit ' + limit);
        predicate(value.when, name + ': limit condition', new Set());
        integer(value.maximum, 0xffff, name + ': maximum');
      }
    }
    if (rule.preserveUnless !== undefined && !['restoreBrokenActors', 'enemyShuffleLaggySpawns'].includes(rule.preserveUnless as string))
      fail(name + ': unknown preservation setting');
    if (rule.allow === undefined && rule.deny === undefined && rule.preserve === undefined &&
      rule.preserveUnless === undefined && rule.maxHeightDifference === undefined && rule.limits === undefined) fail(name + ': missing action');
  }
  for (const [name, value] of Object.entries(record(root.limits ?? {}, 'limits'))) {
    if (!Array.isArray(value) || !value.length) fail(name + ': expected limit list');
    for (const entry of value as unknown[]) {
      const limit = object(entry, ['when', 'maximum'], name + ': limit');
      predicate(limit.when, name + ': limit condition', new Set());
      integer(limit.maximum, 0xffff, name + ': maximum');
    }
  }
  return { macros, rules, ...(root.limits === undefined ? {} : { limits: root.limits }) } as EnemySpawnRules;
}

export function parseEnemySpawnActorTypes(input: unknown): Record<string, EnemySpawnActorType> {
  const result: Record<string, EnemySpawnActorType> = {};
  const ids = new Set<string>();
  for (const [name, value] of Object.entries(record(input, 'actor types'))) {
    const entry = object(value, ['game', 'actorId', 'type', 'stationary'], name);
    if (entry.game !== 'oot' && entry.game !== 'mm') fail(name + ': game');
    const actorId = integer(entry.actorId, 0xffff, name + ': actorId');
    const key = [entry.game, actorId].join('/');
    if (ids.has(key)) fail(name + ': duplicate actor type');
    ids.add(key);
    result[name] = { game: entry.game as Game, actorId, type: integer(entry.type, 4, name + ': type'), stationary: bool(entry.stationary, name + ': stationary') };
  }
  return result;
}

export function parseEnemyOotSpawnTargets(input: unknown): Record<string, EnemyOotSpawnTargetPolicy> {
  const result: Record<string, EnemyOotSpawnTargetPolicy> = {};
  for (const [name, value] of Object.entries(record(input, 'OoT targets'))) {
    const entry = object(value, ['restrictions', 'requires'], 'OoT target ' + name);
    result[name] = {
      restrictions: stringList(entry.restrictions ?? [], name + ': restrictions'),
      requires: stringList(entry.requires ?? [], name + ': requires'),
    };
  }
  return result;
}

function parseEnemyOotSpawnTemplate(value: unknown, name: string, targets: Record<string, EnemyOotSpawnTargetPolicy>): EnemyOotSpawnTemplate {
    const entry = object(value, ['choices', 'restrictions', 'meets', 'disallowed', 'explicit', 'varOverrides', 'logicOverrides', 'skipRaycast', 'broken', 'position'], name);
    const names = (value: unknown, field: string) => {
      const entries = stringList(value, name + ': ' + field);
      for (const target of entries) if (!Object.hasOwn(targets, target)) fail(name + ': unknown target ' + target);
      return entries;
    };
    const varOverrides = record(entry.varOverrides ?? {}, name + ': varOverrides');
    for (const [target, value] of Object.entries(varOverrides)) {
      if (!Object.hasOwn(targets, target)) fail(name + ': unknown variant override ' + target);
      integer(value, 0xffff, name + ': variant');
    }
    const logicOverrides = record(entry.logicOverrides ?? {}, name + ': logicOverrides');
    // These are the imported logic labels, including historical aliases.
    for (const value of Object.values(logicOverrides)) if (typeof value !== 'string' || !value.length) fail(name + ': logic override');
    const position = entry.position ?? [];
    if (!Array.isArray(position)) fail(name + ': position');
    for (const step of position as unknown[]) {
      if (!Array.isArray(step) || step.length !== 3 || !['x', 'y', 'z'].includes(step[0]) ||
          !['set', 'add'].includes(step[1]) || typeof step[2] !== 'number' || !Number.isFinite(step[2])) fail(name + ': position operation');
    }
    return {
      choices: names(entry.choices, 'choices'), restrictions: stringList(entry.restrictions ?? [], name + ': restrictions'),
      meets: stringList(entry.meets ?? [], name + ': meets'), disallowed: names(entry.disallowed ?? [], 'disallowed'),
      explicit: names(entry.explicit ?? [], 'explicit'), varOverrides: varOverrides as Record<string, number>,
      logicOverrides: logicOverrides as Record<string, string>, skipRaycast: bool(entry.skipRaycast ?? false, name + ': skipRaycast'),
      broken: bool(entry.broken ?? false, name + ': broken'), position: position as (string | number)[][],
    };
}

export function parseEnemyOotSpawnTemplates(input: unknown, targets: Record<string, EnemyOotSpawnTargetPolicy>): Record<string, EnemyOotSpawnTemplate> {
  return Object.fromEntries(Object.entries(record(input, 'OoT templates')).map(([name, value]) => [name, parseEnemyOotSpawnTemplate(value, name, targets)]));
}

export function parseEnemyOotSpawnLocations(input: unknown, targets: Record<string, EnemyOotSpawnTargetPolicy>): Record<string, EnemyOotSpawnLocationPolicy> {
  const result: Record<string, EnemyOotSpawnLocationPolicy> = {};
  const seen = new Set<string>();
  for (const [name, value] of Object.entries(record(input, 'OoT locations'))) {
    const { rooms: roomSelectors, ...entry } = record(value, name);
    if (!Array.isArray(roomSelectors) || !roomSelectors.length) fail(name + ': missing rooms');
    const rooms = (roomSelectors as unknown[]).map(value => {
      const room = object(value, ['scene', 'room', 'setup', 'slots', 'mq'], name + ': room');
      const parsed = {
        scene: integer(room.scene, 0xff, name + ': scene'), room: integer(room.room, 0xff, name + ': room'),
        setup: integer(room.setup, 0xff, name + ': setup'), slots: numberList(room.slots, 0xff, name + ': slots', false),
        mq: bool(room.mq, name + ': mq'),
      };
      for (const slot of parsed.slots) {
        const key = [parsed.mq, parsed.scene, parsed.room, parsed.setup, slot].join('/');
        if (seen.has(key)) fail(name + ': duplicate OoT slot ' + key);
        seen.add(key);
      }
      return parsed;
    });
    result[name] = { rooms, ...parseEnemyOotSpawnTemplate(entry, name, targets) };
  }
  return result;
}

export function parseEnemyMmSpawnActors(input: unknown): Record<string, EnemyMmSpawnActorPolicy> {
  const result: Record<string, EnemyMmSpawnActorPolicy> = {};
  const ids = new Set<number>();
  for (const [name, value] of Object.entries(record(input, 'MM actors'))) {
    const entry = object(value, ['actorId', 'type', 'stationary'], name);
    const actorId = integer(entry.actorId, 0xffff, name + ': actorId');
    if (ids.has(actorId)) fail(name + ': duplicate actorId');
    ids.add(actorId);
    result[name] = { actorId, type: integer(entry.type, 4, name + ': type'), stationary: bool(entry.stationary, name + ': stationary') };
  }
  return result;
}

type OotSource = { mq: boolean; scene: number; room: number; setup: number; slot: number };
type OotTarget = { name: string };
type OotLocationPolicy = Omit<EnemyOotSpawnLocationPolicy, 'rooms' | 'choices'> & { choices: number[] };

/* Physical actor tables retain identity/ROM facts. Every placement policy is
 * joined from YAML by the complete game/layout/scene/room/setup/slot key. */
export function applyEnemyOotSpawnPolicies<T extends OotTarget, L extends OotSource, A>(
  facts: { targets: T[]; locations: L[]; aliases: A }, additions: L[], locationsValue: unknown, targetsValue: unknown,
): { targets: (T & EnemyOotSpawnTargetPolicy)[]; locations: (L & OotLocationPolicy)[]; aliases: A } {
  const targetPolicies = parseEnemyOotSpawnTargets(targetsValue);
  const locationPolicies = parseEnemyOotSpawnLocations(locationsValue, targetPolicies);
  const targetIndices = new Map(facts.targets.map((target, index) => [target.name, index]));
  if (targetIndices.size !== facts.targets.length) fail('duplicate raw OoT target name');
  for (const name of Object.keys(targetPolicies)) if (!targetIndices.has(name)) fail('unknown raw OoT target ' + name);
  const targets = facts.targets.map(target => {
    const policy = targetPolicies[target.name];
    if (!policy) fail('missing OoT target policy ' + target.name);
    return { ...target, ...policy };
  });
  const bySlot = new Map<string, OotLocationPolicy>();
  for (const { rooms, choices, ...policy } of Object.values(locationPolicies)) {
    const compiled = { ...policy, choices: choices.map(name => targetIndices.get(name)!) };
    for (const room of rooms) for (const slot of room.slots)
      bySlot.set([room.mq, room.scene, room.room, room.setup, slot].join('/'), compiled);
  }
  const seen = new Set<string>();
  const locations = [...facts.locations, ...additions].map(location => {
    const key = [location.mq, location.scene, location.room, location.setup, location.slot].join('/');
    const policy = bySlot.get(key) ?? fail('missing OoT location policy ' + key);
    if (seen.has(key)) fail('duplicate raw OoT source slot ' + key);
    seen.add(key);
    return { ...location, ...policy };
  });
  for (const key of bySlot.keys()) if (!seen.has(key)) fail('policy references unknown OoT source slot ' + key);
  return { targets, locations, aliases: facts.aliases };
}

export function applyEnemyMmSpawnPolicies<T extends { key: string; actorId: number }>(roster: T[], value: unknown): (T & EnemyMmSpawnActorPolicy)[] {
  const policies = parseEnemyMmSpawnActors(value);
  const seen = new Set<string>();
  const result = roster.map(actor => {
    const policy = policies[actor.key];
    if (!policy || policy.actorId !== actor.actorId) fail('missing or mismatched MM actor policy ' + actor.key);
    seen.add(actor.key);
    return { ...actor, ...policy };
  });
  for (const key of Object.keys(policies)) if (!seen.has(key)) fail('policy references unknown MM actor ' + key);
  return result;
}

/* This projection is generated at build time from the two authored files.
 * Native actor tables are kept compatible with the existing placement solver;
 * editors only maintain point selectors and enemy macros. */
export function compileEnemySpawnPolicies(macrosInput: unknown, placementsInput: unknown) {
  const rawMacros = record(macrosInput, 'macros');
  const rules = parseEnemySpawnRules({ macros: rawMacros, rules: placementsInput });
  type Context = Record<string, unknown>;
  const read = (context: Context, path: string): unknown => path.split('.').reduce<unknown>((value, key) =>
    value && typeof value === 'object' && Object.hasOwn(value, key) ? (value as Context)[key] : undefined, context);
  type Truth = boolean | undefined;
  const all = (values: Iterable<Truth>): Truth => {
    let unknown = false;
    for (const value of values) { if (value === false) return false; unknown ||= value === undefined; }
    return unknown ? undefined : true;
  };
  const any = (values: Iterable<Truth>): Truth => {
    let unknown = false;
    for (const value of values) { if (value === true) return true; unknown ||= value === undefined; }
    return unknown ? undefined : false;
  };
  const targetOnly = new Map<string, boolean>();
  const usesTargetOnly = (predicate: EnemySpawnPredicate): boolean => {
    if (typeof predicate === 'boolean') return true;
    if (typeof predicate === 'string') {
      if (!targetOnly.has(predicate)) targetOnly.set(predicate, usesTargetOnly(rules.macros[predicate]));
      return targetOnly.get(predicate)!;
    }
    if ('all' in predicate) return predicate.all.every(usesTargetOnly);
    if ('any' in predicate) return predicate.any.every(usesTargetOnly);
    if ('not' in predicate) return usesTargetOnly(predicate.not);
    return [predicate.field, predicate.equalsField, predicate.containsField, predicate.subsetOfField]
      .every(field => field === undefined || field.startsWith('target.'));
  };
  Object.keys(rules.macros).forEach(usesTargetOnly);
  const targetCache = new Map<string, Map<string, Truth>>();
  /* Source actor facts and most settings are unavailable during the data build.
   * Unknown predicates must remain candidates until the runtime gate has those
   * facts. Only the shipped relaxed-pool setting is fixed to its native default. */
  const evaluate = (predicate: EnemySpawnPredicate, context: Context): Truth => {
    if (typeof predicate === 'boolean') return predicate;
    if (typeof predicate === 'string') {
      if (!targetOnly.get(predicate)) return evaluate(rules.macros[predicate], context);
      const cache = targetCache.get(predicate) ?? new Map<string, Truth>();
      const key = JSON.stringify(context.target) ?? '';
      if (!cache.has(key)) cache.set(key, evaluate(rules.macros[predicate], context));
      targetCache.set(predicate, cache);
      return cache.get(key);
    }
    if ('all' in predicate) return all(predicate.all.map(child => evaluate(child, context)));
    if ('any' in predicate) return any(predicate.any.map(child => evaluate(child, context)));
    if ('not' in predicate) {
      const value = evaluate(predicate.not, context);
      return value === undefined ? undefined : !value;
    }
    let value = read(context, predicate.field);
    if (value === undefined) return undefined;
    if (predicate.mask !== undefined) value = typeof value === 'number' ? (value & predicate.mask) >>> 0 : undefined;
    if ('equals' in predicate) return value === predicate.equals;
    if ('oneOf' in predicate) return predicate.oneOf!.includes(value as EnemySpawnScalar);
    if ('contains' in predicate) return Array.isArray(value) && value.includes(predicate.contains);
    if ('containsAny' in predicate) return Array.isArray(value) && predicate.containsAny!.some(item => value.includes(item));
    if ('subsetOf' in predicate) return Array.isArray(value) && value.every(item => predicate.except?.includes(item) || predicate.subsetOf!.includes(item));
    if ('equalsField' in predicate || 'containsField' in predicate || 'subsetOfField' in predicate) {
      const other = read(context, (predicate.equalsField ?? predicate.containsField ?? predicate.subsetOfField)!);
      if (other === undefined) return undefined;
      if ('equalsField' in predicate) return value === other;
      if ('containsField' in predicate) return Array.isArray(value) && value.includes(other);
      return Array.isArray(value) && Array.isArray(other) && value.every(item => predicate.except?.includes(item) || other.includes(item));
    }
    if ('exists' in predicate) return predicate.exists;
    if ('nonempty' in predicate) return Array.isArray(value) && (value.length > 0) === predicate.nonempty;
    if ('min' in predicate) return typeof value === 'number' && value >= predicate.min!;
    if ('max' in predicate) return typeof value === 'number' && value <= predicate.max!;
    return fail('unknown compiled predicate');
  };
  const matches = (predicate: EnemySpawnPredicate, context: Context) => evaluate(predicate, context) === true;
  const selection = (value: EnemySpawnRule['allow'], context: Context, fallback: boolean): Truth =>
    value === undefined ? fallback : Array.isArray(value) ? any(value.map(entry => evaluate(entry, context))) : evaluate(value, context);
  const aliases = Object.entries(rawMacros).filter(([name]) => name.startsWith('oot-native/')).map(([macro, value]) => {
    const row = object(value, ['game', 'actor', 'params', 'name', 'habitat'], macro);
    if (row.game !== 'oot' || typeof row.name !== 'string' || row.name !== macro.slice('oot-native/'.length)) fail(macro + ': native enemy alias');
    const actorId = integer(row.actor, 0xffff, macro + ': actor');
    const params = Array.isArray(row.params) ? numberList(row.params, 0xffff, macro + ': params', false)[0] : integer(row.params, 0xffff, macro + ': params');
    if (!['ground', 'air', 'water-surface', 'underwater'].includes(row.habitat as string)) fail(macro + ': native habitat');
    return { name: row.name as string, game: 'oot' as const, actorId, params, habitat: row.habitat as EnemySpawnHabitat };
  });
  if (!aliases.length) fail('no native OoT enemy macros');
  const targets = Object.fromEntries(aliases.map(target => [target.name, {
    restrictions: Object.keys(rules.macros).filter(name => name.startsWith('native-supports/') && matches(name, { target })).map(name => name.slice('native-supports/'.length)),
    requires: Object.keys(rules.macros).filter(name => name.startsWith('native-requires/') && matches(name, { target })).map(name => name.slice('native-requires/'.length)),
  }])) as Record<string, EnemyOotSpawnTargetPolicy>;

  const types = ['ground', 'aquatic', 'flying', 'hanging', 'flexible'];
  const ids = new Set<number>();
  const collectIds = (predicate: EnemySpawnPredicate): void => {
    if (typeof predicate === 'string') { collectIds(rules.macros[predicate]); return; }
    if (typeof predicate === 'boolean') return;
    if ('all' in predicate) predicate.all.forEach(collectIds);
    else if ('any' in predicate) predicate.any.forEach(collectIds);
    else if ('not' in predicate) collectIds(predicate.not);
    else if (predicate.field === 'target.actorId') {
      if (typeof predicate.equals === 'number') ids.add(integer(predicate.equals, 0xffff, 'classification actor'));
      for (const value of predicate.oneOf ?? []) if (typeof value === 'number') ids.add(integer(value, 0xffff, 'classification actor'));
    }
  };
  for (const type of types) {
    const name = 'placement-type/' + type;
    if (!rules.macros[name]) fail('missing placement class ' + name);
    collectIds(rules.macros[name]);
  }
  const actorTypes: Record<string, EnemySpawnActorType> = {};
  for (const game of ['oot', 'mm'] as const) for (const actorId of ids) {
    const target = { game, actorId };
    const matching = types.flatMap((name, index) => matches('placement-type/' + name, { target }) ? [index] : []);
    if (matching.length > 1) fail('overlapping placement classes for ' + game + '/' + actorId);
    if (matching.length) actorTypes[game + '/' + actorId] = { game, actorId, type: matching[0], stationary: matches('placement-stationary', { target }) };
  }
  const mmActors: Record<string, EnemyMmSpawnActorPolicy> = {};
  for (const [macro, value] of Object.entries(rawMacros)) {
    if (!macro.startsWith('mm-native/')) continue;
    const row = object(value, ['game', 'actor'], macro);
    if (row.game !== 'mm') fail(macro + ': native game');
    const actorId = integer(row.actor, 0xffff, macro + ': actor');
    const type = actorTypes['mm/' + actorId] ?? fail(macro + ': missing actor class');
    mmActors[macro.slice('mm-native/'.length)] = { actorId, type: type.type, stationary: type.stationary };
  }

  const locations: Record<string, EnemyOotSpawnLocationPolicy> = {};
  const templates: Record<string, EnemyOotSpawnTemplate> = {};
  const runtime: EnemyOotRuntimeSpawnRule[] = [];
  const limits: NonNullable<EnemySpawnRules['limits']> = {};
  type Point = Omit<EnemySpawnRoom, 'slots'> & { slot?: number };
  type NamedRule = { name: string; rule: EnemySpawnRule };
  const namedRules = Object.entries(rules.rules).map(([name, rule]) => ({ name, rule }));
  const attributeFields = ['restrictions', 'meets', 'explicit', 'varOverrides', 'logicOverrides', 'skipRaycast', 'position'] as const;
  const roomMatches = (room: EnemySpawnRoom, point: Point) => room.game === point.game &&
    (room.scene === undefined || room.scene === point.scene) && (room.room === undefined || room.room === point.room) &&
    (room.setup === undefined || room.setup === point.setup) && (room.mq === undefined || room.mq === !!point.mq) &&
    (room.slots === undefined || point.slot !== undefined && room.slots.includes(point.slot));
  const at = (point: Point, key?: string) => namedRules.filter(({ rule }) =>
    (!rule.rooms || rule.rooms.some(room => roomMatches(room, point))) && (!rule.spawns || key !== undefined && rule.spawns.includes(key)));
  const mergeAttributes = (name: string, attributes: Record<string, unknown>, rule: EnemySpawnRule) => {
    for (const field of attributeFields) {
      if (rule[field] === undefined) continue;
      if (field === 'varOverrides' || field === 'logicOverrides') {
        const previous = attributes[field] as Record<string, unknown> | undefined;
        const next = { ...previous };
        for (const [target, value] of Object.entries(rule[field]!)) {
          if (previous && Object.hasOwn(previous, target) && previous[target] !== value) fail(name + ': conflicting ' + field + ' for ' + target);
          next[target] = value;
        }
        attributes[field] = next;
      } else {
        if (attributes[field] !== undefined && JSON.stringify(attributes[field]) !== JSON.stringify(rule[field]))
          fail(name + ': conflicting native attribute ' + field);
        attributes[field] = rule[field];
      }
    }
  };
  const template = (name: string, point: Point, key?: string): EnemyOotSpawnTemplate => {
    const applicable = at(point, key);
    const attributes: Record<string, unknown> = {};
    for (const { name, rule } of applicable) if (rule.when === undefined) mergeAttributes(name, attributes, rule);
    const context = { location: point, source: { ...point, ...attributes, ...(key === undefined ? {} : { key }) },
      settings: { enemyShuffleIgnoreRestrictions: false } };
    for (const { name, rule } of applicable) if (rule.when !== undefined) {
      const condition = evaluate(rule.when, context);
      if (condition === true) mergeAttributes(name, attributes, rule);
      else if (condition === undefined && attributeFields.some(field => rule[field] !== undefined))
        fail(name + ': conditional native attributes require build-time location facts');
    }
    Object.assign(context.source, attributes);
    const forTarget = (target: typeof aliases[number]) => ({ ...context, target: { ...target,
      params: (attributes.varOverrides as Record<string, number> | undefined)?.[target.name] ?? target.params } });
    const active = (rule: EnemySpawnRule, context: Context) => rule.when === undefined || evaluate(rule.when, context) === true;
    const choices = aliases.filter(target => {
      const withTarget = forTarget(target);
      return applicable.every(({ rule }) => !active(rule, withTarget) ||
        selection(rule.allow, withTarget, true) !== false && selection(rule.deny, withTarget, false) !== true);
    }).map(target => target.name);
    const denied = new Set<string>();
    for (const { rule } of applicable) {
      const direct = Array.isArray(rule.deny) ? rule.deny : [rule.deny];
      const preferred = direct.flatMap(value => typeof value === 'string' && value.startsWith('oot-native/') ? [value.slice('oot-native/'.length)] : []);
      const matching = aliases.filter(target => {
        const withTarget = forTarget(target);
        return active(rule, withTarget) && selection(rule.deny, withTarget, false) === true;
      }).map(target => target.name);
      for (const target of [...preferred.filter(name => matching.includes(name)), ...matching]) denied.add(target);
    }
    const broken = applicable.some(({ rule }) => rule.preserve && active(rule, context));
    return parseEnemyOotSpawnTemplate({ choices, disallowed: [...denied], broken, ...attributes }, name, targets);
  };
  const points = new Map<string, { name: string; point: Point }>();
  const scripted = new Map<string, { name: string; points: Point[] }>();
  for (const [name, rule] of Object.entries(rules.rules)) {
    if (rule.maxHeightDifference !== undefined) {
      if (rule.when !== undefined || rule.spawns || !rule.rooms?.length) fail(name + ': runtime height requires unconditional rooms');
      for (const room of rule.rooms!) {
        if (room.game !== 'oot' || room.scene === undefined || room.room === undefined || room.setup !== undefined || room.slots !== undefined || room.mq !== undefined)
          fail(name + ': runtime height only accepts OoT scene/room selectors');
        const existing = runtime.find(row => row.scene === room.scene && row.room === room.room);
        if (existing) existing.maxHeightDifference = Math.min(existing.maxHeightDifference, rule.maxHeightDifference);
        else runtime.push({ scene: room.scene!, room: room.room!, maxHeightDifference: rule.maxHeightDifference });
      }
    }
    for (const [limit, entries] of Object.entries(rule.limits ?? {})) {
      const locationTerms: EnemySpawnPredicate[] = [];
      if (rule.when !== undefined) locationTerms.push(rule.when);
      if (rule.spawns) locationTerms.push({ field: 'source.key', oneOf: rule.spawns });
      if (rule.rooms) locationTerms.push({ any: rule.rooms.map(room => ({ all: Object.entries(room).map(([field, value]) =>
        field === 'slots' ? { field: 'location.slot', oneOf: value as number[] } : { field: 'location.' + field, equals: value as EnemySpawnScalar }) })) });
      (limits[limit] ??= []).push(...entries.map(entry => ({ when: locationTerms.length ? { all: [...locationTerms, entry.when] } as EnemySpawnPredicate : entry.when, maximum: entry.maximum })));
    }
    if (!rule.rooms?.length || rule.rooms.some(room => room.game !== 'oot')) continue;
    if (rule.spawns) {
      if (rule.allow === undefined && rule.deny === undefined && rule.preserve === undefined) continue;
      for (const key of rule.spawns) {
        const entry = scripted.get(key) ?? { name, points: [] };
        for (const { slots, ...room } of rule.rooms) for (const slot of slots ?? [undefined])
          if (!entry.points.some(point => JSON.stringify(point) === JSON.stringify({ ...room, slot }))) entry.points.push({ ...room, slot });
        scripted.set(key, entry);
      }
    } else {
      const seen = new Set<string>();
      for (const { slots, ...room } of rule.rooms) {
        if (room.scene === undefined || room.room === undefined || room.setup === undefined || !slots || room.mq === undefined) continue;
        for (const slot of slots) {
          const key = [room.mq, room.scene, room.room, room.setup, slot].join('/');
          if (seen.has(key)) fail(name + ': duplicate OoT slot ' + key);
          seen.add(key);
          if (!points.has(key)) points.set(key, { name, point: { ...room, slot } });
        }
      }
    }
  }
  /* Multiple declarations of the same point are intersected before hydration.
   * Equal policies retain their original grouping; point-specific overlays split
   * only the affected slots out of that group. */
  const grouped = new Map<string, Map<string, EnemyOotSpawnLocationPolicy>>();
  for (const { name, point } of points.values()) {
    const policy = template(name, point);
    const signature = JSON.stringify(policy);
    const groups = grouped.get(name) ?? new Map<string, EnemyOotSpawnLocationPolicy>();
    const group = groups.get(signature) ?? { rooms: [], ...policy };
    const room = group.rooms.find(room => room.scene === point.scene && room.room === point.room && room.setup === point.setup && room.mq === point.mq);
    if (room) room.slots.push(point.slot!);
    else group.rooms.push({ scene: point.scene!, room: point.room!, setup: point.setup!, slots: [point.slot!], mq: point.mq! });
    groups.set(signature, group);
    grouped.set(name, groups);
  }
  for (const [name, groups] of grouped) {
    let index = 0;
    for (const group of groups.values()) locations[index++ ? name + ' - Policy ' + index : name] = group;
  }
  for (const [key, { name, points }] of scripted) {
    for (const point of points) {
      const next = template(name, point, key);
      const current = templates[key];
      if (!current) templates[key] = next;
      else {
        for (const field of attributeFields) if (JSON.stringify(current[field]) !== JSON.stringify(next[field]))
          fail(name + ': conflicting native attributes for ' + key);
        current.choices = current.choices.filter(name => next.choices.includes(name));
        current.disallowed = [...new Set([...current.disallowed, ...next.disallowed])];
        current.broken ||= next.broken;
      }
    }
  }
  rules.limits = limits;
  return {
    rules,
    targets: parseEnemyOotSpawnTargets(targets),
    locations: parseEnemyOotSpawnLocations(locations, targets),
    templates: parseEnemyOotSpawnTemplates(templates, targets),
    mmActors: parseEnemyMmSpawnActors(mmActors),
    actorTypes: parseEnemySpawnActorTypes(actorTypes),
    runtime,
  };
}
