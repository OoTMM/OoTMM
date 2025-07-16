import { useCallback, useMemo, useState } from 'react';
import Select from 'react-select';
import { FaXmark } from 'react-icons/fa6';
import { itemName } from '@ootmm/core';

import { useItemPool, useLocations, usePatchSettings, useSetting } from '../contexts/SettingsContext';

export function Plando() {
  const plando = useSetting('plando');
  const [selectedLoc, setSelectedLoc] = useState<string>();
  const [selectedItem, setSelectedItem] = useState<string>();
  const itemPool = useItemPool();
  const locations = useLocations();
  const locsOptions = useMemo(() => locations.map((loc) => ({ value: loc, label: loc })), [locations]);
  const itemOptions = useMemo(() => Object.keys(itemPool).map((item) => ({ value: item, label: itemName(item) })), [itemPool]);
  const patchSettings = usePatchSettings();

  const placeItem = useCallback(() => {
    if (selectedLoc && selectedItem) {
      patchSettings({ plando: { locations: { [selectedLoc]: selectedItem } } });
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
    <main>
      <h1>Plando</h1>
      <nav className="toolbar">
        <Select className="react-select-container" classNamePrefix="react-select" options={locsOptions} onChange={(v) => setSelectedLoc(v?.value)} />
        <Select className="react-select-container" classNamePrefix="react-select" options={itemOptions} onChange={(v) => setSelectedItem(v?.value)} />
        <button className="btn" onClick={placeItem}>Add</button>
        <button className="btn btn-danger" onClick={removeAll}>Remove All</button>
      </nav>
      <ul>
        {Object.entries(plando.locations || {})
          .filter((x) => x[1])
          .map(([loc, item]) => (
            <li key={loc}>
              <span className="list-remove" onClick={() => removeItem(loc)}>
                <FaXmark/>
              </span>
              <span className="list-item">
                {loc}: {itemName(item!)}
              </span>
            </li>
          ))}
      </ul>
    </main>
  );
}
