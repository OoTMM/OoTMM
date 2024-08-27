import React from 'react';

import { Tooltip } from './Tooltip';

type DropdownProps = {
  randomBlock?: any;
  label?: string;
  options: { value: string; name: string }[];
  value: string;
  tooltip?: React.ReactNode;
  onChange: (value: string) => void;
  disabled?: boolean;
}
export const Dropdown = ({ randomBlock, label, options, value, tooltip, onChange, disabled }: DropdownProps) => {
  return (
    <label>
      <>
        {randomBlock}
        {label}
        {tooltip && <Tooltip>{tooltip}</Tooltip>}
      </>
      <select value={value} onChange={(e) => onChange(e.target.value)} disabled={disabled}>
        {options.map((option) => (
          <option key={option.value} value={option.value}>
            {option.name}
          </option>
        ))}
      </select>
    </label>
  );
};
