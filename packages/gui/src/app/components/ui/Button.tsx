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
        'p-2 rounded cursor-pointer',
        'disabled:opacity-50 disabled:cursor-not-allowed',
        'hover:opacity-90',
        (variant === 'primary' || !variant) && 'bg-blue-800 text-white',
        variant === 'danger' && 'bg-red-800 text-white',
        (variant === 'submit' && 'bg-green-800 text-white'),
        className,
      )}
      {...props}
    >
      {children}
    </button>
  );
}
