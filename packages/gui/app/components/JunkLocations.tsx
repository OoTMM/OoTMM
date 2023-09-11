import React from 'react';
import { locationList } from '@ootmm/core';

import { useSettings } from '../contexts/GeneratorContext';
import { ArrayList } from './ArrayList';
import { Group } from './Group';
import { Text } from './Text';

export function JunkLocations() {
  const [settings, setSettings] = useSettings();
  const locs = Object.keys(locationList(settings)).sort();
  const options = locs.map(loc => ({ value: loc, label: loc }));

  const add = (loc: string) => {
    setSettings({ junkLocations: { add: [loc] } });
  };

  const remove = (loc: string) => {
    setSettings({ junkLocations: { remove: [loc] } });
  }

  const clear = () => {
    setSettings({ junkLocations: { set: [] } });
  }

  return (
    <Group direction='vertical' spacing='xxl'>
      <Text size='mg'>Junk Locations</Text>
      <ArrayList options={options} selected={settings.junkLocations} add={add} remove={remove} clear={clear}/>
    </Group>
  );
}
