import { ComponentProps, ReactNode, useId } from 'react';

import { Tooltip } from './Tooltip';
import { Label } from './Label';
import { Select } from './Select';

function hexToCSS(hex: number): string {
  return `#${hex.toString(16).padStart(6, '0')}`;
}

type SelectFieldProps = {
  label?: string;
  tooltip?: ReactNode;
  color?: number | null;
} & Omit<ComponentProps<typeof Select>, 'id'>;

export function SelectField({ label, tooltip, color, ...props }: SelectFieldProps) {
  const id = useId();

  return (
    <div className="flex flex-col gap-2">
      <span className="flex items-center gap-2">
        <Label htmlFor={id} className="flex items-center justify-between gap-2 flex-1">
          {label}
          {color !== undefined && color !== null && (
            <span
              className="w-20 h-4 border border-gray-400 rounded flex-shrink-0"
              style={{ backgroundColor: hexToCSS(color) }}
            />
          )}
        </Label>
        {tooltip && <Tooltip>{tooltip}</Tooltip>}
      </span>
      <Select id={id} {...props}/>
    </div>
  );
}
