import React, { useState } from 'react';
import DarkModeToggle from './DarkMode/DarkModeToggle';
import { DarkModeContext } from '../contexts/DarkModeContext';

export type Tab = {
  name: string;
  component: React.ReactElement;
};

type TabProps = {
  tabs: Tab[];
};

export function TabBar({ tabs }: TabProps) {
  const [activeTab, setActiveTab] = useState(0);
  const { isDark, setIsDark } = DarkModeContext();

  return (
    <>
      <nav className="tab-bar-left">
        {tabs.map((tab, i) => (
          <a
            key={i}
            className={['tab', i === activeTab ? 'active' : 'inactive'].join(' ')}
            href="#"
            onClick={(e) => {
              e.preventDefault();
              setActiveTab(i);
            }}
          >
            {tab.name}
          </a>
        ))}
        <div className="dark-mode-container">
          <DarkModeToggle isDark={isDark} setIsDark={setIsDark} />
        </div>
      </nav>

      <div className="tab-bar-right">{tabs[activeTab].component}</div>
    </>
  );
}
