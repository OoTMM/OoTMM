import React from 'react';
import { DUNGEONS } from '@ootmm/core';

import { useSettings } from '../contexts/GeneratorContext';
import { Dropdown } from './Dropdown';
import Group from './Group';

export function Dungeons() {
  const [settings, setSettings] = useSettings();
  const { dungeon } = settings;
  const options = [{
    name: 'Vanilla',
    value: 'vanilla',
   }, {
    name: 'Master Quest',
    value: 'mq',
  }, {
    name: 'Random',
    value: 'random',
  }];

  return (
    <Group direction='vertical' spacing='24px'>
      <h1>Dungeons</h1>
      <form className="settings">
          <div className="three-column-grid">
            {Object.keys(DUNGEONS).map(d =>
              <Dropdown
                key={d}
                label={(DUNGEONS as any)[d]}
                options={options}
                value={(dungeon as any)[d]}
                onChange={v => setSettings({ dungeon: { [d as keyof typeof DUNGEONS]: v } })}
              />
            )}
          </div>
      </form>
    </Group>
  );
}
