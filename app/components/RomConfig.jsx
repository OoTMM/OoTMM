import React from 'react';

import { FileSelect } from './FileSelect';

export const RomConfig = ({ roms, setRom, error, onGenerate }) => {
  return (
    <div>
      {error && <div className="generator-error">{error}</div>}
      <form target='_self' onSubmit={(e) => {e.preventDefault(); onGenerate();}}>
        <div className="flex-h">
          <FileSelect game="oot" label="Ocarina of Time (1.0, U or J)" file={roms.oot} onChange={f => setRom('oot', f)}/>
          <FileSelect game="mm" label="Majora's Mask (U only)" file={roms.mm} onChange={f => setRom('mm', f)}/>
        </div>
        <br/>
        <button type="submit">Generate</button>
      </form>
    </div>
  );
};

/* <input type="file" onChange={e => setRom('mm', e.target.files[0])}/> */
