import React, { useState } from 'react';

type TabProps = {
  name: string;
  children?: React.ReactNode;
  disabled?: boolean;
};
export function Tab({ disabled, children }: TabProps) {
  return disabled ? null : children;
};

type TabsProps = {
  children?: React.ReactNode;
};
export function Tabs({ children }: TabsProps) {
  const [activeTab, setActiveTab] = useState(0);
  const tabs = React.Children.toArray(children) as React.ReactElement<TabProps>[];

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
