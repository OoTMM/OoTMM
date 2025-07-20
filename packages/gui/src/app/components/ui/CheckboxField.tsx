import { ReactNode, useId } from 'react';

import { Tooltip } from './Tooltip';
import { Checkbox } from './Checkbox';
import { Label } from './Label';

type CheckboxFieldProps = {
  checked: boolean;
  onChange: (checked: boolean) => void;
  label?: string;
  tooltip?: ReactNode;
}

export function CheckboxField({ label, checked, tooltip, onChange }: CheckboxFieldProps) {
  const id = useId();

  return (
    <div className="flex items-center gap-2">
      <Checkbox id={id} checked={checked} onChange={onChange}/>
      <Label htmlFor={id}>{label}</Label>
      {tooltip && <Tooltip>{tooltip}</Tooltip>}
    </div>
  );
}
