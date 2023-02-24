import { Buffer } from 'buffer';
import { generate, GeneratorParams } from '@ootmm/core';

let generator = null;
const startWorker = (params: GeneratorParams) => {
  params.oot = Buffer.from(params.oot);
  params.mm = Buffer.from(params.mm);
  if (params.opts) {
    params.opts.patch = params.opts.patch ? Buffer.from(params.opts.patch) : undefined;
  }
  params.monitor = {
    onLog: (message) => self.postMessage({ type: 'log', message: message }),
  };
  generator = generate(params);
  generator.run().then(result => {
    self.postMessage({ type: 'end', result });
  }).catch((err) => {
    self.postMessage({ type: 'error', message: err.message });
  });
};

self.onmessage = ({ data }) => {
  switch (data.type) {
  case 'start':
    startWorker(data.params);
    break;
  }
};
