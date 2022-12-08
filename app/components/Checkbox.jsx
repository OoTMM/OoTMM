import React from 'react';

export const Checkbox = ({ label, checked, onChange }) => (
  <label className="checkbox">
    <input type="checkbox" checked={checked} onChange={e => onChange(e.target.checked)}/>
    {label}
  </label>
);
