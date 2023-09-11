import React, { useState } from 'react';
import Select from 'react-select';
import { itemName, locationList } from '@ootmm/core';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { solid } from '@fortawesome/fontawesome-svg-core/import.macro';

import { useItemPool, useSettings } from '../contexts/GeneratorContext';
import { Group } from './Group';
import { Text } from './Text';

export function Plando() {
  const [selectedLoc, setSelectedLoc] = useState<string>();
  const [selectedItem, setSelectedItem] = useState<string>();
  const [settings, setSettings] = useSettings();
  const itemPool = useItemPool();
  const locs = Object.keys(locationList(settings)).sort();
  const locsOptions = locs.map(loc => ({ value: loc, label: loc }));
  const itemOptions = Object.keys(itemPool).map(item => ({ value: item, label: itemName(item) }));

  const placeItem = () => {
    if (selectedItem && selectedLoc) {
      setSettings({ plando: { locations: { [selectedLoc]: selectedItem } }});
    }
  };

  const removeItem = (loc: string) => {
    if (loc) {
      setSettings({ plando: { locations: { [loc]: null } }});
    }
  };

  const removeAll = () => {
    setSettings({ plando: { locations: null }});
  };

  return (
    <Group direction='vertical' spacing='xxl'>
      <Text size='mg'>Plando</Text>
      <Group direction='horizontal' spacing='xl'>
        <Select className='plando-select' options={locsOptions} onChange={(v) => setSelectedLoc(v?.value)}/>
        <Select className='plando-select' options={itemOptions} onChange={(v) => setSelectedItem(v?.value)}/>
        <button className="btn-primary" onClick={placeItem}>Add</button>
        <button className="btn-danger" onClick={removeAll}>Remove All</button>
      </Group>
      <Group direction='vertical' spacing='xs'>
        {Object.entries(settings.plando.locations || {}).filter(x => x[1]).map(([loc, item]) => (
          <Group direction='horizontal' key={loc}>
            <span className="plando-remove" onClick={() => removeItem(loc)}><FontAwesomeIcon icon={solid('xmark')}/></span>
            <span className="plando-item">{loc}: {itemName(item!)}</span>
          </Group>
        ))}
      </Group>
    </Group>
  );
}
