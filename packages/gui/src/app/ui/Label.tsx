import { ReactNode, ComponentProps } from 'react';

type LabelProps = {
  htmlFor: string;
  children?: ReactNode;
} & ComponentProps<'label'>;

export function Label({ htmlFor, children, ...props }: LabelProps) {
  return (
    <label htmlFor={htmlFor} {...props}>{children}</label>
  );
}
