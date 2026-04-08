import type { ComponentProps } from 'react';
import clsx from 'clsx';

type InputProps = {
  className?: string;
  value: string;
  onChange: (data: string) => void;
} & Omit<ComponentProps<'input'>, 'value' | 'onChange'>;

export function Input({ value, onChange, className, ...props }: InputProps) {
  return (
    <input value={value} onInput={(e) => onChange(e.currentTarget.value)} className={clsx("ux-bg ux-border ux-hover ux-outline p-2", className)} {...props}/>
  );
}
