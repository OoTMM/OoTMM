import { ComponentChildren, toChildArray, VNode } from 'preact';
import { useState } from 'preact/hooks';

type TabProps = {
  name: string;
  disabled?: boolean;
  children: ComponentChildren;
};
export function Tab({ disabled, children }: TabProps) {
  return disabled ? null : <>{children}</>;
};

type TabsProps = {
  children: ComponentChildren;
};
export function Tabs({ children }: TabsProps) {
  const [activeTab, setActiveTab] = useState(0);
  const tabs = toChildArray(children) as VNode<TabProps>[];

  return (
    <div className="tab-bar">
      <nav className="tab-bar-left">
        {tabs.map((tab, i) =>
          !tab.props.disabled && <a key={i} className={["tab", i === activeTab ? "active" : "inactive"].join(" ")} href="#" onClick={(e) => { e.preventDefault(); setActiveTab(i)}}>{tab.props.name}</a>
        )}
      </nav>
      <div className="tab-bar-right" key={activeTab}>
        {tabs[activeTab]}
      </div>
    </div>
  );
};
