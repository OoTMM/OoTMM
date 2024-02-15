import React from 'react';
import { getTypographySizeRem } from '../utils/getTypographySizeRem';

type TextProps = {
    size: string; 
    children: React.ReactNode;
    style?: React.CSSProperties;
    className?: string;
  }
  
  
  // Create a component with inline styles for the specified typography size
  export function Text({ size, children, style, className}: TextProps)  {
    const typographyStyle = {
      fontSize: getTypographySizeRem(size),
      lineHeight: Number(getTypographySizeRem(size))*2,
    };

    const combinedStyle = { ...typographyStyle, ...style };
  
    return (
      <div style={combinedStyle} className={className}>
        {children}
      </div>
    );
  };
