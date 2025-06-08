import Select, { MultiValue } from 'react-select';
import { SETTINGS, SUBCATEGORIES } from '@ootmm/core';

import { Dropdown } from './Dropdown';
import { Checkbox } from './Checkbox';
import { useSettings } from '../contexts/GeneratorContext';
import { InputNumber } from './InputNumber';

const SET_OPTIONS = [
  { value: 'none', name: 'None' },
  { value: 'all', name: 'All' },
  { value: 'specific', name: 'Specific' },
  { value: 'random', name: 'Random' },
  { value: 'random-mixed', name: 'Mixed Random' },
];

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
  case 'set':
    def = SET_OPTIONS.find(x => x.value == data.default)!.name;
    break;
  }

  return <>
    <pre style={{whiteSpace: "pre-line"}}>
      {description.split('<br>').join('\n')}
      <ul>
        {Object.entries(values).map(x => <li key={x[0]}><strong>{x[0]}</strong>: {x[1]}</li>)}
      </ul>
      Default: <strong>{def}</strong>
    </pre>
  </>;
}

function SettingSet({ setting }: { setting: string }) {
  const [settings, setSettings] = useSettings();
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
    setSettings({ [data.key]: { type: 'specific', values: newValues } as any });
  };

  const handleChangeRandomMixedSet = (v: MultiValue<{ value: string, label: string }>) => {
    const newValues = Array.from(new Set(v.map(x => x.value)));
    setSettings({ [data.key]: { type: 'random-mixed', set: newValues, unset: valuesUnset.map(x => x.value) } as any });
  };

  const handleChangeRandomMixedUnset = (v: MultiValue<{ value: string, label: string }>) => {
    const newValues = Array.from(new Set(v.map(x => x.value)));
    setSettings({ [data.key]: { type: 'random-mixed', set: valuesSet.map(x => x.value), unset: newValues } as any });
  };

  return (
    <span>
      <Dropdown
        value={(settings[data.key] as any).type as string}
        label={data.name}
        options={SET_OPTIONS}
        tooltip={(data as any).description && <SettingTooltip setting={data.key}/>}
        onInput={(v) => setSettings({ [data.key]: { type: v, values: s.values } as any })}
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
        options={(data as any).values.filter((x: any) => x.cond === undefined || x.cond(settings))}
        tooltip={data.description && <SettingTooltip setting={data.key}/>}
        onInput={(v) => setSettings({ [data.key]: v })}
      />
    );
  case 'set': return <SettingSet setting={setting}/>;
  case 'boolean':
    return (
      <Checkbox
        label={data.name}
        tooltip={(data as any).description && <SettingTooltip setting={data.key}/>}
        checked={settings[data.key] as boolean}
        onInput={(v) => setSettings({ [data.key]: v })}
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
        onInput={(v) => setSettings({ [data.key]: v })}
        min={minValue}
        max={maxValue}
      />
    );
  default:
    return null;
  }
}

type SettingsPanelProps = {
  category: string;
};
export function SettingsPanel({ category }: SettingsPanelProps) {
  const settingsData = SETTINGS.filter((s) => s.category === category);

  return (
    <form>
      {settingsData.map(x => <Setting key={x.key} setting={x.key}/>)}
    </form>
  );
};

type SettingsEditorProps = {
  name: string;
  category: string;
};
export function SettingsEditor({ name, category }: SettingsEditorProps) {
  return (
    <main>
      <h1>{name}</h1>
      <SettingsPanel category={category}/>
    </main>
  )
}

type MultipleSettingsEditorProps = {
  name: string;
};
export function MultipleSettingsEditor({ name }: MultipleSettingsEditorProps) {
  const categoriesData = SUBCATEGORIES.find((x) => x.key === name)?.categories;

  if (!categoriesData) {
    return null;
  }

  return (
    <main>
      {categoriesData.map((x) =>
        <>
        <h1>{x.key}</h1>
        <SettingsPanel category={x.category}/>
        </>
      )}
    </main>
  )
}
