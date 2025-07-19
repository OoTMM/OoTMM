import Select, { MultiValue } from 'react-select';
import { SETTINGS } from '@ootmm/core';

import { Dropdown } from '../Dropdown';
import { CheckboxField } from '../ui/CheckboxField';
import { InputNumber } from '../InputNumber';
import { usePatchSettings, useSettings } from '../../contexts/SettingsContext';
import { SelectField } from '../ui';

const SET_OPTIONS = [
  { value: 'none', name: 'None' },
  { value: 'all', name: 'All' },
  { value: 'specific', name: 'Specific' },
  { value: 'random', name: 'Random' },
  { value: 'random-mixed', name: 'Mixed Random' },
];

function SettingTooltip({ setting }: { setting: string }) {
  const settings = useSettings();
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
  case 'set':
    def = SET_OPTIONS.find(x => x.value == data.default)!.name;
    break;
  }

  return <>
    <pre className="whitespace-pre-line flex flex-col gap-4">
      {description.split('<br>').join('\n')}
      <ul className="list-disc pl-4 ml-2">
        {Object.entries(values).map(x => <li key={x[0]}><strong>{x[0]}</strong>: {x[1]}</li>)}
      </ul>
      <span>Default: <strong>{def}</strong></span>
    </pre>
  </>;
}

function SettingSet({ setting }: { setting: string }) {
  const settings = useSettings();
  const patchSettings = usePatchSettings();
  const data = SETTINGS.find(x => x.key === setting)!;
  const s = settings[data.key] as any;

  let valuesSet: typeof options = [];
  let valuesUnset: typeof options = [];

  const options: { value: string, label: string }[] = (data as any).values.map((x: any) => ({ value: x.value, label: x.name }));

  if (s.type === 'specific') {
    valuesSet = options.filter(x => s.values.includes(x.value));
    valuesUnset = [];
  } else if (s.type === 'random-mixed') {
    valuesSet = options.filter(x => s.set.includes(x.value));
    valuesUnset = options.filter(x => s.unset.includes(x.value));
  }

  const optionsSet = options.filter(x => !valuesUnset.map(x => x.value).includes(x.value));
  const optionsUnset = options.filter(x => !valuesSet.map(x => x.value).includes(x.value));

  const handleChangeSpecific = (v: MultiValue<{ value: string, label: string }>) => {
    const newValues = Array.from(new Set(v.map(x => x.value)));
    patchSettings({ [data.key]: { type: 'specific', values: newValues } as any });
  };

  const handleChangeRandomMixedSet = (v: MultiValue<{ value: string, label: string }>) => {
    const newValues = Array.from(new Set(v.map(x => x.value)));
    patchSettings({ [data.key]: { type: 'random-mixed', set: newValues, unset: valuesUnset.map(x => x.value) } as any });
  };

  const handleChangeRandomMixedUnset = (v: MultiValue<{ value: string, label: string }>) => {
    const newValues = Array.from(new Set(v.map(x => x.value)));
    patchSettings({ [data.key]: { type: 'random-mixed', set: valuesSet.map(x => x.value), unset: newValues } as any });
  };

  return (
    <span>
      <Dropdown
        value={(settings[data.key] as any).type as string}
        label={data.name}
        options={SET_OPTIONS}
        tooltip={(data as any).description && <SettingTooltip setting={data.key}/>}
        onInput={(v) => patchSettings({ [data.key]: { type: v, values: s.values } as any })}
      />
      {s.type === 'specific' &&
        <Select
          className="react-select-container"
          classNamePrefix="react-select"
          isMulti
          options={options} value={valuesSet}
          onChange={(v) => handleChangeSpecific(v)}
        />
      }
      {s.type === 'random-mixed' && <>
        <label>Disabled</label>
        <Select
          className="react-select-container"
          classNamePrefix="react-select"
          isMulti
          options={optionsUnset} value={valuesUnset}
          onChange={(v) => handleChangeRandomMixedUnset(v)}
        />
        <label>Enabled</label>
        <Select
          className="react-select-container"
          classNamePrefix="react-select"
          isMulti
          options={optionsSet} value={valuesSet}
          onChange={(v) => handleChangeRandomMixedSet(v)}
        />
      </>}
    </span>
  );
}

export function Setting({ setting }: { setting: string }) {
  const settings = useSettings();
  const patchSettings = usePatchSettings();
  const data = SETTINGS.find(x => x.key === setting)!;
  const cond = (data as any).cond;
  const isDisabled = cond && !cond(settings);

  if (isDisabled) {
    return null;
  }

  switch (data.type) {
  case 'enum':
    return (
      <SelectField
        value={settings[data.key] as string}
        label={data.name}
        options={(data as any).values.filter((x: any) => x.cond === undefined || x.cond(settings)).map((x: any) => ({ label: x.name, value: x.value }))}
        tooltip={data.description && <SettingTooltip setting={data.key}/>}
        onSelect={(v) => patchSettings({ [data.key]: v })}
      />
    );
  case 'set': return <SettingSet setting={setting}/>;
  case 'boolean':
    return (
      <CheckboxField
        label={data.name}
        tooltip={(data as any).description && <SettingTooltip setting={data.key}/>}
        checked={settings[data.key] as boolean}
        onChange={(v) => patchSettings({ [data.key]: v })}
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
        tooltip={(data as any).description && <SettingTooltip setting={data.key}/>}
        value={settings[data.key] as number}
        onInput={(v) => patchSettings({ [data.key]: v })}
        min={minValue}
        max={maxValue}
      />
    );
  default:
    return null;
  }
}
