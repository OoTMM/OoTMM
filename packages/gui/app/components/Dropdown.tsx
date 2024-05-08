import React from 'react';

import { Tooltip } from './Tooltip';

type DropdownProps = {
  label?: string;
  options: { value: string; name: string }[];
  value: string;
  tooltip?: React.ReactNode;
  onChange: (value: string) => void;
}
export const Dropdown = ({ label, options, value, tooltip, onChange }: DropdownProps) => {
  return (
    <label>
      <>
        {label}
        {tooltip && <Tooltip>{tooltip}</Tooltip>}
      </>
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
