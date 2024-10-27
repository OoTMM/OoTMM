import { VNode } from 'preact';
import { Tooltip } from './Tooltip';

type InputNumberProps = {
  label?: string;
  value: number;
  onInput: (value: number) => void;
  min?: number;
  max?: number;
  tooltip?: VNode;
}

export const InputNumber = ({ label, value, onInput, min, max, tooltip }: InputNumberProps) => {
  return (
    <label>
       <>
        <span>
          {label}
          {tooltip && <Tooltip>{tooltip}</Tooltip>}
        </span>
        <input
          type="number"
          min={min ?? 0}
          max={max}
          value={value}
          onInput={(e) => onInput(e.currentTarget.valueAsNumber)}
        />
       </>
    </label>
  );
};
