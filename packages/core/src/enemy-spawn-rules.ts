import { ENEMY_SPAWN_RULES, parseEnemySpawnRules, type Game, type EnemySpawnHabitat, type EnemySpawnPredicate, type EnemySpawnRoom, type EnemySpawnRules, type EnemySpawnRule, type EnemySpawnEncounter } from '@ootmm/data';
import type { Settings } from './settings';

export type EnemySpawnLocation = {
  game: Game;
  scene?: number;
  room?: number;
  setup?: number;
  slot?: number;
  mq?: boolean;
  grotto?: number;
};

export type EnemySpawnSource = { encounterId?: string; actorId: number; params?: number; key?: string; habitat?: EnemySpawnHabitat };
export type EnemySpawnTarget = { game: Game; actorId: number; spawnParams: number; habitat: EnemySpawnHabitat };
export type EnemySpawnContext = {
  source?: object;
  target?: object;
  location?: object;
  settings?: object;
  facts?: object;
};

type Predicate = (context: EnemySpawnContext) => boolean;

const rules = parseEnemySpawnRules(ENEMY_SPAWN_RULES);
const compiled = new WeakMap<EnemySpawnRules, ReturnType<typeof compileRules>>();

function fieldReader(path: string) {
  const keys = path.split('.');
  return (context: EnemySpawnContext): unknown => {
    let value: unknown = context;
    for (const key of keys) {
      if (!value || typeof value !== 'object' || !Object.hasOwn(value, key)) return undefined;
      value = (value as Record<string, unknown>)[key];
    }
    return value;
  };
}

/* Predicates are compiled once. The evaluator knows only field comparisons;
 * actor identities, placement exceptions and limits are authored in YAML. */
