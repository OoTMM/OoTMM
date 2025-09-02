import { ComponentProps, ReactNode, useId } from 'react';

import { Tooltip } from './Tooltip';
import { Label } from './Label';
import { Select } from './Select';

type SelectFieldProps = {
  label?: string;
  tooltip?: ReactNode;
} & Omit<ComponentProps<typeof Select>, 'id'>;

export function SelectField({ label, tooltip, ...props }: SelectFieldProps) {
  const id = useId();

  return (
    <div className="flex flex-col gap-2">
      <span className="flex items-center gap-2"><Label htmlFor={id}>{label}</Label> {tooltip && <Tooltip>{tooltip}</Tooltip>}</span>
      <Select id={id} {...props}/>
    </div>
  );
}
