import React from 'react';

const download = (data, name, mime) => {
  const a = document.createElement('a');
  const blob = new Blob([data], { type: mime });
  a.href = window.URL.createObjectURL(blob);
  a.download = name;
  a.click();
};

export const Result = ({ rom, log }) => {
  return (
    <div>
      <button onClick={() => download(rom, 'OoTMM.z64', 'application/octet-stream')}>Save ROM</button>
      <button onClick={() => download(log, 'spoiler.txt', 'text/plain')}>Save Spoiler Log</button>
    </div>
  );
};
