import { ComponentChildren, toChildArray, VNode, ComponentProps } from 'preact';
import { useState } from 'preact/hooks';
import { Tab } from './Tab';

type TabsProps = {
  children: ComponentChildren;
};
export function NavTop({ children }: TabsProps) {
  const [activeTab, setActiveTab] = useState(0);
  const tabs = toChildArray(children) as VNode<ComponentProps<typeof Tab>>[];

  return (
    null
  );
};
