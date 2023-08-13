import React from 'react';
import { getTypographySizeRem } from '../utils/getTypographySizeRem';

type TextProps = {
    size: string; 
    children: React.ReactNode;
  }
  
  
  // Create a component with inline styles for the specified typography size
  const Text: React.FC<TextProps> = ({ size, children}) => {
    const style = {
      fontSize: getTypographySizeRem(size),
      lineHeight: getTypographySizeRem(size),
    };
  
    return (
      <div style={style}>
        {children}
      </div>
    );
  };
  
  export default Text;