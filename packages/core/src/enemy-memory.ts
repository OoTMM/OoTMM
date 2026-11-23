import { ENEMY_SOUL_PREVIEW_MEMORY, ENEMY_SOUL_PREVIEW_OBJECTS, ENEMY_UTILITY_MODULE_BYTES } from './enemy-soul-preview-memory-data';
import { ENEMY_DYNAMIC_COLLISION } from './enemy-collision-data';
import type { Game } from '@ootmm/data';
import type { Settings } from './settings';
import { ENEMY_MEMORY_FACTS } from './enemy-memory-data';
import { enemy_oot_resources } from './enemy-data';
import { OOT_GAMEPLAY_MEMORY } from './oot-gameplay-memory-data';

export const OOT_GAMEPLAY_POLICY = OOT_GAMEPLAY_MEMORY.policy;

export type EnemyMemoryCost = {
  shared: Readonly<Record<string, number>>;
  instances: number;
  actors: number;
  dynamicCollisions?: number;
  preview?: { key: string; sharedBytes: number; instances: number };
};

type ActorFact = {
  object: number;
  code: number;
  instance: number;
};

const facts = ENEMY_MEMORY_FACTS as {
  actors: Record<string, ActorFact>;
  mmObjectBounds: Record<string, {
    baseBytes: number;
    playerSlots: number;
    horseSlots: number;
  }>;
  scriptedRoots: Record<string, readonly number[]>;
  pinnedMmActors: readonly number[];
  foreign: Record<string, {
    code: number;
    instance: number;
    children?: Record<string, number>;
  }>;
  objects: Record<Game, Record<string, number>>;
  children: Record<string, Record<string, number>>;
  standalone: Record<string, ActorFact>;
  helpers: Record<string, ActorFact>;
  buildBounds: Record<string, {
    memory: number;
    profileOffset: number;
    instance: number;
  }>;
};

export const ENEMY_MEMORY_POLICY = {
  mmRecovered: 0x39000 + 0x25800,
  engineAllowance: 0x20000,
  gameplayReserve: 0x18000,
  fragmentationReserve: 0x10000,
  actorLimit: 243,
  dynamicCollisionLimit: 50,
  dynamicCollisionReserve: 8,
  playerObjectBound: 123472,
  horseObjectBound: 58608,
} as const;

const align = (n: number) => Math.ceil(n / 16) * 16;

export const enemyMemoryBuildBounds = facts.buildBounds;

export const enemySceneObjectBounds = facts.mmObjectBounds;

export const enemyScriptedRootOffsets = facts.scriptedRoots;

export const enemyPinnedMmActors = facts.pinnedMmActors;

export const enemyObjectBytes = (game: Game, id: number) => facts.objects[game][id] ?? 0;

export const enemyNativeActorFact = (game: Game, id: number) => facts.actors[`${game}/${id}`];

export function enemyNativeActorDependencies(game: Game, id: number): Set<number> {
  const ids = new Set<number>();
  const visit = (actor: number) => {
    if (ids.has(actor))
      return;
    ids.add(actor);
    for (const child of Object.keys(facts.children[`${game}/${actor}`] ?? {}))
      visit(+child);
  };
  visit(id);
  return ids;
}

export function enemyNativeDynamicCollisionCount(game: Game, actorId: number, params?: number): number {
  /* Dm_Char01's actor-wide spawn graph merges the four Woodfall scenery
   * variants and both flower-spawning code paths. Each placed variant is
   * narrower: water has one mesh, the cue controller has none, and the
   * temple/ramp each own one mesh and can spawn at most one Deku Flower.
   * Keep the broad fallback for unknown parameters/dependency traversal. */
  if (game === 'mm' && actorId === 0x193 && params !== undefined && [0, 1, 2, 3].includes(params))
    return [1, 0, 2, 2][params];
  const visit = (id: number, seen: readonly number[]): number => {
    if (seen.includes(id)) return 0;
    const own = ENEMY_DYNAMIC_COLLISION.actors[game].includes(id) ? 1 : 0;
    return own + Object.entries(facts.children[`${game}/${id}`] ?? {}).reduce((n, [child, count]) =>
      n + count * visit(+child, [...seen, id]), 0);
  };
  return visit(actorId, []);
}

export function enemyOriginalRoomDynamicCollisions(game: Game, mq: boolean, scene: number, room: number): number {
  return Math.max(0, ...[0, 1, 2, 3].map(setup =>
    (ENEMY_DYNAMIC_COLLISION.rooms as Record<string, number>)[`${game}/${+mq}/${scene}/${room}/${setup}`] ?? 0));
}

const scriptedControllerCost: EnemyMemoryCost = {
  shared: { 'code/mm/helper/Arena': facts.helpers['mm/Arena'].code },
  instances: facts.helpers['mm/Arena'].instance, actors: 1,
};

export function enemyScriptedControllerCost(): EnemyMemoryCost {
  return scriptedControllerCost;
}

export type EnemyMemoryOptions = {
  gerudoFortress?: Settings['gerudoFortress'];
  soulsEnemyLens?: 'none' | 'always' | 'lens';
  enemyShuffleGoldSkulltulas?: boolean;
  enemyShuffleBari?: boolean;
  enemyShuffleEeno?: boolean;
};

const costs = new Map<string, EnemyMemoryCost>();

