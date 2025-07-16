import { useCallback, useEffect, useState } from 'react';
import { itemName, Items } from '@ootmm/core';

import { InputNumber } from './InputNumber';
import { useItemPool, useSetting, useSetSettings } from '../contexts/SettingsContext';

const NAMES = {
  MM: "Majora's Mask",
  OOT: 'Ocarina of Time',
  SHARED: 'Shared',
}

export function StartingItems() {
  const startingItems = useSetting('startingItems');
  const [startingItemsCache, setStartingItemsCache] = useState(startingItems);
  const itemPool = useItemPool();
  const setSettings = useSetSettings();

  const alterItem = useCallback((item: string, count: number) => {
    /* Sanitize count */
    const countMax = itemPool[item] || 0;
    if (count < 0) count = 0;
    if (count > countMax) count = countMax;

    /* Create an update func */
    const updater = (s: Items) => {
      if (count > 0) {
        return { ...s, [item]: count };
      } else {
        const newS = { ...s };
        delete newS[item];
        return newS;
      }
    };

    /* Update */
    setStartingItemsCache(updater);
    setSettings((s) => ({ ...s, startingItems: updater(s.startingItems) }));
  }, [itemPool]);

  const reset = useCallback(() => {
    setStartingItemsCache({});
    setSettings((s) => ({ ...s, startingItems: {} }));
  }, []);

  /* Keep the cache in sync */
  useEffect(() => {
    setStartingItemsCache(startingItems);
  }, [startingItems]);

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
          value={startingItemsCache[item] || 0}
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
