import React from 'react';
import { getTypographySizeRem } from '../utils/getTypographySizeRem';

type TextProps = {
    size: string; 
    children: React.ReactNode;
    style?: React.CSSProperties;
  }
  
  
  // Create a component with inline styles for the specified typography size
  export function Text({ size, children, style}: TextProps)  {
    const typographyStyle = {
      fontSize: getTypographySizeRem(size),
      lineHeight: getTypographySizeRem(size),
    };

    const combinedStyle = { ...typographyStyle, ...style };
  
    return (
      <div style={combinedStyle}>
        {children}
      </div>
    );
  };
