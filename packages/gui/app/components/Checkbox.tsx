import React from 'react';
import { Tooltip } from './Tooltip';

type CheckboxProps = {
  randomBlock?: any;
  label?: string;
  checked: boolean;
  tooltip?: React.ReactNode;
  onChange: (checked: boolean) => void;
  disabled?: boolean;
}

export const Checkbox = ({ randomBlock, label, checked, tooltip, onChange, disabled }: CheckboxProps) => (
  <label>
    <>
      {randomBlock}
      <input
        type="checkbox"
        checked={checked}
        onChange={(e) => onChange(e.target.checked)}
        disabled = {disabled}
        />
        <>
          {label}
          {tooltip && <Tooltip>{tooltip}</Tooltip>}
        </>
    </>
  </label>
);
