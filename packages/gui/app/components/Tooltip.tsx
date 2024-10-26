import React, { useState } from 'react';
import { uniqueId } from 'lodash';
import { createPortal } from 'react-dom';
import { Tooltip as ReactTooltip } from 'react-tooltip';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faQuestionCircle } from '@fortawesome/free-solid-svg-icons';

type TooltipProps = {
  children?: React.ReactNode;
};
export function Tooltip({ children }: TooltipProps) {
  const [id] = useState(uniqueId('tooltip-'));
  const tooltipElement = <ReactTooltip id={id} className="tooltip">{children}</ReactTooltip>;
  const tooltip = createPortal(tooltipElement, document.body, id);

  return <>
    {tooltip}
    <span className="tooltip-link" data-tooltip-id={id}><FontAwesomeIcon icon={faQuestionCircle}/></span>
  </>;
}
