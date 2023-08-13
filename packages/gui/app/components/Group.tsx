import React, { ReactNode } from 'react';
import '../style/group.css';
import { getTypographySizeRem } from '../utils/getTypographySizeRem';

type GroupProps = {
    direction: string;
    spacing?: string;
    children: ReactNode;
}

  const Group: React.FC<GroupProps> = ({ direction, spacing = "BASE", children }) => {
    const isVertical = direction === 'vertical';
    const spacingValue = getTypographySizeRem(spacing);
    const spacingStyle = isVertical
      ? { marginBottom: spacingValue }
      : { marginRight: spacingValue };
  
    return (
      <div className={`group ${isVertical ? 'vertical' : 'horizontal'}`}>
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