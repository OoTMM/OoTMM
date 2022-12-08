import React from 'react';

export const TabBar = ({ children }) => {
  const [activeTab, setActiveTab] = React.useState(0);

  return (
    <div className="tab-bar">
      <nav className="tab-bar-header">
        {React.Children.map(children, (child, i) =>
          <a key={i} className={["tab-bar-header-tab", i === activeTab ? "active" : "inactive"].join(" ")} href="#" onClick={() => setActiveTab(i)}>{child.props.name}</a>
        )}
      </nav>
      <div className="tab-bar-body">
        {React.Children.toArray(children)[activeTab].props.component}
      </div>
    </div>
  );
};

export const Tab = () => {
  return null;
};
