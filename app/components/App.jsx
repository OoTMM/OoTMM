import React, { useState } from 'react';

import { Generator } from './Generator';
import { Progress } from './Progress';
import { Result } from './Result';

export const App = () => {
  const [error, setError] = useState("");
  const [isGenerating, setIsGenerating] = useState(false);
  const [message, setMessage] = useState("");
  const [result, setResult] = useState(null);

  const generate = async ({ roms }) => {
    const [oot, mm] = await Promise.all([
      roms.oot.arrayBuffer(),
      roms.mm.arrayBuffer(),
    ]);
    const ootBuffer = Buffer.from(oot);
    const mmBuffer = Buffer.from(mm);
    const worker = new Worker(new URL('../worker.js', import.meta.url));
    worker.onmessage = ({ data }) => {
      if (data.type === 'log') {
        console.log(data.message);
        setMessage(data.message);
      } else if (data.type === 'error') {
        setError(data.message);
        setIsGenerating(false);
        worker.terminate();
      } else if (data.type === 'end') {
        const { rom, log } = data;
        setResult({ rom, log });
        setIsGenerating(false);
        worker.terminate();
      }
    };
    setError("");
    setIsGenerating(true);
    setMessage("Generating");
    worker.postMessage({ type: 'start', params: { oot: ootBuffer, mm: mmBuffer }});
  };

  return (
    <div>
      <h1 className="generator-title">OoTMM Web Generator</h1>
      <br/>
      <p>Version: {process.env.VERSION}</p>
      <br/>
      <br/>
      {result && <Result rom={result.rom} log={result.log}/>}
      {!result && isGenerating && <Progress message={message}/>}
      {!result && !isGenerating && <Generator error={error} onGenerate={generate}/>}
    </div>
  );
};
