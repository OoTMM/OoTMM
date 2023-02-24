import React from 'react';

type CheckboxProps = {
  label: string;
  checked: boolean;
  onChange: (checked: boolean) => void;
}

export const Checkbox = ({ label, checked, onChange }: CheckboxProps) => (
  <label className="checkbox">
    <input
      type="checkbox"
      checked={checked}
      onChange={(e) => onChange(e.target.checked)}
    />
    {label}
  </label>
);
