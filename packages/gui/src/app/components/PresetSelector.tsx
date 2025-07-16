import { useState } from 'react';
import CreatableSelect from 'react-select/creatable';
import { PRESETS, Settings } from '@ootmm/core';

import { localStoragePrefixedGet, localStoragePrefixedSet } from '../util';
import { useSetSettings, useSettings } from '../contexts/SettingsContext';

const NO_PRESET = "-----";

type Option = {
  label: string;
  value: Settings | null;
};

const customPresets = localStoragePrefixedGet("customPresets", {});

export const PresetSelector = () => {
  const settings = useSettings();
  const setSettings = useSetSettings();

  const makePresetOptions = () => {
    const options: Option[] = [];
    options.push({ label: NO_PRESET, value: null });
    for (const preset of Object.keys(PRESETS)) {
      options.push({ label: preset, value: PRESETS[preset] });
    }
    for (const preset of Object.keys(customPresets)) {
      options.push({ label: preset, value: customPresets[preset] });
    }
    return options;
  };
  const options = makePresetOptions();
  const [value, setValue] = useState<Option>(options[0]);

  const onInput = (o: Option | null) => {
    if (!o || o.value === null) {
      /* Clear */
      delete customPresets[value.label];
      localStoragePrefixedSet("customPresets", customPresets);
      setValue(options[0]);
      return;
    }
    setValue(o);
    setSettings(o.value);
  };

  const onCreate = (x: string) => {
    const newOptionLabel = "Custom: " + x;
    const newOption = { label: newOptionLabel, value: settings };

    /* Also triggers a redraw */
    customPresets[newOptionLabel] = settings;
    localStoragePrefixedSet("customPresets", customPresets);
    setValue(newOption);
  }

  return <label>
    Preset
    <CreatableSelect
      className="react-select-container"
      classNamePrefix="react-select"
      isClearable={!PRESETS.hasOwnProperty(value.label) && value.value !== null}
      options={options as any[]}
      onChange={(x) => onInput(x)}
      onCreateOption={(x) => onCreate(x)}
      value={value}
    />
  </label>;
};
