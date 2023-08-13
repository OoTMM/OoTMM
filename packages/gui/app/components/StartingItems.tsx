import React from 'react';

import { itemName } from '@ootmm/core';
import { useStartingItems } from '../contexts/GeneratorContext';
import Group from './Group';
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

    return (
      <Group direction='vertical'>
        <h2>{NAMES[gamePrefix]}</h2>
        <Group direction='vertical'>
          {items.map((item) => (
            <Group direction='horizontal' key={item}>
              <InputNumber
                value={startingItems[item] || 0}
                onChange={e => alterItem(item, e)}
                min={0}
              />
              {itemName(item)}
            </Group>
          ))}
        </Group>
      </Group>
    );
  };

  return (
    <Group direction='vertical' spacing='24px'>
      <h1>Starting Items</h1>
      <Group direction='vertical'>
        <button className="btn-danger" onClick={reset} style={{width: '200px'}}>
          Reset Starting Items
        </button>
        <Group direction='horizontal'>
          {buildSingleTable('OOT')}
          {buildSingleTable('MM')}
          {buildSingleTable('SHARED')}
        </Group>
      </Group>
    </Group>
  );
}
