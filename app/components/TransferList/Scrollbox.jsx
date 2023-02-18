import React from 'react';
import './transferlist.css';

export const Scrollbox = ({ width, height, children }) => {
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
      </ ol>
    </div>
  );
}