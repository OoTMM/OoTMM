import React from 'react';
import { SETTINGS, SETTINGS_CATEGORIES } from '@ootmm/core';

import { Dropdown } from './Dropdown';
import { Checkbox } from './Checkbox';
import { useSettings } from '../contexts/GeneratorContext';
import { Tooltip } from './Tooltip';

function SettingEnum({ setting }: { setting: string }) {
  const [settings, setSettings] = useSettings();
  const data = SETTINGS.find(x => x.key === setting)!;

  return (
    <Dropdown
      value={settings[data.key] as string}
      label={data.name}
      options={(data as any).values}
      tooltip={(data as any).description ? data.key : undefined}
      onChange={(v) => setSettings({ [data.key]: v })}
    />
  );
}

function SettingBoolean({ setting }: { setting: string }) {
  const [settings, setSettings] = useSettings();
  const data = SETTINGS.find(x => x.key === setting)!;

  return (
    <Checkbox
      label={data.name}
      tooltip={(data as any).description ? data.key : undefined}
      checked={settings[data.key] as boolean}
      onChange={(v) => setSettings({ [data.key]: v })}
    />
  );
}

type SettingsPanelProps = {
  category: string;
};
export function SettingsPanel({ category }: SettingsPanelProps) {
  const [settings, setSettings] = useSettings();
  const settingsData = SETTINGS.filter((s) => s.category === category);
  const enumList = settingsData.filter((x) => x.type === 'enum');
  const booleanList = settingsData.filter((x) => x.type === 'boolean');

  return (
    <form className="settings">
      {booleanList.length > 0 && (
        <div className="checkboxes-lowcount">
          {booleanList.map(setting => <SettingBoolean key={setting.key} setting={setting.key}/>)}
        </div>
      )}
      {enumList.length > 0 && (
        <div className="three-column-grid">
          {enumList.map(setting => <SettingEnum key={setting.key} setting={setting.key}/>)}
        </div>
      )}
      {booleanList.filter(x => (x as any).description).map(setting => (
        <Tooltip key={setting.key} id={setting.key}>
          <p>{(setting as any).description}</p>
          <p>Default: <strong>{setting.default ? 'True' : 'False'}</strong></p>
        </Tooltip>
      ))}
      {enumList.filter(x => (x as any).description).map(setting => (
        <Tooltip key={setting.key} id={setting.key}>
          <p>{(setting as any).description}</p>
          <ul>
            {((setting as any).values as any[]).filter(x => x.description).map((v: any) => (
              <li key={v.value}><strong>{v.name}</strong>: {v.description}</li>
            ))}
          </ul>
          <p>Default: <strong>{((setting as any).values as any[]).find(x => x.value === (setting.default)).name}</strong></p>
        </Tooltip>
      ))}
    </form>
  );
};

type SettingsEditorProps = {
  category: string;
};
export function SettingsEditor({ category }: SettingsEditorProps) {
  const cat = SETTINGS_CATEGORIES.find(x => x.key === category)!;
  const subcategories = cat.subcategories || [];
  return (
    <>
      <SettingsPanel category={category}/>
      {subcategories.map(sub => <div key={sub.key} className='settings-group'>
        <h2>{sub.name}</h2>
        <SettingsPanel category={`${category}.${sub.key}`}/>
      </div>)}
    </>
  )
}
