import React from 'react';

type ProgressProps = {
  message: string;
  progress: number | null;
};
export const Progress = ({ message, progress }: ProgressProps) => {
  return <div className="generator-modal-progress">
    <div className="panel">
      <h1>Your seed is being generated</h1>
      <p>{message}</p>
      {(progress !== null) && <div className="generator-progress">
        <span className="generator-progress-value">{Math.floor(progress)}%</span>
        <progress max="100" value={progress ?? undefined}/>
      </div>}
    </div>
  </div>;
};
