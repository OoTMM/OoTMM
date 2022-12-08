import React from 'react';

import logoOot from '../assets/oot.png';
import logoMm from '../assets/mm.png';

const LOGOS = {
  oot: logoOot,
  mm: logoMm,
};

export const FileSelect = ({ game, label, file, onChange }) => {
  return (
    <label className="file-select">
      {label}
      <div className={`file-select-box ${!file ? 'empty' : ''}`}>
        <img src={LOGOS[game]} width="100%"/>
        <input type="file" onChange={e => onChange(e.target.files[0])}/>
        {!file && <p>No file selected</p>}
        {file && <p>{file.name}</p>}
      </div>
    </label>
  );
};
