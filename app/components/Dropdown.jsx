import React from 'react';

export const Dropdown = ({ label, options, value, onChange }) => (
  <label>
    {label}
    <select value={value} onChange={e => onChange(e.target.value)}>
      {options.map(option => (
        <option key={option.value} value={option.value}>
          {option.name}
        </option>
      ))}
    </select>
  </label>
);
