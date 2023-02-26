import { GeneratorOutput } from '@ootmm/core';
import React from 'react';

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
export const Result = ({ result: { rom, hash, log, patch } }: Props) => <div>
  <button onClick={() => download(rom, appendHash('OoTMM', hash, 'z64'), 'application/octet-stream')}>Save ROM</button>
  {log && <button onClick={() => download(log, appendHash('OoTMM-Spoiler', hash, 'txt'), 'text/plain')}>Save Spoiler Log</button>}
  {patch && <button onClick={() => download(patch, appendHash('OoTMM-Patch', hash, 'ootmm'), 'application/octet-stream')}>Save Patch File</button>}
</div>;
