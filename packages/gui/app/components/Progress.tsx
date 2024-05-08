import React from 'react';

type ProgressProps = {
  message: string;
}

export const Progress = ({ message }: ProgressProps) => {
  return <div className="generator-modal-progress">
    <div className="panel">
      <h1>Your seed is being generated</h1>
      <p>{message}</p>
    </div>
  </div>;
};
