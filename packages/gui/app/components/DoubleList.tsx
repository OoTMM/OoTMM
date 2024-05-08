import React, { useState } from 'react';
import { Button } from './Button';

type DoubleListOption = {
  key: string;
  label: React.ReactNode;
  extra?: React.ReactNode;
};

type DoubleListColProps = {
  values: DoubleListOption[];
  selected: string[];
  onSelect: (k: string, v: boolean) => void;
}
function DoubleListCol({ values, selected, onSelect }: DoubleListColProps) {
  return (
    <ul className="double-list-col">
      {values.map(x =>
        <li className="double-list-row" key={x.key}>
          <input type="checkbox" checked={selected.includes(x.key)} onChange={e => onSelect(x.key, e.target.checked)}/>{x.label}
          {x.extra && <span className="double-list-row-extra">{x.extra}</span>}
        </li>
      )}
    </ul>
  );
}

type DoubleListProps = {
  onAdd: (k: string[]) => void;
  onRemove: (k: string[]) => void;
  onReset?: () => void;
  options: DoubleListOption[];
  selected: string[];
}
export function DoubleList({ onAdd, onRemove, onReset, options, selected }: DoubleListProps) {
  const optionsLeft = options.filter((k) => !selected.includes(k.key));
  const optionsRight = options.filter((k) => selected.includes(k.key));
  const [selectedLeft, setSelectedLeft] = useState<string[]>([]);
  const [selectedRight, setSelectedRight] = useState<string[]>([]);

  const selectLeft = (k: string, v: boolean) => {
    setSelectedLeft(v ? [...selectedLeft, k] : selectedLeft.filter(x => x !== k));
  };

  const selectRight = (k: string, v: boolean) => {
    setSelectedRight(v ? [...selectedRight, k] : selectedRight.filter(x => x !== k));
  };

  const add = () => {
    onAdd(selectedLeft);
    setSelectedLeft([]);
  };

  const remove = () => {
    onRemove(selectedRight);
    setSelectedRight([]);
  };

  const reset = onReset ? () => {
    onReset();
    setSelectedLeft([]);
    setSelectedRight([]);
  } : undefined;


  return (
    <div className="double-list">
      <div className="double-list-tools">
        <Button onClick={add}>Add</Button>
        {reset && <Button type="danger" onClick={reset}>Reset</Button>}
        <Button onClick={remove}>Remove</Button>
      </div>
      <div className="double-list-content">
        <DoubleListCol values={optionsLeft} selected={selectedLeft} onSelect={selectLeft}/>
        <DoubleListCol values={optionsRight} selected={selectedRight} onSelect={selectRight}/>
      </div>
    </div>
  );
}
