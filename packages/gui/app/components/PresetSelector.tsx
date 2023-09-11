import React, { useState } from 'react';
import CreatableSelect from 'react-select/creatable';

import { PRESETS, Settings } from '@ootmm/core';
import { useOverrideSettings, useSettings } from '../contexts/GeneratorContext';
import Group from './Group';

const NO_PRESET = "-----";

type Option = {
  label: string;
  value: Settings | null;
};

const customPresets = JSON.parse(localStorage.getItem("customPresets") || "{}");

export const PresetSelector = () => {
  const overrideSettings = useOverrideSettings();
  const [settings] = useSettings();
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

  const onChange = (o: Option | null) => {
    if (!o || o.value === null) {
      /* Clear */
      delete customPresets[value.label];
      localStorage.setItem("customPresets", JSON.stringify(customPresets));
      setValue(options[0]);
      return;
    }
    setValue(o);
    overrideSettings(o.value);
  };

  const onCreate = (x: string) => {
    const newOptionLabel = "Custom: " + x;
    const newOption = { label: newOptionLabel, value: settings };

    /* Also triggers a redraw */
    customPresets[newOptionLabel] = settings;
    localStorage.setItem("customPresets", JSON.stringify(customPresets));
    setValue(newOption);
  }

  return <label>
    <Group direction='vertical' spacing='xs'>
      <span>
        Preset
      </span>
      <CreatableSelect
        isClearable={!PRESETS.hasOwnProperty(value.label) && value.value !== null}
        options={options as any[]}
        onChange={(x) => onChange(x)}
        onCreateOption={(x) => onCreate(x)}
        value={value}
      />
    </Group>
    
  </label>;
};
