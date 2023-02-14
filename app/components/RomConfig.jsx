import React from 'react';

import { FileSelect } from './FileSelect';

export const RomConfig = ({
  roms,
  setRom,
  seed,
  setSeed,
  error,
  onGenerate,
}) => {
  return (
    <div>
      {error && <div className="generator-error">{error}</div>}
      <form
        target="_self"
        onSubmit={(e) => {
          e.preventDefault();
          onGenerate();
        }}
      >
        <div className="flex-h">
          <FileSelect
            game="oot"
            label="Ocarina of Time (1.0, U or J)"
            file={roms.oot}
            onChange={(f) => setRom('oot', f)}
          />
          <FileSelect
            game="mm"
            label="Majora's Mask (U only)"
            file={roms.mm}
            onChange={(f) => setRom('mm', f)}
          />
        </div>
        <label>
          Seed (leave blank to auto-generate)
          <input
            type="text"
            value={seed}
            onChange={(e) => setSeed(e.target.value)}
          />
        </label>
        <button className="btn-primary sm-margin-top" type="submit">
          Generate
        </button>
      </form>
    </div>
  );
};
