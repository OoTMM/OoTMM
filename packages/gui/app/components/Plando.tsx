import React, { useState } from 'react';
import Select from 'react-select';
import { itemName, locationList } from '@ootmm/core';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faXmark } from '@fortawesome/free-solid-svg-icons';

import { useItemPool, useSettings } from '../contexts/GeneratorContext';

export function Plando() {
  const [selectedLoc, setSelectedLoc] = useState<string>();
  const [selectedItem, setSelectedItem] = useState<string>();
  const [settings, setSettings] = useSettings();
  const itemPool = useItemPool();
  const locs = Object.keys(locationList(settings)).sort();
  const locsOptions = locs.map((loc) => ({ value: loc, label: loc }));
  const itemOptions = Object.keys(itemPool).map((item) => ({ value: item, label: itemName(item) }));

  const placeItem = () => {
    if (selectedItem && selectedLoc) {
      setSettings({ plando: { locations: { [selectedLoc]: selectedItem } } });
    }
  };

  const removeItem = (loc: string) => {
    if (loc) {
      setSettings({ plando: { locations: { [loc]: null } } });
    }
  };

  const removeAll = () => {
    setSettings({ plando: { locations: null } });
  };

  return (
    <main>
      <h1>Plando</h1>
      <nav className="toolbar">
        <Select className="select" options={locsOptions} onChange={(v) => setSelectedLoc(v?.value)} />
        <Select className="select" options={itemOptions} onChange={(v) => setSelectedItem(v?.value)} />
        <button className="btn" onClick={placeItem}>Add</button>
        <button className="btn btn-danger" onClick={removeAll}>Remove All</button>
      </nav>
      <ul>
        {Object.entries(settings.plando.locations || {})
          .filter((x) => x[1])
          .map(([loc, item]) => (
            <li key={loc}>
              <span className="list-remove" onClick={() => removeItem(loc)}>
                <FontAwesomeIcon icon={faXmark} />
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
