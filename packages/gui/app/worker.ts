import { Buffer } from 'buffer';
import { Settings, itemPool, Items, OptionsInput, GeneratorOutput, generate } from '@ootmm/core';

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
  let { oot, mm, patch } = task;
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
  const generator = generate({ oot, mm, opts: { ...task.options, patch }, monitor: { onLog } });
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
