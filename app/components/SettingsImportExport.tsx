import React, { useState } from 'react';
import { Buffer } from 'buffer';
import { useOverrideSettings, useSettings } from '../contexts/GeneratorContext';

import { mergeSettings, DEFAULT_SETTINGS, Settings } from '@ootmm/core';

export const SettingsImportExport = () => {
  const [settings] = useSettings();
  const overrideSettings = useOverrideSettings();
  const buf = Buffer.from(JSON.stringify(settings));
  const str = buf.toString('base64');

  const onChange = (newStr: string) => {
    const newBuf = Buffer.from(newStr, 'base64');
    const newPartialSettings = JSON.parse(newBuf.toString());
    const newSettings = mergeSettings(DEFAULT_SETTINGS, newPartialSettings as Partial<Settings>);
    overrideSettings(newSettings);
  };

  return (
    <label>
      Import/Export Settings
      <textarea className='settings-string' value={str} onChange={x => onChange(x.target.value)}/>
    </label>
  )
};
