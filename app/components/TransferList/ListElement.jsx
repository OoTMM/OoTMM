import React from 'react';
import './transferlist.css';

export const ListElement = ({ label, selected, onClick }) => {
  return (
    <li
      onClick={onClick}
      className={selected ? 'selected' : ''}
    >
      {label}
    </li>
  );
}