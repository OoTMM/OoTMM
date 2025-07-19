import clsx from 'clsx';
import { ComponentProps } from 'react';

type InputProps = {
  className?: string;
} & ComponentProps<'input'>;

export function Input({ className, ...props }: InputProps) {
  return (
    <input className={clsx("ux-bg ux-border ux-hover ux-outline p-2", className)} {...props}/>
  );
}
