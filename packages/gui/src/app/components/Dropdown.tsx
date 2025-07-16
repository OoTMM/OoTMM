import { VNode } from 'preact';
import { Tooltip } from './Tooltip';

type DropdownProps = {
  label?: string;
  options: { value: string; name: string }[];
  value: string;
  tooltip?: VNode;
  onInput: (value: string) => void;
}
export const Dropdown = ({ label, options, value, tooltip, onInput }: DropdownProps) => {
  return (
    <label>
      <>
        {label}
        {tooltip && <Tooltip>{tooltip}</Tooltip>}
      </>
      <select value={value} onInput={(e) => onInput(e.currentTarget.value)}>
        {options.map((option) => (
          <option key={option.value} value={option.value}>
            {option.name}
          </option>
        ))}
      </select>
    </label>
  );
};
