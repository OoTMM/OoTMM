import { VNode } from 'preact';
import { Tooltip } from './Tooltip';

type CheckboxProps = {
  label?: string;
  checked: boolean;
  tooltip?: VNode;
  onInput: (checked: boolean) => void;
}

export const Checkbox = ({ label, checked, tooltip, onInput }: CheckboxProps) => (
  <label>
    <>
      <input
        type="checkbox"
        checked={checked}
        onInput={(e) => onInput(e.currentTarget.checked)}
        />
        <>
          {label}
          {tooltip && <Tooltip>{tooltip}</Tooltip>}
        </>
    </>
  </label>
);
