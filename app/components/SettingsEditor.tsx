import React from 'react';
import { SETTINGS } from '@ootmm/core';

import { Dropdown } from './Dropdown';
import { Checkbox } from './Checkbox';
import { useSettings } from '../contexts/GeneratorContext';

type SettingsProps = {
  category: string;
};

export function SettingsEditor({ category }: SettingsProps) {
  const [settings, setSettings] = useSettings();
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
              onChange={(v) => setSettings({ [setting.key]: v })}
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
              onChange={(v) => setSettings({ [setting.key]: v })}
            />
          ))}
        </div>
      )}
    </form>
  );
};
