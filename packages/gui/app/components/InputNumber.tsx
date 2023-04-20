import React from 'react';

type InputNumberProps = {
  label: string;
  value: number;
  onChange: (value: number) => void;
}

export const InputNumber = ({ label, value, onChange }: InputNumberProps) => (
  <label>
    {label}
    <input
      type="number"
      min={0}
      value={value}
      onChange={(e) => onChange(e.target.valueAsNumber)}
    />
  </label>
);
