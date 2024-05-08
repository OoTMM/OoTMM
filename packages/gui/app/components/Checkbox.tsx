import React from 'react';
import { Tooltip } from './Tooltip';

type CheckboxProps = {
  label?: string;
  checked: boolean;
  tooltip?: React.ReactNode;
  onChange: (checked: boolean) => void;
}

export const Checkbox = ({ label, checked, tooltip, onChange }: CheckboxProps) => (
  <label>
    <>
      <input
        type="checkbox"
        checked={checked}
        onChange={(e) => onChange(e.target.checked)}
        />
        <>
          {label}
          {tooltip && <Tooltip>{tooltip}</Tooltip>}
        </>
    </>
  </label>
);
