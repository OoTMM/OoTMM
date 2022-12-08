import React from 'react';
import { SETTINGS } from '@ootmm/core';

import { Dropdown } from './Dropdown';
import { Checkbox } from './Checkbox';

export const Settings = ({ category, settings, setSetting }) => {
  const settingsData = SETTINGS.filter(s => s.category === category);

  return (
    <form className='settings'>
      {settingsData.map(setting => {
        if (setting.type === 'enum') {
          return (
            <Dropdown
              value={settings[setting.key]}
              key={setting.key}
              label={setting.name}
              options={setting.values}
              onChange={v => setSetting({ [setting.key]: v }) }
            />
          );
        } else if (setting.type === 'boolean') {
          return (
            <Checkbox
              key={setting.key}
              label={setting.name}
              checked={settings[setting.key]}
              onChange={v => setSetting({ [setting.key]: v }) }
            />
          );
        } else {
          return null;
        }
      })}
    </form>
  );
};
