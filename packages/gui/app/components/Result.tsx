import JSZip from 'jszip';
import { GeneratorOutput } from '@ootmm/core';
import React, { useState } from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faTriangleExclamation } from '@fortawesome/free-solid-svg-icons';

type OutFile = {
  name: string;
  mime: string;
  data: Buffer | Blob | string;
};

const download = (file: OutFile) => {
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

const makeZip = async (files: OutFile[], hash: string): Promise<OutFile> => {
  if (files.length === 1) {
    return files[0];
  }

  const name = `OoTMM-${hash}.zip`;
  const mime = 'application/zip';
  const zip = new JSZip();
  files.forEach((file) => {
    zip.file(file.name, file.data);
  });
  const f = await zip.generateAsync({ type: 'blob', compression: 'DEFLATE' });
  return { name, mime, data: f };
}

const appendHash = (str: string, hash: string | null, ext: string) => {
  if (hash) {
    return `${str}-${hash}.${ext}`;
  }
  return `${str}.${ext}`;
};

type Props = {
  result: GeneratorOutput;
}
export const Result = ({ result: { roms, hash, log, patches } }: Props) => {
  const [outFile, setOutFile] = useState<OutFile | null>(null);
  const [outFilePending, setOutFilePending] = useState<boolean>(false);

  /* Generate zip */
  if (!outFile && !outFilePending) {
    setOutFilePending(true);
    const srcFiles: OutFile[] = [];
    if (log) srcFiles.push({ name: appendHash('OoTMM-Spoiler', hash, 'txt'), mime: 'text/plain', data: log });
    if (patches && patches.length === 1) {
      srcFiles.push({ name: appendHash('OoTMM-Patch', hash, 'ootmm'), mime: 'application/octet-stream', data: patches[0] });
    } else {
      patches.forEach((patch, i) => {
        srcFiles.push({ name: appendHash(`OoTMM-Patch-p${i+1}`, hash, 'ootmm'), mime: 'application/octet-stream', data: patch });
      });
    }
    if (roms && roms.length === 1) {
      srcFiles.push({ name: appendHash('OoTMM', hash, 'z64'), mime: 'application/octet-stream', data: roms[0] });
    } else {
      roms.forEach((rom, i) => {
        srcFiles.push({ name: appendHash(`OoTMM-p${i+1}`, hash, 'z64'), mime: 'application/octet-stream', data: rom });
      });
    }
    makeZip(srcFiles, hash).then((f) => {
      setOutFile(f);
      setOutFilePending(false);
    }).catch((e) => {
      console.error(e);
    });
  }

  return (
    <div>
      {log && <div style={{border: "none", borderRadius:"2px", background: "#ff5722", fontWeight:"700", fontSize:"1em", padding: "0.5em 1em", textDecoration: "none", minWidth: "48px"}}>
        <div><span style={{textAlign:"left"}}><FontAwesomeIcon icon={faTriangleExclamation}/> WARNING </span></div>
          <div>You have generated a spoiler log. It is STRONGLY recommended to store it.</div>
          <div>If you run into issues while playing the seed, you will be asked to provide the spoiler log for assistance.</div>
      </div>}
      <div>
        {outFilePending && <div>Building archive...</div>}
        {outFile && <button className='btn-download' onClick={() => download(outFile)}>Save</button>}
      </div>
    </div>
  );
};
