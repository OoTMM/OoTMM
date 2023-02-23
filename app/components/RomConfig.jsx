import React from 'react';
import { Checkbox } from './Checkbox';

import { FileSelect } from './FileSelect';

export const RomConfig = ({
  roms,
  setRom,
  usePatch,
  setUsePatch,
  patch,
  setPatch,
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
            logo="oot"
            label="Ocarina of Time (1.0, U or J)"
            accept=".z64, .n64, .v64"
            file={roms.oot}
            onChange={(f) => setRom('oot', f)}
          />
          <FileSelect
            logo="mm"
            label="Majora's Mask (U only)"
            accept=".z64, .n64, .v64"
            file={roms.mm}
            onChange={(f) => setRom('mm', f)}
          />
          {usePatch &&
            <FileSelect
              logo="ootmm"
              label="OoTMM Patch File"
              accept=".ootmm"
              file={patch}
              onChange={(f) => setPatch(f)}
            />
          }
        </div>
        <Checkbox label="Use a patch file" checked={usePatch} onChange={setUsePatch}/>
        {!usePatch &&
          <label>
            Seed (leave blank to auto-generate)
            <input
              type="text"
              value={seed}
              onChange={(e) => setSeed(e.target.value)}
            />
          </label>
        }
        <button className="btn-primary sm-margin-top" type="submit">
          Generate
        </button>
      </form>
    </div>
  );
};
