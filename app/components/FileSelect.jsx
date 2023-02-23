import React from 'react';

import logoOot from '../assets/oot.png';
import logoMm from '../assets/mm.png';
import logoOotMm from '../assets/logo.png';

const LOGOS = {
  oot: logoOot,
  mm: logoMm,
  ootmm: logoOotMm,
};

export const FileSelect = ({ logo, label, accept, file, onChange }) => {
  return (
    <label className="file-select">
      {label}
      <div className={`file-select-box ${!file ? 'empty' : ''}`}>
        <div className='file-select-box-img-container'>
          <img src={LOGOS[logo]} width="100%"/>
        </div>
        <input type="file" accept={accept} onChange={e => onChange(e.target.files[0])}/>
        {!file && <p>No file selected</p>}
        {file && <p>{file.name}</p>}
      </div>
    </label>
  );
};
