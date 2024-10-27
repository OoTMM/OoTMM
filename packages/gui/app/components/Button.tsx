import { ComponentChildren } from 'preact';

type ButtonProps = {
  onClick?: () => void;
  type?: 'primary' | 'danger';
  children: ComponentChildren;
}
export function Button({ onClick, type, children }: ButtonProps) {
  const klasses = ['btn'];
  if (type) {
    klasses.push(`btn-${type}`);
  }
  return <button className={klasses.join(' ')} onClick={onClick}>{children}</button>;
}
