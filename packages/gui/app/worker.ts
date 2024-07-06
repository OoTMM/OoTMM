import JSZip from 'jszip';
import { Settings, itemPool, Items, OptionsInput, GeneratorOutput, generate } from '@ootmm/core';
import dataVersionZipFile from '@ootmm/core/dist/data.zip?url';

async function makeDataPromise(path: string) {
  const reply = await fetch(path);
  if (reply.ok) {
    const ab = await reply.arrayBuffer();
    return Buffer.from(ab);
  } else {
    throw new Error(`Failed to download ${path}`);
  }
}

const dataPromise = makeDataPromise(dataVersionZipFile).then(data => JSZip.loadAsync(data));

async function resolverFetchFunc(path: string) {
  const zip = await dataPromise;
  const file = zip.file(path);
  if (file) {
    return file.async('arraybuffer').then(ab => Buffer.from(ab));
  } else {
    throw new Error(`Failed to unzip file ${path}`);
  }
}

async function resolverGlobFunc(pattern: RegExp) {
  const zip = await dataPromise;
  return zip.file(pattern).map(f => f.name);
}

export type WorkerTaskItemPool = {
  type: 'itemPool',
  id: number,
  settings: Settings,
}

export type WorkerTaskGenerate = {
  type: 'generate',
  id: number,
  oot: File,
  mm: File,
  patch?: File,
  options: OptionsInput
}

export type WorkerResultItemPool = {
  type: 'itemPool',
  id: number,
  itemPool: Items,
}

export type WorkerResultGenerate = {
  type: 'generate',
  id: number,
  data: GeneratorOutput,
  warnings: string[],
}

export type WorkerResultGenerateLog = {
  type: 'generate-log',
  id: number,
  log: string
}

export type WorkerResultGenerateProgress = {
  type: 'generate-progress',
  id: number,
  progress: number,
  total: number,
}

export type WorkerResultGenerateError = {
  type: 'generate-error',
  id: number,
  error: any,
}

export type WorkerTask = WorkerTaskItemPool | WorkerTaskGenerate;
export type WorkerResult = WorkerResultItemPool | WorkerResultGenerate | WorkerResultGenerateLog | WorkerResultGenerateProgress | WorkerResultGenerateError;

async function readFile(f: File) {
  const reader = new FileReader();
  return new Promise<Buffer>((resolve, reject) => {
    reader.onload = () => {
      const data = reader.result;
      if (!data) {
        reject(new Error('No data'));
        return;
      }
      return resolve(Buffer.from(data as ArrayBuffer));
    };
    reader.onerror = () => {
      reject(reader.error);
    };
    reader.readAsArrayBuffer(f);
  });
}

async function onTaskGenerate(task: WorkerTaskGenerate) {
  let { oot, mm, patch, options } = task;
  options.cosmetics = { ...options.cosmetics };
  const warnings: string[] = [];
  const onLog = (msg: string) => {
    postMessage({
      type: 'generate-log',
      id: task.id,
      log: msg,
    });
  };
  const onWarn = (msg: string) => {
    warnings.push(msg);
  }
  const onProgress = (progress: number, total: number) => {
    postMessage({
      type: 'generate-progress',
      id: task.id,
      progress,
      total,
    });
  };
  const [ootData, mmData] = await Promise.all([readFile(oot), readFile(mm)]);
  let patchData: Buffer | undefined;
  if (patch) {
    patchData = await readFile(patch);
  }
  const resolver = { fetch: resolverFetchFunc, glob: resolverGlobFunc };
  const generator = generate({ oot: ootData, mm: mmData, opts: { ...options, patch: patchData, resolver }, monitor: { onLog, onProgress, onWarn } });
  generator.run().then(result => {
    postMessage({
      type: 'generate',
      id: task.id,
      data: result,
      warnings,
    });
  }).catch((err) => {
    postMessage({
      type: 'generate-error',
      id: task.id,
      error: err,
    });
  });
}

function onTaskItemPool(task: WorkerTaskItemPool) {
  const result = itemPool(task.settings);
  postMessage({
    type: 'itemPool',
    id: task.id,
    itemPool: result,
  });
}

function onMessage(event: MessageEvent<WorkerTask>) {
  const task = event.data;
  switch (task.type) {
  case 'itemPool':
    onTaskItemPool(task);
    break;
  case 'generate':
    onTaskGenerate(task);
    break;
  }
}

self.onmessage = onMessage;

self.onerror = (event) => {
  console.error(event);
}
