import React from 'react';

const styles = {
  div: {
    display: 'flex',
    'align-items': 'center',
    'flex-direction': 'row',
    with: '100%',
  },
};

export const Checkbox = ({ label, checked, onChange }) => (
  <div style={styles.div}>
    <input
      type="checkbox"
      checked={checked}
      onChange={(e) => onChange(e.target.checked)}
    />
    <label onClick={() => onChange(!checked)}>{label}</label>
  </div>
);
