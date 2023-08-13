import React, { ReactNode } from 'react';
import '../style/group.css';
import { getTypographySizeRem } from '../utils/getTypographySizeRem';

type GroupProps = {
    direction: string;
    spacing?: string;
    children: ReactNode;
    style?: React.CSSProperties;
}

  const Group: React.FC<GroupProps> = ({ direction, spacing = "base", style, children }) => {
    const isVertical = direction === 'vertical';
    const spacingValue = getTypographySizeRem(spacing);
    const spacingStyle = isVertical
      ? { marginBottom: spacingValue }
      : { marginRight: spacingValue };
  
    return (
      <div className={`group ${isVertical ? 'vertical' : 'horizontal'}`} style={style}>
        {React.Children.map(children, (child, index) => (
          <>
            {child}
            {index !== React.Children.count(children) - 1 && (
              <div className={`group-space`} style={spacingStyle} />
            )}
          </>
        ))}
      </div>
    );
  };
  
  export default Group;