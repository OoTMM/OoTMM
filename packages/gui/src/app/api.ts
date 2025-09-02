import { Items, Settings, OptionsInput, GeneratorOutput, makeCosmetics, makeSettings, makeRandomSettings } from '@ootmm/core';
import type { WorkerResult, WorkerResultGenerate, WorkerResultGenerateError } from './worker';
import Worker from './worker?worker';
import JSZip from 'jszip';
import { localStoragePrefixedGet } from './util';

export type ResultFile = {
  name: string;
  mime: string;
  data: Uint8Array | Blob | string;
};

type Resolver = {
  resolve: (value: any) => void;
  reject: (reason: any) => void;
};

let workerTaskId = 0;
const worker = new Worker();
const resolvers = new Map<number, Resolver>();
const resolversGenerate = new Map<number, (result: WorkerResultGenerate | WorkerResultGenerateError) => void>();
const loggersGenerate = new Map<number, (log: string) => void>();
const loggersProgress = new Map<number, (progress: number, total: number) => void>();

worker.onmessage = (event: MessageEvent<WorkerResult>) => {
  const result = event.data;
  switch (result.type) {
  case 'call-result': {
    const resolver = resolvers.get(result.id);
    if (resolver) {
      resolvers.delete(result.id);
      const cb = result.success ? resolver.resolve : resolver.reject;
      cb(result.data);
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

function workerCall<T>(func: string, args: any[]): Promise<T> {
  return new Promise((resolve, reject) => {
    const id = workerTaskId++;
    resolvers.set(id, { resolve, reject });
    worker.postMessage({
      type: 'call',
      id,
      func,
      args,
    });
  });
}

export const itemPool = (settings: Settings) => workerCall<Items>('itemPool', [settings]);
export const locationList = (settings: Partial<Settings>) => workerCall<string[]>('locationList', [settings]);
//export const makeSettings = (settings: Partial<Settings>) => workerCall<Settings>('makeSettings', [settings]);
//export const makeRandomSettings = (settings: Partial<OptionRandomSettings>) => workerCall<OptionRandomSettings>('makeRandomSettings', [settings]);
//export const mergeSettings = (settings: Settings, patch: SettingsPatch) => workerCall<Settings>('mergeSettings', [settings, patch]);

export function initialSettings() {
  return makeSettings(localStoragePrefixedGet('settings'));
};

export function initialRandomSettings() {
  return makeRandomSettings(localStoragePrefixedGet('randomSettings'));
}

export function initialCosmetics() {
  return makeCosmetics(localStoragePrefixedGet('cosmetics'));
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
    const prev = items[item];
    if (prev) {
      newItems[item] = Math.min(items[item], pool[item]);
    }
  }
  return newItems;
}
