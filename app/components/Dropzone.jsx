import React, { useCallback } from 'react';
import { useDropzone } from 'react-dropzone';
import { Buffer } from 'buffer';

export const Dropzone = ({ onDrop }) => {
  const onDropInternal = useCallback((acceptedFiles) => {
    acceptedFiles.forEach((file) => {
      console.log(file);
      const reader = new FileReader();

      reader.onabort = () => console.log('file reading was aborted');
      reader.onerror = () => console.log('file reading has failed');
      reader.onload = () => {
        const bin = Buffer.from(reader.result);
        onDrop(bin);
      }
      reader.readAsArrayBuffer(file);
    })
  }, []);

  const {getRootProps, getInputProps} = useDropzone({onDropInternal});

  return (
    <div {...getRootProps()}>
      <input {...getInputProps()} />
      <p>Select file</p>
    </div>
  );
};
