import React from 'react';

const download = (data, name, mime) => {
  const a = document.createElement('a');
  const blob = new Blob([data], { type: mime });
  a.href = window.URL.createObjectURL(blob);
  a.download = name;
  a.click();
};

const appendHash = (str, hash, ext) => {
  if (hash) {
    return `${str}-${hash}.${ext}`;
  }
  return `${str}.${ext}`;
};

export const Result = ({ rom, log, hash }) => {
  return (
    <div>
      <button onClick={() => download(rom, appendHash('OoTMM', hash, 'z64'), 'application/octet-stream')}>Save ROM</button>
      <button onClick={() => download(log, appendHash('spoiler', hash, 'txt'), 'text/plain')}>Save Spoiler Log</button>
    </div>
  );
};
