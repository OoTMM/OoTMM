import React from 'react';
import { Group } from './Group';
import { Tooltip } from './Tooltip';

type InputNumberProps = {
  label?: string;
  value: number;
  onChange: (value: number) => void;
  min?: number;
  max?: number;
  tooltip?: React.ReactNode;
}

export const InputNumber = ({ label, value, onChange, min, max, tooltip }: InputNumberProps) => {
  return (
    <label>
       <Group direction='vertical' spacing='xs'>
        <span>
          {label}
          {tooltip && <Tooltip>{tooltip}</Tooltip>}
        </span>
        <input
          type="number"
          min={min ?? 0}
          max={max}
          value={value}
          onChange={(e) => onChange(e.target.valueAsNumber)}
        />
       </Group>
    </label>
  );
};
