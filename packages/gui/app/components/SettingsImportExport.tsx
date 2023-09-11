import React from 'react';
import { Buffer } from 'buffer';
import { makeSettings } from '@ootmm/core';

import { useOverrideSettings, useSettings } from '../contexts/GeneratorContext';
import Group from './Group';

export const SettingsImportExport = () => {
  const [settings] = useSettings();
  const overrideSettings = useOverrideSettings();
  const buf = Buffer.from(JSON.stringify(settings));
  const str = buf.toString('base64');

  const onChange = (newStr: string) => {
    const newBuf = Buffer.from(newStr, 'base64');
    const newPartialSettings = JSON.parse(newBuf.toString());
    const newSettings = makeSettings(newPartialSettings);
    overrideSettings(newSettings);
  };

  return (
    <label>
      <Group direction='vertical' spacing='xs'>
        Import/Export Settings
        <textarea className='settings-string' value={str} onChange={x => onChange(x.target.value)}/>
      </Group>
    </label>
  )
};
