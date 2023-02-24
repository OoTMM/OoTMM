import React from 'react';

type ProgressProps = {
  message: string;
}

export const Progress = ({ message }: ProgressProps) => {
  return <h2>{message}</h2>;
};
