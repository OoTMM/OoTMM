import React from 'react';

export const RomConfig = ({ setRom, error, onGenerate }) => {
  return (
    <div>
      {error && <div className="generator-error">{error}</div>}
      <form target='_self' onSubmit={(e) => {e.preventDefault(); onGenerate();}}>
        <label>
          Ocarina of Time (1.0, U or J)<br/>
          <input type="file" onChange={e => setRom('oot', e.target.files[0])}/>
        </label>
        <br/>
        <br/>
        <label>
          Majora's Mask (U only)<br/>
          <input type="file" onChange={e => setRom('mm', e.target.files[0])}/>
        </label>
        <br/>
        <br/>
        <button type="submit">Generate</button>
      </form>
    </div>
  );
};