export function enemyMemoryCost(host: Game, game: Game, actorId: number, params: number, options: EnemyMemoryOptions = {}): EnemyMemoryCost {
  const redIce = game === host && (params === 0x7e04 && (game === 'oot' ? actorId === 0x90 : actorId === 0x4c) ||
    (game === 'oot' ? actorId === 0x11 && params === 0x7e80 : actorId === 0xa && params === 0x7e81));
  const randomSplit = game === 'oot' && actorId === 0x63 && options.enemyShuffleBari !== false ||
    game === 'mm' && actorId === 0x1e6 && (params & 255) === 1 && options.enemyShuffleEeno !== false;
  const previews = options.soulsEnemyLens === 'always' || options.soulsEnemyLens === 'lens';
  const cacheKey = `${host}/${game}/${actorId}/${params}/${+randomSplit}/${+previews}/${+!!options.enemyShuffleGoldSkulltulas}`;
  const cached = costs.get(cacheKey);
  if (cached)
    return cached;
  const shared: Record<string, number> = {};

  shared.enemyUtilityModule = ENEMY_UTILITY_MODULE_BYTES;
  const finish = (cost: EnemyMemoryCost): EnemyMemoryCost => {
    const size = previews ? ENEMY_SOUL_PREVIEW_MEMORY[`${game}/${actorId}`] : 0;
    if (size) {
      const object = ENEMY_SOUL_PREVIEW_OBJECTS[`${game}/${actorId}`];
      if (object) {
        const [source, id] = object;
        const key = `model/${source}/${id}`;
        cost.shared = { ...cost.shared, [key]: Math.max(cost.shared[key] ?? 0, align(enemyObjectBytes(source as Game, id)) + 16) };
      }
    }
    if (size) cost.preview = { key: `${game}/${actorId}`, sharedBytes: size, instances: 96 };
    costs.set(cacheKey, cost);
    return cost;
  };
  if (game === 'mm' && actorId === 0x23d && params === 0x14) {
    const swarm = facts.helpers[`${host}/MothSwarm`];
    shared['code/mm/573'] = swarm.code;
    if (host !== game) shared['foreign/mm/573'] = 0;
    return finish({ shared, instances: swarm.instance, actors: 1, dynamicCollisions: 0 });
  }
  if (game === host && actorId === (game === 'oot' ? 0x91 : 0x1e8) && params === 0x4000) {
    const meg = enemyMemoryCost(host, game, actorId, 0, options);
    const controller = facts.helpers[`${host}/PoeEncounter`];
    return finish({ ...meg,
      shared: { ...meg.shared, [`code/${host}/helper/PoeEncounter`]: controller.code },
      instances: meg.instances + controller.instance, actors: meg.actors + 1,
    });
  }
  let instances = 0, actors = 0, dynamicCollisions = 0;
  const add = (id: number, count: number, ancestors: readonly number[]) => {
    const key = `${game}/${id}`, row = facts.actors[key];
    if (!row)
      throw Error(`Missing enemy memory profile: ${key}`);
    const foreign = game !== host;
    if (foreign)
      shared[`foreign/${key}`] = 0;
    const port = foreign ? facts.foreign[key] : undefined;
    shared[`code/${key}`] = Math.max(row.code, port?.code ?? 0);
    if (row.object > 3)
      shared[`model/${game}/${row.object}`] = align(enemyObjectBytes(game, row.object)) + 16;
    instances += count * (Math.max(row.instance, align(port?.instance ?? 0) + 16) + (foreign ? 128 : 0));
    actors += count;
    if (ENEMY_DYNAMIC_COLLISION.actors[game].includes(id)) dynamicCollisions += count;
    for (const [child, n] of Object.entries(port?.children ?? facts.children[key] ?? {}))
      if (!ancestors.includes(+child) && +child !== id && !(redIce && game === 'mm' && id === actorId && +child === 0x8e))
        add(+child, count * n, [...ancestors, id]);
  };
  if (game === 'oot' && actorId === 0x73 || game === 'mm' && actorId === 0x56) {
    return finish({ shared: { ...shared, arwingModel: 0x10010, arwingCode: 0x4010 }, instances: 0x3000, actors: 3 });
  }
  const standaloneNames: Record<number, string> = { 0x182: 'GaroMaster', 0x43: 'Gomess', 0x12c: 'Wart', 0x7: 'Snapper', 0x65: 'Slime', 0x115: 'Igos', 0x28f: 'Keeta', 0x15d: 'Wiz' };
  const standaloneName = game === 'mm' && ((params & 0x80ff) === 0x807f) ? standaloneNames[actorId] : undefined;
  if (standaloneName) {
    const native = facts.standalone[`mm/${standaloneName}`];
    const port = host === 'mm' ? native : facts.foreign[`mm/${actorId}`];
    if (!port || !native)
      throw Error(`Missing standalone memory profile: ${standaloneName}`);
    shared[`code/${host}/standalone/${standaloneName}`] = port.code;
    if (game !== host)
      shared[`foreign/${game}/${actorId}`] = 0;
    if (native.object > 3)
      shared[`model/mm/${native.object}`] = align(enemyObjectBytes('mm', native.object)) + 16;
    instances = port.instance;
    actors = 1;
    const child = standaloneName === 'Gomess' ? ['GomessBat', 10] as const :
      standaloneName === 'Slime' ? ['Minislime', 15] as const : standaloneName === 'Snapper' ? ['SnapperMount', 1] as const :
        standaloneName === 'Wiz' ? ['WizReflectedBolt', 4] as const : undefined;
    if (child) {
      const helper = facts.helpers[`${host}/${child[0]}`];
      if (!helper)
        throw Error(`Missing helper memory profile: ${child[0]}`);
      shared[`code/${host}/helper/${child[0]}`] = helper.code;
      instances += child[1] * helper.instance;
      actors += child[1];
    }
    if (standaloneName === 'Igos') {
      const members = params === 0x907f ? 2 : 1;
      instances += members * port.instance;
      actors += members;
    }
    instances += 0x3000;
    actors++;
    return finish({ shared, instances, actors, dynamicCollisions });
  }
  let count = 1;
  if (game === 'mm' && actorId === 0x23f && params === 0)
    count = 11;
  if ((game === 'oot' && actorId === 0x91 || game === 'mm' && actorId === 0x1e8) && ((params >>> 8) & 3) === 0)
    count += 3;
  if (game === 'oot' && [0x35, 0xde].includes(actorId))
    count += 7;
  if (game === 'mm' && actorId === 0x180)
    count += 5;
  if (game === 'mm' && actorId === 0x14b)
    count += Math.min(20, (params >>> 8) & 255);
  if (game === 'mm' && actorId === 0x15b && !(params & 0x80))
    count += Math.max(0, (params & 31) === 31 ? 0 : (params & 31) - 1);
  if (game === 'mm' && actorId === 0x1e6 && (params & 255) === 1)
    count += 3;
  if (game === 'mm' && actorId === 0x115)
    count += params === 0x807f ? 8 : 2;
  add(actorId, count, []);
  if (game === 'oot' && actorId === 0x0d) {
    const type = params & 0xffff;
    const object = type === 2 || type === 3 ? 0x6e : 0x09;
    shared[`model/oot/${object}`] = align(enemyObjectBytes('oot', object)) + 16;
  }
  const legacy = (enemy_oot_resources.actors as Record<string, {
    instanceBytes: number;
    supportInstanceBytes?: number;
    nativeSupportInstanceBytes?: number;
    code: Record<string, number>;
    models: Record<string, number>;
  }>)[`${game}/${actorId}`];
  if (legacy) {
    instances = Math.max(instances, count * legacy.instanceBytes + (legacy.supportInstanceBytes ?? 0) + (legacy.nativeSupportInstanceBytes ?? 0));
    for (const [key, size] of Object.entries(legacy.code))
      shared[`code/${key}`] = Math.max(shared[`code/${key}`] ?? 0, size);
    for (const [key, size] of Object.entries(legacy.models))
      if (+key.split('/')[1] > 3)
        shared[`model/${key}`] = Math.max(shared[`model/${key}`] ?? 0, size);
  }
  if (redIce) {
    const ice = facts.actors[`${host}/${host === 'oot' ? 0xef : 0x45}`];
    shared[`code/${host}/${host === 'oot' ? 0xef : 0x45}`] = ice.code;
    shared['model/oot/107'] = align(enemyObjectBytes('oot', 0x6b)) + 16;
    instances += ice.instance;
    actors++;
  }
  if (!redIce && game === host && ((game === 'oot' && actorId === 0x90) || (game === 'mm' && actorId === 0x4c)) && (params & 255) === 4)
    add(game === 'oot' ? 0x11e : 0x8e, 1, [actorId]);
  if (game === host && (game === 'oot' ? actorId === 0x11 && params === 0x7f80 : actorId === 0xa && params === 0x7f81)) {
    const ice = game === 'oot' ? 0x11e : 0x8e;
    if (!(facts.children[`${game}/${actorId}`]?.[ice]))
      add(ice, 1, [actorId]);
  }
  if (game === 'oot' && actorId === 0x1d && params === 0) {
    const row = facts.actors['oot/29'], port = host === 'mm' ? facts.foreign['oot/29'] : undefined;
    instances += 3 * (Math.max(row.instance, align(port?.instance ?? 0) + 16) + (host === 'mm' ? 128 : 0));
    actors += 3;
  }
  if (game === 'oot' && (actorId === 0x69 || actorId === 0x1b0)) {
    const pieces = actorId === 0x69 ? 12 : 18;
    add(7, pieces, [actorId]);
    instances += align((pieces + 1) * 70) + 16;
  }
  if (host === 'oot' && game === 'oot' && actorId === 0x90 && [5, 6, 7].includes(params & 255))
    shared.redeadDances = 0xd90;
  if (randomSplit) {
    const children: [
      Game,
      number,
      number
    ][] = [['oot', 0x13, 0], ['oot', 0x34, 0], ['oot', 0x12, 0], ['oot', 0x1b, 0], ['oot', 0x1af, 0xff00],
    ['mm', 0x0c, 0], ['mm', 0x109, 0], ['mm', 0x0b, 0], ['mm', 0x12, 0xffff], ['mm', 0xec, 0]];
    instances += 3 * Math.max(...children.map(([g, id, p]) => enemyMemoryTotal([enemyMemoryCost(host, g, id, p)]).bytes));
    actors += 3;
  }
  return finish({ shared, instances, actors, dynamicCollisions });
}

