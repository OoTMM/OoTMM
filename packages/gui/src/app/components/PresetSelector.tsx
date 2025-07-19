import { isEqual } from 'lodash';
import { PRESETS, Settings } from '@ootmm/core';

import { localStoragePrefixedGet, localStoragePrefixedSet } from '../util';
import { useSetSettings, useSettings } from '../contexts/SettingsContext';
import { Select } from './ui';
import { useState } from 'preact/hooks';

type Option = {
  label: string;
  value: Settings;
  custom: boolean;
};

const customPresets = localStoragePrefixedGet("customPresets", {});

const makePresetOptions = () => {
  const options: Option[] = [];
  for (const preset of Object.keys(PRESETS)) {
    options.push({ label: preset, value: PRESETS[preset], custom: false });
  }
  for (const preset of Object.keys(customPresets)) {
    options.push({ label: preset, value: customPresets[preset], custom: true });
  }
  return options;
};

export const PresetSelector = () => {
  const [options, setOptions] = useState(makePresetOptions);
  const settings = useSettings();
  const setSettings = useSetSettings();
  const selectedOption = options.find(opt => isEqual(opt.value, settings));
  const value = selectedOption ? selectedOption.value : null;

  const onClear = () => {
    delete customPresets[selectedOption?.label || ''];
    localStoragePrefixedSet("customPresets", customPresets);
    setOptions(makePresetOptions());
    setSettings(PRESETS['Default']);
  };

  const onSelect = (v: Settings | null) => {
    if (v) {
      setSettings(v);
    }
  };

  const onCreate = (x: string) => {
    const newOptionLabel = "Custom: " + x;
    customPresets[newOptionLabel] = settings;
    localStoragePrefixedSet("customPresets", customPresets);
    setOptions(makePresetOptions());
  }

  return <label>
    Preset
    <Select
      placeholder='Select a preset, or type to create a new one'
      options={options}
      onSelect={onSelect}
      onCreate={onCreate}
      onClear={onClear}
      clearable={selectedOption?.custom}
      creatable={!selectedOption}
      value={value}
    />
  </label>;
};
