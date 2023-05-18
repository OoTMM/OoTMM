import React from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { solid } from '@fortawesome/fontawesome-svg-core/import.macro';

type DropdownProps = {
  label?: string;
  options: { value: string; name: string }[];
  value: string;
  tooltip?: string;
  onChange: (value: string) => void;
}
export const Dropdown = ({ label, options, value, tooltip, onChange }: DropdownProps) => {
  return (
    <label>
      <span>
        <span className="label-main">{label}</span>
        {tooltip && <a className="tooltip-link" id={tooltip} href="#"><FontAwesomeIcon icon={solid('question-circle')}/></a>}
      </span>
      <select value={value} onChange={(e) => onChange(e.target.value)}>
        {options.map((option) => (
          <option key={option.value} value={option.value}>
            {option.name}
          </option>
        ))}
      </select>
    </label>
  );
};
