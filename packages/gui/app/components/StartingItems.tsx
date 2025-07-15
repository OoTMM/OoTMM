import { useCallback } from 'react';
import { itemName } from '@ootmm/core';

import { InputNumber } from './InputNumber';
import { useItemPool, useSetting, useSetSettings } from '../contexts/SettingsContext';

const NAMES = {
  MM: "Majora's Mask",
  OOT: 'Ocarina of Time',
  SHARED: 'Shared',
}

export function StartingItems() {
  const startingItems = useSetting('startingItems');
  const itemPool = useItemPool();
  const setSettings = useSetSettings();

  const alterItem = useCallback((item: string, count: number) => {
    if (count > 0) {
      setSettings((s) => ({ ...s, startingItems: { ...s.startingItems, [item]: count } }));
    } else {
      setSettings((s) => {
        const newStartingItems = { ...s.startingItems };
        delete newStartingItems[item];
        return { ...s, startingItems: newStartingItems };
      });
    }
  }, []);

  const reset = useCallback(() => {
    setSettings((s) => ({ ...s, startingItems: {} }));
  }, []);

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
          onInput={e => alterItem(item, e)}
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
