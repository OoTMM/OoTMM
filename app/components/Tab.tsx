import React, { useState } from 'react';

export type Tab = {
  name: string;
  component: React.ReactElement;
}

type TabProps = {
  tabs: Tab[];
}

export const TabBar = ({ tabs }: TabProps) => {
  const [activeTab, setActiveTab] = useState(0);

  return (
    <div className="tab-bar">
      <nav className="tab-bar-header">
        {tabs.map((tab, i) =>
          <a key={i} className={["tab-bar-header-tab", i === activeTab ? "active" : "inactive"].join(" ")} href="#" onClick={() => setActiveTab(i)}>{tab.name}</a>
        )}
      </nav>
      <div className="tab-bar-body">
        {tabs[activeTab].component}
      </div>
    </div>
  );
};