const sharedEntryCache = new WeakMap<object, readonly (readonly [string, number, boolean])[]>();

function memorySharedEntries(shared: EnemyMemoryCost['shared']) {
  let entries = sharedEntryCache.get(shared);
  if (!entries) {
    entries = Object.entries(shared).map(([key, size]) => [key, size, key.startsWith('code/') || key === 'arwingCode'] as const);
    sharedEntryCache.set(shared, entries);
  }
  return entries;
}

export function enemyMemoryTotal(costs: readonly EnemyMemoryCost[], prepaid: ReadonlySet<string> = new Set()) {
  const shared = new Map<string, number>();
  let bytes = 0, actors = 0, dynamicCollisions = 0;
  const hasPreviews = costs.some(cost => !!cost.preview);
  const owners = new Map<string, string | null>();
  const previews = new Map<string, { size: number; count: number; sum: number; positive: number; positiveCount: number; min: number; max: number }>();
  for (const cost of costs) {
    bytes += cost.instances;
    actors += cost.actors;
    dynamicCollisions += cost.dynamicCollisions ?? 0;
    const preview = cost.preview;
    if (preview) {
      const group = previews.get(preview.key) ?? { size: 0, count: 0, sum: 0, positive: 0, positiveCount: 0, min: Infinity, max: -Infinity };
      const delta = preview.instances - cost.instances;
      group.size = Math.max(group.size, preview.sharedBytes);
      group.count++;
      group.sum += delta;
      if (delta > 0) {
        group.positive += delta;
        group.positiveCount++;
      }
      group.min = Math.min(group.min, delta);
      group.max = Math.max(group.max, delta);
      previews.set(preview.key, group);
    }
    for (const [key, size, code] of memorySharedEntries(cost.shared)) {
      if (!prepaid.has(key)) shared.set(key, Math.max(shared.get(key) ?? 0, size));
      if (hasPreviews && code) {
        const owner = preview?.key ?? null;
        if (!owners.has(key)) owners.set(key, owner);
        else if (owners.get(key) !== owner) owners.set(key, null);
      }
    }
  }
  let largest = 0;
  for (const size of shared.values()) {
    bytes += size;
    largest = Math.max(largest, size);
  }
  const credits = new Map<string, number>();
  for (const [key, owner] of owners) if (owner !== null)
    credits.set(owner, (credits.get(owner) ?? 0) + (shared.get(key) ?? 0));
  for (const [key, group] of previews) {
    const allMissing = group.size + group.sum - (credits.get(key) ?? 0);
    let mixed = -Infinity;
    if (group.count > 1) {
      const bestSubset = group.positiveCount === group.count ? group.positive - group.min :
        group.positiveCount ? group.positive : group.max;
      mixed = group.size + bestSubset;
    }
    bytes += Math.max(0, allMissing, mixed);
    largest = Math.max(largest, group.size);
  }
  return { bytes, actors, largest, shared, dynamicCollisions };
}

