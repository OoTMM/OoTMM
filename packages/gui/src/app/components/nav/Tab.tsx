import { ComponentChildren } from 'preact';
import { ComponentType } from 'react';

type TabProps = {
  name: string;
  icon?: ComponentType;
  disabled?: boolean;
  children?: ComponentChildren;
  component?: ComponentType;
};
export function Tab({ disabled, component: Component, children }: TabProps) {
  return disabled ? null : Component ? <Component/> : <>{children}</>;
};
