import React, { useState } from 'react';
import { Buffer } from 'buffer';

export const GeneratorConfig = () => {
  const [romOot, setRomOot] = useState(null);
  const [romMm, setRomMm] = useState(null);
  const [message, setMessage] = useState("");

  const generateRom = async () => {
    const [oot, mm] = await Promise.all([
      romOot.arrayBuffer(),
      romMm.arrayBuffer(),
    ]);
    const ootBuffer = Buffer.from(oot);
    const mmBuffer = Buffer.from(mm);
    const worker = new Worker(new URL('./worker.js', import.meta.url));
    worker.onmessage = ({ data }) => {
      console.log(data);
      if (data.type === 'log') {
        setMessage(data.message);
      }
    };
    worker.postMessage({ type: 'start', params: { oot: ootBuffer, mm: mmBuffer }});
  };

  return (
    <form target='_self' onSubmit={(e) => {e.preventDefault(); generateRom();}}>
      <label>
        Ocarina of Time (1.0, U or J)<br/>
        <input type="file" onChange={e => setRomOot(e.target.files[0])}/>
      </label>
      <br/>
      <label>
        Majora's Mask (U only)<br/>
        <input type="file" onChange={e => setRomMm(e.target.files[0])}/>
      </label>
      <br/>
      <button type="submit">Generate</button>
      <h1>{message}</h1>
    </form>
  );
};
