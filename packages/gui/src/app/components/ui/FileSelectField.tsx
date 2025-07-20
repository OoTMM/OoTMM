import { ComponentProps, useId } from 'react';

import { Label } from './Label';
import { FileSelect } from './FileSelect';

type FileSelectFieldProps = {
  label?: string;
} & ComponentProps<typeof FileSelect>;

export function FileSelectField({ label, ...props }: FileSelectFieldProps) {
  const id = useId();

  return (
    <div className="flex flex-col items-center gap-2">
      <Label htmlFor={id}>{label}</Label>
      <FileSelect id={id} {...props}/>
    </div>
  );
}
