import React from 'react';
import { SETTINGS, SETTINGS_CATEGORIES } from '@ootmm/core';

import { Dropdown } from './Dropdown';
import { Checkbox } from './Checkbox';
import { useSettings } from '../contexts/GeneratorContext';
import { Tooltip } from './Tooltip';
import { InputNumber } from './InputNumber';

function Setting({ setting }: { setting: string }) {
  const [settings, setSettings] = useSettings();
  const data = SETTINGS.find(x => x.key === setting)!;
  const cond = (data as any).cond;

  if (cond && !cond(settings)) {
    return null;
  }

  switch (data.type) {
  case 'enum':
    return (
      <Dropdown
        value={settings[data.key] as string}
        label={data.name}
        options={(data as any).values}
        tooltip={(data as any).description ? data.key : undefined}
        onChange={(v) => setSettings({ [data.key]: v })}
      />
    );
  case 'boolean':
    return (
      <Checkbox
        label={data.name}
        tooltip={(data as any).description ? data.key : undefined}
        checked={settings[data.key] as boolean}
        onChange={(v) => setSettings({ [data.key]: v })}
      />
    );
  case 'number':
    const min = (data as any).min;
    const max = (data as any).max;
    let minValue: number | undefined;
    let maxValue: number | undefined;

    if (min !== undefined) {
      minValue = typeof min === 'function' ? min(settings) : min;
    }
    if (max !== undefined) {
      maxValue = typeof max === 'function' ? max(settings) : max;
    }

    return (
      <InputNumber
        label={data.name}
        tooltip={(data as any).description ? data.key : undefined}
        value={settings[data.key] as number}
        onChange={(v) => setSettings({ [data.key]: v })}
        min={minValue}
        max={maxValue}
      />
    );
  default:
    return null;
  }
}

function SettingTooltip({ setting }: { setting: string }) {
  const [settings] = useSettings();
  const data = SETTINGS.find(x => x.key === setting)!;
  const description = (data as any).description;

  const values: {[k: string]: string} = {};
  let def = '';

  if (!description) {
    return null;
  }

  switch (data.type) {
  case 'boolean':
    def = data.default ? 'true' : 'false';
    break;
  case 'enum':
    for (const v of (data as any).values) {
      if (v.description) {
        values[v.name] = v.description;
      }
    }
    def = ((data as any).values as any[]).find(x => x.value === (data.default)).name;
    break;
  case 'number':
    const min = (data as any).min;
    const max = (data as any).max;

    if (min !== undefined) {
      const str: string = typeof min === 'function' ? `${min(settings)} (dynamic)` : `${min}`;
      values['Minimum'] = str;
    }

    if (max !== undefined) {
      const str: string = typeof max === 'function' ? `${max(settings)} (dynamic)` : `${max}`;
      values['Maximum'] = str;
    }

    def = data.default.toString();
    break;
  }

  return (
    <Tooltip id={data.key}>
      <p>{description}</p>
      <ul>
        {Object.entries(values).map(x => <li key={x[0]}><strong>{x[0]}</strong>: {x[1]}</li>)}
      </ul>
      <p>Default: <strong>{def}</strong></p>
    </Tooltip>
  );
}

type SettingsPanelProps = {
  category: string;
};
export function SettingsPanel({ category }: SettingsPanelProps) {
  const settingsData = SETTINGS.filter((s) => s.category === category);

  return (
    <form className="settings">
      <div className="three-column-grid">
        {settingsData.map(setting => <Setting key={setting.key} setting={setting.key}/>)}
      </div>
      {settingsData.map(setting => <SettingTooltip key={setting.key} setting={setting.key}/>)}
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
