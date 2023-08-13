import React from 'react';
import { SETTINGS, SETTINGS_CATEGORIES } from '@ootmm/core';

import { Dropdown } from './Dropdown';
import { Checkbox } from './Checkbox';
import { useSettings } from '../contexts/GeneratorContext';
import { Tooltip } from './Tooltip';
import { InputNumber } from './InputNumber';
import { Group } from './Group';
import { Text } from './Text';

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
      <Text size='md' style={{whiteSpace:"pre-line"}}>{description.split('<br>').join('\n')}</Text>
      <ul>
        {Object.entries(values).map(x => <li key={x[0]}><strong>{x[0]}</strong>: {x[1]}</li>)}
      </ul>
      <Text size='md'>Default: <strong>{def}</strong></Text>
    </Tooltip>
  );
}

type SettingsPanelProps = {
  category: string;
};
export function SettingsPanel({ category }: SettingsPanelProps) {
  const settingsData = SETTINGS.filter((s) => s.category === category);
  const booleans = settingsData.filter(setting => setting.type === 'boolean')
  const nonBooleans = settingsData.filter(setting => setting.type !== 'boolean')

  return (
    <form>
      <Group direction='vertical' spacing='xl'>
        <div>
          <div className="three-column-grid">
            {nonBooleans.map(setting => <Setting key={setting.key} setting={setting.key}/>)}
          </div>
          {nonBooleans.map(setting => <SettingTooltip key={setting.key} setting={setting.key}/>)}
        </div>
        <div>
          <div className="three-column-grid">
            {booleans.map(setting => <Setting key={setting.key} setting={setting.key}/>)}
          </div>
          {booleans.map(setting => <SettingTooltip key={setting.key} setting={setting.key}/>)}
        </div>
      </Group>
       
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
    <Group direction='vertical' spacing='xxl'>
      <Text size='mg' style={{textTransform: 'capitalize'}}>{category}</Text>
      <Group direction='vertical' spacing='jb'>
        <SettingsPanel category={category}/>
        {subcategories.map(sub => <div key={sub.key}>
          <Group direction='vertical' spacing='lg'>
            <Text size='jb'>{sub.name}</Text>
            <SettingsPanel category={`${category}.${sub.key}`}/>
          </Group>
        </div>)}
      </Group>
    </Group>
  )
}