export function makeEnemyMemoryEvaluator(prepaid: ReadonlySet<string> = new Set()) {
  type Compiled = { keys: number[]; sizes: number[]; preview: number };
  const resources = new Map<string, number>(), previewIds = new Map<string, number>();
  const compiled = new WeakMap<EnemyMemoryCost, Compiled>();
  const codes: boolean[] = [], kinds: number[] = [];
  const seen: number[] = [], sizes: number[] = [], owners: number[] = [], touched: number[] = [];
  const previewSeen: number[] = [], previewTouched: number[] = [], previewSizes: number[] = [], counts: number[] = [];
  const sums: number[] = [], positives: number[] = [], positiveCounts: number[] = [], minima: number[] = [], maxima: number[] = [], credits: number[] = [];
  let epoch = 0;
  const compile = (cost: EnemyMemoryCost): Compiled => {
    const old = compiled.get(cost);
    if (old) return old;
    const keys: number[] = [], sizes: number[] = [];
    for (const [key, size, code] of memorySharedEntries(cost.shared)) {
      if (prepaid.has(key)) continue;
      let id = resources.get(key);
      if (id === undefined) {
        id = resources.size;
        resources.set(key, id);
        codes[id] = code;
        kinds[id] = key.startsWith('foreign/') ? 1 : key.startsWith('model/') ? 2 : 0;
      }
      keys.push(id);
      sizes.push(size);
    }
    let preview = -1;
    if (cost.preview) {
      const key = cost.preview.key;
      preview = previewIds.get(key) ?? previewIds.size;
      previewIds.set(key, preview);
    }
    const result = { keys, sizes, preview };
    compiled.set(cost, result);
    return result;
  };
  return (costs: readonly EnemyMemoryCost[]) => {
    if (++epoch === Number.MAX_SAFE_INTEGER) {
      seen.length = 0;
      previewSeen.length = 0;
      epoch = 1;
    }
    let bytes = 0, actors = 0, dynamicCollisions = 0, largest = 0, foreign = 0, models = 0;
    let resourceCount = 0, previewCount = 0;
    for (const cost of costs) {
      const row = compile(cost), owner = row.preview;
      bytes += cost.instances;
      actors += cost.actors;
      dynamicCollisions += cost.dynamicCollisions ?? 0;
      if (owner >= 0) {
        const preview = cost.preview!, delta = preview.instances - cost.instances;
        if (previewSeen[owner] !== epoch) {
          previewSeen[owner] = epoch;
          previewTouched[previewCount++] = owner;
          previewSizes[owner] = 0;
          counts[owner] = 0;
          sums[owner] = 0;
          positives[owner] = 0;
          positiveCounts[owner] = 0;
          minima[owner] = Infinity;
          maxima[owner] = -Infinity;
          credits[owner] = 0;
        }
        previewSizes[owner] = Math.max(previewSizes[owner], preview.sharedBytes);
        counts[owner]++;
        sums[owner] += delta;
        if (delta > 0) {
          positives[owner] += delta;
          positiveCounts[owner]++;
        }
        minima[owner] = Math.min(minima[owner], delta);
        maxima[owner] = Math.max(maxima[owner], delta);
      }
      for (let i = 0; i < row.keys.length; ++i) {
        const id = row.keys[i], size = row.sizes[i];
        if (seen[id] !== epoch) {
          seen[id] = epoch;
          touched[resourceCount++] = id;
          sizes[id] = Math.max(0, size);
          owners[id] = owner;
          if (kinds[id] === 1) ++foreign;
          else if (kinds[id] === 2) ++models;
        } else {
          sizes[id] = Math.max(sizes[id], size);
          if (owners[id] !== owner) owners[id] = -1;
        }
      }
    }
    for (let i = 0; i < resourceCount; ++i) {
      const id = touched[i], size = sizes[id], owner = owners[id];
      bytes += size;
      largest = Math.max(largest, size);
      if (codes[id] && owner >= 0) credits[owner] += size;
    }
    for (let i = 0; i < previewCount; ++i) {
      const id = previewTouched[i], size = previewSizes[id];
      const allMissing = size + sums[id] - credits[id];
      let mixed = -Infinity;
      if (counts[id] > 1) {
        const bestSubset = positiveCounts[id] === counts[id] ? positives[id] - minima[id] :
          positiveCounts[id] ? positives[id] : maxima[id];
        mixed = size + bestSubset;
      }
      bytes += Math.max(0, allMissing, mixed);
      largest = Math.max(largest, size);
    }
    return { bytes, actors, largest, dynamicCollisions, foreign, models };
  };
}

export class EnemyMemoryAllocationError extends Error {
}

type WeightedMemoryChoice<T> = {
  value: T;
  weight: number;
  cost: number;
};

export function solveWeightedMemory<T>(pools: readonly (readonly WeightedMemoryChoice<T>[])[], next: (max: number) => number, fits: (selected: readonly (T | undefined)[]) => boolean): T[] {
  if (!pools.length) {
    if (!fits([]))
      throw new EnemyMemoryAllocationError('No feasible enemy allocation: fixed scene costs exceed the budget');
    return [];
  }
  if (pools.some(pool => !pool.length || pool.some(c => !(c.weight > 0) || !Number.isFinite(c.weight))))
    throw Error('Invalid weighted memory pool');
  const weighted = (pool: readonly WeightedMemoryChoice<T>[]) => pool.map(c => ({
    c,
    priority: -Math.log((next(0x1000000) + 1) / 0x1000001) / c.weight,
  })).sort((a, b) => a.priority - b.priority).map(x => x.c);
  const draw = (pool: readonly WeightedMemoryChoice<T>[]) => {
    let point = next(0x1000000) / 0x1000000 * pool.reduce((sum, c) => sum + c.weight, 0);
    for (const c of pool) {
      point -= c.weight;
      if (point < 0)
        return c.value;
    }
    return pool[pool.length - 1].value;
  };
  const shuffle = (ids: number[]) => {
    for (let i = ids.length - 1; i > 0; --i) {
      const j = next(i + 1);
      [ids[i], ids[j]] = [ids[j], ids[i]];
    }
    return ids;
  };
  let result: (T | undefined)[] = pools.map(draw);
  if (fits(result))
    return result as T[];
  let solved = false, anchor = -1;
  for (let attempt = 0; attempt < 8 && !solved; attempt++) {
    const order = shuffle(pools.map((_, i) => i));
    const preferences = pools.map(weighted);
    result = Array(pools.length).fill(undefined);
    let nodes = 0;
    const search = (depth: number): boolean => {
      if (depth === order.length)
        return true;
      if (++nodes > 4096)
        return false;
      const i = order[depth];
      const candidates = depth === 0 ? preferences[i] : [...preferences[i]].sort((a, b) => a.cost - b.cost);
      for (const c of candidates) {
        result[i] = c.value;
        if (fits(result) && search(depth + 1))
          return true;
        if (nodes > 4096)
          break;
      }
      result[i] = undefined;
      return false;
    };
    solved = search(0);
    if (solved)
      anchor = order[0];
  }
  if (!solved)
    throw new EnemyMemoryAllocationError('No feasible enemy allocation found within the bounded scene search');
  for (let pass = 0; pass < 16; pass++)
    for (const i of shuffle(pools.map((_, i) => i))) {
      if (i === anchor)
        continue;
      const previous = result[i];
      result[i] = draw(pools[i]);
      if (!fits(result))
        result[i] = previous;
    }
  for (let pass = 0; pass < 8; pass++) {
    const proposal = result.slice();
    const count = pass === 7 ? pools.length : Math.min(8, pools.length);
    for (const i of shuffle(pools.map((_, i) => i)).filter(i => i !== anchor).slice(0, count))
      proposal[i] = draw(pools[i]);
    if (fits(proposal))
      result = proposal;
  }
  if (!fits(result))
    throw Error('Enemy memory solver lost its allocation invariant');
  return result as T[];
}

export type AdaptiveMemoryChoice<T> = {
  value: T;
  key: string;
  family: string;
  variety?: string;
  weight: number;
  maxGroupSize?: number;
};

export type AdaptiveMemoryAssessment = {
  excess: number;
  slots: readonly number[];
  roomCount?: number;
  individual?: boolean;
};

