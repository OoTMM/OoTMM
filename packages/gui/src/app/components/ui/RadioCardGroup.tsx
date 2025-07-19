import { ReactNode } from 'react';

type RadioCardGroupProps = {
  children?: ReactNode;
};
export function RadioCardGroup({ children }: RadioCardGroupProps) {
  return (
    <div>{children}</div>
  );
}
