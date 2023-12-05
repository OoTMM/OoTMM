import React, { useMemo } from 'react';
import { Buffer } from 'buffer';
import { importSettings, exportSettings } from '@ootmm/core';

import { useOverrideSettings, useSettings } from '../contexts/GeneratorContext';
import Group from './Group';

export const SettingsImportExport = () => {
  const [settings] = useSettings();
  const overrideSettings = useOverrideSettings();
  const settingsString = useMemo(() => exportSettings(settings), [settings]);

  const onChange = (newStr: string) => {
    const newSettings = importSettings(newStr);
    overrideSettings(newSettings);
  };

  return (
    <label>
      <Group direction='vertical' spacing='xs'>
        Import/Export Settings
        <textarea className='settings-string' value={settingsString} onChange={x => onChange(x.target.value)}/>
      </Group>
    </label>
  )
};
