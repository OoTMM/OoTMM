import React from 'react';

import { itemName } from '@ootmm/core';
import { useStartingItems } from '../contexts/GeneratorContext';
import { InputNumber } from './InputNumber';

const NAMES = {
  MM: "Majora's Mask",
  OOT: 'Ocarina of Time',
  SHARED: 'Shared',
}

export function StartingItems() {
  const { startingItems, itemPool, alterItem, reset } = useStartingItems();

  const buildSingleTable = (gamePrefix: 'OOT' | 'MM' | 'SHARED') => {
    const items = Object.keys(itemPool).filter((item) => item.startsWith(gamePrefix));

    if (items.length === 0) {
      return null;
    }

    return <form className="vertical">
      <h1>{NAMES[gamePrefix]}</h1>
      {items.map((item) => (
        <InputNumber
          key={item}
          value={startingItems[item] || 0}
          onChange={e => alterItem(item, e)}
          min={0}
          label={itemName(item)}
        />
      ))}
    </form>;
  };

  return <main>
    <h1>Starting Items</h1>
    <div>
      <button className="btn-danger" onClick={reset} style={{width: '200px'}}>
        Reset Starting Items
      </button>
      <div className="dual-panels">
        {buildSingleTable('OOT')}
        {buildSingleTable('MM')}
        {buildSingleTable('SHARED')}
      </div>
    </div>
  </main>;
}
