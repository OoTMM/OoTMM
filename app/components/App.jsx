import React, { useState } from 'react';
import { RomConfig } from './RomConfig';
import { Progress } from './Progress';
import { Result } from './Result';

export const App = () => {
  const [roms, setRoms] = useState({ oot: null, mm: null });
  const [error, setError] = useState("");
  const [isGenerating, setIsGenerating] = useState(false);
  const [message, setMessage] = useState("");
  const [result, setResult] = useState(null);

  const setRom = (game, data) => setRoms({ ...roms, [game]: data });

  const generateRom = async () => {
    const [oot, mm] = await Promise.all([
      roms.oot.arrayBuffer(),
      roms.mm.arrayBuffer(),
    ]);
    const ootBuffer = Buffer.from(oot);
    const mmBuffer = Buffer.from(mm);
    const worker = new Worker(new URL('../worker.js', import.meta.url));
    worker.onmessage = ({ data }) => {
      console.log(data);
      if (data.type === 'log') {
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
      {!result && !isGenerating && <RomConfig setRom={setRom} error={error} onGenerate={generateRom}/>}
    </div>
  );
};
