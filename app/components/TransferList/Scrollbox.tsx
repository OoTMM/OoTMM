import React from 'react';
import './transferlist.css';

type ScrollboxProps = {
  width: string;
  height: string;
  children: React.ReactNode;
};

export const Scrollbox = ({ width, height, children }: ScrollboxProps) => {
  const boxStyle = {
    div: {
      width: width,
      height: height,
    }
  }

  return (
    <div className='no-overflow' style={boxStyle.div}>
      <ol style={{ height: height }}>
        {children}
      </ol>
    </div>
  );
}
