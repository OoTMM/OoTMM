import { useCallback, useMemo, useState } from 'react';
import { FaXmark } from 'react-icons/fa6';
import { itemName } from '@ootmm/core';
import { Select, Button, Card } from './ui';

import { useItemPool, useLocations, usePatchSettings, useSetting } from '../contexts/SettingsContext';

export function Plando() {
  const plando = useSetting('plando');
  const [selectedLoc, setSelectedLocRaw] = useState<string | null>(null);
  const [selectedItem, setSelectedItemRaw] = useState<string | null>(null);
  const itemPool = useItemPool();
  const locations = useLocations();
  const locsOptions = useMemo(() => locations.map((loc) => ({ value: loc, label: loc })), [locations]);
  const itemOptions = useMemo(() => Object.keys(itemPool).map((item) => ({ value: item, label: itemName(item) })), [itemPool]);
  const patchSettings = usePatchSettings();

  const setSelectedLoc = (loc: string | null) => {
    if (!loc) return;
    setSelectedLocRaw(loc);
  };

  const setSelectedItem = (item: string | null) => {
    if (!item) return;
    setSelectedItemRaw(item);
  };

  const placeItem = useCallback(() => {
    if (selectedLoc && selectedItem) {
      patchSettings({ plando: { locations: { [selectedLoc]: selectedItem } } });
      setSelectedLocRaw(null);
      setSelectedItemRaw(null);
    }
  }, [selectedLoc, selectedItem]);

  const removeItem = useCallback((loc: string) => {
    if (loc) {
      patchSettings({ plando: { locations: { [loc]: null } } });
    }
  }, []);

  const removeAll = useCallback(() => {
    patchSettings({ plando: { locations: null } });
  }, []);

  return (
    <main className="h-full flex flex-col">
      <nav className="flex gap-2">
        <div className="flex-1"><Select searcheable placeholder="Location" options={locsOptions} onSelect={setSelectedLoc} value={selectedLoc}/></div>
        <div className="flex-1"><Select searcheable placeholder="Item" options={itemOptions} onSelect={setSelectedItem} value={selectedItem}/></div>
        <Button onClick={placeItem}>Add</Button>
        <Button variant="danger" onClick={removeAll}>Remove All</Button>
      </nav>
      <Card className="flex-[1_1_0] overflow-y-auto mt-4 gap-0">
        {Object.keys(plando.locations || {}).length === 0 &&
          <div className="flex items-center justify-center h-full">
            <span className="text-gray-500 text-3xl">No Plando Items</span>
          </div>
        }
        {Object.entries(plando.locations || {})
          .sort()
          .filter((x) => x[1])
          .map(([loc, item]) => (
            <div key={loc} className="flex items-center gap-1">
              <span className="hover:text-gray-500 cursor-pointer" onClick={() => removeItem(loc)}><FaXmark/></span>
              <span>{loc}: {itemName(item!)}</span>
            </div>
          ))}
      </Card>
    </main>
  );
}
