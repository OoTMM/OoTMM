import type { ComponentProps } from 'react';
import { cn } from '@/app/util';

type InputProps = {
  className?: string;
  value: string;
  onChange: (data: string) => void;
} & Omit<ComponentProps<'input'>, 'value' | 'onChange'>;

export function Input({ value, onChange, className, ...props }: InputProps) {
  return (
    <input value={value} onInput={(e) => onChange(e.currentTarget.value)} className={cn("ux-bg ux-border ux-hover ux-outline p-2", className)} {...props}/>
  );
}
