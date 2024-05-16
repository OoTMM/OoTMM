import React, { useState } from 'react';

import logoOot from '../assets/oot.png';
import logoMm from '../assets/mm.png';
import logoOotMm from '../assets/logo.png';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faXmark } from '@fortawesome/free-solid-svg-icons';

const LOGOS = {
  oot: logoOot,
  mm: logoMm,
  ootmm: logoOotMm,
};

type FileSelectProps = {
  logo: keyof typeof LOGOS;
  label: string;
  accept: string;
  file: File | null;
  onChange: (file: File | null) => void;
}

export const FileSelect = ({ logo, label, accept, onChange, file }: FileSelectProps) => {
  return (
    <label className="file-select-label">
      {label}
      <div className={`file-select-box ${!file ? 'empty' : ''}`}>
        <div className='file-select-box-img-container'>
          <img src={LOGOS[logo]} width="100%"/>
        </div>
        <input type="file" accept={accept} onChange={e => e.target.files && e.target.files[0] && onChange(e.target.files[0])}/>
        {!file && <>No file selected</>}
        {file && <>
          {file.name}
          <span className="file-select-box-delete" onClick={(e) => { onChange(null); e.preventDefault(); }}><FontAwesomeIcon icon={faXmark} /></span>
        </>}
      </div>
    </label>
  );
};
