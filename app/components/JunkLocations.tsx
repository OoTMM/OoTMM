import { Settings } from '@ootmm/core';
import React from 'react';
import { TransferList } from './TransferList';

type JunkLocationsProps = {
  settings: Settings;
  setSetting: (settings: Partial<Settings>) => void;
  locList: any;
};

export const JunkLocations = ({ settings, setSetting, locList }: JunkLocationsProps) =>
  <TransferList
    label='junkLocations'
    locList={locList}
    settings={settings}
    setSetting={setSetting}
  />;
