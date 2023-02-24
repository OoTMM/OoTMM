import React, { useState } from 'react';

import { Generator } from './Generator';
import { Progress } from './Progress';
import { Result } from './Result';

export const App = () => {
  const [error, setError] = useState('');
  const [isGenerating, setIsGenerating] = useState(false);
  const [message, setMessage] = useState('');
  const [result, setResult] = useState(null);

  const generate = async ({ roms, opts }: { roms: any, opts: any }) => {
    const [oot, mm] = await Promise.all([
      roms.oot.arrayBuffer(),
      roms.mm.arrayBuffer(),
    ]);
    const ootBuffer = Buffer.from(oot);
    const mmBuffer = Buffer.from(mm);
    if (opts.patch) {
      opts.patch = Buffer.from(await opts.patch.arrayBuffer());
    }
    const worker = new Worker(new URL('../worker.ts', import.meta.url));
    worker.onmessage = ({ data }) => {
      if (data.type === 'log') {
        console.log(data.message);
        setMessage(data.message);
      } else if (data.type === 'error') {
        setError(data.message);
        setIsGenerating(false);
        worker.terminate();
      } else if (data.type === 'end') {
        setResult(data.result);
        setIsGenerating(false);
        worker.terminate();
      }
    };
    setError('');
    setIsGenerating(true);
    setMessage('Generating');
    worker.postMessage({
      type: 'start',
      params: { oot: ootBuffer, mm: mmBuffer, opts },
    });
  };

  return (
    <div>
      <h1>OoTMM Web Generator</h1>
      <h2>Version: {process.env.VERSION}</h2>
      {result && (
        <Result data={result} />
      )}
      {!result && isGenerating && <Progress message={message} />}
      {!result && !isGenerating && (
        <Generator error={error} onGenerate={generate} />
      )}
    </div>
  );
};
