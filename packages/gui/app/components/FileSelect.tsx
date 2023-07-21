import React, { useState } from 'react';

import logoOot from '../assets/oot.png';
import logoMm from '../assets/mm.png';
import logoOotMm from '../assets/logo.png';
import { useFiles } from '../contexts/GeneratorContext';

const LOGOS = {
  oot: logoOot,
  mm: logoMm,
  ootmm: logoOotMm,
};

type FileSelectProps = {
  logo: keyof typeof LOGOS;
  label: string;
  accept: string;
  file: string;
  onChange: (file: Buffer) => void;
}

export const FileSelect = ({ logo, label, accept, onChange, file }: FileSelectProps) => {
  const [files, setFile] = useFiles();
  const fileData = files[file];
  const [pending, setPending] = useState(false);

  const onChangeImpl = (f: File) => {
    setFile(file, f);
    setPending(true);
    const reader = new FileReader();
    reader.onload = () => {
      setPending(false);
      onChange(Buffer.from(reader.result as ArrayBuffer));
    };
    reader.readAsArrayBuffer(f);
  };

  return (
    <label className="file-select">
      {label}
      <div className={`file-select-box ${!fileData ? 'empty' : ''}`}>
        <div className='file-select-box-img-container'>
          <img src={LOGOS[logo]} width="100%"/>
        </div>
        <input type="file" accept={accept} onChange={e => e.target.files && e.target.files[0] && onChangeImpl(e.target.files[0])}/>
        {pending && <p>...</p>}
        {!pending && !fileData && <p>No file selected</p>}
        {!pending && fileData && <p>{fileData.name}</p>}
      </div>
    </label>
  );
};