export type AdaptiveMemoryLayout<T = unknown> = {
  onProgress?: (message: string) => void;
  positions: readonly (readonly {
    x: number;
    y: number;
    z: number;
  }[])[];
  canShare?: (first: number, second: number) => boolean;
  canGroup?: (first: number, second: number) => boolean;
  distinctFamilies?: (first: number, second: number) => boolean;
  jointSlots?: readonly number[];
  varietyScopes?: readonly (readonly number[])[];
  assessScopes?: (selected: readonly T[]) => readonly AdaptiveMemoryAssessment[];
  fitsScopes?: (selected: readonly (T | undefined)[], affected: ReadonlySet<number>) => boolean;
  prepareFitsScopes?: (affected: ReadonlySet<number>) => (selected: readonly (T | undefined)[]) => boolean;
};

function chooseDistinctEnemyFamilies<T extends {
  family: string;
  weight: number;
}>(pools: readonly (readonly T[])[], next: (max: number) => number): T[] | undefined {
  const families = pools.map(pool => {
    const map = new Map<string, T[]>();
    for (const choice of pool) {
      if (!(choice.weight > 0) || !Number.isFinite(choice.weight))
        throw Error('Invalid enemy family weight');
      const forms = map.get(choice.family) ?? [];
      if (forms.length && forms[0].weight !== choice.weight)
        throw Error('Conflicting enemy family weights');
      forms.push(choice);
      map.set(choice.family, forms);
    }
    return map;
  });
  const used = new Set<string>();
  const canMatch = (indices: readonly number[]): boolean => {
    const owner = new Map<string, number>();
    const augment = (i: number, seen: Set<string>): boolean => {
      for (const family of families[i].keys()) {
        if (used.has(family) || seen.has(family))
          continue;
        seen.add(family);
        const previous = owner.get(family);
        if (previous === undefined || augment(previous, seen)) {
          owner.set(family, i);
          return true;
        }
      }
      return false;
    };
    return indices.every(i => augment(i, new Set()));
  };
  const pending = pools.map((_, i) => i), selected: T[] = Array(pools.length);
  for (let i = pending.length - 1; i > 0; --i) {
    const j = next(i + 1);
    [pending[i], pending[j]] = [pending[j], pending[i]];
  }
  if (!canMatch(pending))
    return undefined;
  while (pending.length) {
    pending.sort((a, b) => [...families[a].keys()].filter(f => !used.has(f)).length -
      [...families[b].keys()].filter(f => !used.has(f)).length);
    const i = pending.shift()!;
    const candidates = [...families[i]].filter(([family]) => !used.has(family)).map(([family, forms]) => ({ family, forms, priority: -Math.log((next(0x1000000) + 1) / 0x1000001) / forms[0].weight }))
      .sort((a, b) => a.priority - b.priority);
    let assigned = false;
    for (const candidate of candidates) {
      used.add(candidate.family);
      if (canMatch(pending)) {
        selected[i] = candidate.forms[next(candidate.forms.length)];
        assigned = true;
        break;
      }
      used.delete(candidate.family);
    }
    if (!assigned)
      throw Error('Enemy family matching lost a feasible assignment');
  }
  return selected;
}

