import React from 'react';

import { itemName } from '@ootmm/core';
import { useStartingItems } from '../contexts/GeneratorContext';
import { InputNumber } from './InputNumber';
import { Group } from './Group';
import { Text } from './Text';


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
        <Text size='jb'>{NAMES[gamePrefix]}</Text>
        <Group direction='vertical'>
          {items.map((item) => (
            <Group direction='horizontal' key={item}>
              <InputNumber
                value={startingItems[item] || 0}
                onChange={e => alterItem(item, e)}
                min={0}
              />
              <Text size='xl' style={{paddingTop: '12px', fontWeight: startingItems[item] > 0 ? 'bold': 'Normal'}}>{itemName(item)}</Text>
            </Group>
          ))}
        </Group>
      </Group>
    );
  };

  return (
    <Group direction='vertical' spacing='xxl'>
      <Text size='mg'>Starting Items</Text>
      <Group direction='vertical' spacing='xl'>
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
