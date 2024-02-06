import React from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faQuestionCircle } from '@fortawesome/free-solid-svg-icons';
import { Group } from './Group';

type InputNumberProps = {
  label?: string;
  value: number;
  onChange: (value: number) => void;
  min?: number;
  max?: number;
  tooltip?: string;
}

export const InputNumber = ({ label, value, onChange, min, max, tooltip }: InputNumberProps) => {
  return (
    <label>
       <Group direction='vertical' spacing='xs'>
        <span>
          {label}
          {tooltip && <a className="tooltip-link" id={tooltip} href="#"><FontAwesomeIcon icon={faQuestionCircle}/></a>}
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