function compileRules(data: EnemySpawnRules) {
  const macros = new Map<string, Predicate>();
  const macro = (name: string): Predicate => {
    let result = macros.get(name);
    if (!result) {
      if (!Object.hasOwn(data.macros, name)) throw new Error('Unknown enemy spawn macro: ' + name);
      result = compile(data.macros[name]);
      macros.set(name, result);
    }
    return result;
  };
  const compile = (predicate: EnemySpawnPredicate): Predicate => {
    if (typeof predicate === 'boolean') return () => predicate;
    if (typeof predicate === 'string') return macro(predicate);
    if ('all' in predicate) {
      const children = predicate.all.map(compile);
      return context => children.every(child => child(context));
    }
    if ('any' in predicate) {
      const children = predicate.any.map(compile);
      return context => children.some(child => child(context));
    }
    if ('not' in predicate) {
      const child = compile(predicate.not);
      return context => !child(context);
    }
    const read = fieldReader(predicate.field);
    const value = predicate.mask === undefined ? read : (context: EnemySpawnContext) => {
      const input = read(context);
      return typeof input === 'number' ? (input & predicate.mask!) >>> 0 : undefined;
    };
    if ('equals' in predicate) return context => value(context) === predicate.equals;
    if ('oneOf' in predicate) {
      const allowed = new Set<unknown>(predicate.oneOf);
      return context => allowed.has(value(context));
    }
    if ('contains' in predicate) return context => {
      const input = value(context);
      return Array.isArray(input) && input.includes(predicate.contains);
    };
    if ('containsAny' in predicate) return context => {
      const input = value(context);
      return Array.isArray(input) && predicate.containsAny!.some(item => input.includes(item));
    };
    if ('subsetOf' in predicate) {
      const allowed = new Set<unknown>([...predicate.subsetOf!, ...(predicate.except ?? [])]);
      return context => {
        const input = value(context);
        return Array.isArray(input) && input.every(item => allowed.has(item));
      };
    }
    if ('equalsField' in predicate) {
      const other = fieldReader(predicate.equalsField!);
      return context => value(context) === other(context);
    }
    if ('containsField' in predicate) {
      const other = fieldReader(predicate.containsField!);
      return context => {
        const input = value(context);
        return Array.isArray(input) && input.includes(other(context));
      };
    }
    if ('subsetOfField' in predicate) {
      const other = fieldReader(predicate.subsetOfField!);
      const except = new Set<unknown>(predicate.except);
      return context => {
        const input = value(context), allowed = other(context);
        return Array.isArray(input) && Array.isArray(allowed) && input.every(item => except.has(item) || allowed.includes(item));
      };
    }
    if ('exists' in predicate) return context => (value(context) !== undefined) === predicate.exists;
    if ('nonempty' in predicate) return context => {
      const input = value(context);
      return Array.isArray(input) && (input.length > 0) === predicate.nonempty;
    };
    if ('min' in predicate) return context => {
      const input = value(context);
      return typeof input === 'number' && input >= predicate.min!;
    };
    if ('max' in predicate) return context => {
      const input = value(context);
      return typeof input === 'number' && input <= predicate.max!;
    };
    throw new Error('Invalid enemy spawn predicate');
  };
  for (const name of Object.keys(data.macros)) macro(name);
  const selection = (value: EnemySpawnPredicate | EnemySpawnPredicate[] | undefined, fallback: boolean): Predicate => {
    if (value === undefined) return () => fallback;
    if (!Array.isArray(value)) return compile(value);
    const alternatives = value.map(compile);
    return context => alternatives.some(predicate => predicate(context));
  };
  const compilePolicy = (rule: EnemySpawnRule) => ({
    ...rule,
    when: compile(rule.when ?? true),
    allow: selection(rule.allow, true),
    deny: selection(rule.deny, false),
  });
  const entries = Object.values(data.rules).map(compilePolicy);
  const localEntries = new Map(Object.entries(data.encounters ?? {}).map(([id, row]) => [id, Object.values(row.policies).map(compilePolicy)]));
  const globalEntries: typeof entries = [];
  const byScene = new Map<string, Set<typeof entries[number]>>();
  for (const rule of entries) {
    if (!rule.rooms) {
      globalEntries.push(rule);
      continue;
    }
    for (const room of rule.rooms) {
      const key = room.game + '/' + (room.scene ?? '*');
      const matches = byScene.get(key) ?? new Set();
      matches.add(rule);
      byScene.set(key, matches);
    }
  }
  const pointEntries = new Map<string, typeof entries>();
  const at = (location: EnemySpawnLocation, sourceKey: string | undefined, encounterId?: string) => {
    const key = JSON.stringify([location.game, location.scene, location.room, location.setup, location.slot, !!location.mq, location.grotto, sourceKey, encounterId]);
    let result = pointEntries.get(key);
    if (!result) {
      const candidates = new Set([...globalEntries, ...(byScene.get(location.game + '/*') ?? []),
        ...(byScene.get(location.game + '/' + location.scene) ?? [])]);
      result = [...candidates].filter(rule =>
        (!rule.rooms || rule.rooms.some(room => roomMatches(room, location))) &&
        (!rule.spawns || sourceKey !== undefined && rule.spawns.includes(sourceKey)));
      if (encounterId !== undefined) result.push(...(localEntries.get(encounterId) ?? []));
      pointEntries.set(key, result);
    }
    return result;
  };
  const limits = new Map(Object.entries(data.limits ?? {}).map(([name, entries]) => [name,
    entries.map(entry => ({ when: compile(entry.when), maximum: entry.maximum }))]));
  return { macros, at, limits };
}

function compiledRules(data: EnemySpawnRules) {
  let result = compiled.get(data);
  if (!result) {
    result = compileRules(data);
    compiled.set(data, result);
  }
  return result;
}

export function enemySpawnMacroMatches(name: string, context: EnemySpawnContext, data: EnemySpawnRules = rules): boolean {
  const predicate = compiledRules(data).macros.get(name);
  if (!predicate) throw new Error('Unknown enemy spawn macro: ' + name);
  return predicate(context);
}

