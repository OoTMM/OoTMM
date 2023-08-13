import React, { ReactNode } from 'react';
import '../style/group.css';
type GroupProps = {
    direction: string;
    spacing?: string;
    children: ReactNode;
}

  const Group: React.FC<GroupProps> = ({ direction, spacing = '16px', children }) => {
    const isVertical = direction === 'vertical';
    const spacingStyle = isVertical
      ? { marginBottom: spacing }
      : { marginRight: spacing };
  
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