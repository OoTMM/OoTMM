import { Items, Settings, OptionsInput, makeSettings, makeCosmetics, makeRandomSettings } from '@ootmm/core';
import type { WorkerResult, WorkerResultGenerate, WorkerResultGenerateError, WorkerResultItemPool } from './worker';

let workerTaskId = 0;
const worker = new Worker(new URL('./worker.ts', import.meta.url));
const resolversItemPool = new Map<number, (result: WorkerResultItemPool) => void>();
const resolversGenerate = new Map<number, (result: WorkerResultGenerate | WorkerResultGenerateError) => void>();
const loggersGenerate = new Map<number, (log: string) => void>();

worker.onmessage = (event: MessageEvent<WorkerResult>) => {
  const result = event.data;
  switch (result.type) {
  case 'itemPool': {
    const resolver = resolversItemPool.get(result.id);
    if (resolver) {
      resolversItemPool.delete(result.id);
      resolver(result);
    }
    break;
  }
  case 'generate':
  case 'generate-error': {
    const resolver = resolversGenerate.get(result.id);
    if (resolver) {
      resolversGenerate.delete(result.id);
      loggersGenerate.delete(result.id);
      resolver(result);
    }
    break;
  }
  case 'generate-log': {
    const logger = loggersGenerate.get(result.id);
    if (logger) {
      logger(result.log);
    }
    break;
  }
  }
};

export function initialSettings() {
  const oldSettings = JSON.parse(localStorage.getItem('settings') ?? "{}");
  return makeSettings(oldSettings);
};

export function initialRandomSettings() {
  const oldRandomSettings = JSON.parse(localStorage.getItem('randomSettings') ?? "{}");
  return makeRandomSettings(oldRandomSettings);
}

export async function itemPoolFromSettings(settings: Settings): Promise<Items> {
  const id = workerTaskId++;
  const result = await new Promise<WorkerResultItemPool>(resolve => {
    resolversItemPool.set(id, result => {
      resolve(result);
    });
    worker.postMessage({
      type: 'itemPool',
      id,
      settings,
    });
  });
  return result.itemPool;
}

export function initialCosmetics() {
  const oldCosmetics = JSON.parse(localStorage.getItem('cosmetics') ?? "{}");
  return makeCosmetics(oldCosmetics);
}

export async function generate(files: { oot: Buffer, mm: Buffer, patch?: Buffer }, options: OptionsInput, log: (msg: string) => void) {
  const result = await new Promise<WorkerResultGenerate | WorkerResultGenerateError>(resolve => {
    const id = workerTaskId++;
    resolversGenerate.set(id, result => {
      resolve(result);
    });
    loggersGenerate.set(id, log);
    worker.postMessage({
      type: 'generate',
      id,
      oot: files.oot,
      mm: files.mm,
      patch: files.patch,
      options,
    });
  });
  if (result.type === 'generate-error') {
    throw result.error;
  }
  return result.data;
}

export function restrictItemsByPool(items: Items, pool: Items) {
  const newItems: Items = {};
  for (const item in pool) {
    if (items[item]) {
      newItems[item] = Math.min(items[item], pool[item]);
    }
  }
  return newItems;
}
