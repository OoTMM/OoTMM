import { useCallback, useEffect, useMemo, useState } from 'react';
import { itemName, Items } from '@ootmm/core';

import { useItemPool, useSetting, useSetSettings } from '../contexts/SettingsContext';
import { Button, Card, InputField, Select } from './ui';

function itemNameSort(a: string, b: string) {
  const nameA = itemName(a);
  const nameB = itemName(b);
  if (nameA < nameB) return -1;
  if (nameA > nameB) return 1;
  return 0;
}

export function StartingItems() {
  const startingItems = useSetting('startingItems');
  const [startingItemsCache, setStartingItemsCache] = useState(startingItems);
  const itemPool = useItemPool();
  const setSettings = useSetSettings();
  const options = useMemo(() => Object.keys(itemPool).filter(x => !startingItemsCache[x]).sort(itemNameSort).map(item => ({ label: itemName(item), value: item })), [itemPool, startingItemsCache]);

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

  return (
    <div className="h-full flex flex-col">
      <Card className="flex-1 p-4 gap-4">
        <div className="flex gap-4">
          <div className="flex-1">
            <Select searcheable placeholder="Search for items" value={null} options={options} onSelect={(item) => alterItem(item, 1)} />
          </div>
          <Button className="w-[200px]" variant="danger" onClick={reset}>Reset Starting Items</Button>
        </div>
        <Card className="flex-[1_1_0] overflow-y-auto">
          {Object.keys(startingItemsCache).length === 0 ? (
            <div className="text-gray-500 text-3xl flex items-center justify-center h-full">
              No starting items
            </div>
          ) : (
            <div className="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 gap-4">
              {Object.keys(startingItemsCache).sort(itemNameSort).map(item => (
                <div key={item}>
                  <InputField label={itemName(item)} type="number" min="0" max={itemPool[item]} value={startingItemsCache[item].toString()} onChange={v => alterItem(item, Number(v))}/>
                </div>
              ))}
            </div>
          )}
        </Card>
      </Card>
    </div>
  );
}
