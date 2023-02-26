import React, { useMemo } from 'react';
import { locationList } from '@ootmm/core';

import { useSettings } from '../contexts/GeneratorContext';
import { TransferList } from './TransferList';

export function JunkLocations() {
  const [settings, setSettings] = useSettings();
  const locList = useMemo(() => locationList(settings), []);

  return (
    <TransferList
      label='junkLocations'
      locList={locList}
      settings={settings}
      setSetting={setSettings}
    />
  );
}

