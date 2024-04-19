import React, { ReactNode } from 'react';
import '../style/group.css';
import { getTypographySizeRem } from '../utils/getTypographySizeRem';

type GroupProps = {
  direction: string;
  spacing?: string;
  children: ReactNode;
  style?: React.CSSProperties;
  className?: string;
  skipSpace?: boolean;
};

export function Group({ direction, spacing = 'base', style, className, skipSpace, children }: GroupProps) {
  const isVertical = direction === 'vertical';
  const spacingValue = getTypographySizeRem(spacing);
  const spacingStyle = isVertical ? { marginBottom: spacingValue } : { marginRight: spacingValue };

  return (
    <div className={`group ${isVertical ? 'vertical' : 'horizontal'} ${className}`} style={style}>
      {React.Children.map(children, (child, index) => (
        <>
          {child}
          {index !== React.Children.count(children) - 1 && !skipSpace && <div className={`group-space`} style={spacingStyle} />}
        </>
      ))}
    </div>
  );
}

export default Group;
