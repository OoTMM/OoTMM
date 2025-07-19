import { ReactNode, ComponentProps } from 'react';
import clsx from 'clsx';

type ButtonProps = {
  className?: string;
  variant?: 'primary' | 'danger' | 'submit';
  children?: ReactNode;
} & ComponentProps<'button'>;

export function Button({ className, variant, children, ...props }: ButtonProps) {
  return (
    <button
      className={clsx(
        'p-2 border rounded cursor-pointer',
        'disabled:opacity-50 disabled:cursor-not-allowed',
        'hover:opacity-90',
        (variant === 'submit' && 'bg-green-600 text-white border-green-500'),
        className,
      )}
      {...props}
    >
      {children}
    </button>
  );
}
