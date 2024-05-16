import { Items, Settings, OptionsInput, makeSettings, makeCosmetics, makeRandomSettings, GeneratorOutput } from '@ootmm/core';
import type { WorkerResult, WorkerResultGenerate, WorkerResultGenerateError, WorkerResultItemPool } from './worker';
import Worker from './worker?worker';
import JSZip from 'jszip';

export type ResultFile = {
  name: string;
  mime: string;
  data: Buffer | Blob | string;
};

let workerTaskId = 0;
const worker = new Worker();
const resolversItemPool = new Map<number, (result: WorkerResultItemPool) => void>();
const resolversGenerate = new Map<number, (result: WorkerResultGenerate | WorkerResultGenerateError) => void>();
const loggersGenerate = new Map<number, (log: string) => void>();
const loggersProgress = new Map<number, (progress: number, total: number) => void>();

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
  case 'generate-progress': {
    const logger = loggersProgress.get(result.id);
    if (logger) {
      logger(result.progress, result.total);
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

export async function generate(files: { oot: File, mm: File, patch?: File }, options: OptionsInput, log: (msg: string) => void, progress: (current: number, total: number) => void) {
  const result = await new Promise<WorkerResultGenerate | WorkerResultGenerateError>(resolve => {
    const id = workerTaskId++;
    resolversGenerate.set(id, result => {
      resolve(result);
    });
    loggersGenerate.set(id, log);
    loggersProgress.set(id, progress);
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
  return { data: result.data, warnings: result.warnings };
}

export async function archive(result: GeneratorOutput): Promise<ResultFile> {
  const { hash, files } = result;

  if (files.length === 1) {
    return files[0];
  }

  const name = `OoTMM-${hash}.zip`;
  const mime = 'application/zip';
  const zip = new JSZip();
  files.forEach((file) => {
    zip.file(file.name, file.data);
  });
  const f = await zip.generateAsync({ type: 'blob', compression: 'DEFLATE' });
  return { name, mime, data: f };
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
