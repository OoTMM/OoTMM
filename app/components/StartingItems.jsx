import React from 'react';

import { itemName } from '@ootmm/core';

export const StartingItems = ({ settings, setSetting, itemPool }) => {
  const alterItem = (item, delta) => {
    const { startingItems } = settings;
    const count = startingItems[item] || 0;
    if (count + delta <= 0) {
      delete startingItems[item];
    } else {
      startingItems[item] = count + delta;
      if (startingItems[item] > itemPool[item]) {
        startingItems[item] = itemPool[item];
      }
    }
    setSetting({ startingItems });
  };

  const resetItems = () => {
    setSetting({ startingItems: {} });
  };

  // Valid gamePrefix are "MM" and "OOT"
  const buildSingleTable = (gamePrefix) => {
    return (
      <table>
        <thead>
          <tr>
            <th colSpan="2">
              {gamePrefix === 'MM' ? "Majora's Mask" : 'Ocarina of Time'}
            </th>
          </tr>
        </thead>
        <tbody>
          {Object.keys(itemPool)
            .filter((item) => item.startsWith(gamePrefix))
            .map((item) => (
              <tr
                key={item}
                className={
                  settings.startingItems[item] > 0 ? 'active' : 'inactive'
                }
              >
                <td className="count">
                  <button
                    className="count-adjust"
                    onClick={() => alterItem(item, -1)}
                  >
                    -
                  </button>
                  {settings.startingItems[item] || 0}
                  <button
                    className="count-adjust"
                    onClick={() => alterItem(item, 1)}
                  >
                    +
                  </button>
                </td>
                <td>{itemName(item)}</td>
              </tr>
            ))}
        </tbody>
      </table>
    );
  };

  return (
    <React.Fragment>
      <button className="btn-danger" onClick={() => resetItems()}>
        Reset Starting Items
      </button>
      <div className="starting-items section-margin-top">
        {buildSingleTable('MM')}
        {buildSingleTable('OOT')}
      </div>
    </React.Fragment>
  );
};
