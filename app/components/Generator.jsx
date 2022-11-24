import React, { useState } from 'react';
import { Buffer } from 'buffer';

import { Dropzone } from './Dropzone';

const download = (data, name, mime) => {
  const a = document.createElement('a');
  const blob = new Blob([data], { type: mime });
  a.href = window.URL.createObjectURL(blob);
  a.download = name;
  a.click();
};

export const Generator = () => {
  const [generating, setGenerating] = useState(false);
  const [error, setError] = useState("");
  const [romOot, setRomOot] = useState(null);
  const [romMm, setRomMm] = useState(null);
  const [message, setMessage] = useState("");
  const [result, setResult] = useState(null);

  const generateRom = async () => {
    const [oot, mm] = await Promise.all([
      romOot.arrayBuffer(),
      romMm.arrayBuffer(),
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
        setMessage("");
        setGenerating(false);
        worker.close();
      } else if (data.type === 'end') {
        const { rom, log } = data;
        setMessage("");
        setResult({ rom, log });
        setGenerating(false);
        worker.close();
      }
    };
    setError("");
    setGenerating(true);
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
      {error && <div className="generator-error">{error}</div>}
      {!(generating || result) &&
        <form target='_self' onSubmit={(e) => {e.preventDefault(); generateRom();}}>
          <label>
            Ocarina of Time (1.0, U or J)<br/>
            <input type="file" onChange={e => setRomOot(e.target.files[0])}/>
          </label>
          <br/>
          <br/>
          <label>
            Majora's Mask (U only)<br/>
            <input type="file" onChange={e => setRomMm(e.target.files[0])}/>
          </label>
          <br/>
          <br/>
          <button type="submit">Generate</button>
        </form>
      }
      {result && <div>
        <button onClick={() => { download(result.rom, 'OoTMM.z64', 'application/octet-stream'); }}>Save ROM</button>
        <button onClick={() => { download(result.log, 'spoiler.txt', 'text/plain'); }}>Save Spoiler Log</button>
      </div>}
      <h1>{message}</h1>
    </div>
  );
};
