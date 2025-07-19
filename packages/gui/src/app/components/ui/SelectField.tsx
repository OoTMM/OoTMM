import { ComponentProps, ReactNode, useId } from 'react';

import { Tooltip } from './Tooltip';
import { Checkbox } from './Checkbox';
import { Label } from './Label';
import { Select } from './Select';

type SelectFieldProps = {
  label?: string;
  tooltip?: ReactNode;
} & ComponentProps<typeof Select>;

export function SelectField({ label, tooltip, ...props }: SelectFieldProps) {
  const id = useId();

  return (
    <div className="flex flex-col gap-2">
      <span className="flex items-center gap-2"><Label htmlFor={id}>{label}</Label> {tooltip && <Tooltip>{tooltip}</Tooltip>}</span>
      <Select {...props}/>
    </div>
  );
}
