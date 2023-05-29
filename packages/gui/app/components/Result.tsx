import { GeneratorOutput } from '@ootmm/core';
import React from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { solid } from '@fortawesome/fontawesome-svg-core/import.macro';

const download = (data: Buffer | string, name: string, mime: string) => {
  const a = document.createElement('a');
  const blob = new Blob([data], { type: mime });
  a.href = window.URL.createObjectURL(blob);
  a.download = name;
  a.click();
};

const appendHash = (str: string, hash: string | null, ext: string) => {
  if (hash) {
    return `${str}-${hash}.${ext}`;
  }
  return `${str}.${ext}`;
};

type Props = {
  result: GeneratorOutput;
}
export const Result = ({ result: { rom, hash, log, patch } }: Props) => 
<div>
  {log && <div style={{border: "none", borderRadius:"2px", background: "#ff5722", fontWeight:"700", fontSize:"1em", padding: "0.5em 1em", textDecoration: "none", minWidth: "48px"}}>
    <div><span style={{textAlign:"left"}}><FontAwesomeIcon icon={solid("triangle-exclamation")}/> WARNING </span></div>
      <div>Be advised that this page is only accessible ONCE, it is extremely recommended to save the spoiler log.</div>
      <div>There are a few scenarios where you may encounter unbeatable seeds, in which case, you would need to report them in the Discord.</div>
  </div>}
  <div>
    <button className='btn-download' onClick={() => download(rom, appendHash('OoTMM', hash, 'z64'), 'application/octet-stream')}>Save ROM</button>
    {log && <button className='btn-download' onClick={() => download(log, appendHash('OoTMM-Spoiler', hash, 'txt'), 'text/plain')}>Save Spoiler Log</button>}
    {patch && <button className='btn-download' onClick={() => download(patch, appendHash('OoTMM-Patch', hash, 'ootmm'), 'application/octet-stream')}>Save Patch File</button>}
  </div>
</div>;
