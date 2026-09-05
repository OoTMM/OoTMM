import type { ReactNode } from 'react';
import { SETTINGS } from '@ootmm/generator';

import { InputField, SelectField, CheckboxField, Select } from '@/app/components/ui';
import { xor } from 'lodash-es';
import { useStore } from '@/app/store';

const SET_OPTIONS = [
  { value: 'none', label: 'None' },
  { value: 'all', label: 'All' },
  { value: 'specific', label: 'Specific' },
  { value: 'random', label: 'Random' },
  { value: 'random-mixed', label: 'Mixed Random' },
];

function SettingTooltip({ setting }: { setting: string }) {
  const settings = useStore(state => state.settings);
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
    def = SET_OPTIONS.find(x => x.value == data.default)!.label;
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

function SettingSet({ setting, label }: { setting: string; label?: string }) {
  const settings = useStore(state => state.settings);
  const patchSettings = useStore(state => state.patchSettings);
  const data = SETTINGS.find(x => x.key === setting)!;
  const s = settings[data.key] as any;

  let valuesSet: string[];
  let valuesUnset: string[];

  const options: { value: string, label: string }[] = (data as any).values.map((x: any) => ({ value: x.value, label: x.name }));
  const values = options.map(x => x.value);

  if (s.type === 'specific') {
    valuesSet = values.filter(x => s.values.includes(x));
    valuesUnset = [];
  } else if (s.type === 'random-mixed') {
    valuesSet = values.filter(x => s.set.includes(x));
    valuesUnset = values.filter(x => s.unset.includes(x));
  } else {
    valuesSet = [];
    valuesUnset = [];
  }

  const optionsSet = options.filter(x => !valuesUnset.includes(x.value));
  const optionsUnset = options.filter(x => !valuesSet.includes(x.value));

  const onToggleSpecific = (v: string | null) => {
    if (!v) return;
    const newValue = xor(s.values, [v]);
    patchSettings({ [data.key]: { type: 'specific', values: newValue }});
  };

  const onClearSpecific = () => {
    patchSettings({ [data.key]: { type: 'specific', values: [] }});
  };

  const onToggleRandomSet = (v: string | null) => {
    if (!v) return;
    const newValues = xor(s.set, [v]);
    patchSettings({ [data.key]: { type: 'random-mixed', set: newValues, unset: s.unset } as any });
  };

  const onToggleRandomUnset = (v: string | null) => {
    if (!v) return;
    const newValues = xor(s.unset, [v]);
    patchSettings({ [data.key]: { type: 'random-mixed', set: s.set, unset: newValues } as any });
  };

  const onClearRandomSet = () => {
    patchSettings({ [data.key]: { type: 'random-mixed', set: [], unset: s.unset } as any });
  };

  const onClearRandomUnset = () => {
    patchSettings({ [data.key]: { type: 'random-mixed', set: s.set, unset: [] } as any });
  };

  const onTypeChange = (type: typeof SET_OPTIONS[number]['value']) => {
    if (s.type === type) return;

    switch (type) {
    case 'none':
    case 'all':
    case 'random':
      patchSettings({ [data.key]: { type }});
      break;
    case 'specific':
      patchSettings({ [data.key]: { type: 'specific', values: [] }});
      break;
    case 'random-mixed':
      patchSettings({ [data.key]: { type: 'random-mixed', set: [], unset: [] }});
      break;
    }
  }

  return (
    <span>
      <SelectField
        value={(settings[data.key] as any).type as string}
        label={label ?? data.name}
        options={SET_OPTIONS}
        tooltip={(data as any).description && <SettingTooltip setting={data.key}/>}
        onSelect={onTypeChange as any}
      />
      {s.type === 'specific' &&
        <Select
          multi
          clearable
          options={options}
          value={valuesSet}
          onSelect={onToggleSpecific}
          onUnselect={onToggleSpecific}
          onClear={onClearSpecific}
        />
      }
      {s.type === 'random-mixed' && <>
        <label>Disabled</label>
        <Select
          multi
          clearable
          options={optionsUnset} value={valuesUnset}
          onSelect={onToggleRandomUnset}
          onUnselect={onToggleRandomUnset}
          onClear={onClearRandomUnset}
        />
        <label>Enabled</label>
        <Select
          multi
          clearable
          options={optionsSet} value={valuesSet}
          onSelect={onToggleRandomSet}
          onUnselect={onToggleRandomSet}
          onClear={onClearRandomSet}
        />
      </>}
    </span>
  );
}

type SettingProps = {
  setting: string;
  showDisabled?: boolean;
  label?: string;
};

export function Setting({ setting, showDisabled = false, label }: SettingProps) {
  const settings = useStore(state => state.settings);
  const patchSettings = useStore(state => state.patchSettings);
  const data = SETTINGS.find(x => x.key === setting)!;
  const cond = (data as any).cond;
  const isDisabled = !!cond && !cond(settings);
  const settingLabel = label ?? data.name;

  if (isDisabled && !showDisabled) {
    return null;
  }

  let content: ReactNode;

  switch (data.type) {
  case 'enum':
    content = (
      <SelectField
        value={settings[data.key] as string}
        label={settingLabel}
        options={(data as any).values.filter((x: any) => x.cond === undefined || x.cond(settings)).map((x: any) => ({ label: x.name, value: x.value }))}
        tooltip={data.description && <SettingTooltip setting={data.key}/>}
        onSelect={(v) => patchSettings({ [data.key]: v })}
      />
    );
    break;
  case 'set':
    content = <SettingSet setting={setting} label={settingLabel}/>;
    break;
  case 'boolean':
    content = (
      <CheckboxField
        label={settingLabel}
        tooltip={(data as any).description && <SettingTooltip setting={data.key}/>}
        checked={settings[data.key] as boolean}
        onChange={(v) => patchSettings({ [data.key]: v })}
      />
    );
    break;
  case 'number': {
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

    content = (
      <InputField
        type="number"
        label={settingLabel}
        tooltip={(data as any).description && <SettingTooltip setting={data.key}/>}
        value={settings[data.key].toString()}
        onChange={(v) => patchSettings({ [data.key]: Number(v) })}
        min={minValue}
        max={maxValue}
      />
    );
    break;
  }
  default:
    return null;
  }

  return (
    <div
      className={isDisabled ? 'opacity-50' : undefined}
      aria-disabled={isDisabled || undefined}
      title={isDisabled ? 'Unavailable with the current settings' : undefined}
    >
      <fieldset
        disabled={isDisabled}
        className={`m-0 min-w-0 border-0 p-0 ${isDisabled ? 'pointer-events-none' : ''}`}
      >
        {content}
      </fieldset>
    </div>
  );
}
