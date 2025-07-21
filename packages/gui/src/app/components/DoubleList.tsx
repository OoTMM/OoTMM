import { VNode } from 'preact';
import { useState } from 'preact/hooks';
import { Button, Card, CheckboxField } from './ui';

type DoubleListOption = {
  key: string;
  label: string;
  extra?: VNode;
};

type DoubleListColProps = {
  name: string;
  values: DoubleListOption[];
  selected: string[];
  onSelect: (k: string, v: boolean) => void;
}
function DoubleListCol({ name, values, selected, onSelect }: DoubleListColProps) {
  return (
    <Card className="flex-[1_1_0]">
      <div className="text-center text-xl border-b pb-4 dark:border-gray-600">
        {name}
      </div>
      <div className="h-full overflow-y-auto flex flex-col gap-2">
        {values.map(x =>
          <div className="flex items-center gap-2" key={x.key}>
            <CheckboxField label={x.label} checked={selected.includes(x.key)} onChange={v => onSelect(x.key, v)}/>
            <span className="flex-1"/>
            {x.extra && <span className="flex gap-2 mr-4">{x.extra}</span>}
          </div>
        )}
      </div>
    </Card>
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
    <div className="h-full flex flex-col gap-4">
      <div className="flex gap-2">
        <Button onClick={add}>Add</Button>
        <Button onClick={remove}>Remove</Button>
        <span className="flex-1"/>
        {reset && <Button variant="danger" onClick={reset}>Reset</Button>}
      </div>
      <div className="min-h-0 flex flex-[1_1_0] gap-4">
        <DoubleListCol name="Disabled" values={optionsLeft} selected={selectedLeft} onSelect={selectLeft}/>
        <DoubleListCol name="Enabled" values={optionsRight} selected={selectedRight} onSelect={selectRight}/>
      </div>
    </div>
  );
}
