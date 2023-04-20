import React, { useState } from 'react';

export type Tab = {
  name: string;
  component: React.ReactElement;
}

type TabProps = {
  tabs: Tab[];
}

export function TabBar({ tabs }: TabProps) {
  const [activeTab, setActiveTab] = useState(0);

  return (
    <>
      <nav className="tab-bar-left">
        {tabs.map((tab, i) =>
          <a key={i} className={["tab", i === activeTab ? "active" : "inactive"].join(" ")} href="#" onClick={(e) => { e.preventDefault(); setActiveTab(i)}}>{tab.name}</a>
        )}
      </nav>
      <div className="tab-bar-right">
        {tabs[activeTab].component}
      </div>
    </>
  );
};
