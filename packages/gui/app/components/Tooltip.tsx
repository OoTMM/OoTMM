import { ComponentChildren } from 'preact';
import { useState } from 'preact/hooks';
import { createPortal } from 'preact/compat';
import { uniqueId } from 'lodash';
import { Tooltip as ReactTooltip } from 'react-tooltip';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faQuestionCircle } from '@fortawesome/free-solid-svg-icons';

type TooltipProps = {
  children: ComponentChildren;
};
export function Tooltip({ children }: TooltipProps) {
  const [id] = useState(uniqueId('tooltip-'));
  const tooltipElement = <ReactTooltip id={id} className="tooltip">{children}</ReactTooltip>;
  const tooltip = createPortal(tooltipElement, document.body);

  return <>
    {tooltip}
    <span className="tooltip-link" data-tooltip-id={id}><FontAwesomeIcon icon={faQuestionCircle}/></span>
  </>;
}
