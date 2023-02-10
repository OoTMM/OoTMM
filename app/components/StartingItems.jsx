import React from 'react';

export const StartingItems = ({ settings, setSetting, itemPool }) => {
  const alterItem = (item, delta) => {
    const { startingItems } = settings;
    const count = startingItems[item] || 0;
    if (count + delta < 0) {
      delete startingItems[item];
    } else {
      startingItems[item] = count + delta;
      if (startingItems[item] > itemPool[item]) {
        startingItems[item] = itemPool[item];
      }
    }
    setSetting({ startingItems });
  };

  return (
    <table>
      <thead>
        <tr>
          <th>Item</th>
          <th>Count</th>
        </tr>
      </thead>
      <tbody>
        {Object.keys(itemPool).map(item =>
          <tr key={item} className={settings.startingItems[item] > 0 ? "active" : "inactive"}>
            <td>{item}</td>
            <td>
              <button onClick={() => alterItem(item, -1)}>-</button>
              {settings.startingItems[item] || 0}
              <button onClick={() => alterItem(item, 1)}>+</button>
            </td>
          </tr>
        )}
      </tbody>
    </table>
  );
};
