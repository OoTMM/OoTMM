import React from 'react';

type DropdownProps = {
  label: string;
  options: { value: string; name: string }[];
  value: string;
  onChange: (value: string) => void;
}

export const Dropdown = ({ label, options, value, onChange }: DropdownProps) => (
  <label>
    {label}
    <select value={value} onChange={(e) => onChange(e.target.value)}>
      {options.map((option) => (
        <option key={option.value} value={option.value}>
          {option.name}
        </option>
      ))}
    </select>
  </label>
);
