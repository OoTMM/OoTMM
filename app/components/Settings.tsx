import React from 'react';
import { SETTINGS, Settings as CoreSettings } from '@ootmm/core';

import { Dropdown } from './Dropdown';
import { Checkbox } from './Checkbox';

type SettingsProps = {
  category: string;
  settings: CoreSettings;
  setSetting: (settings: Partial<CoreSettings>) => void;
};

export const Settings = ({ category, settings, setSetting }: SettingsProps) => {
  const settingsData = SETTINGS.filter((s) => s.category === category);
  const enumList = settingsData.filter((x) => x.type === 'enum');
  const booleanList = settingsData.filter((x) => x.type === 'boolean');

  return (
    <form className="settings">
      {booleanList.length > 0 && (
        <div className="checkboxes-lowcount">
          {booleanList.map((setting) => (
            <Checkbox
              key={setting.key}
              label={setting.name}
              checked={settings[setting.key] as boolean}
              onChange={(v) => setSetting({ [setting.key]: v })}
            />
          ))}
        </div>
      )}
      {enumList.length > 0 && (
        <div className="three-column-grid">
          {enumList.map((setting) => (
            <Dropdown
              value={settings[setting.key] as string}
              key={setting.key}
              label={setting.name}
              options={(setting as any).values}
              onChange={(v) => setSetting({ [setting.key]: v })}
            />
          ))}
        </div>
      )}
    </form>
  );
};
