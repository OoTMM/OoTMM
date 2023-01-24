import { Buffer } from 'buffer';
import { generate } from '@ootmm/core';

let generator = null;
const startWorker = (params) => {
  params.oot = Buffer.from(params.oot);
  params.mm = Buffer.from(params.mm);
  params.monitor = {
    onLog: (message) => self.postMessage({ type: 'log', message: message }),
  };
  generator = generate(params);
  generator.run().then(result => {
    self.postMessage({ type: 'end', rom: result.rom, log: result.log, hash: result.hash });
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
