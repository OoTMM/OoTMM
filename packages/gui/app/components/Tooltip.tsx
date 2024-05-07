import React from 'react';
import { Tooltip as ReactTooltip } from 'react-tooltip';

type TooltipProps = {
  children?: React.ReactNode;
  id: string;
};
export function Tooltip({ children, id }: TooltipProps) {
  return (
    <ReactTooltip className="tooltip" anchorSelect={`#${id}`}>
      {children}
    </ReactTooltip>
  );
}
