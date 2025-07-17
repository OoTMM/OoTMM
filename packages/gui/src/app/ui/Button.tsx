import { ReactNode, ComponentProps } from 'react';

type ButtonProps = {
  variant?: 'primary' | 'danger';
  children?: ReactNode;
} & ComponentProps<'button'>;

export function Button({ type, children, ...props }: ButtonProps) {
  return (
    <button {...props}>{children}</button>
  );
}