export function enemySpawnMacroGroup(group: string, context: EnemySpawnContext, data: EnemySpawnRules = rules): string[] {
  const prefix = group + '/';
  return [...compiledRules(data).macros].flatMap(([name, predicate]) =>
    name.startsWith(prefix) && predicate(context) ? [name.slice(prefix.length)] : []);
}

export function enemySpawnLimit(name: string, context: EnemySpawnContext, fallback = Infinity, data: EnemySpawnRules = rules): number {
  let result = fallback;
  for (const rule of compiledRules(data).limits.get(name) ?? [])
    if (rule.when(context)) result = Math.min(result, rule.maximum);
  return result;
}

function roomMatches(room: EnemySpawnRoom, location: EnemySpawnLocation): boolean {
  return room.game === location.game && (room.scene === undefined || room.scene === location.scene) &&
    (room.room === undefined || room.room === location.room) &&
    (room.setup === undefined || room.setup === location.setup) &&
    (room.slots === undefined || location.slot !== undefined && room.slots.includes(location.slot)) &&
    (room.grotto === undefined || room.grotto === location.grotto) &&
    (room.mq === undefined || room.mq === !!location.mq);
}

export function enemySpawnSourceAllowed(settings: Settings, location: EnemySpawnLocation, source: EnemySpawnSource,
  data: EnemySpawnRules = rules): boolean {
  const context = { source: { game: location.game, ...source }, location, settings };
  return compiledRules(data).at(location, source.key, source.encounterId).every(rule => !rule.when(context) ||
    !rule.preserve && (!rule.preserveUnless || settings[rule.preserveUnless]));
}

export function enemySpawnTargetAllowed(location: EnemySpawnLocation, source: EnemySpawnSource, target: EnemySpawnTarget,
  settings?: Settings, data: EnemySpawnRules = rules): boolean {
  const context = { source: { game: location.game, ...source }, location, settings, target: { ...target, params: target.spawnParams } };
  return compiledRules(data).at(location, source.key, source.encounterId).every(rule => !rule.when(context) ||
    rule.allow(context) && !rule.deny(context));
}

export type SpawnInventoryRow = {
  id: string;
  game: Game;
  actorId: number;
  sourceKey?: string;
  locations: string[];
  sharedController: boolean;
};

export type SpawnRegistryRow = EnemySpawnEncounter;

const registry = rules.encounters ?? {};

export function spawnEncounter(id: string): SpawnRegistryRow {
  if (!registry || !Object.hasOwn(registry, id)) throw new Error('Unclassified enemy spawn: ' + id + '; add the encounter to spawn-encounters.yml.');
  const row = registry[id];
  if (!row || typeof row.minibossEncounter !== 'boolean' || typeof row.excluded !== 'boolean')
    throw new Error('Invalid enemy spawn classification: ' + id);
  return row;
}

export function spawnEncounterAllows(id: string, settings: Settings, game: Game): boolean {
  const row = spawnEncounter(id);
  const only = game === 'oot' ? settings.enemyShuffleOotMinibossOnly : settings.enemyShuffleMmMinibossOnly;
  return !row.excluded && (!only || row.minibossEncounter) &&
    (!settings.enemyShufflePreserveMinibosses || only || !row.minibossEncounter);
}

export function validateSpawnRegistry(inventory: readonly SpawnInventoryRow[]): void {
  const expected = new Set<string>();
  for (const source of inventory) {
    if (expected.has(source.id)) throw new Error('Duplicate inventory spawn: ' + source.id);
    expected.add(source.id);
    const row = spawnEncounter(source.id);
    if (row.game !== source.game || row.actorId !== source.actorId || row.sourceKey !== source.sourceKey ||
      row.sharedController !== source.sharedController || !Array.isArray(row.locations) ||
      JSON.stringify([...row.locations].sort()) !== JSON.stringify(source.locations))
      throw new Error('Stale enemy spawn registry entry: ' + source.id);
  }
  for (const id of Object.keys(registry)) if (!expected.has(id))
    throw new Error('Unknown enemy spawn registry entry: ' + id);
}
