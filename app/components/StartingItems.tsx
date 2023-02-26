import React from 'react';

import { itemName, Settings } from '@ootmm/core';
import { useStartingItems } from '../contexts/GeneratorContext';

export function StartingItems() {
  const { startingItems, itemPool, incr, decr, reset } = useStartingItems();

  // Valid gamePrefix are "MM" and "OOT"
  const buildSingleTable = (gamePrefix: 'MM' | 'OOT') => {
    return (
      <table>
        <thead>
          <tr>
            <th colSpan={2}>
              {gamePrefix === 'MM' ? "Majora's Mask" : 'Ocarina of Time'}
            </th>
          </tr>
        </thead>
        <tbody>
          {Object.keys(itemPool)
            .filter((item) => item.startsWith(gamePrefix))
            .map((item) => (
              <tr key={item} className={startingItems[item] > 0 ? 'active' : 'inactive'}>
                <td className="count">
                  <button className="count-adjust" onClick={() => decr(item)}>-</button>
                  {startingItems[item] || 0}
                  <button className="count-adjust" onClick={() => incr(item)}>+</button>
                </td>
                <td>{itemName(item)}</td>
              </tr>
            ))}
        </tbody>
      </table>
    );
  };

  return (
    <>
      <button className="btn-danger" onClick={reset}>
        Reset Starting Items
      </button>
      <div className="starting-items section-margin-top">
        {buildSingleTable('MM')}
        {buildSingleTable('OOT')}
      </div>
    </>
  );
}
