import React from "react";

const styles = {
  div: {
    display: "flex",
    "flex-direction": "column",
    with: "100%",
  },
  label: {
    "margin-bottom": "8px",
  },
};

export const Dropdown = ({ label, options, value, onChange }) => (
  <div style={styles.div}>
    <label style={styles.label}>{label}</label>
    <select value={value} onChange={(e) => onChange(e.target.value)}>
      {options.map((option) => (
        <option key={option.value} value={option.value}>
          {option.name}
        </option>
      ))}
    </select>
  </div>
);
