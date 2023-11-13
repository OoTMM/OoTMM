import { Buffer } from 'buffer';
import JSZip from 'jszip';
import { Settings, itemPool, Items, OptionsInput, GeneratorOutput, generate, COSMETICS } from '@ootmm/core';

const VERSION = process.env.VERSION as string;

async function makeDataPromise(path: string) {
  const reply = await fetch(path);
  if (reply.ok) {
    const ab = await reply.arrayBuffer();
    return Buffer.from(ab);
  } else {
    throw new Error(`Failed to download ${path}`);
  }
}

const dataPromise = makeDataPromise(`/data-${VERSION}.zip`).then(data => JSZip.loadAsync(data));

async function fetchFunc(path: string) {
  const zip = await dataPromise;
  const file = zip.file(path);
  if (file) {
    return file.async('arraybuffer').then(ab => Buffer.from(ab));
  } else {
    throw new Error(`Failed to unzip file ${path}`);
  }
}

export type WorkerTaskItemPool = {
  type: 'itemPool',
  id: number,
  settings: Settings,
}

export type WorkerTaskGenerate = {
  type: 'generate',
  id: number,
  oot: Buffer,
  mm: Buffer,
  patch?: Buffer,
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
}

export type WorkerResultGenerateLog = {
  type: 'generate-log',
  id: number,
  log: string
}

export type WorkerResultGenerateError = {
  type: 'generate-error',
  id: number,
  error: any,
}

export type WorkerTask = WorkerTaskItemPool | WorkerTaskGenerate;
export type WorkerResult = WorkerResultItemPool | WorkerResultGenerate | WorkerResultGenerateLog | WorkerResultGenerateError;

function onTaskGenerate(task: WorkerTaskGenerate) {
  let { oot, mm, patch, options } = task;
  options.cosmetics = { ...options.cosmetics };
  for (const c of COSMETICS) {
    if ((c.type === 'zobj' || c.type === 'zip') && options.cosmetics[c.key]) {
      options.cosmetics[c.key] = Buffer.from(options.cosmetics[c.key] as any);
    }
  }
  [oot, mm] = [oot, mm].map(b => Buffer.from(b));
  if (patch) {
    patch = Buffer.from(patch);
  }
  const onLog = (msg: string) => {
    postMessage({
      type: 'generate-log',
      id: task.id,
      log: msg,
    });
  };
  const generator = generate({ oot, mm, opts: { ...options, patch, fetch: fetchFunc }, monitor: { onLog } });
  generator.run().then(result => {
    postMessage({
      type: 'generate',
      id: task.id,
      data: result,
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
