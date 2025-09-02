import { ReactNode, useId, ComponentProps } from 'react';

import { Tooltip } from './Tooltip';
import { Input } from './Input';
import { Label } from './Label';

type InputFieldProps = {
  label?: string;
  tooltip?: ReactNode;
} & Omit<ComponentProps<typeof Input>, 'id'>;

export function InputField({ label, tooltip, ...props }: InputFieldProps) {
  const id = useId();

  return (
    <div className="flex flex-col gap-2">
      <span className="flex items-center gap-2"><Label htmlFor={id}>{label}</Label>{tooltip && <Tooltip>{tooltip}</Tooltip>}</span>
      <Input id={id} {...props} />
    </div>
  );
}
