import React from 'react';
import './transferlist.css';

type ListElementProps = {
  label: string;
  selected: boolean;
  onClick: () => void;
}

export const ListElement = ({ label, selected, onClick }: ListElementProps) => {
  return (
    <li
      onClick={onClick}
      className={selected ? 'selected' : ''}
    >
      {label}
    </li>
  );
}
