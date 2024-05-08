import { GeneratorOutput } from '@ootmm/core';
import React, { useEffect, useState } from 'react';
import { ResultFile } from '../api';
import { useGenerator } from '../contexts/GeneratorContext';

const download = (file: ResultFile) => {
  const a = document.createElement('a');
  let blob: Blob;
  if (file.data instanceof Blob) {
    blob = file.data;
  } else {
    blob = new Blob([file.data], { type: file.mime });
  }
  a.href = window.URL.createObjectURL(blob);
  a.download = file.name;
  a.click();
};

type ResultProps = {
  archive: ResultFile | null;
}
export const Result = ({ archive }: ResultProps) => {
  return (
    <div className="panel">
      <h1>Your seed has been generated</h1>
      {!archive && <p>Building archive...</p>}
      {archive && <>
        <p>You can now download the seed.<br/>If there are multiple files (spoiler log, etc.), they will be bundled in a zip archive.</p>
        <button className='btn-primary' onClick={() => download(archive)}>Download</button>
      </>}
    </div>
  );
};
