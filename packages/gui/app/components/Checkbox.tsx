import React from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { solid } from '@fortawesome/fontawesome-svg-core/import.macro';

type CheckboxProps = {
  label: string;
  checked: boolean;
  tooltip?: string;
  onChange: (checked: boolean) => void;
}

export const Checkbox = ({ label, checked, tooltip, onChange }: CheckboxProps) => (
  <label className="checkbox">
    <input
      type="checkbox"
      checked={checked}
      onChange={(e) => onChange(e.target.checked)}
      />
    {label}
    {tooltip && <a className="tooltip-link" id={tooltip} href="#"><FontAwesomeIcon icon={solid('question-circle')}/></a>}
  </label>
);
