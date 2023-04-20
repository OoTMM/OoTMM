import React from 'react';

import { itemName } from '@ootmm/core';
import { useSettings, useStartingItems } from '../contexts/GeneratorContext';

const NAMES = {
  MM: "Majora's Mask",
  OOT: 'Ocarina of Time',
  SHARED: 'Shared',
}

export function StartingItems() {
  const { startingItems, itemPool, incr, decr, reset } = useStartingItems();

  const buildSingleTable = (gamePrefix: 'OOT' | 'MM' | 'SHARED') => {
    const items = Object.keys(itemPool).filter((item) => item.startsWith(gamePrefix));

    if (items.length === 0) {
      return null;
    }

    return (
      <table>
        <thead>
          <tr>
            <th colSpan={2}>
              {NAMES[gamePrefix]}
            </th>
          </tr>
        </thead>
        <tbody>
          {items.map((item) => (
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
        {buildSingleTable('OOT')}
        {buildSingleTable('MM')}
        {buildSingleTable('SHARED')}
      </div>
    </>
  );
}
