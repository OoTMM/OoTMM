import { ComponentChildren, toChildArray, VNode, ComponentProps, ComponentType } from 'preact';
import { useState } from 'preact/hooks';
import clsx from 'clsx';

import { Tab } from './Tab';

export type NavSideTab = {
  name: string;
  component: ComponentType;
  icon?: ComponentType;
  disabled?: boolean;
}

function NavSideTabImpl({ tab, isActive, onClick }: { tab: VNode<ComponentProps<typeof Tab>>, isActive: boolean, onClick: () => void }) {
  return (
    <a
      className={clsx("inline-flex items-center gap-3 [font-variant:small-caps] m-2 p-2 transition-all rounded", isActive ? 'font-semibold dark:bg-gray-800' : 'hover:dark:bg-gray-900')}
      href="#"
      onClick={(e) => {
        e.preventDefault();
        onClick();
      }}
    >
      {tab.props.icon && <tab.props.icon/>}
      {tab.props.name}
    </a>
  );
}

type TabsProps = {
  children: ComponentChildren;
};
export function NavSide({ children }: TabsProps) {
  const [activeTab, setActiveTab] = useState(0);
  const tabs = toChildArray(children) as VNode<ComponentProps<typeof Tab>>[];

  return (
    <div className="flex flex-row h-full w-full min-h-screen">
      <nav className="w-[200px] flex flex-col dark:bg-gray-950 border-r dark:border-gray-800">
        {tabs.filter(t => !t.props.disabled).map((tab, i) => <NavSideTabImpl key={i} tab={tab} isActive={i === activeTab} onClick={() => setActiveTab(i)} />)}
      </nav>
      <div key={activeTab} className="h-full w-full overflow-y-auto">
        {tabs[activeTab]}
      </div>
    </div>
  );
};