export function solveAdaptiveMemory<T>(pools: readonly (readonly AdaptiveMemoryChoice<T>[])[], next: (max: number) => number, fits: (selected: readonly (T | undefined)[]) => boolean, assess: (selected: readonly T[]) => AdaptiveMemoryAssessment, layout?: AdaptiveMemoryLayout<T>): {
  selected: T[];
  groups: number[][];
  protectedSlots: number[];
} {
  type Choice = Omit<AdaptiveMemoryChoice<T>, 'value'> & {
    values: [
      number,
      T
    ][];
  };
  type Group = {
    slots: number[];
    choices: Choice[];
    selected: Choice;
  };
  if (pools.some(pool => !pool.length || pool.some(c => !(c.weight > 0) || !Number.isFinite(c.weight))))
    throw Error('Invalid adaptive enemy pool');
  const shuffle = <U>(values: readonly U[]): U[] => {
    const copy = [...values];
    for (let i = copy.length - 1; i > 0; --i) {
      const j = next(i + 1);
      [copy[i], copy[j]] = [copy[j], copy[i]];
    }
    return copy;
  };
  const draw = (choices: readonly Choice[]): Choice => {
    const forms = new Map<string, number>();
    for (const c of choices)
      forms.set(c.family, (forms.get(c.family) ?? 0) + 1);
    const weights = choices.map(c => c.weight / forms.get(c.family)!);
    let point = next(0x1000000) / 0x1000000 * weights.reduce((a, b) => a + b, 0);
    for (let i = 0; i < choices.length - 1; i++) {
      point -= weights[i];
      if (point < 0)
        return choices[i];
    }
    return choices[choices.length - 1];
  };
  const singletons = pools.map((pool, index) => {
    const forms = new Map<string, Choice>();
    for (const c of pool) {
      const previous = forms.get(c.key);
      if (previous && (previous.family !== c.family || previous.weight !== c.weight))
        throw Error('Conflicting adaptive enemy form');
      if (!previous)
        forms.set(c.key, { ...c, values: [[index, c.value]] });
    }
    return [...forms.values()];
  });
  const select = (groups: readonly Group[]): T[] => {
    const selected: T[] = Array(pools.length);
    for (const group of groups)
      for (const [i, value] of group.selected.values)
        selected[i] = value;
    return selected;
  };
  const finish = (groups: readonly Group[], selected: T[]) => {
    if (!fits(selected))
      throw Error('Spatial grouping lost its memory invariant');
    return { selected, groups: groups.map(g => [...g.slots]).sort((a, b) => a[0] - b[0]), protectedSlots: groups.filter(g => g.choices.length === 1 && g.slots.length === 1).flatMap(g => g.slots) };
  };
  const initial = singletons.map((choices, i) => ({ slots: [i], choices, selected: draw(choices) }));
  const draft = select(initial);
  if (fits(draft))
    return finish(initial, draft);
  if (layout?.jointSlots?.length) {
    const failing = new Set((layout.assessScopes?.(draft) ?? [assess(draft)]).flatMap(scope => scope.slots));
    const joint = layout.jointSlots.filter(i => failing.has(i));
    if (joint.length)
      try {
        const weighted = joint.map(i => {
          const counts = new Map<string, number>();
          for (const c of singletons[i])
            counts.set(c.family, (counts.get(c.family) ?? 0) + 1);
          return singletons[i].map(c => ({ value: c, weight: c.weight / counts.get(c.family)!, cost: 0 }));
        });
        const choices = solveWeightedMemory(weighted, next, choices => {
          const values: (T | undefined)[] = [...draft];
          joint.forEach((i, j) => {
            values[i] = choices[j]?.values[0][1];
          });
          return fits(values);
        });
        joint.forEach((i, j) => {
          initial[i].selected = choices[j];
          draft[i] = choices[j].values[0][1];
        });
        return finish(initial, draft);
      }
      catch (error) {
        if (!(error instanceof EnemyMemoryAllocationError))
          throw error;
      }
  }
  if (!fits(Array(pools.length).fill(undefined)))
    throw new EnemyMemoryAllocationError('No feasible enemy allocation: fixed scene costs exceed the budget');
  if (!!layout?.assessScopes !== !!layout?.fitsScopes)
    throw Error('Incomplete room-scoped enemy memory checks');
  const affected = new Set<number>();
  const failures = (selected: readonly T[]) => {
    const scopes = layout?.assessScopes?.(selected) ?? [{ ...assess(selected), slots: pools.map((_, i) => i) }];
    return scopes.filter(s => s.excess > 0).slice().sort((a, b) => (a.roomCount ?? 1) - (b.roomCount ?? 1) || b.excess - a.excess);
  };
  let preparedFits: ((selected: readonly (T | undefined)[]) => boolean) | undefined;
  const fitsAffected = (selected: readonly (T | undefined)[]) => preparedFits?.(selected) ?? layout?.fitsScopes?.(selected, affected) ?? fits(selected);
  const unassigned = (): (T | undefined)[] => draft.map((value, i) => affected.has(i) ? undefined : value);
  const mergeCache = new Map<string, Choice[]>();
  const merge = (a: Group, b: Group): Choice[] => {
    if (layout?.canGroup && !a.slots.every(i => b.slots.every(j => layout.canGroup!(i, j))))
      return [];
    if (layout?.canShare && !a.slots.every(i => b.slots.every(j => layout.canShare!(i, j))))
      return [];
    const key = [...a.slots, ...b.slots].sort((x, y) => x - y).join(',');
    const cached = mergeCache.get(key);
    if (cached)
      return cached;
    const other = new Map(b.choices.map(c => [c.key, c]));
    const choices: Choice[] = [];
    let ratio: number | undefined;
    for (const first of a.choices) {
      const second = other.get(first.key);
      if (!second)
        continue;
      const maxGroupSize = Math.min(first.maxGroupSize ?? Infinity, second.maxGroupSize ?? Infinity);
      if (a.slots.length + b.slots.length > maxGroupSize)
        continue;
      const relative = first.weight / second.weight;
      if (first.family !== second.family || ratio !== undefined && Math.abs(relative / ratio - 1) > 1e-9) {
        mergeCache.set(key, []);
        return [];
      }
      ratio = relative;
      choices.push({ ...first, maxGroupSize, values: [...first.values, ...second.values] });
    }
    const partial: (T | undefined)[] = Array(pools.length).fill(undefined);
    if (!choices.some(choice => {
      for (const [i, value] of choice.values)
        partial[i] = value;
      return fits(partial);
    }))
      choices.length = 0;
    mergeCache.set(key, choices);
    return choices;
  };
  const separation = (a: readonly number[], b: readonly number[]): number => {
    if (!layout)
      return 0;
    let distance = Infinity;
    for (const i of a)
      for (const j of b)
        for (const first of layout.positions[i] ?? [])
          for (const second of layout.positions[j] ?? [])
            distance = Math.min(distance, (first.x - second.x) ** 2 + (first.y - second.y) ** 2 + (first.z - second.z) ** 2);
    return distance === Infinity ? 0 : distance;
  };
  const familyCache = new WeakMap<readonly Choice[], ReadonlySet<string>>();
  const choiceFamilies = (choices: readonly Choice[]) => {
    let families = familyCache.get(choices);
    if (!families) {
      families = new Set(choices.map(c => c.family));
      familyCache.set(choices, families);
    }
    return families;
  };
  const similarityCache = new WeakMap<Group, WeakMap<Group, number>>();
  const similarity = (a: Group, b: Group) => {
    let peers = similarityCache.get(a);
    if (!peers) {
      peers = new WeakMap();
      similarityCache.set(a, peers);
    }
    const cached = peers.get(b);
    if (cached !== undefined) return cached;
    const common = choiceFamilies(merge(a, b)).size;
    const first = choiceFamilies(a.choices), second = choiceFamilies(b.choices);
    let all = first.size;
    for (const family of second) if (!first.has(family)) ++all;
    const value = common / Math.max(1, all);
    peers.set(b, value);
    return value;
  };
  const comparePeers = (first: Group, a: Group, b: Group) => similarity(first, b) - similarity(first, a) ||
    separation(first.slots, b.slots) - separation(first.slots, a.slots);
  const joined = (a: Group, b: Group, choices: Choice[]): Group => ({
    slots: [...a.slots, ...b.slots].sort((x, y) => x - y), choices, selected: choices[0],
  });
  const pairAll = (members: readonly Group[], anchor: Group | undefined): Group[] => {
    const pending = shuffle(members.filter(g => g !== anchor)).map(g => ({ ...g })), groups: Group[] = anchor ? [{ ...anchor, choices: [anchor.selected] }] : [];
    while (pending.length) {
      const first = pending.pop()!;
      const peers = shuffle(pending).sort((a, b) => comparePeers(first, a, b));
      let group = first;
      for (const peer of peers) {
        const choices = merge(first, peer);
        if (!choices.length)
          continue;
        group = joined(first, peer, choices);
        pending.splice(pending.indexOf(peer), 1);
        break;
      }
      groups.push(group);
    }
    for (let pass = 0; pass < 4; pass++) {
      let changed = false;
      for (let i = 0; i < groups.length; i++)
        for (let j = i + 1; j < groups.length; j++) {
          const a = groups[i], b = groups[j];
          if (a.slots.length !== 2 || b.slots.length !== 2)
            continue;
          const before = [separation([a.slots[0]], [a.slots[1]]), separation([b.slots[0]], [b.slots[1]])].sort((x, y) => x - y);
          for (const order of [[0, 1], [1, 0]]) {
            const x = initial[a.slots[0]], y = initial[b.slots[order[0]]], u = initial[a.slots[1]], v = initial[b.slots[order[1]]];
            const after = [separation(x.slots, y.slots), separation(u.slots, v.slots)].sort((x, y) => x - y);
            if (after[0] < before[0] || after[0] === before[0] && after[1] <= before[1])
              continue;
            const first = merge(x, y), second = merge(u, v);
            if (!first.length || !second.length)
              continue;
            const oldSimilarity = similarity(initial[a.slots[0]], initial[a.slots[1]]) + similarity(initial[b.slots[0]], initial[b.slots[1]]);
            if (similarity(x, y) + similarity(u, v) < oldSimilarity - 1e-9)
              continue;
            groups[i] = joined(x, y, first);
            groups[j] = joined(u, v, second);
            changed = true;
            break;
          }
        }
      if (!changed)
        break;
    }
    return groups;
  };
  const rollGroups = (groups: Group[], attempts: number): T[] | undefined => {
    for (let attempt = 0; attempt < Math.min(attempts, 2); attempt++) {
      const conflicting = groups.map(a => groups.map(b => a.slots.some(i => b.slots.some(j =>
        (layout?.distinctFamilies ?? layout?.canShare)?.(i, j) ?? true))));
      const families = groups.map(g => [...new Set(g.choices.map(c => c.family))]);
      const familyIds = new Map<string, number>();
      for (const list of families) for (const family of list)
        if (!familyIds.has(family)) familyIds.set(family, familyIds.size);
      const familySets = families.map(list => {
        const present = new Uint8Array(familyIds.size);
        for (const family of list) present[familyIds.get(family)!] = 1;
        return present;
      });
      const blocked = groups.map(() => new Uint32Array(familyIds.size));
      const available = families.map(list => list.length);
      const peers = conflicting.map(row => row.flatMap((conflict, j) => conflict ? [j] : []));
      const markFamily = (index: number, family: number, delta: number) => {
        for (const j of peers[index]) {
          if (!familySets[j][family]) continue;
          const before = blocked[j][family];
          blocked[j][family] += delta;
          if (!before) --available[j];
          else if (!blocked[j][family]) ++available[j];
        }
      };
      const allCoexist = groups.every((_, i) => groups.slice(i + 1).every((_, j) => conflicting[i][i + j + 1]));
      if (allCoexist && !chooseDistinctEnemyFamilies(groups.map(g => g.choices), next))
        return undefined;
      const order = shuffle(groups.map((_, i) => i)).sort((a, b) => families[a].length - families[b].length);
      const preferences = groups.map(g => {
        const families = new Map<string, Choice[]>();
        for (const c of g.choices) {
          const forms = families.get(c.family) ?? [];
          forms.push(c);
          families.set(c.family, forms);
        }
        return [...families.values()].map(forms => ({ forms: shuffle(forms), priority: -Math.log((next(0x1000000) + 1) / 0x1000001) / forms[0].weight }))
          .sort((a, b) => a.priority - b.priority).flatMap(c => c.forms)
          .map(candidate => ({ candidate, family: familyIds.get(candidate.family)! }));
      });
      const values = unassigned();
      let nodes = 0;
      const search = (depth: number): boolean => {
        if (depth === order.length)
          return true;
        if (nodes > 4096)
          return false;
        const index = order[depth], group = groups[index];
        for (const { candidate, family } of preferences[index]) {
          if (++nodes > 4096)
            break;
          if (blocked[index][family])
            continue;
          for (const [i, value] of candidate.values)
            values[i] = value;
          markFamily(index, family, 1);
          let canComplete = true;
          for (let j = depth + 1; j < order.length; ++j)
            if (!available[order[j]]) {
              canComplete = false;
              break;
            }
          if (canComplete && fitsAffected(values) && search(depth + 1)) {
            group.selected = candidate;
            return true;
          }
          markFamily(index, family, -1);
          if (nodes > 4096)
            break;
        }
        for (const i of group.slots)
          values[i] = undefined;
        return false;
      };
      if (search(0))
        return values as T[];
    }
    return undefined;
  };
  const diversify = (groups: Group[], selected: T[]): {
    groups: Group[];
    selected: T[];
  } => {
    if (!layout?.varietyScopes || !groups.some(g => g.slots.length > 2))
      return { groups, selected };
    const scopes = layout.varietyScopes.filter(scope => scope.length > 2 && scope.some(i => affected.has(i)));
    if (!scopes.length)
      return { groups, selected };
    const picks = singletons.map((pool, i) => pool.find(c => c.values[0][1] === selected[i])!);
    const groupBySlot = new Map(groups.flatMap(group => group.slots.map(i => [i, group] as const)));
    const species = (choice: Choice) => choice.variety ?? choice.family;
    const score = () => scopes.map(scope => {
      const counts = new Map<string, number>();
      for (const i of scope) {
        const key = species(picks[i]);
        counts.set(key, (counts.get(key) ?? 0) + 1);
      }
      return [...counts.values()].reduce((sum, n) => sum + n * (n - 1) / 2, 0);
    });
    let previous = score();
    const locked = new Set(groups.filter(g => g.slots.length === 1 && g.choices.length === 1).flatMap(g => g.slots));
    const members = () => shuffle([...affected].filter(i => !locked.has(i)));
    const weighted = (pool: readonly Choice[]) => {
      const families = new Map<string, Choice[]>();
      for (const c of pool) {
        const forms = families.get(c.family) ?? [];
        forms.push(c);
        families.set(c.family, forms);
      }
      return [...families.values()].map(forms => ({ forms: shuffle(forms), priority: -Math.log((next(0x1000000) + 1) / 0x1000001) / forms[0].weight }))
        .sort((a, b) => a.priority - b.priority).flatMap(row => row.forms);
    };
    const tryPicks = (changes: readonly (readonly [
      number,
      Choice
    ])[]) => {
      const old = changes.map(([i]) => [i, picks[i]] as const);
      for (const [i, candidate] of changes)
        picks[i] = candidate;
      const allowed = changes.every(([i, candidate]) => {
        const group = groupBySlot.get(i)!, limit = candidate.maxGroupSize ?? Infinity;
        return group.slots.length <= limit ||
          group.slots.filter(j => picks[j].key === candidate.key).length <= limit;
      });
      const proposed = score();
      const improves = allowed && !proposed.some((n, j) => n > previous[j]) && proposed.some((n, j) => n < previous[j]);
      if (improves) {
        for (const [i, candidate] of changes)
          selected[i] = candidate.values[0][1];
        if (fits(selected)) {
          previous = proposed;
          return true;
        }
      }
      for (const [i, candidate] of old) {
        picks[i] = candidate;
        selected[i] = candidate.values[0][1];
      }
      return false;
    };
    const tryPick = (i: number, candidate: Choice) => species(picks[i]) !== species(candidate) && tryPicks([[i, candidate]]);
    for (const i of members())
      tryPick(i, initial[i].selected);
    let jointAttempts = 128;
    for (const joint of shuffle(layout.jointSlots ?? []).filter(i => affected.has(i) && !locked.has(i))) {
      for (const candidate of weighted(singletons[joint])) {
        if (candidate === picks[joint])
          continue;
        for (const i of members().filter(i => i !== joint && !layout.jointSlots?.includes(i))) {
          if (!jointAttempts--)
            break;
          if (tryPicks([[joint, candidate], [i, initial[i].selected]]))
            break;
        }
        if (jointAttempts <= 0)
          break;
      }
      if (jointAttempts <= 0)
        break;
    }
    for (const freshOnly of [true, false])
      for (let pass = 0; pass < 2; pass++) {
        let changed = false;
        for (const i of members()) {
          const neighbours = new Set(scopes.filter(scope => scope.includes(i)).flatMap(scope => scope.map(j => species(picks[j]))));
          const pool = singletons[i].filter(c => !freshOnly || !neighbours.has(species(c)));
          for (const candidate of weighted(pool))
            if (tryPick(i, candidate)) {
              changed = true;
              break;
            }
        }
        if (!changed)
          break;
      }
    const repeated = new Map<string, number[]>();
    for (const i of members()) {
      const key = species(picks[i]), slots = repeated.get(key) ?? [];
      slots.push(i);
      repeated.set(key, slots);
    }
    let paletteAttempts = 128;
    for (const slots of shuffle([...repeated.values()].filter(slots => slots.length > 2))) {
      if (!paletteAttempts)
        break;
      const oldSpecies = species(picks[slots[0]]);
      const forms = new Map<string, Choice>();
      for (const i of slots)
        for (const c of singletons[i])
          if (species(c) !== oldSpecies && !forms.has(c.key))
            forms.set(c.key, c);
      const order = weighted([...forms.values()]);
      let changed = false;
      for (let first = 0; first < order.length && !changed && paletteAttempts; first++) {
        for (let second = first + 1; second < order.length && !changed && paletteAttempts; second++) {
          const a = order[first], b = order[second];
          if (species(a) === species(b))
            continue;
          --paletteAttempts;
          const options = slots.map(i => ({ i, a: singletons[i].find(c => c.key === a.key), b: singletons[i].find(c => c.key === b.key) }));
          if (options.some(o => !o.a && !o.b))
            continue;
          const flexible = shuffle(options.filter(o => o.a && o.b));
          const forced = options.filter(o => !o.a || !o.b);
          const forcedA = forced.filter(o => o.a).length;
          for (const count of new Set([1, 2, Math.floor(slots.length / 2), slots.length - 2, slots.length - 1])) {
            const take = count - forcedA;
            if (take < 0 || take > flexible.length)
              continue;
            const changes: [
              number,
              Choice
            ][] = [...forced.map(o => [o.i, (o.a ?? o.b)!] as [
              number,
              Choice
            ]),
            ...flexible.map((o, i) => [o.i, (i < take ? o.a : o.b)!] as [
              number,
              Choice
            ])];
            if (tryPicks(changes)) {
              changed = true;
              break;
            }
          }
        }
      }
      if (changed)
        for (const i of slots)
          tryPick(i, initial[i].selected);
    }
    const refined: Group[] = [];
    for (const group of groups) {
      const buckets = new Map<string, number[]>();
      for (const i of group.slots) {
        const slots = buckets.get(picks[i].key) ?? [];
        slots.push(i);
        buckets.set(picks[i].key, slots);
      }
      if (buckets.size === 1 && picks[group.slots[0]].key === group.selected.key) {
        refined.push(group);
        continue;
      }
      for (const slots of buckets.values()) {
        const choice = {
          ...picks[slots[0]], values: slots.map(i => [i, selected[i]] as [
            number,
            T
          ])
        };
        refined.push({ slots, choices: [choice], selected: choice });
      }
    }
    return { groups: refined, selected };
  };
  const solveAffected = (): {
    groups: Group[];
    selected: T[];
  } | undefined => {
    layout?.onProgress?.('grouping crowded rooms');
    const members = initial.filter(g => affected.has(g.slots[0]));
    if (failures(draft).some(scope => scope.individual && scope.slots.some(i => affected.has(i)))) {
      const selected = rollGroups(members, 2);
      if (selected) return { groups: members, selected };
    }
    const anchors = shuffle(members).filter(g => {
      if (layout?.jointSlots?.includes(g.slots[0]))
        return false;
      const values = unassigned();
      for (const [i, value] of g.selected.values)
        values[i] = value;
      return fitsAffected(values);
    }).slice(0, 2);
    const partitions = [...anchors, undefined, undefined].map(anchor => ({ anchor, groups: pairAll(members, anchor) }));
    for (const { groups } of partitions) {
      const selected = rollGroups(groups, 2);
      if (selected)
        return { groups, selected };
    }
    for (let cap = 4; cap < members.length * 2; cap *= 2) {
      layout?.onProgress?.(`trying groups of up to ${cap}`);
      let changed = true;
      while (changed) {
        changed = false;
        for (const partition of partitions) {
          const { anchor } = partition;
          let { groups } = partition;
          let pair: [
            Group,
            Group,
            Choice[]
          ] | undefined;
          const seeds = shuffle(groups.filter(g => !anchor || !g.slots.includes(anchor.slots[0])))
            .sort((a, b) => a.slots.length - b.slots.length);
          for (const first of seeds) {
            const peers = shuffle(groups.filter(g => g !== first && (!anchor || !g.slots.includes(anchor.slots[0])) && first.slots.length + g.slots.length <= cap))
              .sort((a, b) => comparePeers(first, a, b));
            for (const second of peers) {
              const choices = merge(first, second);
              if (choices.length) {
                pair = [first, second, choices];
                break;
              }
            }
            if (pair)
              break;
          }
          if (!pair)
            continue;
          const [first, second, choices] = pair;
          groups = groups.filter(g => g !== first && g !== second);
          groups.push(joined(first, second, choices));
          partition.groups = groups;
          changed = true;
          const selected = rollGroups(groups, 2);
          if (selected)
            return { groups, selected };
        }
      }
    }
    return undefined;
  };
  let selected = draft;
  let pending = failures(selected);
  while (pending.length) {
    const nextScope = pending.find(scope => scope.slots.some(i => !affected.has(i)));
    if (!nextScope)
      break;
    for (const i of nextScope.slots) {
      if (!Number.isInteger(i) || i < 0 || i >= pools.length)
        throw Error('Invalid enemy memory scope');
      affected.add(i);
    }
    preparedFits = layout?.prepareFitsScopes?.(affected);
    const result = solveAffected();
    if (result) {
      selected = result.selected;
      if (fits(selected)) {
        for (let i = 0; i < selected.length; i++)
          if (!affected.has(i) && selected[i] !== draft[i])
            throw Error('Room-scoped grouping changed an unaffected roll');
        layout?.onProgress?.('improving enemy variety');
        const varied = diversify(result.groups, [...selected]);
        return finish([...initial.filter(g => !affected.has(g.slots[0])), ...varied.groups], varied.selected);
      }
      pending = failures(selected);
    }
    else {
      pending = failures(selected).sort((a, b) => Number(b.slots.some(i => affected.has(i))) - Number(a.slots.some(i => affected.has(i))) ||
        (a.roomCount ?? 1) - (b.roomCount ?? 1) || b.excess - a.excess);
    }
  }
  throw new EnemyMemoryAllocationError('No feasible weighted allocation found within the over-budget room scopes');
}
