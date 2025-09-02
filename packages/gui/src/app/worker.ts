import { itemPool, OptionsInput, GeneratorOutput, generate, locationList, makeSettings, makeRandomSettings, mergeSettings } from '@ootmm/core';

export type WorkerTaskCall = {
  type: 'call',
  id: number,
  func: string,
  args: any[],
};

export type WorkerTaskGenerate = {
  type: 'generate',
  id: number,
  oot: File,
  mm: File,
  patch?: File,
  options: OptionsInput
}

export type WorkerResultCall = {
  type: 'call-result',
  id: number,
  success: boolean,
  data: any,
};

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

export type WorkerTask = WorkerTaskCall | WorkerTaskGenerate;
export type WorkerResult = WorkerResultCall | WorkerResultGenerate | WorkerResultGenerateLog | WorkerResultGenerateProgress | WorkerResultGenerateError;

async function readFile(f: File) {
  const reader = new FileReader();
  return new Promise<Uint8Array>((resolve, reject) => {
    reader.onload = () => {
      const data = reader.result;
      if (!data) {
        reject(new Error('No data'));
        return;
      }
      return resolve(new Uint8Array(data as ArrayBuffer));
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
  let patchData: Uint8Array | undefined;
  if (patch) {
    patchData = await readFile(patch);
  }
  const generator = generate({ oot: ootData, mm: mmData, opts: { ...options, patch: patchData }, monitor: { onLog, onProgress, onWarn } });
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
    console.error(err);
  });
}

const FUNCS: {[k: string]: (...args: any[]) => any} = {
  'itemPool': itemPool,
  'locationList': locationList,
  'makeSettings': makeSettings,
  'makeRandomSettings': makeRandomSettings,
  'mergeSettings': mergeSettings,
};

function taskCallCompletion(id: number, success: boolean, data: any) {
  postMessage({
    type: 'call-result',
    id,
    success,
    data,
  });
}

function taskCallSuccess(id: number, data: any) {
  taskCallCompletion(id, true, data);
}

function taskCallError(id: number, data: any) {
  taskCallCompletion(id, false, data);
}

function onTaskCall(task: WorkerTaskCall) {
  try {
    const func = FUNCS[task.func];
    const result = func(...task.args);
    const resultP = Promise.resolve(result);
    resultP.then((data) => {
      taskCallSuccess(task.id, data);
    }).catch((e) => {
      taskCallError(task.id, e);
    });
  } catch (e) {
    taskCallError(task.id, e);
  }
}

function onMessage(event: MessageEvent<WorkerTask>) {
  const task = event.data;
  switch (task.type) {
  case 'call':
    onTaskCall(task);
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
